// NCreatureMode::CModeHoverNormal::GetEffectName() -- the hover mode's effect name: "FLIGHT"
// Leaf: ?GetEffectName@CModeHoverNormal@NCreatureMode@@
// Literal (NF418, 2026-11-25): was extern data `kActionName_01268848` (table ?kActionName_01268848@@3QBDB -> 0x01268848); now spelled at the use site:
//   "FLIGHT", retail's pooled literal at 0x01268848 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/74/CActionDoCreatureAction_GetActionName_00744110.cpp: vtable slot 14 of NCreatureMode::CModeHoverNormal; Ego_r.pdb lists GetEffectName on CModeHoverNormal and CModeHoverBase; literal "FLIGHT"
// (was: NCreatureMode::CModeHoverNormal::GetEffectName @ 00744110)
// Retail: __fastcall, constructs CCharString(const char*, long) into the hidden
// return slot and returns it (ret 4 cleans the struct-return pointer).

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};


namespace NCreatureMode {
struct CModeHoverNormal {
    CCharString GetEffectName();
};
}

CCharString NCreatureMode::CModeHoverNormal::GetEffectName()
{
    return CCharString("FLIGHT", -1);
}
