# Landing 2026-11-06i: chunk 808 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows (sharing the lane with the chunk 807, then 809, agents). The six rows were unnamed gap-scan Mains
(4.9-5.5 KB). The agent proved every name from the vtable slot the landed Alloc stores (each address has one .rdata reference) and
both PDB name sets. The landed row is plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E78630 | NScript::CV_ExposeMayorScript::CEM_Sheriff::Main | RELOCATION_MATCH PASS (first attempt, on the landed CEM_SuspiciousVillager model; no vland warnings) |

- **Headers** (`hdr_README.md`): the default paths are textually unchanged.
  - engine/CV_ExposeMayorScript.h: FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_SHERIFF (nested CEM_Sheriff).
  - engine/CQ_RansomVictimScript.h: FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_CHECKVICTIMHIT (`CheckVictimHit(CScriptThing)`).
  - New engine/CV_GuildMasterScript.h, engine/CV_HauntedHouseScript.h and engine/CV_FishermanScript.h, each behind
    `#ifdef FABLE_RETAIL_NSCRIPT` with no `#error` branch; no landed row includes them yet.
- **Names.** Labels: `ghidra_out/labels_landing_2026-11-06i.tsv` (the six Mains plus the vtables 0x012EA430
  `CPredicate_And<CIsThingAlive,CIsVariableFalse>` and 0x012EA43C `CIsVariableFalse`). 6 queue lines (tag `land0611i`), all on
  landed rows:
  - the landed CRingFighter::Main 0x00E8B020 binds 0x00E7E020 and the vtable 0x012EA430 as `CAndPredicate` / `CIsFlagFalse`
    (neither in the PDBs);
  - the landed 0x00E95F10 binds the vtable 0x012ECBA8 as `CSpawnedFunc<CV_ExposeMayorScript>`; it is
    `CSpawnedFunc<CV_HauntedHouseScript>` (as 0x00E93DA0 binds it), and its source calls 0x00E953E0 by a `DoMission` stand-in
    (it is `CheckRegionLeaving`);
  - 0x00E95380 / 0x00E95390: identical-code-folded bodies, notes only.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case; the row adds no new relocation binding.

## Parked (`pending/2026-11-06_linux_hand808/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DE8370 NScript::CQ_RansomVictimScript::CRansommer::Main | DIFFER(4894v4894), 10 attempts | register choice at two sites only (retail eax + `lea`, ours ecx + `add`) |
| 0x00DE6C20 NScript::CQ_RansomVictimScript::CVictim::Main | DIFFER(5479v5511), 10 attempts | retail expands one `~CScriptThing` (first speech line) and calls the others; two handle releases `add` vs `lea` |
| 0x00E90BE0 NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main | DIFFER(4977v4914), 10 attempts | movie ctor / dtor expanded at the two inner-loop sites and called at the outer ones |
| 0x00E93EB0 NScript::CV_HauntedHouseScript::Main | DIFFER(5171v5321), 10 attempts | the function-wide inline budget: retail declines some IsAlive wrappers, keeps the `Return` stub call 0x00E95380 and expands the end-of-function dtors |
| 0x00E7C8E0 NScript::CV_FishermanScript::CFisherman::Main | DIFFER(5358v5358), 10 attempts | the help timer's index kept in esi across the loop; the shared terminate-exit block placed differently; one register |
