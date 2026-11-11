#ifndef FABLE_ENGINE_CTHINGFILTER_HASSCRIPTEDCONTROL_H
#define FABLE_ENGINE_CTHINGFILTER_HASSCRIPTEDCONTROL_H
// CThingFilter_HasScriptedControl (new, chunk 2015, row 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp).
// Layout: Ego_r.pdb struct_layouts_egor.tsv, 1 byte (`bool CheckForActive` +0). Names: Ego_r.pdb / FableWin.pdb
// ??0CThingFilter_HasScriptedControl@@QAE@_N@Z (inline in retail: the caller stores the 1 into the filter byte, 0x0097FF63) and
// ??RCThingFilter_HasScriptedControl@@QBE_NABVCThing@@@Z (out of line at 0x006A8560: `cmp byte ptr [edi],0` reads CheckForActive).
// The std::unary_function<CThing,bool> base is what std::unary_negate<> needs (both PDBs name
// ??R?$unary_negate@VCThingFilter_HasScriptedControl@@@std@@QBE_NABVCThing@@@Z), as the shared CThingFilter_HasTC model.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_HasScriptedControl : public std::unary_function<CThing,bool> {
private:
    bool CheckForActive;                                                 // +0x00
public:
    CThingFilter_HasScriptedControl(bool check_for_active) : CheckForActive(check_for_active) {}
    bool operator()(const CThing& thing) const;                         // 0x006A8560
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_HasScriptedControl)==1);
#endif
