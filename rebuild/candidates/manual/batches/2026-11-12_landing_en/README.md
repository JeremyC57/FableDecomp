# 2026-11-12 landing en -- 0x004B4260 CQuestManager::ActivateMultipleQuests (held since landing eh)

Source: `pending/2026-11-12_linux_handWF6/` (one-row run of `author_rows.js`: scout, Fable author, two independent Opus verifiers;
VERIFIED with no open issue). RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1.
The row byte-matched in workflow wf1 but only with an unproven same-TU body of ActivateQuests 0x004B3CE0. That function landed in
landing em; the row now carries the landed row's real body verbatim (the coordinator compared the two bodies: identical apart from
white space and comments), with the real bodies of CBasicString<char>::operator< 0x00429950 and LoadResourcesForQuests 0x004B2510 that
it needs. FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE is required: retail calls the push_back<CActivateQuestInfo> COMDAT 0x004BB720 at
both sites (503 v 359 without it). No header change.
Checks: reserved_check 0, purity clean, bindings_multi_check at baseline, funclet_includes_check 1317 checked, 0 broken.
With this row the six author workflows of 2026-11-12 (wf1..wf4, wf6) have matched and landed 52 of 52 rows.
