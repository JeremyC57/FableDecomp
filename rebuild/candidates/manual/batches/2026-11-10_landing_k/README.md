# 2026-11-10 landing k -- chunk 898 (parked-row retries), 9 rows

Source: `pending/2026-11-10_linux_hand898/` (one Opus expert agent on 22 parked near misses, avoiding the Codex-reserved rows).
Before landing, the six rows that still defined classes with shared headers (0x00654438, 0x00732D40, 0x0079CF30, 0x009E2020,
0x00A3BF10, 0x00D9EB30) were rebased onto rebuild/include/engine/ (the owner's chunk 897 review bar); all stayed byte-identical.

| address | function | lever |
|---|---|---|
| 0x00D9EB30 | NScript::CQ_MinionCampScript::CMC_MinionGuard::Main | one whole `if` per IsNull arm so the identical tails merge |
| 0x007E4530 | CTCWillResponse::IsThingLightningable | engine/CThing.h PeekTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED) |
| 0x009E2020 | GFABoxToVBox (C2DBoxF) | the landed 0x009E21D0 GFAYToVY spelling |
| 0x0084CD10 | NCameraModes::SetCameraRoll | parenthesised squares in `len2` |
| 0x00732D40 | CTCWeatherVane::FrameUpdate | `wind / wind.GetMagnitude()` through the shared C3DVector |
| 0x0079CF30 | CTCBulletTime::Pause | the whole accessor chain in one call expression |
| 0x008F8540 | LHNetworkLib::LSocket::CheckForActivity | chained `exceptSet = readSet = writeSet = 0;` |
| 0x00654438 | NPlayerGui::CDrawQuestInfo::Draw | the C2DBoxF extents constructor |
| 0x00A3BF10 | C3DAnimationBlendState::CreateInterpolate | parenthesised lerp right-hand side |

Headers: 14 engine headers gained `FABLE_RETAIL_*` opt-ins (default path byte-identical, hdrcheck2 default=same for each; list in
`pending/2026-11-10_linux_hand898/hdr/README.md`). Landed with `vland --land` and no overlay: 9 RELOCATION_MATCH, tests PASS.
Warnings at landing: RELOC_NAME 0x00437870 (now labelled CPlayerGui::IsMiniMapDrawing) and 0x006B83F0 (queued, below); two import
RELOC_ALIAS notes. `bindings_multi_check.py` prints 1 (CScriptThing::NullCoord), unchanged.
Names: `ghidra_out/labels_landing_2026-11-10k.tsv`. Queue `land1011k`: 0x006B83F0 is
`CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float, void*)` (landed as an allocator helper; callers 0x0079D020 /
0x0079D110 to respell), and engine/CQ_MinionCampScript.h's `CMCMinionGuard` should be `CMC_MinionGuard`.
Parked (13, 4-6 lines each): see the hand898 STATUS.tsv / NOTES.md.
