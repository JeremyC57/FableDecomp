// CTCVolumeContainmentTracker::FrameUpdate() -- while tracking is enabled and the tracked volume is active, re-runs the containment Update every 5 seconds' worth of frames (every 0.8 s when the camera is within 10 units of the thing), staggered by the thing's UID; immediately when the def says so or the tracker is not yet updated
// Leaf: ?FrameUpdate@CTCVolumeContainmentTracker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785B40, 318 bytes (plain `ret`, ebp frame).
// Callees: CTCBase::GetWorld() 0x00686A70; CThing::GetPosition() const 0x004C73D0; the world's slot 8 (+0x20) camera
//          position getter (C3DVector by hidden pointer); GFGetCurrentFrame 0x0049D870; __allrem 0x00BFEC30;
//          CTCVolumeContainmentTracker::Update() 0x00785620.
// Globals: the tracking-enabled byte 0x01375757, g_framesPerSecond 0x01375550; constants 5.0f 0x0126B664, 0.8f 0x0126B660,
//          100.0f 0x01230A08, 0.5f/1.0f as immediates.
// Layout (Ego_r.pdb): CTCVolumeContainmentTracker PreviouslyUpdated +0x18, PDef +0x1c (def +0x25 an always-update flag);
//          CThing UID (unsigned __int64) +0x18.
#pragma optimize("y", off)
// Retail-asm: 0x00785B8E..0x00785BAE `fld [ebp - 0x10] ; fadd [ebp - 0xc] ; fistp [ebp - 4] ; fild [ebp - 4] ; fsub [ebp - 8] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x10] ; cmp eax, [ebp - 0x14] ; jne 0x785bab ; dec [ebp - 4] ; mov ecx, [ebp - 4] ; mov [ebp - 4], ecx` and 0x00785C1F..0x00785C3F `fld [ebp - 0xc] ; fadd [ebp - 0x10] ; fistp [ebp - 4] ; fild [ebp - 4] ; fsub [ebp - 0x14] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0x785c3c ; dec [ebp - 4] ; mov ecx, [ebp - 4] ; mov [ebp - 4], ecx` (the 5 s and 0.8 s intervals) -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
extern unsigned char g_bVolumeTrackingEnabled;  // 0x01375757
extern long g_framesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870

struct CWorldView {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetCameraPosition();      // slot 8 (+0x20)
};
class CThing {
public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UID;                       // +0x18
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CVolumeContainmentTrackerDef {
    unsigned char _pad_0x00[0x25];
    bool          AlwaysUpdate;                 // +0x25
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}

class CTCVolumeContainmentTracker { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x10];
    bool          PreviouslyUpdated;            // +0x18
    CVolumeContainmentTrackerDef* PDef;         // +0x1c
    CWorldView* GetWorld();                     // 0x00686A70 (CTCBase)
    void Update();                              // 0x00785620
    virtual void FrameUpdate();
};

void CTCVolumeContainmentTracker::FrameUpdate()
{
    if (!g_bVolumeTrackingEnabled)
        return;
    if (!PDef->AlwaysUpdate && PreviouslyUpdated) {
        long interval = CeilToLong(g_framesPerSecond * 5.0f);
        CWorldView* world = GetWorld();
        const C3DVector& pos = ParentThing->GetPosition();
        if ((world->GetCameraPosition() - pos).GetMagnitudeSquared() < 100.0f)
            interval = CeilToLong(g_framesPerSecond * 0.8f);
        unsigned __int64 uid = ParentThing->UID;
        if ((uid + GFGetCurrentFrame()) % interval != 0)
            return;
    }
    Update();
}