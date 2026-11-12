## 0x004B4260


## 0x004B4260 CQuestManager::ActivateMultipleQuests -- RELOCATION_MATCH (wf6, 3 variants + final + strict = 5 grades)
* What made it match: the old draft's ActivateMultipleQuests body unchanged, with the unproven same-TU ActivateQuests replaced by the
  LANDED row rebuild/src/compiled/00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp verbatim (defines, includes, noinline real bodies of
  CBasicString<char>::operator< 0x00429950 and LoadResourcesForQuests 0x004B2510, ActivateQuests 0x004B3CE0), plus
  engine/NProgressDisplay.h, a declaration-only CScriptInfoManager and FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE.
* PUSHBACK_NOINLINE is required: without it VC7.1 expands both push_back<CActivateQuestInfo> sites (503 v 359); retail calls the COMDAT
  0x004BB720. ActivateQuests under `#pragma auto_inline(off)` (v3) was not needed / not graded once v2 matched.
* Side effect to know: the macro is TU-wide, so the ActivateQuests copy in this TU calls push_back<CCharString> out of line where
  retail 0x004B3CE0 expands it. That body is not graded by this row (its own landed row is the byte proof); the row still matches.
* Headers / flags: no overlay (hdr/ empty), no flags. Opt-ins all in repo headers. Passes LANDVERIFY_STRICT_HEADERS=1.
* Test: 004b4260_test.cpp (from wf1, vt.py PASS). Made-up name: g_QuestScriptsEnabled 0x01375454 (inherited from the landed row).
* Name fix candidate (not proven wrong here, from the dossier): 00/89/CGameScriptInterface_ActivateMultipleQuests_00892ea0.cpp and
  00/4b/CQuestManager_ActivateQuest_004b4a10.cpp use stand-in spellings for this callee; real name/signature is
  ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z.

