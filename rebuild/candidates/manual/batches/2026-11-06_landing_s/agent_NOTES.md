# Chunk 818 notes

Six rows: one matched (RELOCATION_MATCH), five parked. All three new names were proven from two sources (entity vtable slot 1 + both PDB name
sets). Grades used FABLE_HDR_OVERLAY=hdr/. Every attempt is in STATUS.tsv / ATTEMPTS.log. Tests pass under g++ and VC7.1.

## 0x00EB9980 NScript::CV_MayorsInvitationScript::CLadyGrey::Main: RELOCATION_MATCH (src/00eb9980.cpp, a13)
- Extent: Main is 12753 bytes, up to its `ret` at 0x00EBCB50. The chunk oracle row (12944 bytes) runs on past the int3 padding into
  `CV_MayorsInvitationScript::MonitorParanoidQuest` 0x00EBCB60. That is the spawned-function target Main stores at 0x00EBAFEE; Ego_r.pdb
  and FableWin.pdb both hold `?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ`. Grades and the read-only vland ran on a
  trimmed oracle (tmp/oracle_trim.tsv, 00eb9980 cut to 12753 bytes and ending in `c3`). finish.py's oracle.tsv still carries the
  12944-byte row from chunk818.tsv. **The landing must use the 12753-byte extent.** vland: RELOCATION_MATCH PASS. Its 3 RELOC_NAME
  warnings are placeholder labels of the std::map ctor/dtor COMDATs (LABELS.tsv).
- Shape: the quest stages by WhichQuest (0 intro + quest card, 1 black rose / carrot, 2 house, 3 necklace / Thunder, 4 the deal), then the
  Thunder-duel wait and the ending (marriage offer or rejection). The levers that mattered:
  - the final section in its own block (frame 0x130 -> 0x104, a2);
  - no `if (c) { while (c) }` wrappers (a3);
  - QuestCardGiven inside the WhichQuest==0 block (a10);
  - the intro cut scene in its own block (a11, fixes the ebx flags reload);
  - plain T instead of a do-while(0) split (a12).
  There is no do-while(0) in the final source, so it needs no Inline-split note.
- relpair: 135 string literals in order, every binding the same as reloc_bindings.tsv.

## 0x00E20600 NScript::CQR_EscortTraderScript::CTraderToEscort::Main: parked DIFFER(14494v14510), 681 lines (differ/00e20600.cpp, e9)
- Name proven: slot 1 of vtable 0x012E151C (Alloc 0x00E1E7A0) + both PDB name sets. Header: the new TRADERTOESCORT opt-in (hdr/README.md).
- Residue:
  1. The CIsThingAlive copy-inline blocker in the predicate: retail expands ~CIsThingAlive but calls ~CScriptThing 0x004AA840 inside it, and
     calls ~CIsThingAlive in the CPredicate_And members. Tried beyond GOTCHAS (e2-e4): an inline ~CIsThingAlive with and without an
     inline ~CScriptThing, and an explicit ~CIsThingAlive specialization.
  2. The first return pad: retail homes it at the QuestStage==2 check 0x00E208A4; ours homes it at the first site.
  3. Per-site inline splits:
     - CScriptThing::GetPos is expanded at the TotallyLost exit marker and called at the arrival exit marker. An inline GetPos expands
       both sites (e5-e7); declared-only calls both (e8).
     - IsOpenDoor is expanded at the door-open wait in retail; ours calls it. The while(true) / for(;;)-with-break forms (e10-e11) left it
       unchanged.
     - One more expanded movie / CTimer exit pad, and the pad at 0x00E23167.
- Key spellings:
  - Success chain: T inside each branch plus `else { TT; }`. This reproduces retail's hoisted thread check.
  - START contains its own Release: e1-e8 had a stray RELEASE before START(3).
  - Movie dtor: the declared-only `CCountedPointer<CScriptGameResourceObjectMovieBase>` dtor specialization.
- Callees: GetRandomSpeech 0x00E23F30, ResetTraderRandomSpeechTime 0x00E1FEB0, GetDistanceBetweenThings 0x00CBE512. 0x00E1FEB0 is
  landed under two wrong names (NAME_FIXES.tsv).

## 0x00E64FB0 NScript::CV_ChickenKickingScript::CChickenMaster::Main: parked DIFFER(15210v15210), 16 lines (differ/00e64fb0.cpp, c3)
- Name proven: slot 1 of vtable 0x012E747C (Alloc 0x00E634B0) + both PDB name sets. Headers: the CHICKENMASTER opt-in, plus the
  FORCEINLINE_ASSIGN opt-in on CScriptGameResourceObjectScriptedThingBase. This TU expands `seh_ChickenMaster = seh_me` (0x00E65052), as the
  landed PlayCutscene 0x00E68B20 does.
- Residue is register colouring only, at two sites:
  - the third FadeScreenOut 0x00E6716A: retail uses edx for the vptr, ours eax;
  - the spectator call 0x00E671E0: eax and edx are swapped.
  c4-c10 (block scope, nested ifs, named colour, kept result, do-while(0), braces, 3-argument colour ctor) are all unchanged.

## 0x00E2C100 CAS_Trickster::Main: parked DIFFER(8945v8945), 2 lines (retry; differ/00e2c100.cpp = chunk 816 source)
- The only residue is the push/mov order in the last PauseMyEntitiesPlease exit pad 0x00E2E38E (esp+0xf0 movie, the HELP_GOOD / EVIL scene).
- 12 new attempts:
  - t1-t10 unchanged: hand-expanded TS_SAY arms with TRetVoid / do-while(0) / goto exits, and Release duplicated per arm;
  - t11 worse: one movie per arm;
  - t12 worse: a ?: text.
- Callees: relpair binds ResetRandomSpeechTime -> 0x00E2E400 and GetRandomSpeech -> 0x00E2E440. That proves both landed misnamings
  (NAME_FIXES.tsv).

## 0x00D5AE70 CTheRealGuildmaster::Main: parked DIFFER(11636v11636), 235 lines (retry; differ/00d5ae70.cpp = chunk 816 source)
- Residue: stack-slot colouring of the scalars and temporaries at esp+0x44..0x4c.
- 10 new attempts (g1-g10: types, declaration order, block scopes, const, a named movie name). None was better; g2, g4 and g6 were worse.

## 0x00E4B5B0 CTollBandit::Main: parked DIFFER(10631v10594), 923 lines (retry; differ/00e4b5b0.cpp, t1; r0 was 1143 lines on this grader)
- t1 fixes the frame: `{ CTimer call_timer; ...state-2 loop...; if (T) return; }` as a sibling block gives sub esp 0x118 as in retail, so
  the state-2 cut-scene objects can overlay call_timer.
- Left:
  - the conditional-dtor flag / temporary slot order (retail flag 0x30, predicate temp 0x4c; ours 0x4c / 0x50);
  - one ~CTimer retail calls;
  - one ~MovieBase / ~vector exit order.
- t2-t10 tried other block placements; none was better.
- Not matched, so the `vland --recheck-landed` of its 11 includers was not required. Their preprocessed text is identical anyway
  (SIBLINGS.tsv).

## Labels and names
- LABELS.tsv: the three new Mains, MonitorParanoidQuest 0x00EBCB60, ResetRandomSpeechTime 0x00E2E400 / 0x00E1FEB0, GetRandomSpeech
  0x00E2E440, and the 3 std::map placeholder labels.
- NAME_FIXES.tsv:
  - 0x00E1FEB0: landed as `?ResetRandomSpeechTime@CV_AmbushScamScript@@`, and bound by its callers as
    `?RestartRandomSpeechTimer@CQR_EscortTraderScript@@`. It is `?ResetTraderRandomSpeechTime@CQR_EscortTraderScript@NScript@@QAEXXZ`.
  - 0x00E2E440: landed as `CAC_Owner::GetRandomSpeech`. It is `CAS_Trickster::GetRandomSpeech`.
  - The two chunk 816 TollBandit fixes, carried unchanged.
- Lead, not proven, no fix written: 0x00E53CB0 (also landed as CAC_Owner::GetRandomSpeech) directly follows
  CV_BeardyBaldyScript::ResetRandomSpeechTime 0x00E53C70. It is likely `CBB_BeardyBaldyMan::GetRandomSpeech` (egor 00ea4f00), but no caller
  binding was checked.

## Header debt
None beyond the opt-ins in hdr/README.md. The two chunk 816 drafts (CV_BanditTollScript.h, CQ_GuildTrainingSkillScript.h) are still
pending in the repo.
