#include <cstdio>
// behaviour test of CTCVolumeContainmentTracker::FrameUpdate (0x00785B40): after the first update the tracker re-runs every ceil(5 fps) frames, or every ceil(0.8 fps) frames when within 10 units of the camera, staggered by the thing's UID
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
typedef unsigned __int64 Uid64;
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }
typedef unsigned long long Uid64;
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
unsigned char g_bVolumeTrackingEnabled = 1;
long g_framesPerSecond = 15;
long g_currentFrame = 0;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
long CWorld::GetFrame() { return g_currentFrame; }
struct CWorldView { C3DVector Camera; C3DVector GetCameraPosition() { return Camera; } };
class CThing { public: Uid64 UID; C3DVector Position; const C3DVector& GetPosition() const { return Position; } };
struct CVolumeContainmentTrackerDef { bool AlwaysUpdate; };
class CTCVolumeContainmentTracker { public:
    CThing* ParentThing; bool PreviouslyUpdated; CVolumeContainmentTrackerDef* PDef; CWorldView* World; long Updates;
    CWorldView* GetWorld() { return World; }
    void Update() { ++Updates; }
    void FrameUpdate()
    {
        if (!g_bVolumeTrackingEnabled)
            return;
        if (!PDef->AlwaysUpdate && PreviouslyUpdated) {
            long interval = CeilToLong(g_framesPerSecond * 5.0f);
            CWorldView* world = GetWorld();
            const C3DVector& pos = ParentThing->GetPosition();
            if ((world->GetCameraPosition() - pos).GetMagnitudeSquared() < 100.0f)
                interval = CeilToLong(g_framesPerSecond * 0.8f);
            Uid64 uid = ParentThing->UID;
            if ((uid + CWorld::GetFrame()) % interval != 0)
                return;
        }
        Update();
    }
};
static long Run(CTCVolumeContainmentTracker& t, long frames)
{
    t.Updates = 0;
    for (g_currentFrame = 0; g_currentFrame < frames; ++g_currentFrame) t.FrameUpdate();
    return t.Updates;
}
int main()
{
    CWorldView world; world.Camera = C3DVector(0.0f, 0.0f, 0.0f);
    CThing thing; thing.UID = 7; thing.Position = C3DVector(100.0f, 0.0f, 0.0f);
    CVolumeContainmentTrackerDef def; def.AlwaysUpdate = false;
    CTCVolumeContainmentTracker t; t.ParentThing = &thing; t.PreviouslyUpdated = true; t.PDef = &def; t.World = &world;
    bool ok = Run(t, 150) == 2;                          // far away: every 75 frames (frames 68 and 143 for UID 7)
    thing.Position = C3DVector(6.0f, 8.0f, 0.0f);        // exactly 10 units: not within
    ok = ok && Run(t, 150) == 2;
    thing.Position = C3DVector(3.0f, 4.0f, 0.0f);        // near the camera: every ceil(12.0) = 12 frames
    ok = ok && Run(t, 120) == 10;
    g_framesPerSecond = 13;                              // 0.8 * 13 = 10.4 -> 11
    ok = ok && Run(t, 110) == 10;
    g_framesPerSecond = 15;
    t.PreviouslyUpdated = false;                         // never updated yet: every frame
    ok = ok && Run(t, 10) == 10;
    t.PreviouslyUpdated = true; def.AlwaysUpdate = true; // def says always
    ok = ok && Run(t, 10) == 10;
    def.AlwaysUpdate = false; g_bVolumeTrackingEnabled = 0;
    ok = ok && Run(t, 150) == 0;
    std::printf(ok ? "00785b40_TEST PASS\n" : "00785b40_TEST FAIL\n");
    return ok ? 0 : 1;
}