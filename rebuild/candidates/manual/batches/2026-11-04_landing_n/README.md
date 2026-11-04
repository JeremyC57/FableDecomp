# Landing 2026-11-04n: chunk 751 (retry of 40 parked same-length rows, C++ only), 17 landed

## Result
- **Landed 17 of 40** (`logs/land.log`). Before landing, all 17 were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): 16 RELOCATION_MATCH and 1 MATCH, all PASS.
  - The rows: CPersistContext::Transfer<CWideString> / <CCharString>, CSoundMap / CSimVoice operator=,
    the CEnginePrimitive2DSprite ctor, CTCInventoryBase::SetInventoryAnimRenderBox, IsWillingToAttackHeroOnOpinion,
    SetupFireballSpell, UpdateDistanceFromPerceivedThing, CTCEffectOnDie::OnDie, CTCActionUseBase::OnInitialActivate,
    CMiniMapDisplay::CMarkerInfo::IsReversedOnMap, SpiritReturnToHero::PerformAction, MazeShieldedChase::IsStillRunnable,
    CStringParser::ReadAsFloat, CEmitterData::BuildEmitterTemplate and CParticleSystem::DeleteAllParticles.
  - 006817f0 carries RELOC_UNCHECKED: its TU emits more than one object block of other lengths, so the relocations were not cross-checked.
- **Asm rules (owner, 2026-11-04):**
  - No row source or test has asm, a helper, a macro or `volatile`. The `volatile` in the old 006af3f0 draft was removed.
  - All 40 residues were C++ shapes; the agent's `agent_NOTES.md` records what fixed each one.
  - Pragmas used:
    - `#pragma optimize("s"/"y")` (6 rows): models the retail size-optimised TU, as in 6,935 landed rows.
    - One `#pragma auto_inline(off)` around a same-TU callee stand-in: the EXTRA_RULES device for a callee body that lives in the
      same retail TU, as in 2,194 landed rows.
    - Both model compile settings. Neither isolates asm.
- **Parked 23** (`pending/2026-11-04_linux_hand751/differ/`, each with a `// Grade:` line and a passing test):
  - Residues of 1-22 diff lines: register rotation, load order, a dead-slot home.
  - Eleven rows have fewer than 10 grades (`STATUS.tsv`), four of them only one pass-1 grade: 0076e290, 0064a3ed, 0066c880, 0056ef2d.

## Headers (15 changed; each change sits behind a retail macro)
- **C2DBoxF.h was changed by both chunk 749 (landed earlier) and this overlay.** It was three-way merged with `git merge-file`
  against the overlay base 7d2859ef7, which merged cleanly. The other 14 headers were unchanged in the repo since that base.
- hdrcheck2 on the merged overlay: `default=same` for all 15 (`logs/hdrcheck2_pre_merge.txt`).
- New sub-macros:
  - FABLE_RETAIL_CVECTORMAP_ASSIGN (a `__forceinline` CVectorMap::operator=)
  - FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
  - FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
  - FABLE_RETAIL_CSURFACE_PIXELFORMAT
  - FABLE_RETAIL_NCAMERAMODES_CTOOLS
- New declarations behind existing retail branches: CThing, CThingCreatureBase, CMatrix3x4, CParsedItem, CStringParser,
  CTCSoundPlayer, CWorld, CAIStateGroupBase and CDisplayManager.
- **Siblings:** `includers.py` found 653 files; `ppcmp.py` 595 same and 58 different (`logs/ppcmp.txt`). All 58 were re-graded on
  the merged tree (`logs/recheck.log`): 57 RELOCATION_MATCH and 1 MATCH, all PASS. The agent had not re-graded siblings.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04n.tsv` (32 labels). `apply_labels.py` made 21 catalog and 19 manifest renames.
  - Five of the labels qualify the bare catalog names vland wrote for 0045cfad, 0046313e, 0072c930, 007447e0 and 00af56e0
    (GOTCHA: otherwise sync moves them to `_global`).
  - Parameter lists were stripped from label names to follow the catalog convention.
- **Queued PRIORITY** (tag `land0411n` in `NAME_FIXES_QUEUE.tsv`):
  - the C2DBoxF two-vector ctor takes references, not a pointer pair;
  - CCountedPointer<CParticleEmitter>::Reset;
  - CTCSoundPlayer::PlayAttachedCriteria;
  - GFGetShortestDistanceSquaredBetweenPointAndLine.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0, after the qualifying labels.

## Coverage (`write_decomp_dashboard.py`, `update_readme_progress.py`)
46,622 genuine of 49,568 (94.06%); 5,801,862 function bytes (56.74%).
