// NCreatureAction::CActionEventCameraShake::GetHook() -- the animation hook name this creature-action event listens for: "RUMBLE"
// Leaf: ?GetHook@CActionEventCameraShake@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_00692190.cpp: vtable slot 2 of NCreatureAction::CActionEventCameraShake; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "RUMBLE" is an animation hook tag; method name inferred from that list (not from a slot dump)
class CCharString { public:
    // real member ctor => __fastcall(->__fastcall): this in ECX, (lit,-1) pushed, callee-cleaned.
    CCharString(const char* lit, int len);
    int dummy;
};

namespace NCreatureAction {
struct CActionEventCameraShake {
    CCharString GetHook();
};
}

CCharString NCreatureAction::CActionEventCameraShake::GetHook()
{
    return CCharString((const char*)0x125c110, -1);
}
