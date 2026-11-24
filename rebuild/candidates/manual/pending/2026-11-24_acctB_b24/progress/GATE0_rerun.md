# Gate 0 rerun of the object-order build mode -- B24A9, 2026-11-24: FAIL (built this time; stock c2 does not reproduce retail's calls)
Scratch only (`$S/w22/b24a/gate0r/`), stock cl 13.10.3077, BASE_FLAGS `/O2 /Oy /W3`, no gdb hook, no patched c2, no caller-growth bump,
no forcing device. Same oracle (auto-re-candidates 0x00E93EB0, 5321 bytes), same grader core (`verify_and_land.parity_of`, relocation
masking) as fg.py / q.py. Composer: `gate0r/compose.py` (read it: every rule below is code there); runner `gate0r/run_tu2.sh`;
built TUs `gate0r/tu_<tag>.cpp`, logs `gate0r/tu_<tag>.log`, objdumps `gate0r/w/<tag>/objdump.txt`.

## Answer
Built: yes (first time this object composes into one TU). Observed with stock c2 in the composed TU, retail address order:
| call in ?Main@CV_HauntedHouseScript | retail | composed TU | Main alone |
|---|---|---|---|
| TRetVoid::Return 0x00E95380 (final check, 0x00E9524E) | 1 | **0** | 0 |
| vector<CScriptThing>::operator[] 0x00CD1FB1 (last candle loop, 0x00E9520A) | 1 | **0** | 0 |
| ~vector<CScriptThing> 0x008AC970 | 1 | 1 | 1 |
| ~CScriptThing 0x004AA840 | 6 | 5 | 5 |
Main in the TU = **DIFFER(5193v5321)**, and its bytes are IDENTICAL to Main compiled alone (5193). Grouping changed nothing for Main:
retail's two odd calls do not appear. The approved option still has no working form.
Why address order cannot help here: both callees are HEADER-INLINE bodies -- `TRetVoid::Return` is `void Return() { }` in
engine/TRetVoid.h:9 (the 0x00E95380 row only emits it, through its scaffolding helper), and vector<CScriptThing>::operator[] is the
STLport header template (no row). They are therefore defined before Main in EVERY arrangement of the rows; reordering rows only moves
the emitting row. Observed emission order in the TU object (`w/clean/objdump.txt`): `?Return@TRetVoid@@QAEXXZ` COMDAT is the first
function emitted (line 25), CheckRegionLeaving at 672, Main at 2049 -- retail has Main (0x00E93EB0) before Return (0x00E95380).

## What was built
- Mates: landed rows of the retail object 0x00E93D50..0x00E96E10 under `rebuild/src/compiled/00/e9` at HEAD 18aba79bd3 (after nf336).
- Main: `gate0r/main_clean.cpp` = `$S/w22/b24a/00e93eb0/v1.cpp` with the Codex pointer-to-member Return REMOVED (final checks are
  `if (IsActiveThreadTerminating()) return _Fail_Val.Return();`) AND v1's `#pragma inline_depth(0)` / `inline_depth()` pair removed
  (it forces one Return site to a call: a forcing device, not allowed here). For comparison with the first attempt, `main_v1noptm.cpp`
  (pragma kept) was also built: DIFFER(5145v5321) in the TU = the same number as alone in GATE0.md; still 0 Return / 0 operator[] calls.
- One header configuration for the object, fixed before grading: `gate0r/C1.txt` = the mates' common FABLE_RETAIL_* set (the 12 of
  0x00E93D80) + Main's three extras (CCOUNTEDPOINTER_RELEASEDATA, CSCRIPTTHING_ORIGINAL_ACCESS, BASE_OBJECT), defined once at the top;
  each part's own `#define FABLE_RETAIL_*` lines removed. Check (`compat_C1.txt`): every included row graded alone with C1 instead of
  its own set gives the SAME function bytes as its own set (Main included). The mates' set alone (`compat_Cmaj.txt`) changes Main
  (5193 -> 5171), so Main's three extras are needed. Note: RELEASEDATA does change the mates' pick-any CCountedPointer copies (adds a
  ReleaseData COMDAT): a COMDAT-set conflict between Main's draft and the mates, recorded here, not their own function bytes.
- Duplicates: 11 textually identical definitions dropped (kept the first): the nested-class ctor + Alloc of CM_QuestControl,
  CM_UndeadHere, CM_HeroInsideHouse, CHHUndead defined again in their `??_G` rows (0x00E95670 / 0x00E95750 / 0x00E95CB0 / 0x00E95E60),
  and 0x00E95770's inline `CScriptThing::IsDead / CScriptThing() / ~CScriptThing()` (identical to Main's). No non-identical duplicate.
- Pragmas: none left in the included parts (0x00E96110's balanced inline_depth pairs are out with that row).
- Included (17, address order): 0x00E93EB0 (Main), 0x00E95380 TRetVoid::Return, 0x00E953D0, 0x00E953E0, 0x00E955B0, 0x00E95670,
  0x00E95690, 0x00E95750, 0x00E95770, 0x00E95BF0, 0x00E95CB0, 0x00E95CD0, 0x00E95D80, 0x00E95DC0, 0x00E95E60, 0x00E95E80, 0x00E96090.

## Left out, and why (each is a conflict = ERROR per the Safeguards, not merged)
- 0x00E93D50, 0x00E93E60, 0x00E96DD0 v 0x00E96E10: opposite `~CLevelScriptBase<T>` models (FABLE_RETAIL_CLEVELSCRIPTBASE unset v set).
- 0x00E95F10 v 0x00E95F80: opposite `~CSpawnedFuncBase` models (DTOR_DECL_ONLY); 0x00E95F80 also sets three CCountedPointer forms.
- 0x00E96C30: still a local STLport vector / CCharString / CV_HauntedHouseScript model (nf336 Q1).
- 0x00E96660 (13 local classes incl. CCharString / CScriptThing), 0x00E95390 (local CCharString; CQ_EndGameFocalSites binding),
  0x00E960E0 (local CSpawnedFuncBase): local models colliding with the shared headers (not in nf336's set).
- gapscan rows 0x00E95650 / 660 / 730 / 740 / C90 / CA0 / D70: stand-in `struct T` models (two of them collide with each other) and
  `#pragma pack` state.
- 0x00E96110 CM_QuestControl::Main: its 19-define set differs from C1 (STLP map/tree DECL_ONLY, CDEFSTRINGTABLE, MovieBase forms, and
  it lacks CLEVELSCRIPTBASE); with C1 in place of its set it is SRC_FAIL. C1 plus its own set (a union) was NOT tried: left out, untested.
- 0x00E93D80, 0x00E93DA0, 0x00E93E30, 0x00E93E40 (the mates BEFORE Main): Main's draft declares
  `template<> CSpawnedFunc<CV_HauntedHouseScript>::CSpawnedFunc(...)` and `template<> CEntityScriptBinding<...>::CEntityScriptBinding(...)`
  (decl-only explicit specialisations: a per-row model keeping the ctors out of line), while mates before it instantiate the header's
  template body first: C2908 "explicit specialization ... has already been instantiated". Dropping 0x00E93DA0 alone still fails;
  which of DA0 / E30 / E40 triggers it was not isolated. 0x00E93D80 Init does NOT conflict: variant `keepinit` (D80 kept, 18 parts)
  builds, Main identical (5193); the `clean` build dropped D80 without a demonstrated conflict.

## Controls (same grader)
- Permuted order (parts reversed, Main last): Main DIFFER(5193v5321), bytes identical to address order -> row order has no effect on
  Main here (expected: the callees in question are header-inline, see above).
  As a negative control it grades DIFFER as required; it cannot distinguish orders because no order changes the code.
- Wrong body (last candle loop `EntitySetLightAsOn(Candles[i], true)`): DIFFER(4921v5321). Required DIFFER: yes.

## Regression control (landed rows in the TU v alone)
Coverage: 6 of 16 included mates measured by an unambiguous symbol. SAME: GetEntityScriptName, CHHUndead::Init, OnPredicateFail,
0x00E96090 SuspendableProcess; 0x00E95380 "SAME" compared the 6-byte scaffolding helper picked by leaf `Return` (graded 6v1 alone),
not the Return body itself.
**0x00E953E0 `?CheckRegionLeaving@CV_HauntedHouseScript@NScript@@QAEXXZ`: 457 bytes alone (landed RELOCATION_MATCH) -> 597 in the
TU** -- a finding, not patched; the cause is NOT isolated (Main's draft text -- local inline CScriptThing members, specialisations --
or another mate visible before it). The ambiguous leaves (`Alloc`, `Main`, `??_G`) were not resolved by the substring extractor; their "CHANGED" lines in
the logs are leaf-ambiguity artefacts, not measurements.

## Reading
Gate 0 fails on the substance this time, not on composability: with 17 landed mates + Return in one TU, stock c2 still inlines
Return and operator[] into Main exactly as alone. Retail's calls need Main compiled before its callees, and a TU's definition order
does not give that. Do not build the mode on this form; a real working form would have to change c2's processing order without a
forcing device, which nobody has shown. Open: Main's own draft carries per-row devices (decl-only specialisations, local inline
CScriptThing bodies) that a real object build would also have to settle.
