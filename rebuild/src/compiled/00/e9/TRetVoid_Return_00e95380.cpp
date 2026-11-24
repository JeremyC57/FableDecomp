// TRetVoid::Return() -- the generated-script void return helper: an empty public member, compiled to a bare `ret`
// Leaf: ?Return@TRetVoid@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95380, 1 byte (`ret`), thiscall.
// Identity (two sources): FableWin.pdb ?Return@TRetVoid@@QAEXXZ (TPI fa51: the empty class TRetVoid, sizeof 1, public void Return(),
//   script_language.hpp:23) + the retail call site: its only caller in Fable.exe is NScript::CV_HauntedHouseScript::Main 0x00E93EB0,
//   `lea ecx,[_Fail_Val] ; call 0x00E95380` at 0x00E9524E (the generated `return _Fail_Val.Return();`; acctB B24A5,
//   pending/2026-11-24_acctB_b24/progress/00e93eb0/NAME_FIXES.tsv). Ego_r.pdb (release) has no out-of-line copy. Was the stand-in
//   C00e95380::M (catalog _global::Nop_ret0_00e95380); no landed row binds either spelling (exe census: no landed caller).
// NF336 (2026-11-24): onto engine/TRetVoid.h, whose in-class inline body is the definition; this row emits it as a pick-any COMDAT.
#include "engine/TRetVoid.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline TRetVoid::Return so this object emits it out of line as
//   a pick-any COMDAT (the nf86 / nf88 form); the helper's name is hashed per TU and unreferenced.
void (TRetVoid::*ReferenceReturn())()
{
    return &TRetVoid::Return;
}
}
