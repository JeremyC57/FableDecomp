// NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame() const -- true when the owner's def forces it or a movie sequence runs; otherwise refresh the stale rendered flag once a frame has passed and report it
// Leaf: WasRenderedLastFrame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD200, length 211 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?WasRenderedLastFrame@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ`.
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (twice), CGameScriptInterface::IsInMovieSequence() const (slot 395, +0x62C),
//          CWorld::GetWorldFrame (static) @ 0x0049D870 (twice), CTCGraphicAppearance::UpdateRenderedLastFrameFlag @ 0x004BC620,
//          the def's virtual slot 1.
// Shape: `forced = GetDef() && GetDef()->ForceRenderedLastFrameOverride` over a by-value CDefPointer getter: two
//        conditional temporaries tracked by construction flags in ebx (1, then 3), destroyed in reverse at the end of the
//        full expression, the result kept in a frame byte; the movie test shares the `mov al,1` exit
//        (two separate early returns) and reaches the script interface through an inline accessor (straight into ecx);
//        the last render frame is read into a local before the frame call (the +1 follows the call).
// Layout: retail CThing PBaseDef +0x70; retail CThingBaseDef ForceRenderedLastFrameOverride +0x43 bit 2 (PDB +0x47);
//         retail CTCGraphicAppearance FrameUpdatedWasRenderedFlag +0x54, RenderedLastFrame +0x59 bit 6; retail CWorld
//         PGameScriptInterface +0x38 and the last render frame +0x8C.
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
};
class CThingBaseDef : public CDefClassBase {
public:
    unsigned char _pad08[0x43 - 0x08];
    unsigned char DisableOcclusionTesting : 1;      // +0x43 bit 0
    unsigned char DrawAfterWater : 1;
    unsigned char ForceRenderedLastFrameOverride : 1;   // bit 2
    unsigned char AddToComboMultiplierOnHit : 1;
    unsigned char GiveHeroStatChangesOnBeingHit : 1;
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;      // +0x70
    CDefPointer<const CThingBaseDef> GetDef() const { return PBaseDef; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CWorld { public:
    unsigned char _pad00[0x38];
    CGameScriptInterface* PGameScriptInterface;     // +0x38
    unsigned char _pad3c[0x8c - 0x3c];
    long LastRenderFrame;                           // +0x8C
    static long __fastcall GetFrame();                    // 0x0049D870
    CGameScriptInterface* GetGameScriptInterface() const { return PGameScriptInterface; }
};

class CTCBase { protected:
    CWorld& GetWorld() const;                       // 0x00686A70 (Ego_r.pdb CTCBase, protected)
};

namespace NGraphicAppearance {

class CTCGraphicAppearance : public ::CTCBase {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    long          FrameUpdatedWasRenderedFlag;      // +0x54
    unsigned char UpdatePrimitiveReasons;           // +0x58
    unsigned char SelfIlluminating : 1;             // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    unsigned char DisableOcclusionTesting : 1;
    unsigned char DrawAfterWater : 1;
    unsigned char CachedStaticFocalPos : 1;
    unsigned char RenderedLastFrame : 1;            // bit 6
    unsigned char PrimitivesDrawn : 1;
    void UpdateRenderedLastFrameFlag() const;       // 0x004BC620
    bool WasRenderedLastFrame() const;
};

bool CTCGraphicAppearance::WasRenderedLastFrame() const
{
    bool forced = ParentThing->GetDef().Object && ParentThing->GetDef()->ForceRenderedLastFrameOverride;
    if (forced)
        return true;
    if (GetWorld().GetGameScriptInterface()->IsInMovieSequence())
        return true;
    if (FrameUpdatedWasRenderedFlag < CWorld::GetFrame()) {
        long lastRenderFrame = GetWorld().LastRenderFrame;
        if (lastRenderFrame + 1 < CWorld::GetFrame())
            UpdateRenderedLastFrameFlag();
    }
    return RenderedLastFrame;
}

}