// NTCScriptedControl::CActionTalkedTo::GetActionName() -- the scripted action's name for logging: "CActionTalkedTo"
// Leaf: ?GetActionName@CActionTalkedTo@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/71/CActionDoCreatureAction_GetActionName_00718d90.cpp: vtable slot 4 of NTCScriptedControl::CActionTalkedTo (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionTalkedTo"
class CCharString { public:
    // real member ctor => this in ECX, (lit,-1) pushed, callee-cleaned.
    CCharString(const char* lit, int len);
    int dummy;
};

namespace NTCScriptedControl {
struct CActionTalkedTo {
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionTalkedTo::GetActionName()
{
    return CCharString((const char*)0x1265824, -1);
}
