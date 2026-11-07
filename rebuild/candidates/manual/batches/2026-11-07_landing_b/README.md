# Landing 2026-11-07b: chunk 826 (0 of 6 rows landed, 6 parked)

One expert agent authored the rows, sharing the lane with the chunk 827 and 828 agents. A container restart stopped it early; it was
resumed from its transcript and ATTEMPTS.log. All six rows were retries with proven names. None matched, so finish.py wrote an empty
payload and nothing was landed.

| Row | Grade | Lines (md, parked -> now) | Note |
|---|---|---|---|
| 0x00960120 CAIStateGroup_DeedReactionRoot::Update | DIFFER(1580v1600) | 438 -> 135, 18 attempts | the parked model read PCurrentAction 4 bytes off (`sizeof(CThing)` padding); CDefPointer releases and IsEmpty now inline. Left: the bit-16 presence byte load, frame +4, per-site CIntelligentPointer copy |
| 0x00ED5F50 NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main | DIFFER(4971v5024) | 381 -> 22, 7 attempts | an explicit `CCharString("")` region name and each `rand()` count as its own statement fixed every frame slot. Left: the function-final ~CScriptThing split (the INLINE_DEPTH_REVISIT unexplained case) |
| 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover | DIFFER(2248v2272) | 394 -> 357, 7 + 2 diagnostic | a named bool for the break test keeps the distance maths above the marker calls. Left: frame 4 short; the Forward ctor expanded at all three arms where retail expands one |
| 0x00850A80 CContextSensitiveItemDef::IsValidOverride | DIFFER | 300 -> 301, 10 attempts | the frame now matches; retail keeps 0 in edi and shares one release-and-return-false tail |
| 0x00E8EF80 NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main | DIFFER(6038v5996) | 59, unchanged, 2 attempts | per-exit seh_me inline split (blocker); tried a cost-only ~CCountedPointer specialisation and `/d2inlT20..40` diagnostics, not in GOTCHAS before |
| 0x00D7FE90 NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena | DIFFER(2105v3148) | 398, unchanged, 1 + diagnostics | per-site ~vector<CScriptThing> split (blocker); no `/d2inl` setting reproduces retail |

- **Headers:** the new opt-in `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY` in fable_string.h (forceinline GetLength / IsEmpty) is used
  only by the parked 0x00960120. It stays in `pending/2026-11-07_linux_hand826/hdr/`.
- **Names:**
  - LABELS.tsv and NAME_FIXES.tsv are empty.
  - Open question, not proven (NOTES.md): 0x00851490's propagated label names a std::list / CFilter_Bind2 instantiation, but the
    0x00850A80 caller passes a vector and a HasTC filter. Compare the body before any rename.
