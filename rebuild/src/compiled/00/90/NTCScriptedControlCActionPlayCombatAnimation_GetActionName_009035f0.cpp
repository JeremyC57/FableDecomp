// NTCScriptedControl::CActionPlayCombatAnimation::GetActionName() -- the scripted action's name for logging: "CActionPlayCombatAnimation"
// Leaf: ?GetActionName@CActionPlayCombatAnimation@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_009035f0.cpp: vtable slot 4 of NTCScriptedControl::CActionPlayCombatAnimation (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionPlayCombatAnimation"
// (was: NTCScriptedControl::CActionPlayCombatAnimation::GetActionName  (retail 0x009035f0))
// __fastcall, returns CCharString by value (hidden retbuf as first stack arg).
// Body: return CCharString("...", -1);  -- 'this' unused.

class CCharString {
public:
    CCharString(const char *s, long n);   // 0x0099ebf0, out-of-line ctor
private:
    char *m_p;
};

namespace NTCScriptedControl {
class CActionPlayCombatAnimation {
public:
    CCharString GetActionName();
    // some data so 'this' is a real object (unused by the body)
    int m_dummy;
};
}

CCharString NTCScriptedControl::CActionPlayCombatAnimation::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}
