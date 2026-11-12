# 2026-11-12 landing dw -- chunk Q27: 2 of 5 large parked rows matched

Source: `pending/2026-11-12_linux_handQ27/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00EB40C0 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoWatchingForTrouble, 0x00D39960
NScript::CQ_FireHeartScript::StartRound.

Headers: only engine/CQ_FireHeartScript.h (TRIGGERPROPHETCOMMENT, an added declaration) is landed. The chunk's other 13 overlay
headers serve the three rows that still differ and are parked with them (`pending/.../hdr/`). The three-way merge showed
stlport/fable_map.h conflicting with landing dv, and the C3DMesh2.h copy changing its default path, so they were not landed.
Coordinator fix: the payload's name column repeated the class (`Class::Method`); stripped to the method before landing.
Checks: reserved_check 0 reserved rows (one reserved callee cited in a comment); bindings_multi_check at baseline;
funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dw): 0x006EC850 is CDefendingCombatantInfo::CCombatWheel::Init (an unlanded row's manifest label).
