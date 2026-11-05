// NCameraModes::CScriptModuleConversationOverShoulderOfSpokenTo::Update() -- with two distinct conversation things, put the camera over the spoken-to's shoulder (2 units back along the speaker-to-spoken-to XY direction, sideways by 2 x RandomDirection, 4 up) looking at the speaker's focal point
// Leaf: ?Update@CScriptModuleConversationOverShoulderOfSpokenTo
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float height) const;       // 0x004C12D0
};
}
using NGraphicAppearance::CTCGraphicAppearance;
class CBaseObject;
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (retail)
    const C3DVector& GetPos() const;            // 0x004C73D0
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
    C3DVector GetXYDirectionTo(const C3DVector& pos) const
    {
        const C3DVector& here = GetPos();
        return C3DVector(here.X - pos.X, here.Y - pos.Y, 0.0f);
    }
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointer_CThingConst {
    virtual void v00();
    void* m_pItem;
    CThing* operator->() const { return ((CThing*)((CBaseIntelligentPointer*)this)->GetPItem()); }
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

class CScriptModuleBase { public:
    virtual void v00();
    CIntelligentPointer_CThingConst PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CCameraState*       POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
};
struct CScriptModuleConversationBase : CScriptModuleBase {
    CIntelligentPointer_CThingConst PSpeaker;        // +0x4c
    CIntelligentPointer_CThingConst PSpokenTo;       // +0x54
    float RandomDirection;                           // +0x5c
    bool IsThingsOk() const
    {
        const CThing* speaker = ((CThing*)((CBaseIntelligentPointer*)&PSpeaker)->GetPItem());
        const CThing* spokenTo = ((CThing*)((CBaseIntelligentPointer*)&PSpokenTo)->GetPItem());
        return speaker && spokenTo && speaker != spokenTo;
    }
};
struct CScriptModuleConversationOverShoulderOfSpokenTo : CScriptModuleConversationBase {
    void Update();
};

void CScriptModuleConversationOverShoulderOfSpokenTo::Update()
{
    if (IsThingsOk()) {
        C3DVector direction = PSpeaker->GetXYDirectionTo(PSpokenTo->GetPos());
        direction.AccurateNormalise();
        C3DVector side(direction.Y, -direction.X, 0.0f);
        C3DVector spokenToPos = PSpokenTo->GetPos();
        float scale = RandomDirection * 2.0f;
        C3DVector focal = PSpeaker->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
        POwner->Position = spokenToPos - direction * 2.0f + side * scale + C3DVector(0.0f, 0.0f, 4.0f);
        POwner->LookAt = focal;
    }
}

}