// NTCScriptedControl::CActionMoveTo::GetActionName() -- the scripted action's name for logging: "CActionMoveTo"
// Leaf: ?GetActionName@CActionMoveTo@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/7e/CActionDoCreatureAction_GetActionName_007e7280.cpp: vtable slot 4 of NTCScriptedControl::CActionMoveTo (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionMoveTo"
// (was: NTCScriptedControl::CActionMoveTo::GetActionName @ 0x007e7280)
// Returns a CCharString constructed from a fixed string literal.

class CCharString {
public:
    CCharString(const char* s, long n);
    void* p;
};

namespace NTCScriptedControl {
class CActionMoveTo {
public:
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionMoveTo::GetActionName()
{
    return CCharString((const char*)0x01270934, -1);
}
