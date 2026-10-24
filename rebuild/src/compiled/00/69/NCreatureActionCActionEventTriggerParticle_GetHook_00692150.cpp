// NCreatureAction::CActionEventTriggerParticle::GetHook() -- the animation hook name this creature-action event listens for: "PARTICLE"
// Leaf: ?GetHook@CActionEventTriggerParticle@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_00692150.cpp: vtable slot 2 of NCreatureAction::CActionEventTriggerParticle; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "PARTICLE" is an animation hook tag; method name inferred from that list (not from a slot dump)
#pragma optimize("g",on)

// Retail: value-returning __fastcall. ecx = CActionEventTriggerParticle* (this, unused);
// hidden return-struct pointer arrives on the stack. Body constructs a CCharString
// from a string literal (0x125c0ec) via ctor 0x99ebf0 and returns it by value.
//   push esi; mov esi,[esp+8]; push -1; push 0x125c0ec; mov ecx,esi;
//   call 0x99ebf0; mov eax,esi; ret 4
// esi = hidden return slot, ecx set to it for the ctor thiscall.
struct CCharString {
    int dummy;
    CCharString(const char* lit, int len);
};

namespace NCreatureAction {
struct CActionEventTriggerParticle {
    CCharString GetHook();
};
}

CCharString NCreatureAction::CActionEventTriggerParticle::GetHook()
{
    return CCharString((const char*)0x125c0ec, -1);
}
