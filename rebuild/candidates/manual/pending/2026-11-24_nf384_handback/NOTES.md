# NF384: T1 split_fixable binders (116 targets), hand-back notes

Agent NF384 for Account A. Work dir `$S/w22/agentNF384`. Nothing was written into the repo and no git command changed anything. One read-only `git status` was run by mistake; it printed nothing.

## Result
- **batches/b1: 10 binder rows, all ACCEPT.**
- **Table:** 10 lines in `TABLE_CHANGES.tsv` (7 replace, 3 delete) and 5 table-only lines in `HAND_TABLE_CHANGES.tsv` (2 rewitness, 3 delete).
- **STATUS.tsv** covers all 116 targets:

  | Status | Count |
  |---|---|
  | done | 8 |
  | done-partial | 3 |
  | skipped-offlimits | 82 |
  | skipped-port | 18 |
  | skipped-held | 3 |
  | skipped-identity | 1 |
  | skipped-t3-entangled | 1 |

- **Compiles:** 84 object grades (each with its test), plus one vt.py test compile:
  - runpair r1: 10 rows, HEAD + new = 20;
  - r2 and r3: 0x0089FBB0 again after its test fix, 4;
  - r4: 2 wasted. An evidence-comment edit to 00838bc0 failed its assert and r4 graded the unchanged text;
  - r5: 00838bc0 regraded with the corrected comment, 2. objsec r1n v r5n is ALL_EQ with 0 renames, so the r1 link and census objects stand for it;
  - census c1: 21 HEAD (stale witnesses plus callee / context owners);
  - census c2: 35 HEAD (every other retail caller of 0x007137D0 and 0x006C9FA0).

## Triage (tools/screen384.py, look384.py; work/screen.tsv)
The screen uses the full reloc_bindings lines, not the TARGETS `spellings` column, which is cut. Each target has wrong-spelling binder lines; 137 binder lines in all.

Stay-off flags on the binder:
- T3 means the binder's address is in column 2, or a `0x` address in column 3, of T3_PORT_QUEUE. Unlike NF374's screen, a mention in the reason column does not count, per this brief. 91 lines: 89, plus 2 whose target also carries the B claim.
- P28 (Codex reservation), 1 line: 00647ab4.
- Account B claim on the target, 2026-11-20, 3 lines: 0x00859EF0.
  - The newest line for that item is still `claimed`, but B landed the row the same day (COORDINATION_LOG 2026-11-20 B->A, batch landing_p25_00859ef0).
  - I edited only free binders of it (00908b50, 00906380), never B's row. Ruling asked below.

**All 91 T3 lines are skipped.** 82 targets have no free binder at all (skipped-offlimits).

The 45 free lines were read by hand. Each is one of:
- a **declaration-only respell**: taken, 10 rows;
- a **stale witness**: the HEAD object already binds the PDB form; table only;
- a **port**: the binder's own symbol is a stand-in that the callee's class would re-spell (a self port). Or the fix changes a template instance or vtable, or needs `/alternatename`;
- **held**: 0052c7e0 (held list, nf380 q7), and 00ad77f0 (NF374 b1_hold RELOC_CONFLICT);
- **identity**: 00698d10. The target self row 0x00789CC0 is itself wrong; see NAME_FIXES.

The reasons per line are in STATUS.tsv and tools/status384.py.

## batches/b1 (land with land_scripted; binder run, every reason carries `; target 0x...`)

| Row | Target(s) | Change (local model only) | New callee spelling(s) |
|---|---|---|---|
| 00b428e0 | 00bda070 | made-up `CStaticMapPool` -> `CEngineLandscapeMemoryManager` | `?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z` |
| 0089fbb0 | 00779e90 | free `CreateExperienceOrbThing` -> static `CTCDExperienceOrb::Create`; +0 `static_cast<CThingNative*>` of the CThing* result | `?Create@CTCDExperienceOrb@@SIPAVCThing@@JABVC3DVector@@@Z` |
| 006d5260 | 006d4fd0 | AddEmoteIcon protected, `CThing&`; calls pass `*GetPItem()` | `?AddEmoteIcon@CTCFollowed@@IAEXAAVCThing@@@Z` |
| 00960760 | 006c9fa0, 006cef20 | `CTCTutorialGiver` -> `CTCHero` (ETutorialCategory via engine/ETutorialCategory.h as 005ee418, bool); `CTCTutorial::OnDeedReactionDone` -> `CTCWife::InitiateDivorce` | `?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z`, `?InitiateDivorce@CTCWife@@QAEXXZ` |
| 00838bc0 | 007cca60, 007cca80 | made-up `CTCWeaponModifier` -> `CTCMultiStrike`; CausesKnockDown -> `IsLastHit() const`; GetDamageMultiplier -> `GetPerHitDamageMultiplier() const` | `?IsLastHit@CTCMultiStrike@@QBE_NXZ`, `?GetPerHitDamageMultiplier@CTCMultiStrike@@QBEMXZ` |
| 0091a9b0 | 00662970 | `NewCreatureInteraction(ECreatureInteractionType)`: MS forward enum as engine/CCreatureInteraction.h, literal `(ECreatureInteractionType)8`, no made-up enumerator | `?NewCreatureInteraction@CThingCreatureBase@@QAEXW4ECreatureInteractionType@@@Z` |
| 00733950 | 00663260 | `const CAnimationSet& GetCombatAnimationSet() const`; locals take its address (as NF374's 00733910) | `?GetCombatAnimationSet@CThingCreatureBase@@QBEABVCAnimationSet@@XZ` |
| 00713ee0 | 00664cb0 | `CThing::GetSneakSpeedTowards` (made up) -> `CThingCreatureBase::GetMaxSneakingSpeed const`; reached by a +0 `static_cast<CThingCreatureBase*>(GetCreature())` | `?GetMaxSneakingSpeed@CThingCreatureBase@@QBEMABVC3DVector@@@Z` |
| 00908b50 | 00859ef0 | `CCombatActionBase::OnAnimationLoaded` protected (as its FinishAction already is) | `?OnAnimationLoaded@CCombatActionBase@@MAEXXZ` |
| 00906380 | 00859ef0 | the same, sibling with the identical model; no table line names it | the same |

- **New spellings:** all 11 are exact lines in both PDB name sets (evidence/pdb_exact.txt).
- **Owners:** each new spelling is defined in the link by its target's landed owner (evidence/self_names.txt, link_r1.txt), except GetPerHitDamageMultiplier. 0x007CCA80's own row still defines a stand-in free function, so that name is unresolved in the link, as the old one was (NF302 Q2; NAME_FIXES / QUEUE nf384).
- **0x007CCA80 identity** (a second target, moved with 00838bc0's class):
  - Ego_r 0x007CAFB0 / 0x007CAFD0 and FableWin 0x02731DD0 / 0x02731E50 list IsLastHit then GetPerHitDamageMultiplier, the same order as retail 0x007CCA60 / 0x007CCA80.
  - The retail body returns PDef->PerHitDamagerMultiplier[SpellLevel]. In Ego_r, CTCMultiStrike has PDef at +0x0C and SpellLevel at +0x10. The def's NumHits / PerHitDamagerMultiplier vectors sit at retail +0x78 / +0x84. IsLastHit compares NumHits (+0x1C) with NumHits[SpellLevel]-1.
  - The catalog's CTCDivineWrath::GetDamageMultiplier label is propagated over five addresses.
- **00960760 leaves `CTCTutorial::IsActive`** (0x006CFE80 = PDB CTCWife::IsAttractedToHero) alone: 0x006CFE80 is a T3 nf303 column-2 address. Interface 0xD7 is therefore modelled by two local classes until that port.
- **Tests:** two are respelled, the vland "new" runs used them, and they are in b1/tests/.
  - 00b428e0: whole-word CStaticMapPool.
  - 0089fbb0: decl, call and stub, with C-style casts, because the test's CThingNative does not derive from CThing.
  - land_scripted copies rows/ only, so copy tests/ by hand (apply_rows.py).
  - Three tests keep their own copies of the old stand-in names: 00960760 (CTCTutorial / CTCTutorialGiver with inline bodies), 00838bc0 (a data-only CTCWeaponModifier) and 0091a9b0 (an inline NewCreatureInteraction(long)). They are standalone behaviour harnesses that bind nothing, and respelling them needs the enums. Left as test-name debt.
  - The 00908b50 / 00906380 tests keep a public OnAnimationLoaded: same name, different access.
  - The 006d5260, 00733950 and 00713ee0 tests name none of the changed members.

## Gates (every row; evidence/ and b1/evidence/)
- **vland:** HEAD and new are RELOCATION_MATCH with the test PASS, at one LANDVERIFY_WORK path. Eight rows ran in r1 (work/wd_r1h, wd_r1n); 0089fbb0 ran in r3 and 00838bc0 in r5.
  - r1's new 0089fbb0 test failed to compile (TCC_FAIL) before the test fix.
- **objsec374:** ALL_EQ, `.drectve` BYTE_EQUAL, COMDAT counts equal, and only the intended renames:
  - one rename each in eight rows;
  - two in 00960760;
  - three in 00838bc0. The third is its forceinline `CThing::GetInterface<T>` COMDAT, which follows the class: `??$GetInterface@UCTCWeaponModifier...` -> `??$GetInterface@VCTCMultiStrike...`. That instance is in no name set, no table line and no self.
- **pk_all + pickany_vs_retail:** HEAD = new under the renames for 10/10 rows, and 0 reach flips (pkdiff_r1.txt, reachflip_r1.txt).
  - A first pk run without the lane env gave `leaf None`. It was rerun with env.sh sourced; those are the results above.
- **/FORCE /MAP link:** the 10 rows plus 15 callee / context objects (link_r1.txt).
  - The same 2 duplicates HEAD and new: two CVectorMap LowerBound copies in 00960760 and another context object. 0 new.
  - Nothing HEAD resolved is unresolved new.
  - 11 old names went from unresolved to gone. The new names resolve to the target owners, except GetPerHitDamageMultiplier (above).
- **xr** (xr_r1.txt): each renamed leaf relocation resolves to its retail target.
  - 00908b50 and 00906380 bind OnAnimationLoaded only through their emitted vftable, so the link shows it (MAE defined in 00859ef0.obj).
- **Census by compile** for every deleted / replaced / rewitnessed spelling (census384.py; census_c1.txt, census_c2.txt). Only row objects are counted, because the `.t.obj` test objects define stubs.
  - Replace lines: 00b428e0, 0089fbb0, 00960760 (OnDeedReactionDone), 00838bc0 (x2), 0091a9b0 and 00713ee0 are each the only retail caller of their target (callers_exe.tsv).
  - Old names: no other source names CStaticMapPool, CreateExperienceOrbThing, CTCWeaponModifier, CausesKnockDown, GetSneakSpeedTowards, CTCTutorialGiver or OnDeedReactionDone.
  - AddEmoteIcon PAV: 006d50c0 compiled, does not bind it. Delete.
  - GetCombatAnimationSet QAEPAV / QAEPBV: all 3 retail callers (00733910, 00733950, 008e7040) compiled; none binds either after b1. Delete both; QAEPBV is a HAND line, its witness 00733910 being a stale NF374 row.
  - CTCTutorialGiver form: all 20 retail callers of 0x006C9FA0 compiled; none binds it after b1. Delete.
  - J form `?GiveTutorialForCategory@CTCHero@@QAE_NJ_N@Z`: 005ee418 does not bind it (stale witness); 0057c2e6 (T3, compiled read-only) binds it. HAND rewitness.
  - AddAction PAU-global (007e7760) and PAV-NTCScriptedControl (00714d30): all 20 retail callers of 0x007137D0 compiled; none binds either. 18 bind the `PAUCActionBase@NTCScriptedControl` form, 00904cd0 binds PAX, and the two witnesses bind the PDB form. HAND delete x2.
  - OnAnimationLoaded UAE: 00908a20 (T3, compiled read-only) still binds it; 006ebdd0 does not. HAND rewitness, applied only when 00908b50 lands.
- **apply_table.py --dry** (apply_table_dry.txt): TABLE, HAND and both combined give 0 misses; multi-target 1 before and 1 after, no new one.
- **WARN delta** (b1/logs):
  - SHARED_HEADER_AVAILABLE 69 -> 74: the five new local models CEngineLandscapeMemoryManager, CTCDExperienceOrb, CTCWife, CTCHero and CTCMultiStrike. The old stand-ins had no header. Local models are kept as in NF381, because the rows' CTCBase / CThing stand-ins would collide with the headers.
  - RELOC_NAME 19 -> 17. The one new warn is the wrong 0x007CCA80 catalog label (NAME_FIXES).
- **Claims screen:**
  - None of the ten rows is in T3 column 2/3, Codex RESERVED, COORDINATION_CLAIMS (B), UNMATCHED_ROWS, NF382 (nf380 q2/q6/q9), NF383 (NF378 skipped-conflict, nf380 q5) or stayoff_nf378_b12.
  - 00960760 appears only as a comment in an old B header copy (pending/2026-11-11_acctB_c2004/hdr/engine/CAIStateGroup_DeedReactionRoot.h); it is not a staged row.
  - No row has an includer: naming_status.includers_of returns [] for all ten. So there is no option S or funclet regrade.
  - Open claims (land_scripted step 1): the COORDINATION_CLAIMS lines naming 0089fbb0 (A q-claims) and 006d5260 (A NF370 / NF373) each have a newer `done` line, so no hold is expected.
  - 0057c2e6 (a HAND witness): its claim lines are A `done` lines; its acctB hits are copies of engine/CTCHeroStats.h in old pending header folders, not a staged row.
- **Pick-any validity:** all 20 pk_gate/*.pkall.tsv files show `source_a=yes`.
- **Shipped equals graded:** mkrun384.py asserts that rows/ and tests/ equal the new payload texts, and that orig/ and head/payload.json equal the landed repo text.

## Rulings asked
1. **Casts at the call into a real PDB class:**
   - 00713ee0 does a +0 `static_cast<CThingCreatureBase*>(GetCreature())`; T3 nf305 calls the CThing -> CThingCreatureBase move a class port.
   - 0089fbb0 does a +0 `static_cast<CThingNative*>` of `CTCDExperienceOrb::Create`'s CThing*.
   - Both match NF377's "a cast at the call" (0x007FDF60). Bytes and objsec are equal.
2. **0x00859EF0** carries Account B's 2026-11-20 author claim (newest line `claimed`), but B landed it that day. I treated only the binders as mine. Hold 00908b50 / 00906380 if the claim still binds.
3. **00838bc0 also names 0x007CCA80**, which is not one of the 116 targets. The evidence is in NAME_FIXES. If the coordinator wants only the target's change, the alternative is a cast between a stand-in and the real type (nf105 debt), which I avoided.

## Hand steps
1. Release check: NF384's ten locks are released (evidence/locks_final.txt), so land_scripted can lock them as COORD.
2. Land b1 with land_scripted. Copy b1/tests/ (2 files) by hand. Apply b1/HAND_TABLE_CHANGES.tsv after a dry run:
   - the OnAnimationLoaded rewitness only if 00908b50 landed;
   - the other four lines are independent of the landing.
3. NAME_FIXES.tsv:
   - 0x007CCA80 label / self;
   - 0x00789CC0 self + label (wrong self: CTCHeroRangedAttacks::GetProjectileWeaponFirstPersonTargetingModeZoomFOV);
   - 0x006CFE80 binder note.
4. QUEUE_PROPOSALS.tsv: 7 lines (nf384). No ACCEPT address appears in column 2 or 3.
5. Shared-header debt: the five new local models above. Port them when the rows' other stand-ins move; not queued, so as not to reserve ACCEPT rows.

## Tools (tools/)
- Copies of the NF374 / NF377 / NF381 tools, retargeted to NF384: nf.py, objsec374.py, pk374.py, pkdiff374.py, reachflip_coordinator.py, runpair.sh, headonly.sh, callers377.py, link377.py and others.
- New:
  - **screen384.py:** the stay-off screen per this brief.
  - **look384.py:** binder source lines.
  - **incl384.py:** includers.
  - **gen384.py:** the exact row and test edits.
  - **census384.py:** old-spelling census over compiled objects.
  - **status384.py:** STATUS.tsv.
  - **mkrun384.py:** the run dir.
