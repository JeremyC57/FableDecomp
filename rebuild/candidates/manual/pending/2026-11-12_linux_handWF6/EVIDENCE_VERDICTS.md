# Workflow wf6 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 004b4260 -- VERIFIED, author attempts 1

- What made it match: Kept the old draft's ActivateMultipleQuests body unchanged and replaced the unproven same-TU ActivateQuests with the landed row rebuild/src/compiled/00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp verbatim (its defines, includes, the out-of-line real bodies of CBasicString<char>::operator< 0x00429950 and CQuestManager::LoadResourcesForQuests 0x004B2510, then ActivateQuests 0x004B3CE0). Added engine/NProgressDisplay.h, a declaration-only CScriptInfoManager (no shared header exists) and FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE, which is required: retail calls the push_back<CActivateQuestInfo> COMDAT 0x004BB720 at both sites, and without the macro VC7.1 expands them (503 v 359). Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf6/004b4260/004b4260.cpp; registered with addmatch.py (payload.json + oracle.tsv in the same directory); NOTES.md and hdr/README.md written.

