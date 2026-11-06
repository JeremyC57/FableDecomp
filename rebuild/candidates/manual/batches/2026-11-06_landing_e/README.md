# Landing 2026-11-06e: chunk 804 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows (sharing the lane with the chunk 803, then 805, agents). The six rows came from the gap scan
unnamed. The agent proved every name from vtable slot 1 (one .rdata reference), the ctor store in Alloc and both PDB name sets. The
landed row is plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D8ABF0 | NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main | RELOCATION_MATCH PASS (4 grades; a declared-only `CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` 0x006E7AB0 placed before the movie header; all 40 strings in retail order) |

- **Headers** (`hdr_README.md`): three new per-quest headers with no earlier includer: engine/CQ_ArenaHoldingScriptScript.h,
  CV_SingingStonesScript.h, CQ_HerosOldHouseScript.h. Each is behind `#ifdef FABLE_RETAIL_NSCRIPT`, with no `#error` branch. No
  repo header changed.
- **vland warnings:** 1 RELOC_NAME on the map dtor 0x00CDBFB0 (already queued by chunks 796, 798, 799).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06e.tsv` (the six Mains).
  - 4 queue lines (tag `land0611e`). Among them, `FadeOutAndKillEntity` 0x00CBE716 has the PDB spelling
    `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z` (thing, float, bool, bool). The `_NM1` spelling is bound for 0x00E0F4E0 and
    declared in 13 landed rows (same code, wrong symbol).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand804/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00CF18D0 NScript::CQ_ArenaHoldingScriptScript::COakvaleFan::Main | DIFFER(4676v4539), 10 attempts | VC7.1's caller-size inline cap: the movie dtor stays a call at the second-loop and exit sites where retail expands every site |
| 0x00CF2B70 ...::CBowerstoneFan::Main | DIFFER(4454v4298), 11 attempts | the same cap |
| 0x00CF3D20 ...::CHookCoastFan::Main | DIFFER(4464v4298), 15 attempts | the same cap |
| 0x00ED17D0 NScript::CV_SingingStonesScript::CManWithDoorName::Main | DIFFER(4149v4279), 13 attempts | the CScriptThing copy inside the expanded CIsThingAlive ctor is called in retail; only the bribe-site movie ctor is expanded; two exit cascades are expanded |
| 0x00DC4D70 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main | DIFFER(4275v4319), 10 attempts | the nested copy / dtor inside the expanded CIsThingConscious are called in retail (ebp == 0 knock-on); the first movie ctor expanded, the second called |

**Open question for the owner (the three fans):** each fan file grades RELOCATION_MATCH when compiled with the undocumented
diagnostic flag `/d2inlS20` (Bowerstone also with `/d2inlS15`, not with `/d2inlS13`). That flag raises VC7.1's limit on how large a
caller may be and still inline. It is not a landing option, and nothing shows retail was built with it. The rows stay parked until a
plain C++ spelling shrinks each function under the default cap, or the owner rules on the flag.
