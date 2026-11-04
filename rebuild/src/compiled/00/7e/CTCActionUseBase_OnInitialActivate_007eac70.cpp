// CTCActionUseBase::OnInitialActivate() -- run the base slot, then give the parent thing a CTCEntityEvents component if it has none
// Leaf: ?OnInitialActivate@CTCActionUseBase@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EAC70, 63 bytes; CTCBase vtable slot 6 (Ego_r.pdb ?OnInitialActivate@CTCActionUseBase@@UAEXXZ;
//   the catalog vfunc_6 placeholder).
// Callees: CTCBase::OnInitialActivate 0x00686870 (the base slot, called qualified), CCharString(const char*, long) 0x0099EBF0,
//   CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60 (Ego_r.pdb ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z),
//   ~CCharString 0x0099EAE0.
// Shape: HasTC(TCI_ENTITY_EVENTS = 66: the UsingComponents bit, `test byte [eax+28h],4`), then the literal passed straight to
//   AddTC: the implicit conversion temporary is built before ParentThing is loaded for the call (a named string local
//   gives the same bytes; the explicit CCharString("...") temporary is built after the pushes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCBase.h"
class CTCActionUseBase : public CTCBase {
public:
    virtual void OnInitialActivate();
};

void CTCActionUseBase::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    if (!ParentThing->HasTC(TCI_ENTITY_EVENTS)) {
        ParentThing->AddTC("CTCEntityEvents", false, 0);
    }
}