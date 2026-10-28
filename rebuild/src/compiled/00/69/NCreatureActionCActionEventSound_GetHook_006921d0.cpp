// NCreatureAction::CActionEventSound::GetHook() -- the animation hook name this creature-action event listens for: "SOUND"
// Leaf: ?GetHook@CActionEventSound@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_006921d0.cpp: vtable slot 2 of NCreatureAction::CActionEventSound; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "SOUND" is an animation hook tag; method name inferred from that list (not from a slot dump)
class CCharString { public:
    int d;
    CCharString(const char* lit, long len);
};

namespace NCreatureAction {
struct CActionEventSound {
    CCharString GetHook();
};
}

// __fastcall member returning CCharString by value: this in ecx, hidden return
// ptr on stack, ret 4. this is unused.
CCharString NCreatureAction::CActionEventSound::GetHook()
{
    return CCharString((const char*)0x125c130, -1);
}
