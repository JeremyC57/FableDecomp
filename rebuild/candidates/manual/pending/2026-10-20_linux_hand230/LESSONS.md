# Chunk 230 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- LTCG-convention callees (first two args in ecx/edx, the rest on the stack, callee-clean, hidden return pointer in ecx when present) reproduce exactly when the stand-in callee is declared `__fastcall` -> 0080d750 (remove_if 0x0080E2F0); also right for 006a2540 (sort 0x006A3F20) and 007ceb00 (__copy 0x007CE1F0).
- An out-param `GetSubDef(CDefPointer&)` modelled as a same-TU `__declspec(noinline)` body under `#pragma auto_inline(off)` lets VC reuse the dead out word for a later key/temp -> 0073afc0 (and needed, though not sufficient, in 006a2540 / 007ceb00).
- Implicit (compiler-generated) dtor drops the derived vtable reset that a user-written `~T(){}` emits -> 004e160c.
- A by-value inline accessor returning C3DVector with a user copy ctor gives retail's half-FPU temporary (two fld/fstp, one kept on the stack) -> 006701a0; the same trick fixed the x/y loads in 00730f90.
- STLport rb-tree walks in templates: a `static inline _M_increment(node)` helper (not the iterator operator++) reproduces the walk; initialise the best/result to 0 before the loop -> 008128f0, 00812a10.
- Empty tag structs (forward_iterator_tag etc.) passed by reference need a user ctor, otherwise VC zero-fills them with `mov byte [esp+x],0` and spends a slot -> 007ceb00.
- Float members/locals for frame-time words keep VC from materialising a zero register -> 007308f0.
- `optimize("y",off)` + `optimize("s",on)` reproduces frame-pointer size-optimised template bodies (strength-reduced counted outer loop) -> 006610c7.
- Statement-order x87 sum `s = z*z; s += y*y; s += x*x;` matches retail's fld/fmul/fadd order when the expression form reorders.
- Discarding a hidden-pointer return still costs a frame slot; which dead word VC picks depends on local lifetimes (007ceb00 parked on that alone).

Review at landing:
- 00567400 (differ/): byte-identical except the trailing int3 after a noreturn call -- grader gap (same as 00a5b260); land by hand, Leaf `append`. Rows 00567400/00567d60 are STLport string, not CTCElectrocutedEffect.
- 00567d60 catalog row holds two functions (append copy + 13-byte thunk); 007c977c catalog row starts mid-function (entry 0x007C9770).
- Leaf lines on src/: 004e160c `??1CTCDPhysicalObstruction`, 00666090 `??0CThingCreatureBase`, 0079e990 `??1CTCPhysicalShieldSpell`, 008ecac0 `OnParentHealthChanged`, 0092e630 / 006610c7 `FindMostRecentDeed`. On differ/: 00567400 `append`, 00739360 `ResetBlockPercentageChance`, 0076c8a0 `??0CTCEnemy`, 007ceb00 `??0CTCElectrocutedEffect`; Flags `/EHsc` on differ/00c14f90.
- Size-TU pragmas: 004e160c (`optimize("s",on)`), 006610c7 (`optimize("y",off)` + `("s",on)`).
- `__fastcall` stand-ins for LTCG-convention callees in 0080d750 (src) and differ/006a2540, differ/007ceb00, differ/0080c590 -- these are calling-convention models of real retail functions, keep the comment when landing.
- Same-TU noinline stand-in bodies (GetSubDef / GetDef) in 0073afc0 and others: they must stay in the TU (not replaced by the engine header's inline) or the frame words change.
- Labels: 00666090 -> CThingCreatureBase ctor, 0079e990 -> ~CTCPhysicalShieldSpell, 00c14f90 not CTCDiceGame (unidentified no-RTTI class); ??_G 0x004E15F0 labelled CLandscapeBackgroundPatch calls the CTCDPhysicalObstruction dtor -- verify.
- Local base models: rows with an engine header (CTCCombat, CTCDoor, CTCCreatureGroupBoastingCrowd, CTCCreatureOpinionOfHero) use self-contained local layouts per the lane rule; retype with `retype_landed.py` at landing.
