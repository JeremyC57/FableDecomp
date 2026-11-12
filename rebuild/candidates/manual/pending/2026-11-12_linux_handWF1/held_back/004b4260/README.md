# 0x004B4260 CQuestManager::ActivateMultipleQuests -- held back (workflow wf1, 2026-11-12)

RELOCATION_MATCH (359 bytes) and test PASS under VC7.1, but only with a body of the private CQuestManager::ActivateQuests 0x004B3CE0
visible earlier in the same file (a declaration alone gives 349 v 359). That body is an UNPROVEN reconstruction: row 0x004B3CE0 is
parked DIFFER (1266v1285, pending/2026-10-23_linux_hand322). Rule: only a callee's REAL body may sit in a row file.

To land: match 0x004B3CE0 first (the body in 004b4260.cpp is the best starting draft), then regrade this row with the matched body.
The overlay headers this row needs are in hdr/ (CQuestManager.h opt-in FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS,
CScriptInfo.h opt-in FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC); they are NOT landed. Fix the parameter names of the header
declaration to (quest_names, game_script, load_resources) when landing.
