// NCreatureMode::CModeBase::GetEffectName() -- the default effect name of a creature mode: the empty string
// Leaf: ?GetEffectName@CModeBase@NCreatureMode@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/74/CActionDoCreatureAction_GetActionName_00743dd0.cpp: shared by several NCreatureMode classes at vtable slot 14 (CModeWalkInAllDirections, CModeHoverBase, ...) = the base default of the slot CModeHoverNormal::GetEffectName (0x00744110) overrides; returns ""; the declaring base is named from the shared vtable position, not from a PDB slot dump
// (was: NCreatureMode::CModeBase::GetEffectName @ 0x00743dd0)
// Real __fastcall member returning CCharString by value: this in ECX,
// hidden struct-return pointer on the stack (ret 4). this is unused.

class CCharString {
public:
    CCharString(const char* text, long length);
    long storage_;
};

namespace NCreatureMode {
class CModeBase {
public:
    CCharString GetEffectName();
};
}

CCharString NCreatureMode::CModeBase::GetEffectName()
{
    return CCharString("DoCreatureAction", -1);
}
