# Landing 2026-11-07g: chunk 833 (1 of 6 rows landed, 5 parked)

One agent authored six entity Mains (PDB-proven names, each confirmed by its entity vtable slot 1) with Codex pass-13 attempts as prior art.

| Row | Grade | Note |
|---|---|---|
| 0x00E19D80 NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main | **RELOCATION_MATCH + PASS, landed** | one `do { } while (0);` round the two cast-map assignments (line-3 `// Inline-split:` note) expands the implicit STB operator= as retail does; header opt-in `FABLE_RETAIL_CQ_WHITEBALVERINEWWSCRIPT_CKG_CHIEF` copied into rebuild/include (additive; no landed file defines it; the 10 landed includers regraded RELOCATION_MATCH with it) |
| 0x00D17F20 CBH_GangDeputy1::Main | DIFFER(3035v3068), 78 lines | MovieBase dtor called where retail expands it; matches under the diagnostic /d2inlS15 (caller-size budget; park) |
| 0x00D19BD0 CBH_Hostage1::Main | DIFFER(2807v2918), 188 lines | STB operator= called at 5 cast-map sites + the movie dtor |
| 0x00D1A740 CBH_Hostage2::Main | DIFFER(2662v2743), 167 lines | as Hostage1 |
| 0x00E17840 KG CKG_Chief::Main | DIFFER(2501v2515), 97 lines | mixed MovieBase ctor (chunk 823 blocker), CScriptThing copy split, per-return pause-entities dtor |
| 0x00DB1F00 CNOVI_AffairWoman::Main | DIFFER(2858v2714), 135 lines | per-exit ~CScriptThing split; predicate slot 0x6c v 0x60 |

- **Headers:** the BountyHunt, KnotholeGlade and NewOakValeIntro opt-ins serve only parked rows and stay in `pending/2026-11-07_linux_hand833/hdr/`.
- **Names:** NAME_FIXES.tsv empty. Landed-row label fixes queued (`land0711g`): the map dtor 0x00CDBFB0 (the 2026-11-03 sync restored the wrong
  CRetailTree label), 0x00E15500 WatchForTermination, 0x00D18B20 WatchForDeputy1GruntsKilled.
- Retail sets `InitialChatted = false` on the first chat (0x00E1A208): an original-script quirk, kept.
