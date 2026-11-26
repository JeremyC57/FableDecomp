# NF456 hand-back (2026-11-26): respell units landed together with their callers

NF456 continued NF455 under the caller-census rule: callers are found from retail call sites plus compiled objects, and a self respell lands only together with every landed caller that spells it differently. `NOTES.md` covers its method and resume point. `t5_creature_census.tsv` is the census of the `(CThingCreatureBase&)` creature actions.

## Landed: 60 rows in 30 units (+50 proven, +5 best guesses)
| batch | rows | what |
|---|---:|---|
| b1u_dtor_units | 6 | Fireheart, WeaponTrail and LocalDetailPrimitiveMesh destructors QAE→UAE; each deleting destructor now calls `p->X::~X()` instead of a free `X_dtor` stand-in |
| b1n_r456a | 4 | CModeStrafe and CModeSleepBed, the same, with the model moved into `NCreatureMode` (ruling r456a) |
| b2u_shot_alloc_units | 6 | NThingShot ctors 0x004DB15E / 0x004DCAB9 / 0x007BBC65 → `(CThing&)`, with their factories respelt onto the PDB `?Alloc@X@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z` |
| b2fu_fwonly_alloc_units | 10 | 5 FableWin-only ctors (r455c: `one-source:fablewin-pdb` best guesses) plus their factories, whose `Alloc` names are proven |
| b4_thingref_alloc_units | 34 | 17 `(CThing&)` ctors plus their factories |

- **Table:** 13 `add` lines (callers the table did not have) and 17 `replace` lines.
- **Gates (per NF456):** vland RELOCATION_MATCH both sides, objproof ALL_EQ under the intended renames only, link 0 duplicates.
  - Name ties: every deleting destructor sits at slot 0 of its class's vftable, and every ctor ends `ret 4` and stores its RTTI vftable.
- **Coordinator checks:**
  - bindings_multi: 1;
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 2,016 lines, 0 FAIL.
  - Tracker: 25 split_fixable and 25 agree_unverified → agree_pdb, 5 → agree_descriptive, and nothing worse.

## Rulings (coordinator, on standing precedent)
- **r456a: granted.** Moving the NCreatureMode deleting destructors' model into its real namespace renames an invented self to another invented self that nothing binds. This is the same call as r449a (rescope into the PDB namespace).
- **r456b: declined.** The particle-group unit (b1p, 7 rows) would model a class that has a shared header locally, which is the r450b call. Re-port it with an add-only opt-in `virtual ~CEngineInternalPrimitiveParticleGroup();` declaration in `engine/CEngineInternalPrimitiveParticleGroup.h`, and check it with equiv. The rows are under COORD_HOLD_NF456.

## Held or next
- **0x007026A0 CTCQuestCard (+ factory 0x004D2E44):** 0x00702A50 already defines the PDB-spelled ctor, so the respell would duplicate it.
- **0x0072E690 CTCActionUseBuyHouse (+ 0x004D321B):** 0x0072F040's in-class inline ctor must join that unit.
- **0x005EC8A0 CTCTavernGame:** it has 5 callers, one of them under the NF453 lock.
- **0x007413C0 / 0x007437C0 / 0x00743BE0:** their call sites fall outside catalog rows.
- **Creature actions:** 34 of the 55 one-argument `(CThingCreatureBase&)` ctors have one landed caller and no table line. They are the next easy targets.
