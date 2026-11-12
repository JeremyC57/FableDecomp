# 2026-11-12 landing ec -- chunk Q30: 1 row matched before the lane switched to Fable rapid-fire agents

Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x007F67C0
CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit. No header changes. The four filter classes and CFilter_Bind4
have no shared header and are local classes on Ego_r layouts.
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
RELOC_NAME notes (queued land1011ec): 0x00686D90 is called as CTCBase::PeekThingSearchTools (landed GetThingSearchTools); 0x007F70D0's
landed GetPBestThingInRoughArea label names different filter types than this caller's instantiation; 0x00878A80's manifest label
"GetRotatedPosition" is wrong (GetCamPosOffsetXY, which the landed source already uses).

Owner decisions recorded with this commit (2026-11-12): author agents are Fable 5.2 with the rapid-fire rules (chunk_pool/BRIEF.md
"Rapid fire": every call ends in a grade, up to 3 variants per call through lane_tools/fast/fg.py, no grade limit), still one row at
a time with no switching until the row matches. Chunks F1 (Q30's rows), F2 (six new rows) and F3 (the two stubborn rows 0x008031E0 /
0x006B05A0) are running.
