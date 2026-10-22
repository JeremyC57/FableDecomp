# agent283 (chunk 283) notes

40 rows, all attempted. 22 matched (src/ + test/, every test PASS under g++ and VC7.1), 18 parked (differ/), 0 skipped.
`STATUS.tsv` has one line per row with the reason.

## Matched (RELOCATION_MATCH)
0069f9e0 008430b0 00845350 008455c0 00846100 00846410 008b76b0 008c2240 008c64f0 008cb940 008f5220 00995770
00995fb0 009ad6e0 009ade90 009ae150 009cd850 00c1efc0 00d3a1a0 00e0a510 00ed9cf0 00eda0c0

- 00846100: `gall.sh` / q.py prints DIFFER(687v686). That is a q.py artifact, not a real miss: the function ends in
  `jmp rel8` = `eb 90`, and q.py strips trailing 0x90/0xCC bytes from the oracle. With the full PROMPT row bytes,
  `V.parity_of` gives RELOCATION_MATCH 687v687.
- 00c1efc0 needs `// Flags: /EHsc` (it has a `push -1 ; push handler` prologue, and `new T(...)` gets EH states 0..11
  in source case order).
- 008f5220 was matched through the advisor's final.cpp.

## Parked (differ/)
00845040 00846bc0 00849700 00854b90 0087cb10 008b6c30 008b7040 008bf280 008bf800 008cb250 009c8990 (earlier
sessions, see STATUS.tsv), plus these rows from this session:
- 009acee0 DIFFER 1030v1017: the separator is cached in edi across the call (lane gap), and the name/separator frame words are swapped.
- 009c9280 DIFFER 923v861: first pass. The frame is 0x18 v 0x20 and the register roles differ.
- 006a03a0 DIFFER 827v825: the frame is one word smaller, zero is held in edi v ebp, and the creature C2DCoordI converts X before Y.
- 00e83230 DIFFER 818v820: branch 1 keeps Game in esi and reloads its vtable for GetHero.
  The inline counted-pointer assign was fixed by loading info and data into locals first (vs6).
- 008c4fd0 DIFFER 722v722 (same length): three things differ.
  - Frame-slot assignment: retail puts the index, def pointer and boss at 0x18/0x10/0x14.
  - The CDefPointer release shape.
  - The init ctor reuses eax.
- 00854dd0 DIFFER 702v720: the x87 schedule of the direction normalise differs, and the edi/ebp roles are swapped.
- 008548b0 DIFFER 748v723: first-pass shape. Register roles differ, and the release/store scheduling differs.
- Advisors B4 (0087cb10, 009c8990) and A3 (008b7040) both recommended parking; their probes are under probe/advisor/.

## Wrong / unhelpful catalog labels
- 00e0a510 HandleTrader and 00e83230 HandleVillageSelection are entity-script methods, not
  CCreatureAction_TrollWhackGroundBase. Their layout is {vptr, Game +4, CScriptThing Me +8, ParentClass +0x14}.
- 00d3a1a0 is CDF_Fireheart::Main.
- 00c1efc0 "CCreatureAction_UnsheatheWeapons::Create" is a forge autoname. It is a packed-type (0x10101..0x20203)
  factory over 12 product classes, and the class is not confirmed.
  - The ctors at 0xC1F640/0xC1F720 take ONE stack argument (ret 4), not the creature plus type that Ghidra shows.
  - 0xC20230 is not CIntelligentPointer<CThing const>: it is a 2-argument ctor (ret 8).
- 0x9C9070 is GetPTextEntry (not GetPGroupEntry); 0x9C8890 is GetPGroupEntry.
- 0x706B50 is the conflicting-modifiers list, not FilterAbilityClasses.
- 0x6A1440 is GetThingInPrimarySlot; 0x9BA900 is PeekNextItemType.
- 0x692940 is GetPWorld and 0x692950 is GetWorld.
- 0x5D9050 is an unlabelled inventory getter.
- 0x6DE8B0 and 0x6DE8C0 are CTCTrophy members.
- 009cd850 is CBankFile::InternalClose; 00ed9cf0 and 00eda0c0 are CV_TalentlessBardScript.
- 0x670000 (labelled GetAirResistance on CTCPhysicsBase) is called on the co-op spirit TC (interface 0xC7) as its attack range.
- 0x669D30 (CCRC::CalcAndLookup) is used here as a CSpecialEffectsDef lookup with an out-param (name, long*).
- 0x13BAC48 is a global CCharString (the emitter helper name); the "GetKeyState" text there is a false string read.
