// CTCTargetingSpirit::vfunc_22() -- return a default-constructed 0x004D5853 object (hidden result pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEC09, length 16 bytes (no padding: slot 8 follows at 0x004DEC19).
// Vtable 0x012419A4 (CTCTargetingSpirit, rtti-vtables.tsv) slot 22; part of the six-body catalogue row 004debf7
// (skipped ledger, chunk 903). Callee 0x004D5853: a thiscall with no arguments on the hidden result -- the default
// constructor of the returned type (its manifest label CActionDoCreatureAction::GetActionName is propagated).
// A size-optimised TU (#pragma optimize("s", on)): the result pointer is re-read from its slot after the call instead of
// being parked in esi.
// Shape: `mov ecx,[esp+4] ; call 0x4D5853 ; mov eax,[esp+4] ; ret 4` -- the result constructed in place and its
// address returned.
#pragma optimize("s", on)

struct CTargetingResult_Retail {
    long m_value;
    CTargetingResult_Retail();                 // 0x004D5853
};

struct CTCTargetingSpirit_Calls {              // vtable 0x012419A4
    CTargetingResult_Retail vfunc_22();
};

CTargetingResult_Retail CTCTargetingSpirit_Calls::vfunc_22()
{
    return CTargetingResult_Retail();
}
