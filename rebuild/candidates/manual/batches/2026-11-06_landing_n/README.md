# Landing 2026-11-06n: chunk 813 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows. It shared the lane with the chunk 812 and 814 agents, and a container restart stopped it once;
it was resumed from its transcript. The six rows were unnamed gap-scan entity Mains, and the agent proved every name from vtable slot 1
and both PDB name sets. The landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D524A0 | NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeWhisper::Main | RELOCATION_MATCH PASS (3 attempts; the compared number is a named local, so the int compare's `sete bl` no longer lands before the CCharString dtor) |
| 0x00F17C70 | NScript::CQ_ArenaScript::CArenaCellDoorGuard::Main | RELOCATION_MATCH PASS (3 attempts; the main loop is `for (;; NSF) { if (T) return ...; }`, which gives retail's expanded ~seh_me at the natural exit, the first wait pad joining it, and one shared return tail; strings 48/48) |

- **Headers:** none copied into the repo, because neither landed row needs an overlay. The overlay stays in
  `pending/2026-11-06_linux_hand813/hdr/`, as for chunks 810-812. It holds:
  - `CQ_NewOakValeIntroScript.h` with the additive opt-in `_BARRELMAN_THERESA` (only `>` lines against the repo; the four landed includers were rechecked RELOCATION_MATCH + PASS);
  - two new headers, `CV_BeggarAndChildScript.h` and `CV_FisticuffsClubScript.h`.
- **vland:** no warnings. Every binding already existed in the same spelling, so reloc_bindings.tsv is unchanged.
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06n.tsv` holds the six Mains plus 0x00E83920 `CV_FisticuffsClubScript::CFisticuffsMember::CFisticuffsMember` (FableWin.pdb). That function's catalog row already has the right name; its Ghidra labels were the autonames `CScriptThing_Constructor_Helper` / `ScriptThingContext_Construct_A`.
  - Two descriptive data globals used only by the parked BarrelMan row, 0x013AC854 and 0x013AC858, are left out.
- **Name-fix queue:** 2 lines, tag `land0611n`.
  - Landed row 00e0f4e0 binds FadeOutAndKillEntity 0x00CBE716 with `_NM1`. Both PDBs spell it `M_N1`: (const CScriptThing&, float, bool, bool).
  - Landed row 00e8b020 binds the CPredicate_And<CIsThingAlive,CIsVariableFalse> ctor 0x00E7E020 as `??0CAndPredicate@@...CIsFlagFalse`. Neither class is in either PDB.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`) and the labels are applied.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand813/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00E5B3B0 NScript::CV_BeggarAndChildScript::CBeggarBully::Main | DIFFER(6996v7012), 14 attempts, 138 lines | only the CIsThingAlive copy blocker: retail expands the by-value Me copy for SetIsPushableByHero and ours calls it, which changes register colouring downstream. The cleanest witness of that blocker so far |
| 0x00E84B80 NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main | DIFFER(8968v8836), 10 attempts, 190 lines | VC7.1 expands the implicit ~seh_me at one more exit than retail (the chunk 809 class) |
| 0x00DB97A0 NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Main | DIFFER(6954v7013), 6 attempts, 181 masked lines | the CIsThingConscious copy and dtor blocker |
| 0x00DB5330 NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main | DIFFER(6228v6148), 5 attempts, 317 masked lines | the same blocker (down from 789 masked lines after dropping MOVIEBASE_OUTOFLINE_CTOR and switching to the `for (;; NSF)` loop) |

**Blocker status:** two more spellings of the CIsThingAlive / CIsThingConscious copy produce identical bytes, so neither gets past it:
- the predicate bound to a const-reference temporary;
- a named predicate local in its own block.

0x00E84B80 also declares a row-local `NScript::CIsVariableFalse`: vtable 0x012EA43C, slots 0x00E7DDD0 / 0x00E7DDE0 / 0x00E7DE10. It should move to a shared header when one is made.
