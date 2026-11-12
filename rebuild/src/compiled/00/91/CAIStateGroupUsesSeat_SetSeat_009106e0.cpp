// CAIStateGroup_UsesSeat::SetSeat(CThingObject* seat) -- PSeat (+0x14) = CIntelligentPointer<CThingObject>(seat), through a stack temporary
// Leaf: ?SetSeat@CAIStateGroup_UsesSeat@@IAEXPAVCThingObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009106E0, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetSeat@CAIStateGroup_UsesSeat@@IAEXPAVCThingObject@@@Z (protected); Ego_r CAIStateGroup_UsesSeat
//   PSeat is CIntelligentPointer<CThingObject> at +0x14 (struct_layouts_egor.tsv); the landed caller 0x0094B8B0 binds this exact spelling.
//   Was the stand-in CClass_009106e0::AssignTarget_009106e0.
// Body: the (T*) constructor of a temporary CIntelligentPointer<CThingObject> (0x00A01B10, vptr, SetPItem 0x00A01B90), the inline
//   operator= (GetPItem 0x00A01B50, SetPItem on PSeat), ~CBaseIntelligentPointer 0x00A01C10.
// Layout: no shared header for CAIStateGroup_UsesSeat; the state-group base (vptr, PBrain ..) is the 0x14 bytes before PSeat.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"

class CAIStateGroup_UsesSeat { public:
    unsigned char _base[0x14];                         // CAIStateGroupBase: vptr, PBrain, ..
    CIntelligentPointer<CThingObject> PSeat;           // +0x14
protected:
    void SetSeat(CThingObject* seat);
};

void CAIStateGroup_UsesSeat::SetSeat(CThingObject* seat)
{
    PSeat = CIntelligentPointer<CThingObject>(seat);
}
