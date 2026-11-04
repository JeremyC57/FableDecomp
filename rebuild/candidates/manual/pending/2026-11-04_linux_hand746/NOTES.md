# Chunk 746 -- CV_MurderTwistFinalScript sheriff / sheriff's-guard dialogue methods

## Results
| Address | Symbol | Grade | Grades | Notes |
|---|---|---|---|---|
| 0x00EB0830 | `?DoRemindingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ` | RELOCATION_MATCH | 1 | src + test, vland PASS |
| 0x00EB2900 | `?DoRemindingHeroToKillAssassin@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ` | RELOCATION_MATCH | 2 | catalog said DoAskingHeroToSpeakToBossAgain |
| 0x00EB2DC0 | `?DoAskingHeroToSpeakToBossAgain@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ` | RELOCATION_MATCH | 1 | |
| 0x00EB3280 | `?DoCongratulatingHero@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ` | RELOCATION_MATCH | 1 | catalog said DoAskingHeroToSpeakToBossAgain |
| 0x00EAF810 | `?DoAskingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ` | DIFFER(3021v2945) | 13 | parked, differ/ (dl=99) |

vland (read-only): 4/4 RELOCATION_MATCH + test PASS (only RELOC_NAME label-text warnings for `??BCCharString@@QBEPBDXZ`).

## Identity of the three 1203-byte "DoAskingHeroToSpeakToBossAgain" rows
They are three different methods of NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard (private, non-virtual, AAE; no
vtable slot -- the entity vtable 0x012EFEE8 holds only dtor / Main 0x00EB2140 / Init 0x00EB2070 / GetParentScript 0x00EB2060 / base slots):
- Ego_r.pdb and FableWin.pdb list the guard's members in the order DoAskingHeroToSpeakToBoss, DoRemindingHeroToKillAssassin,
  DoAskingHeroToSpeakToBossAgain, DoCongratulatingHero, DoSayingThankYouToHero (FableWin 0x00F1D0C0.. / Ego_r 0x03A5E6D0..);
  retail has five bodies in that order: 0x00EB23E0 (landed), 0x00EB2900, 0x00EB2DC0, 0x00EB3280, 0x00EB3740 (landed).
- The guard Main 0x00EB2140 (vtable 0x012EFEE8 slot 1) calls them in the same order (0x00EB2269, 0x00EB22B4, 0x00EB22FE,
  0x00EB2345, 0x00EB2385), each in a state loop.
- Texts: TASK_ACCEPTED (+_ATTACKED) = reminding the hero who accepted the task; TASK_INCOMPLETE = asking him back to the boss;
  ASSASSIN_DEFEATED = congratulating. Byte-identical apart from the two literals; different functions (no ICF).
The sheriff rows: donor member order DoAskingHeroToKillAssassin / DoRemindingHeroToKillAssassin / DoBecomingSuspiciousOfHero /
DoGivingRewardToHero / DoSayingWellDoneToHero / AttackHero = retail 0x00EAF810 / 0x00EB0830 / 0x00EB10C0 / 0x00EB15C0 / 0x00EB19C0 /
0x00EB1EC0; the sheriff Main 0x00EAEF30 (vtable 0x012EFA0C slot 1) calls the first two at 0x00EAF039 / 0x00EAF087 (and its landed
source already binds those two spellings).

## Model (no devices)
- New overlay header `hdr/engine/CV_MurderTwistFinalScript.h` (no repo header existed; hdrcheck2: `NEW engine/CV_MurderTwistFinalScript.h`):
  the level script (retail 0x88: SheriffState +0x48, AssassinState +0x4C, TaskDeclined +0x50, PreemptiveAttackInProgress +0x51,
  Sheriff +0x54, Guards +0x60, seh_Sheriff +0x6C, seh_Guards +0x7C) with nested CMurderTwistSheriff (0x38) and
  CMurderTwistSheriffGuard (0x38), members / access from Ego_r.pdb. Enumerator names of ESheriffState / EAssassinState are not in
  either PDB dump: descriptive SHERIFF_STATE_n / ASSASSIN_STATE_n. The assassin / assassin-guard classes are not modelled yet.
- True NScript model: shared CEntityScriptBase / ScriptedThingBase / MovieBase (implicit ctor/dtor) /
  CWideScreenMagicPauseEntities RAII / TRetVoid; `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();`
  keeps the PImp release out of line, and VC7.1 then reproduces retail's per-site MovieBase ctor/dtor split by itself (expanded at
  the hot exits, 0x006E7B80 / 0x006E7B60 at the others). No `#pragma inline_depth`, no two-view classes, no goto/`if (0)` exits.
- `return _Fail_Val.Return();` (TRetVoid) vs plain `return;`: identical bytes for the four matched rows; for 0x00EAF810 only the
  TRetVoid form puts the merged first-check exit block where retail has it (at the intro copy, the fickle arm jumping back). All five
  sources use it.
- No asm and no helper calls in any row (no rounding sites).

## Parked: 0x00EAF810 DoAskingHeroToKillAssassin (DIFFER 3021v2945, dl=99)
Residue: one extra `mov [esp+20h],ebx` after the talk block's `PauseAllNonScriptedEntities(true)`: VC7.1 homes the pause object's
Game pointer and, in the "declined" arm's Speak, keeps `Game` (for GetHero across the `operator const char*` call) in ebp and the
Speak vtable in ebx, reloading ebx afterwards; retail keeps the pause pointer in ebx throughout and parks `Game` in a frame temp.
The home slot shifts every later temporary 4 bytes (hit block too) and splits the declined arm's exits. Everything else matches.
Tried (13 grades, ~30 compile probes): plain vs TRetVoid returns (fixed the exit placement), swapped TaskDeclined arms, four
answer-wait loop forms (incl. `long answer;` before the question as the landed YNQAnswer rows), switch on the answer,
`answer != 1`, do-while(0) terminate checks (all / block-initial / loop), bool as const / unsigned char / `== false` / reversed
ternary / function scope, plain-inline pause ctor, CScriptThing implicit-copy opt-in, `const char*` line locals.
Diagnostics: removing either the accepted or the declined ternary, or the whole hit block, removes the spill (no single hit-block
piece does) -- a global allocator tie-break, not a local shape.
Next idea: the function-local records of this function in Ego_r.pdb / FableWin.pdb (owner's machine): a scope or declaration
difference of the accepted / declined texts, the answer or the distrust bool is the likeliest lever.

## Header debt
- engine/CV_MurderTwistFinalScript.h is new (overlay); the landed MurderTwist final-script rows (0x00EB23E0, 0x00EB3740, 0x00EB10C0,
  0x00EB15C0, 0x00EB19C0, 0x00EB4F80, ...) still carry local models of the class and could migrate to it.

## Callee names (all as bound in reloc_bindings / PDB)
CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CV_MurderTwistFinalScript::HasHeroEnflamedEntity 0x00EB07D0 /
WaitForEnflameReactionIfNecessary 0x00EB0700 / DoesAssassinTrustHero 0x00EB03A0 (NScript spellings); GFIntToCharString 0x0099F830;
`operator+(const char*, const CCharString&)` 0x0099F690; `operator+(const CCharString&, const CCharString&)` 0x0099F570;
GetHeroTitleAsLHTSTag 0x00CBEBA7; IsDistanceBetweenThingsUnder 0x00CBE2FF; pDSTGame 0x0143E8F8 (CTimer GetTime / operator= inline).

## Name fixes (NAME_FIXES.tsv)
Catalog labels of 0x00EB2900 / 0x00EB3280 (propagated DoAskingHeroToSpeakToBossAgain) and the class nesting of all five rows; the
landed 0x00EB23E0 / 0x00EB3740 / 0x00EB4F80 rows are defined on global `struct` stand-ins (wrong symbols, should be the NScript-nested
private AAE members); reloc_bindings carries non-NScript spellings for 0x00EB07D0 and 0x00EB0700 next to the right ones.

## GOTCHAS one-liners
- NScript dialogue rows: with the shared MovieBase (implicit dtor) plus `template<> CCountedPointer<MovieBase>::~CCountedPointer();`
  and RAII CWideScreenMagicPauseEntities, VC7.1 reproduces the per-exit inline/out-of-line MovieBase split with plain returns.
- `return _Fail_Val.Return();` (TRetVoid) is not always byte-neutral: it decides which copy of two identical early-return blocks
  survives cross-jumping (0x00EAF810); use it uniformly in NScript entity methods.
- Propagated labels across same-size NScript bodies: name them by donor member order + the caller's call order + text literals.
