# 00d3a4a0 (p25A, Account B, 2026-11-20): NScript::CQ_FireHeartScript::CFH_Prisoner::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00D3A4A0 | NScript::CQ_FireHeartScript::CFH_Prisoner::Main() | 1140 | RELOCATION_MATCH (no flags) | PASS (330 targeted + 6,000 random runs) | 43 / 43 killed |

Strict vland: `RELOCATION_MATCH PASS`. Two warnings: RELOC_ALIAS `_rand` -> the MSVCR71 import thunk (expected), and RELOC_NAME
0x00D3ACE0 labelled CFishCompOrganiser::SpeakConversationLine. That label is wrong; see NAME_FIXES.tsv (MakeComment). Purity: plain C++.
Grades: 3 (the port, plus the CSE fix, plus the commented final). The 2026-11-07/08 drafts took 87 grades.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 961..1022) and locals (`_Fail_Val`; `CutsceneName`, `iAnim` in the killed branch). Each Ego_d
line's size was checked against a VC7.1 /Od compile of the statement set (tools/odlines.py), and one source line is written per Ego_d line.
963 holds the predicate, frame and terminate test. 965 is the scripting start (release if valid, the wait, the terminate test). 966 is
ProphetsScripted++. Each loop or branch head carries its terminate test on its own line. This is the generated quest-script text of the landed Mains.
- `// Fidelity:` CScriptGameResourceObjectScriptedThingBase::IsValid's real body, `inline __declspec(noinline)` in the TU. This is the
  landed text of 0x00EB3BA0 (GOTCHAS chunk Q32). c2 sees that IsValid stores nothing, so it computes &seh_Prophet[MyCageNumber] once for
  IsValid and Release (`lea edi,[edx+ecx]`, 0x00D3A506) and recomputes it for StartScriptingEntity, as retail does. With the declaration
  only, the address is recomputed (1145v1140). The drafts' named reference local (Ego_d.pdb has no such local) colours it `mov edi,[esi+20h] ; shl edi,4 ; add edi,edx` (1140v1140).
  The TU's IsValid COMDAT is the /O2 spelling (11 bytes). Retail's linked copy 0x00CD23B9 is a size-optimised 9-byte body (the landed
  row 0x00CD23B9). The same holds for the landed 0x00EB3BA0.
- `// Inferred:` the rand() divisor (`long iAnim=rand()%2;`). The value is dead: retail keeps only the call at 0x00D3A75E. Ego_d line 998
  is 20 bytes, the /Od size of `rand()%2^k`; an odd divisor gives 16.

## Header overlay (hdr/, hdr.diff; additive. The only `-` line is the existing size assert respelled FABLE_STATIC_ASSERT_AT by pin_asserts.py)
- engine/CQ_FireHeartScript.h: NEW opt-in `FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER`. It adds UpdateHistory(bool) 0x00D3A920 and
  CheckCompletion() 0x00D3AD60 to the class. After the class it defines `class CQ_FireHeartScript::CFH_Prisoner : CEntityScriptBase`:
  Alloc, GetParentScript, private Main / Init / OnPredicateFail / MakeRoundStartComment / MakeComment / GetEndCutscene /
  GetEndSpeech, plus the Ego_r members ParentClass, PMasterData, Chosen, MyCageNumber. sizeof 0x24 is asserted. Vtable 0x012CCF10.
- Regrade with the overlay: all 7 landed includers stay RELOCATION_MATCH (0x00D347B0, 0x00D38F70, 0x00D35090, 0x00D368D0, 0x00D38D20, 0x00D39330, 0x00D39960).
- No new header file. Re-port backlog: the landed CFH_Prisoner rows still on local models with under-qualified names (Alloc 0x00D36540,
  Init 0x00D36620, GetParentScript 0x00D365E0, OnPredicateFail 0x00D365F0, MakeRoundStartComment 0x00D3A9B0, GetEndCutscene 0x00D3AB00,
  GetEndSpeech 0x00D3ABE0, MakeComment 0x00D3ACE0 = "CFishCompOrganiser::SpeakConversationLine").

## Test (test/00d3a4a0.cpp)
The test links the real row object.
- Fixtures: raw prisoner and parent objects; csargs is a real two-node STLport tree ("$CAMERA", "$LINE"), so the TU's own
  operator[] lookup runs; byte vectors for the released / killed / speak flags; a 16-byte handle vector; a fake Game vtable (slots 7, 8,
  108, 156, 255); a test CCharString with real concatenation and int-to-string.
- Scripted inputs: the world changes the round and the cage's flags at every NewScriptFrame. Termination, StartScriptingEntity results,
  handle validity and operator new failure are scripted too.
- Checks: the event order against an independent model, then the parent's counters and flags, the csargs strings, the cutscene names,
  UpdateHistory / morality arguments, the predicate's thing copy and its destruction, the spawned thread (new(0x3C),
  CSpawnedFuncBase("ParentClass.CheckCompletion", 0), PClass, group "", slot 4 runs CheckCompletion and sets StopRunning) and string balance.

## Mutants (mutants/)
43 one-decision mutants, 43 killed. One was rerun at occurrence 1 because its text occurs once.
