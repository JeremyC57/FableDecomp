# Hand chunk 822 (CBH_GangDeputy2 .. CV_MayorsInvitationScript): 2 of 6 byte-exact

## What matched (2)
- NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather::Main 0x00DB86B0 (4325 B, RELOCATION_MATCH; retry of chunk 807's park, md 91 ->
  0). The hero's father: intro cutscene + highlighting hint once, then a penny per new good deed, deed / chocolate-box comments, temper
  when hit. Shape: the final hint thread check written at the end of BOTH `IsXbox()` arms (not once after the if / else). VC7.1 then
  cross-jumps the two arm-end checks into the single call retail makes at 0x00DB8A3B (own PauseAll pad), and the four hint-loop
  returns share retail's pad 0x00DB8963. Name proven (chunk 807: vtable 0x012D8388 slot 1 + Ego_r/FableWin symbol); every relpair
  target single and right, strings (incl. the `_PC` / Xbox pair) checked against retail; bindchk: no conflicts. New test written
  (test/00db86b0.cpp, control-flow model, PASS under g++ and VC).
- NScript::CV_MayorsInvitationScript::MonitorParanoidQuest 0x00EBCB60 (176 B, RELOCATION_MATCH, first grade; new row). Spawned watcher:
  ActivateQuest("V_ParanoidWhispers"), then `while (!MsgOnQuestCompleted(..)) { NSF; if (T) return; }` and a trailing `if (T) return;`.
  Name proven by two sources: the CLadyGrey::Main 0x00EB9980 CSpawnedFunc store at 0x00EBAFEE and Ego_r/FableWin
  `?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ` (emitted symbol confirmed). Callees 0x0099EBF0 / 0x0099EAE0 /
  0x00CB7940 and the string 0x012C4CA0 bound right (relpair). Header model engine/CV_MayorsInvitationScript.h, no local struct.

## Parked for Fable 5.1 (4)
- 0x00D18DF0 NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main DIFFER(3337v3321): the two MovieBase dtor sites (0x00D1995D, 0x00D19A44) carry an extra MovieBase vtable store from the user-declared __forceinline dtor (retail expands the IMPLICIT dtor: call 0x006E7AB0 ; GROBase vptr 0x0126008C ; call 0x0099A430) | tried: implicit dtor + declared-only PImp spec + chunk-821 SAY do{}while(0) (3287, 6 lines: both sites call COMDAT 0x006E7B80), do-while round the movie part / the whole intro body / the whole intro if, wrapped T checks, nested scope, forceinline GetPos / CIsThingAlive, row-local GetPos, 16 diagnostics; meters: /d2inlS15 expands both sites (one cross-jump target left: jne 0x00D195CF -> 0xd19a8b vs 0xd19977), T72/T78/T90 no change | next: lane gap: inline caller-size budget for an implicit dtor in a 3.3 KB body (a reduced body with the same block expands; heat and caller size combine)
- 0x00DAF080 NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::Main DIFFER(5571v5504): the predicate temp's expanded ~CIsThingConscious also expands its member ~CScriptThing where retail calls 0x004AA840 (one-level nesting); the other 15 ~CScriptThing sites split exactly as retail | tried: row-local inline ~CScriptThing (b1: the split right but plain-inline outer declined, 147 lines), + __forceinline outer (b3, 30 lines), loop condition, do-while round SetPredicate, named predicate (with plain-inline and forceinline outer), definition order, an implicit outer dtor via header opt-in (42 lines: drops the vtable store); meter: b3 and b1 both RELOCATION_MATCH under the diagnostic /d2inlT60 (T72/T66/T40/S13 = 30, T20 1906: non-monotonic) | next: lane gap: nested inline depth/threshold (shared-model ~CScriptThing cost respelling across the CScriptThing-predicate rows)
- 0x00DAC760 NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main DIFFER(5906v5867): the hit block's entry thread check (0x00DADB4D) gets its own expanded ~seh_me (STB) pad where retail jumps to the common called pad 0x00DADE37 (39 bytes, 47 masked lines) | tried: bool hit local, plain returns (one site / all), negated if/else, SAY do-while, break to the natural end, cold else-return arm, heating the hit block (worse: 103), diagnostic `if (!T) { body }` (wrong semantics) = 5867v5867 with only that jne target off -- proves the single site | next: lane gap: STB-dtor inline threshold (RELOCATION_MATCH under /d2inlT72, as 0x00E8EF80)
- 0x00DADF80 NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main DIFFER(2790v2787): the eight speech-list picks fold the begin load (`sub eax,[ecx+begin]`) and drop retail's dead `add ecx,begin` | tried: `.size()` (retail's spelling, dead add present, but parent eax / size ecx swapped and one more expanded ~CScriptThing pad: 2846, 184), size() + named index, const-ref static_cast, this->ParentClass, named vector reference (2598), `*(begin()+i)` (2633), `begin()[i]` (2633), `+ 0` (= a0) | next: find the header change since chunk 797 measured size() at 2787v2787 (fable_containers.h / CScriptThing.h), then retry size(); else lane gap: register colouring

## Skipped
None.

## Label corrections
- 0x00EF7AB0: latest label is the placeholder `CScriptThingFunctor_00ef7ab0::~CScriptThingFunctor_00ef7ab0`; it is
  NScript::CIsThingConscious::~CIsThingConscious (engine/CIsThingConscious.h, Ego_r/FableWin `??1CIsThingConscious@NScript@@UAE@XZ`,
  vtable 0x012C2FE8). reloc_bindings.tsv binds seven spellings to this address (one address, many names): NAME_FIXES.tsv lists them.
- 0x00CDBFB0 (known, not new): vland warns RELOC_NAME for the db86b0 row because the catalog label is still
  `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a`; the labels files already carry the proven
  `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map` (naming wave 1, chunk 584), and our symbol matches it.

## Names
All six names proven (five from their earlier chunks, 0x00EBCB60 by two sources above); no descriptive names. NAME_FIXES.tsv re-files
0x00DAEBF0 GetVillagerSpeechIndex (chunk 797, still unapplied: the landed row uses a global CNOVI_Villager class) and adds 0x00EF7AB0.

## Header debt
No shared header changed (hdr/ empty). Rows use engine/CQ_BountyHuntScript.h (opt-in GANGDEPUTY2), engine/CQ_NewOakValeIntroScript.h,
engine/CIsThingConscious.h, engine/CScriptGameResourceObjectMovieBase.h (FORCEINLINE / FORCEINLINE_CTOR / OUTOFLINE_DTOR opt-ins),
engine/CV_MayorsInvitationScript.h.
