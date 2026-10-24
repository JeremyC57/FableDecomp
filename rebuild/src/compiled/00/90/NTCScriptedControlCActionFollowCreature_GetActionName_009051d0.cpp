// NTCScriptedControl::CActionFollowCreature::GetActionName() -- the scripted action's name for logging: "CActionFollowCreature"
// Leaf: ?GetActionName@CActionFollowCreature@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_009051d0.cpp: vtable slot 4 of NTCScriptedControl::CActionFollowCreature (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionFollowCreature"
// (was: NTCScriptedControl::CActionFollowCreature::GetActionName  @ 0x009051d0)
// Returns a CCharString by value, constructed from a fixed string literal
// with length -1 (NUL-terminated). 'this' is unused.

class CCharString {
public:
    CCharString(const char* s, int len);
private:
    char* m_data;
};

// The literal at 0x128d850 (content irrelevant to codegen shape).
extern const char kActionName_0128d850[];

namespace NTCScriptedControl {
class CActionFollowCreature {
public:
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionFollowCreature::GetActionName()
{
    return CCharString(kActionName_0128d850, -1);
}
