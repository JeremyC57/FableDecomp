// CTCDriverBase::CTCDriverBase(CThing* parent) -- CTCBase(parent), own vtable, then makes the parent thing drawable
// Leaf: ??0CTCDriverBase@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCDriverBase@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771200, length 35 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (vtable 0x0125BE0C = CTCBase); CThing vtable slot 28
// (`call [edx+70h]`, CThing vtable 0x01239ABC slot 28 = 0x004C8E50, manifest CThing::SetAsDrawable(bool)).
// Vtable 0x0126A79C = CTCDriverBase (rtti-vtables.tsv). CTCDriverBase adds no data members (it is the
// base of the CTCD* "drawable" components: CTCDHeroSuit 0x004E079C, CTCDPhysicalObstruction 0x004DF14B).
// Shape: speed TU (argument loaded into eax and pushed); the parent pointer at +4 is re-read from the
// base after the base constructor and its vtable fetched before the vtable store of this class lands
// (the virtual call is on the PARENT, not on `this`, so it stays a direct body call).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);              // slot 28 (+0x70), 0x004C8E50
};

class CTCDriverBase : public CTCBase { public:
    CTCDriverBase(CThing& parent);
};

CTCDriverBase::CTCDriverBase(CThing& parent)
    : CTCBase(parent)
{
    ParentThing->SetAsDrawable(true);
}