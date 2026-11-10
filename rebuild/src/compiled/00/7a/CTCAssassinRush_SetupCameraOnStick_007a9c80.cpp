// CTCAssassinRush::SetupCameraOnStick() -- read the hero's camera-stick helper point; store it relative to the hero (LocalRelativeVectorForCamera) and the camera relative to it (RelativeVectorForCamera), then hand over to the camera setup
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A9C80, length 221 bytes (+ int3 padding).
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (on
// ParentThing+0x64, the helper-name CCharString global 0x013BAC4C), CThing::GetPos @ 0x004C73D0, CTCBase::GetWorld
// @ 0x00686A70, CGameCameraManager::PeekCurrentPos (virtual slot 11, +0x2C, C3DVector by hidden pointer; world +0x34), and the next
// member @ 0x007A9B10 (manifest "CCreatureAction_ReleasePhysicalShieldSpell::FinishAction" is propagated; it is called on
// `this`, name unproven).
// Layout: Ego_r.pdb CTCAssassinRush LocalRelativeVectorForCamera +0x78 / RelativeVectorForCamera +0x84 sit at retail +0x74 /
// +0x80 (the ItemAlphaList vector is 12 bytes in retail, 16 in the PDB).
// Shape: C3DVector has a user copy constructor and no operator=, so each difference built by `return C3DVector(...)` lands in a
// frame temporary and is copied through a `lea` pointer register; the hero position is copied by value (x, y on the x87
// stack, z through the frame); the camera manager comes through an inline accessor so it loads straight into ecx.
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
};
extern CCharString g_CameraStickHelperName;     // 0x013BAC4C
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float time) const;   // 0x004C0560 (Ego_r/FableWin ?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z)
};
}
class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector PeekCurrentPos() const;      // slot 11 (+0x2C) 0x0069E1F0
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCAssassinRush : public CTCBase {
public:
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x74 - 0x08];
    C3DVector LocalRelativeVectorForCamera;     // +0x74
    C3DVector RelativeVectorForCamera;          // +0x80
    void SetupCameraFromStick_007a9b10();                // 0x007A9B10 (descriptive)
    void SetupCameraOnStick();
};

void CTCAssassinRush::SetupCameraOnStick()
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
    if (appearance == 0)
        return;
    C3DVector stick;
    if (!appearance->GetHelperPoint(g_CameraStickHelperName, 0, 0, stick, 1.0f))
        return;
    C3DVector heroPosition = ParentThing->GetPos();
    LocalRelativeVectorForCamera = stick - heroPosition;
    RelativeVectorForCamera = GetWorld().GetCameraManager()->PeekCurrentPos() - stick;
    SetupCameraFromStick_007a9b10();
}