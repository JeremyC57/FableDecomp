# 2026-11-12 landing du -- chunk Q26: 1 of 8 parked rows matched

Source: `pending/2026-11-12_linux_handQ26/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00846DD0 CCreatureAction_PerformExpression::FinishAction.

Headers (three-way merged against 8f7c57d01; Q26's copies of Q25's twelve headers equal the repo now): CThingAICreature.h
ISRESPONDINGTOFOLLOWANDWAIT, CBankFile.h READENTRYDATA, CStreamingFontBank.h CTOR. hdrcheck2 default=same; the one DIFF macro is new.
Checks: reserved_check 0 reserved rows (one reserved callee cited in a comment); bindings_multi_check at baseline;
funclet_includes_check 1317 checked, 0 broken.
Queued (land1011du): global 0x013BDA50 is spelled `g_StreamingFontBanks_013bda50` in landed 0x009A1090 (an address-suffixed
made-up name): give it one name.
Lesson for chunk cutting: eight 1.3-1.8 KB rows left four with no real port. Keep chunks at five or six rows and require the port.
