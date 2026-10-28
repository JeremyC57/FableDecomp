// NCreatureAction::CActionEventExplosion::GetHook() -- the animation hook name this creature-action event listens for: "EXPLOSION"
// Leaf: ?GetHook@CActionEventExplosion@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_00692250.cpp: vtable slot 2 of NCreatureAction::CActionEventExplosion; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "EXPLOSION" is an animation hook tag; method name inferred from that list (not from a slot dump)
// (was: NCreatureAction::CActionEventExplosion::GetHook @ 00692250)
// Returns a CCharString by value constructed from a literal string pointer.

class CCharString { public:
    int d;
    CCharString(const char* s, long n);
};

namespace NCreatureAction {
struct CActionEventExplosion {
    CCharString GetHook();
};
}

CCharString NCreatureAction::CActionEventExplosion::GetHook()
{
    return CCharString((const char*)0x125c168, -1);
}
