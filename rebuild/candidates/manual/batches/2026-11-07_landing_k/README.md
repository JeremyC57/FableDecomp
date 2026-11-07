# Landing 2026-11-07k: chunk 834 (2 of 6 rows landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D368D0 NScript::CQ_FireHeartScript::CFH_GuildMaster::Main | **RELOCATION_MATCH + PASS, landed** | movie forceinline ctor / out-of-line dtor, STLP map; RELOC_ALIAS on the `_rand` import thunk only |
| 0x00D1FD30 NScript::CQ_BreakSiegeScript::CDisplacedTrader::Main | **RELOCATION_MATCH + PASS, landed** | one `do { } while (0);` round the TimesSpokenTo SAY switch (line-2 `// Inline-split:` note) took it from 86 lines to a match |
| 0x00E72630 NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main | DIFFER(2236v2252), 22 | retail dead store `mov [esp+0x3c],ebp` at entry and 16-byte loop-head padding; the do-while that fixes length moves the seh_me dtor |
| 0x00E25F70 NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main | DIFFER(2049v2049), 89 | GOTCHAS Blocker 1 (CIsThingAlive nested copy); `probe/00e25f70_p4.cpp` is down to that one call |
| 0x00D886B0 NScript::CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath::Main | DIFFER(2128v2064), 150 | per-site movie ctor/dtor split; two movie frame slots swapped |
| 0x00D24A40 NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main | DIFFER(2440v2403), 175 | zero register edi v ebx, push order, cut-scene frame slots, REMINDER movie dtor called |

- **Names:** 2 new queue lines (`land0711k`): 0x00E72F00 DressedAsBandit (bare Leaf and catalog label lack `NScript::CV_DemonDoorsScript::`) and
  0x008AA8E0 ShareData (placeholder pointee; this row proves the STB instantiation, possible ICF fold: coordinator decision). The agent's other
  NAME_FIXES lines (0x00CBE716, 0x00704580, 0x008AB1E0, 0x00CDBF70, 0x00CDBFB0, 0x00CBFB7D) were already queued.
- **Headers:** the CV_DemonDoorsScript `FABLE_RETAIL_CDF_THREESUITS` opt-in serves only the parked row; it stays in
  `pending/2026-11-07_linux_hand834/hdr/` (19 landed includers rechecked with it, all MATCH / RELOCATION_MATCH).
