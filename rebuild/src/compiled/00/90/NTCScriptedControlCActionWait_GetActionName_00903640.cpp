// NTCScriptedControl::CActionWait::GetActionName() -- the scripted action's name for logging: "CActionWait"
// Leaf: ?GetActionName@CActionWait@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00903640.cpp: vtable slot 4 of NTCScriptedControl::CActionWait (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionWait"
// (was: NTCScriptedControl::CActionWait::GetActionName  @ 0x00903640)
// Returns a CCharString by value: constructs it from a fixed literal + (-1).
// __fastcall struct-return: the hidden return buffer is a stack arg cleaned by ret 4.

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};

namespace NTCScriptedControl {
struct CActionWait {
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionWait::GetActionName()
{
    return CCharString("CACTION_DO_CREATURE_ACTION", -1);
}
