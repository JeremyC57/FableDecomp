// Behaviour test for 007525c0: the model below is a copy of src/007525c0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <math.h>
static int g_turnCalls = 0; static bool g_turnResult = true; static float g_outYZ = 0.0f, g_outXY = 0.0f;
struct CLookDef {
    unsigned char _pad00[0x30];
    float MaxHeadAngleXY;                       // +0x30 (degrees)
    float MaxHeadAngleYZUp;                     // +0x34 (degrees)
    float MaxHeadAngleYZDown;                   // +0x38 (degrees)
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CTCLook {
    virtual void v00() {}
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];
    void* PParentCreature;                      // +0x10
    CDefPointer<const CLookDef> PDef;           // +0x14
    unsigned char _pad18[0x64 - 0x18];
    long  LookFlags;                            // +0x64
    static long IsWithinRange(float angle, float threshold)
    {
        if ((float)fabs(angle) * 2.0f <= threshold)
            return 1;
        return 0;
    }
    bool GetTurningAngles(float* angleYZ, float* angleXY) { ++g_turnCalls; *angleYZ = g_outYZ; *angleXY = g_outXY; return g_turnResult; }
    bool GetTurningAnglesAndCheckForTurningBody(float* angleYZ, float* angleXY);
};

bool CTCLook::GetTurningAnglesAndCheckForTurningBody(float* angleYZ, float* angleXY)
{
    if (!GetTurningAngles(angleYZ, angleXY))
        return false;
    float threshold;
    long flags = LookFlags;
    if (!((flags & 1) && !(flags & 6)) && !(flags & 0x80))
        threshold = PDef->MaxHeadAngleXY * 0.0027777778f;
    else
        threshold = 1e-4f;
    return !(unsigned char)IsWithinRange(*angleXY, threshold);
}

// ---- test doubles and driver ----
static bool Run(bool turn, long flags, float maxXY, float xy, bool expect)
{
    CLookDef def; std::memset(&def, 0, sizeof(def)); def.MaxHeadAngleXY = maxXY;
    CTCLook look; std::memset(&look.ParentThing, 0, sizeof(look) - sizeof(void*));
    look.PDef.Object = &def; look.LookFlags = flags;
    g_turnResult = turn; g_outXY = xy; g_outYZ = 0.25f; g_turnCalls = 0;
    float yz = -1.0f, xyOut = -1.0f;
    bool r = look.GetTurningAnglesAndCheckForTurningBody(&yz, &xyOut);
    if (g_turnCalls != 1 || yz != 0.25f || xyOut != xy) return false;
    return r == expect;
}
int main()
{
    bool ok = true;
    ok = ok && Run(false, 0, 90.0f, 1.0f, false);        // no turning angles: false
    // flags 0: threshold = 90/360 = 0.25; |xy|*2 <= 0.25 -> within -> false
    ok = ok && Run(true, 0, 90.0f, 0.1f, false);
    ok = ok && Run(true, 0, 90.0f, -0.1f, false);        // fabs
    ok = ok && Run(true, 0, 90.0f, 0.2f, true);          // 0.4 > 0.25: turning body
    // flag 1 without 2/4: the "turning via animation" case uses the 1e-4 threshold
    ok = ok && Run(true, 1, 90.0f, 0.1f, true);
    ok = ok && Run(true, 1, 90.0f, 0.00004f, false);
    ok = ok && Run(true, 1 | 2, 90.0f, 0.1f, false);     // flag 1 with 2: back to the def threshold
    ok = ok && Run(true, 0x80, 90.0f, 0.1f, true);       // flag 0x80: tiny threshold
    ok = ok && Run(true, 4, 360.0f, 0.49f, false);       // flag 4 alone: def threshold 1.0
    if (!ok) { std::printf("007525c0_TEST FAIL\n"); return 1; }
    std::printf("007525c0_TEST PASS\n");
    return 0;
}