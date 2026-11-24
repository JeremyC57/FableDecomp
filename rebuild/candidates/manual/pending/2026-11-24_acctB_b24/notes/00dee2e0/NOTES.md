# 0x00DEE2E0 NScript::CQ_SecretPassageScript::CMotherTorture::Main -- B24S2 (2026-11-24)

## Lessons that applied
- Port first (WF_BRIEF Q32): the parked draft did not compile on today's headers (its CMOTHERTORTURE opt-in only lived in the old
  hand838 overlay); re-applied onto the CURRENT engine/NScript_CQ_SecretPassageScript.h with ParentClass a POINTER (Ego_r.pdb
  struct_layouts CMotherTorture 28 bytes), which alone took the row from DIFFER(3134v3102) to RELOCATION_MATCH.
- Model 0x00DE9690 / 0x00ED8680 preamble: H40 inline_depth(0) round the row-local CIsThingAlive ctor / dtor + CPREDICATEBASE_NOVTABLE;
  CTimer ctor `: TimerIndex(pDSTGame->RegisterTimer())` / dtor plain inline, operators expanded.
- FableWin (0x038A0690): for-scope opener before the main loop only -> `if (false) {} else for (;; Game->NewScriptFrame())`; the speech
  timer test is ??8CTimer -> `MotherSpeechTimer==0`. Local names (FableWin.pdb, register_scripts.obj module 11): seh_me,
  MotherSpeechTimer, _Fail_Val, MotherLever, MotherCellDoor, speech_movie, PauseMyEntitiesPlease, HLStyleConversationIndex = the draft's.

## B24S2: RELOCATION_MATCH, stock flags (3 grades). vt PASS, strict vland PASS (no WARN / RELOC_ / NOT_GENUINE lines). payload.json written.
- What made it match: the port above in one step (c1/v1.cpp); v2 also spells the inline CScriptThing::MsgIsUsedBy as
  `const CScriptThing* imp = PImp.Data; if (!imp) return false; return imp->MsgIsUsedBy(user);` (landed ChapelOfEvil form) so its
  pick-any copy equals retail 0x004AAB10 (the `PImp.Data->` form was NEQ); row bytes unchanged.
- Overlay: hdr/engine/NScript_CQ_SecretPassageScript.h, one new opt-in FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE (hdr/README.md).
- Pick-any (pickany_vs_retail.py + pk/pkall.py, every copy with a retail address): all EQ except
  ??1CScriptGameResourceObjectMovieBase 0x006E7B80 (LEAF-REFERENCED; byte-identical to the copy landed 0x00D62480 emits -- the
  ESCALATE question already open for 0x00ED8680), ??1CIsThingAlive 0x00E241B0 and ??_GCScriptThing 0x004AB170 (not leaf-referenced;
  byte-identical to the copies of handed-back 0x00DE9690). See ESCALATE.md.
- Test: 00dee2e0_test.cpp (model + body verbatim, 7 scenarios); mutations (MotherFound not set; flourish %300) FAIL.
- LESSON: a parked row whose residue was "frame one dword short" was a model error (reference v pointer ParentClass in an old overlay
  + stale preamble): re-port onto today's headers with a matched sibling's preamble before any frame work.
- Pick-any addendum (B24S2): copies with no table address checked by hand: ??_GCIsThingAlive (vtable 0x012C32F0 slot 0 = 0x00CDF120) and
  ??_GCScriptGameResourceObjectMovieBase (vtable 0x0126008C slot 0 = 0x006E70A0) differ from retail; the first is byte-identical to
  0x00DE9690's copy, the second to landed 0x00D62480's copy (pk/cmp2.py); ??_GCScriptGameResourceObjectBase and the CTimer operator
  copies equal 0x00DE9690's. All are not leaf-referenced HEAD debt (ESCALATE.md).
