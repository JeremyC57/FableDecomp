// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Leaf: ?SetLastAwareInfo@CTCPerceiveThing@@AAEXXZ
struct CObj34 { char pad[0x44]; float f44; };

class CTCBase;
class CThing;
#include "engine/C3DVector.h"
class CTCPerceiveThing { public:
    char pad0[4];      // +0x00
    CThing* ParentThing; // +0x04 (Ego_r.pdb CTCBase::ParentThing)
    char pad8[4];      // +0x08
    char field_c[0xc]; // +0x0c getter subobject (address taken)
    float f18;         // +0x18
    char pad1c[4];     // +0x1c
    C3DVector v20;     // +0x20 (12 bytes -> +0x2c)
    char pad2c[8];     // +0x2c
    CObj34* p34;       // +0x34
    private: void SetLastAwareInfo();
};

// NF51 (round 7): the player GUI comes from the shared engine/CPlayerGui.h (was a made-up GObj with NotifyA / NotifyB members and an
// extern g_obj_13b8790 pointer): 0x013B8790 is the private static CCountedPointer<CPlayerGui>
// ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A read through the inline
// CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get() (engine/CTBaseSingletonNoDefaultAlloc.h), and the callees 0x004378F0 / 0x00437900 are
// Ego_r.pdb ?AddPerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z / ?RemovePerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z (header opt-in
// FABLE_RETAIL_CPLAYERGUI_PERCEIVERS), taking the component's parent thing (+0x04) by reference.
class CThing;
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PERCEIVERS
#include "engine/CPlayerGui.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// NF155 (q4046): the position getter 0x004C73D0 is Ego_r.pdb / FableWin.pdb ?GetPos@CThing@@QBEABVC3DVector@@XZ (engine/CThing.h;
// was the free stand-in Getter_d82ca0), called on the perceived thing the intelligent pointer at +0x0C holds.
#include "engine/CThing.h"

void CTCPerceiveThing::SetLastAwareInfo()
{
    CTCPerceiveThing* self = this;
    const CThing* sub = static_cast<const CThing*>(((CBaseIntelligentPointer*)(self->field_c))->GetPItem());
    self->v20 = sub->GetPos();

    float val = self->p34->f44;
    self->f18 = val;

    if (val != 0.0f) {
        if (((CBaseIntelligentPointer*)(self->field_c))->GetPItem()) {
            CPlayerGui::Get().AddPerceivingThing(*self->ParentThing);
        } else {
            CPlayerGui::Get().RemovePerceivingThing(*self->ParentThing);
        }
    }
}