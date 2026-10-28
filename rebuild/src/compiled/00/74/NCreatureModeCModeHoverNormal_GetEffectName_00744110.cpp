// NCreatureMode::CModeHoverNormal::GetEffectName() -- the hover mode's effect name: "FLIGHT"
// Leaf: ?GetEffectName@CModeHoverNormal@NCreatureMode@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/74/CActionDoCreatureAction_GetActionName_00744110.cpp: vtable slot 14 of NCreatureMode::CModeHoverNormal; Ego_r.pdb lists GetEffectName on CModeHoverNormal and CModeHoverBase; literal "FLIGHT"
// (was: NCreatureMode::CModeHoverNormal::GetEffectName @ 00744110)
// Retail: __fastcall, constructs CCharString(const char*, long) into the hidden
// return slot and returns it (ret 4 cleans the struct-return pointer).

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

extern const char kActionName_01268848[];

namespace NCreatureMode {
struct CModeHoverNormal {
    CCharString GetEffectName();
};
}

CCharString NCreatureMode::CModeHoverNormal::GetEffectName()
{
    return CCharString(kActionName_01268848, -1);
}
