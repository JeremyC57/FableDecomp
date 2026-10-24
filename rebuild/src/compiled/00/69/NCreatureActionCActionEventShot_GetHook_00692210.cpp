// NCreatureAction::CActionEventShot::GetHook() -- the animation hook name this creature-action event listens for: "SHOT"
// Leaf: ?GetHook@CActionEventShot@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_00692210.cpp: vtable slot 2 of NCreatureAction::CActionEventShot; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "SHOT" is an animation hook tag; method name inferred from that list (not from a slot dump)
// (was: NCreatureAction::CActionEventShot::GetHook - retail 0x00692210)
// Returns a fixed CCharString by value (NRV hidden return pointer).

class CCharString {
public:
    CCharString(const char* s, int n);   // extern ctor -> real call (matches call 0x99ebf0)
    char* m_data;
};

namespace NCreatureAction {
class CActionEventShot {
public:
    CCharString GetHook();
    int m_dummy;
};
}

CCharString NCreatureAction::CActionEventShot::GetHook() {
    return CCharString("DO_CREATURE_ACTION", -1);
}
