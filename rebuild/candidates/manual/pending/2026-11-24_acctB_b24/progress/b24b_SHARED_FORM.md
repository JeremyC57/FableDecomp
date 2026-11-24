# CTimer shared form (B24B3, 2026-11-24) -- one text for every quest-script row

## Result: the CURRENT landed CTimer text is FableWin-exact for all four bodies. No new opt-in is needed.
```
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}          // (= `{ TimerIndex = pDSTGame->RegisterTimer(); }` at /Od)
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }   // FORCEINLINE where a row needs it
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime() == time; }
```

## Evidence (exact, not sizes): FableWin's Ghidra instruction-signature HASHES
`ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv` column strict_signature = sha256(canonical)[:16] of
`tools/ghidra_scripts/ExportInstructionSignatures.java`. I re-implemented the canonical form and calibrated it on FableWin bodies
(empty /Od member 40da775125258918; unwind funclets `lea ecx,[ebp-x]; jmp`, `mov eax,[ebp-x]; push eax; call; pop ecx; ret`):
register `200:R:<reg>,`; [ebp+x] inside a framed function and absolute globals = `DATA` (stack / data references); direct
call/jmp/jcc = `FLOW`; `ret n` = `4000:S16:n,`; other memory `400000:R:<base>,[S32:<disp>,]` (no scalar when disp is 0); Ghidra
mnemonics (sete -> setz, je -> jz). Tool: `$S/w22/b24b/00f1eed0/gsig/ghash.py <obj> <regex>` (compile at /Od first, e.g. with
fwsize.py; probe TU builder `$S/w22/b24b/00f1eed0/ct3/mk.sh <tag> <bodies.txt>`).

Hashes of the text above compiled by VC7.1 /Od, with each `call dword ptr [vt+X]` rewritten as `mov eax,[vt+X]; call eax`
(`edx` when vt is eax): ctor 10a72533ff24d802, dtor 76f99ac8cda23ac6, operator= 64e3f8b767f90f67, GetTime caf303759493c892 --
ALL FOUR EQUAL FableWin's (same vtable offsets 0x15C/0x160/0x164/0x168, same registers).

So fwsize's "+1 instruction / +2 bytes per body" is FableWin's COMPILER (VC10 /Od splits every virtual call into
`mov reg,[vtbl+X]; call reg`: +1 instruction, 6+2 v 6 bytes), not a different text. operator== likewise: FableWin's
`xor ecx,ecx; cmp eax,[ebp+8]; sete cl; mov al,cl` (cc133e69e4b2141a, hash-verified by hand) is VC10's idiom for
`return GetTime() == time;` (VC7.1 /Od emits sub/neg/sbb/inc, 29 b).

Not FableWin's text (no donor evidence for them; FableWin is a 2012 revision built with VC10, GOTCHAS 2026-11-19, so this is
evidence for structure, not proof of retail's text -- do NOT respell landed rows that byte-match retail): `CGameScriptInterfaceBase* game = pDSTGame; game->DeregisterTimer(TimerIndex);` (B24B2 d1/d1b,
16i), `long index = TimerIndex; ...` (d2/d2b), `const long& index = TimerIndex; ...` in operator= / GetTime (6 + 2 landed rows use
that ref-local spelling: FableWin says the plain text). The ctor's init-list and body-assignment spellings give identical /Od bytes
(both hash-match); keep the 119-row init-list spelling.

## Effect on rows
- ~CTimer stays sz 14ish (c1 14 at early sites, 25 at late sites with pen 11): the ~CTimer inline cost is NOT a header-text lever.
  The cold-return `inline_depth(0)` regions (0x00F1EED0) are not removed by any CTimer text; they remain an Inline-budget device
  (or the cost difference sits on the other side: ~MovieBase / GROBase, B24A's SHARED_FORM).
- Retail side agrees: B24B2's pick-any check found the emitted CTimer copies equal to Fable.exe.
- Lane lesson: before reading an fwsize delta as text evidence, subtract VC10 /Od idioms: +1i/+2b per virtual call (split call),
  +1b per byte parameter load (VC10 `movzx r32,byte` v VC7.1 `mov r8,byte`; CRGBColour(EEE) FableWin +3b = 3 byte loads, hash-
  verified `Initialise(r,g,b,255)` = our text), `==` returned as bool (VC10 xor/cmp/setz/mov v VC7.1 sub/neg/sbb/inc, +2b).
  Then verify with ghash.py (it tries split / movzx variants).
- ghash.py validation (B24B3): MATCH on 9 non-trivial bodies (4 CTimer, CRGBColour(EEE), Initialise, ~CScriptGameResourceObjectBase,
  CScriptGameResourceObjectBase(), CCountedPointer<CScriptThing> ctor/dtor, ??_GCScriptThing, TRetVoid::Return). ONLY A MATCH IS
  EVIDENCE; "no" means unknown: tokens NOT calibrated: x87 operands, fs:[0] / EH frames (C3DVector(MMM), CScriptThing() are size-
  SAME but print "no"), [esp+x], index/scale operands, [reg+global], other VC10 idioms.

## For B24A (family inequality), from row 0x00D0C410 (B24B3)
Original ~CTimer text measures c1 14 (sz 20, pen 0 at every site of CBanditForger::Main), NOT >= 20, so "S15 + ~CTimer c1 20..23" is closed.
Stock + plain inline ~CTimer reproduces retail's ~CTimer exit map exactly (expanded at f .2283 / .0408, called at f <= .0278); the only
wrong thing at stock is ~MovieBase called at cold exits. Two observations, cause OPEN (do not chase a single "c1 <= 13" form): in 0x00D0C410 retail expands ~MovieBase even at f .0003
(as if always-inline), but in 0x00DC93F0 retail CALLS ~MovieBase 0x006E7B80 at the helmet-arm exit where our trace gives f .1159
(our stock rule expands it there). No single MovieBase cost or threshold fits both rows; the frequencies themselves may differ
(all quest scripts and the CTimer bodies are in ONE object, register_scripts.obj, 6,473 functions in egor_obj_functions.tsv:
an untested lead). A non-`inline` auto-inline ~CTimer definition behaves the same as `inline`.

## Row 0x00DB3FA0 (RELOCATION_MATCH staged, HELD): one-level predicate-destructor expansion -- NOT FOR REUSE until the NEQ copy is ruled on
Where retail expands a predicate temporary's destructor (CIsThingConscious / CIsThingAlive family: vtable store) but CALLS the nested
~CScriptThing 0x004AA840, put `#pragma inline_depth(0)` / `#pragma inline_depth()` round the FABLE_FORCEINLINE dtor DEFINITION
(`FABLE_FORCEINLINE NScript::CIsThingConscious::~CIsThingConscious() {}`). Not round the call statement (that blocks the forced dtor).
Side effect: the emitted ??1 copy (virtual, out of line) then calls ~CScriptThing and differs from retail's 0x00EF7AB0, which has its own
landed row (pre-device copy EQ) -> a NEW differing pick-any copy, held under EXTRA_RULES 2026-11-20 until the lander rules.
Also: `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (declared-only) makes the emitted
??1CScriptGameResourceObjectMovieBase copy NEQ to retail 0x006E7B80 (leaf-referenced in rows that call it): check whether the visible
header template keeps the row matched (it did here) before handing back.

## Unchecked
CWideScreenMagicPauseEntities ctor/dtor still differ from FableWin after the split-call correction (ours 18i/15i v FableWin 17i/13i);
never hash-checked (FableWin is a 2012 revision; the class may differ there).
