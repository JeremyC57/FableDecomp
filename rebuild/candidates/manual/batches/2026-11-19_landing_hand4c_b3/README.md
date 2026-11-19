# Landing hand4c b3 (2026-11-19): CThingSearchTools::GetAllTCsInRoughArea<CTCCreatureGenerator, CThingFilter_IsInQuestOrNullQuest, ..> (0x00518340)

vland --land, strict headers: RELOCATION_MATCH + PASS; no flags. The old local model ported onto shared headers: the Ego_d rough-area walk + new opt-in
FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD (out-of-class GetAllTCsInRoughArea body, Ego_d locals, FableWin order; appended so no assert moves),
the world_map TU opt-ins of landed 0x00501450 (CTHING_GETTC_EXPANDED, CVECTORMAP_INLINE_LOWERBOUND) and STLP_VECTOR_OVERFLOW_OUTOFLINE. NEW
engine/CThingFilter_IsInQuestOrNullQuest.h (Ego_d class record, Ego_r layout). All 320 landed CThingSearchTools.h users rechecked.
0x005AC10E bracket follow-up (5 natural placements, hand4c): the same 2 instructions at 0x005ACB58 -- stays open-escalated.
