// NTCScriptedControl::CActionSummonerLightningAttackTarget::GetActionName() -- the scripted action's name for logging: "CActionSummonerLightningAttackTarget"
// Leaf: ?GetActionName@CActionSummonerLightningAttackTarget@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/71/CActionDoCreatureAction_GetActionName_00718dd0.cpp: vtable slot 4 of NTCScriptedControl::CActionSummonerLightningAttackTarget (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionSummonerLightningAttackTarget"
// (was: NTCScriptedControl::CActionSummonerLightningAttackTarget::GetActionName @ 0x00718dd0)
// __fastcall returning CCharString by value (hidden return ptr).
// Body: CCharString(0x1265884, -1); this (ecx) unused.

class CCharString {
public:
    char* m_p;
    CCharString(const char* s, long n);
};

namespace NTCScriptedControl {
class CActionSummonerLightningAttackTarget {
public:
    int dummy;
    CCharString GetActionName();
};
}

// member => this in ecx, hidden ret ptr on stack => ret 4
CCharString NTCScriptedControl::CActionSummonerLightningAttackTarget::GetActionName()
{
    return CCharString((const char*)0x1265884, -1);
}
