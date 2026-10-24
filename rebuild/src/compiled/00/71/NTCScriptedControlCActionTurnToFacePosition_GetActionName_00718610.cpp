// NTCScriptedControl::CActionTurnToFacePosition::GetActionName() -- the scripted action's name for logging: "CActionTurnToFacePosition"
// Leaf: ?GetActionName@CActionTurnToFacePosition@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/71/CActionDoCreatureAction_GetActionName_00718610.cpp: vtable slot 4 of NTCScriptedControl::CActionTurnToFacePosition (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionTurnToFacePosition"
struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

namespace NTCScriptedControl {
struct CActionTurnToFacePosition {
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionTurnToFacePosition::GetActionName() {
    return CCharString((const char*)0x126554c, -1);
}
