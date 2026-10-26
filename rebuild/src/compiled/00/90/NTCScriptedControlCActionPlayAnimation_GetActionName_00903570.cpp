// NTCScriptedControl::CActionPlayAnimation::GetActionName() -- the scripted action's name for logging: "CActionPlayAnimation"
// Leaf: ?GetActionName@CActionPlayAnimation@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00903570.cpp: vtable slot 4 of NTCScriptedControl::CActionPlayAnimation (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionPlayAnimation"
// (was: NTCScriptedControl::CActionPlayAnimation::GetActionName  @ 0x00903570)
// Returns a CCharString by value, constructed from a fixed string literal.
// The 'this' (CActionPlayAnimation*) is passed in ecx but unused; the
// hidden struct-return buffer is the sole cleaned stack arg (ret 4).

class CCharString { public:
    CCharString(const char* s, int n);
};

namespace NTCScriptedControl {
struct CActionPlayAnimation {
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionPlayAnimation::GetActionName()
{
    return CCharString((const char*)0x128d70c, -1);
}
