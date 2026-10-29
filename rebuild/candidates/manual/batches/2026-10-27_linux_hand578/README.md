# Chunk 578 (2026-10-29): 9 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1). Rows: CQS_MeetSisterScript::Main, CQR_EscortTraderScript::TravellingWrongPath,
CV_MazeResearchScript::Main, CQS_GuardianTrophyDealerInfoScript::Main, CFeatDef::Transfer, CQ_AmbushTradersScript::
CAT_BanditAlly::Init, CV_FisticuffsClubScript::CFisticuffsMember::Init, CV_HauntedHouseScript::SendBodyguardsToTheRightPlace,
std::vector<CFeatDef>::operator=. Parked: 0x00EB55D0 CMurderTwistAssassinGuard::Main (DIFFER 362v370, one extra zeroed frame
dword + dead `mov bl,al`; next: grade with its twin 0x00EB3F40), in ../../pending/2026-10-27_linux_hand578/differ/.

Coordinator review before landing: the agent's local CSpawnedFuncBase stand-ins (wrong vtables) were replaced by the new retail
opt-in FABLE_RETAIL_SUSPENDABLE_PROCESS in the shared headers (NOTES.md "Header debt"); 0x00F05F30 then failed on its own local
CBaseClass, was switched to engine/CBaseClass.h and landed in a second vland pass (verify.log is the first pass). Recheck of every
row on the two retail opt-ins (0x00E27920, 0x00F05F30, 0x00F2D330, 0x00D34D40, 0x00E54AA0): all RELOCATION_MATCH + PASS.
NAME_FIXES.tsv: 14 rows queued (11 from the agent; 3 from review: 0x00A44740 / 0x00A44620 microthread ctor/dtor and
0x0099A2E0 ~CBaseClass carry invented bindings).
