# Chunk 682 (NA025, 257-512 band) -- 7 of 10 landed (2026-11-01)

Seven RELOCATION_MATCH rows landed (`verify.log`; bindings multi-check 0), six at the first or second probe: the gap-scan body 0x0121E260 is
the dynamic initializer of `CTCWife::WifeAppearanceDialogueGroupStemNames` (.CRT$XCU order of Ego_r's three CTCWife initializers), the two
"Transfer<signed_char>" labels are `CPersistContext::TransferVectorOfSubComponents<CArenaCreatureDef>` 0x00F26250 and `<CFeatDef>` 0x00F34330
(two bodies of different length could not be one instantiation; proven by caller key / element size / ctor-dtor), ~CEngineWeatherRenderer,
~CEnginePrimitiveManagerDecal, ~CQ_HangingTreeEvilScript, and CTCMeleeTrap::ProcessPossibleStrike 0x00788617 (/Os TU: `#pragma optimize("s", on)`
gives the biased `lea ebp,[esp-64h]` frame). Labels: `ghidra_out/labels_chunk682_2026-10-27.tsv`.

Decisions at landing (coordinator, under the owner delegation): the `_$E2` RELOC_CONFLICT on 0x0121E260 is the per-TU static-destructor thunk
(as landed 0x01219F00) -- expected, not a wrong binding; local models beside flattened engine headers (CTCBase, CDefPointer, CWorld,
CThingSearchTools, the 0x00F349F0 inline stream models, the CTCWife static array) stay as header debt for migration wave 10; the landed-source
errors the agent found (0x00788AEF `MeleeTrapHelper` __stdcall for the private member PerformPossibleTrapDamage, 0x00BBBBA0 dtor spelling,
0x00985F40 array name, 0x007898CC file name, 0x008169B0 CTriangle, 0x00783730 CrowdSizesSuppressed offset, the `<const CThing>` spellings of the
CIntelligentPointer<CThing> ctors 0x004A96F0 / 0x004A9730 in 0x00788D68 and three other TUs) are queued c682 for naming wave 18. The three parked
rows are not retried now (register / evaluation-order / x87-rank residues, all lane gaps documented in differ/ line 2 and probe/).

Parked: PerformPossibleTrapDamage 0x0078891D (DIFFER 463v466, 23 probes: radius-before-centre evaluation order, mirrored frame slots, `pop ecx; idiv ecx`
re-read), CTCBoastingCrowdControl::Startup 0x00783850 (443v443, 26 probes: the CThingCompare_Nearest 12-byte position copy interleaved with the
argument leas), CTCCoopSpirit::UpdateMovement 0x00670820 (483v483, 31 probes: x87 order of CSE'd squares, a lane gap). Best sources in
`../../pending/2026-10-27_linux_hand682/differ/`. NAME_FIXES.tsv: 15 rows (queued c682). Lessons in LESSONS.md.
