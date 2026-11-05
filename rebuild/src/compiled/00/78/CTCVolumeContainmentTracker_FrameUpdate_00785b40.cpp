// CTCVolumeContainmentTracker::FrameUpdate() -- while tracking is enabled and the tracked volume is active, re-runs the containment Update every 5 seconds' worth of frames (every 0.8 s when the camera is within 10 units of the thing), staggered by the thing's UID; immediately when the def says so or the tracker is not yet updated
// Leaf: ?FrameUpdate@CTCVolumeContainmentTracker@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetMagnitudeSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785B40, 318 bytes (plain `ret`, ebp frame).
// Callees: CTCBase::GetWorld() const 0x00686A70; CThing::GetPos() const 0x004C73D0; the world's slot 8 (+0x20) camera
//          position getter (C3DVector by hidden pointer); CWorld::GetFrame 0x0049D870; __allrem 0x00BFEC30;
//          CTCVolumeContainmentTracker::Update() 0x00785620.
// Globals: the tracking-enabled byte 0x01375757, CMainGameComponent::GetConstantFPS() 0x01375550; constants 5.0f 0x0126B664, 0.8f 0x0126B660,
//          100.0f 0x01230A08, 0.5f/1.0f as immediates.
// Layout (Ego_r.pdb): CTCVolumeContainmentTracker PreviouslyUpdated +0x18, PDef +0x1c (def +0x25 an always-update flag);
//          CThing UID (unsigned __int64) +0x18.
#pragma optimize("y", off)
// Retail-asm: 0x00785B8E..0x00785BAE `fld [ebp - 0x10] ; fadd [ebp - 0xc] ; fistp [ebp - 4] ; fild [ebp - 4] ; fsub [ebp - 8] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x10] ; cmp eax, [ebp - 0x14] ; jne 0x785bab ; dec [ebp - 4] ; mov ecx, [ebp - 4] ; mov [ebp - 4], ecx` and 0x00785C1F..0x00785C3F `fld [ebp - 0xc] ; fadd [ebp - 0x10] ; fistp [ebp - 4] ; fild [ebp - 4] ; fsub [ebp - 0x14] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0x785c3c ; dec [ebp - 4] ; mov ecx, [ebp - 4] ; mov [ebp - 4], ecx` (the 5 s and 0.8 s intervals) -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
class CThing;
#include "engine/C3DVector.h"
extern unsigned char g_bVolumeTrackingEnabled;  // 0x01375757
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetCameraPosition();      // slot 8 (+0x20) (name from use)
    static long __fastcall GetFrame();          // 0x0049D870
};
#include "engine/CThing.h"
struct CVolumeContainmentTrackerDef {
    unsigned char _pad_0x00[0x25];
    bool          AlwaysUpdate;                 // +0x25
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
#include "engine/GFFloatToLongCeil.h"

class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    CWorld& GetWorld() const;                    // 0x00686A70 (Ego_r.pdb CTCBase, protected)
    const CWorld& PeekWorld() const;             // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCVolumeContainmentTracker : public CTCBase { public:
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x10];
    bool          PreviouslyUpdated;            // +0x18
    CVolumeContainmentTrackerDef* PDef;         // +0x1c
    void Update();                              // 0x00785620
    virtual void FrameUpdate();
};

void CTCVolumeContainmentTracker::FrameUpdate()
{
    if (!g_bVolumeTrackingEnabled)
        return;
    if (!PDef->AlwaysUpdate && PreviouslyUpdated) {
        long interval = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * 5.0f);
        CWorld* world = &GetWorld();
        const C3DVector& pos = ParentThing->GetPos();
        if ((world->GetCameraPosition() - pos).GetSquaredMagnitude() < 100.0f)
            interval = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * 0.8f);
        unsigned __int64 uid = ParentThing->GetUniqueID();
        if ((uid + CWorld::GetFrame()) % interval != 0)
            return;
    }
    Update();
}