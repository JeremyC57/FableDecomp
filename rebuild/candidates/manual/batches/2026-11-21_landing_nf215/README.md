# Landing nf215 (Account A, 2026-11-21): q12300, q12339, q12262 (6 rows)

Agent NF215 (owner, 2026-11-21: "start an agent to continue the naming"; paused for nf214 and resumed). Its brief, state files, notes and
blocked drafts are in `pending/2026-11-21_nf215_handback/`.

- 6 rows land: 1 MATCH and 5 RELOCATION_MATCH, every test PASS.
  - q12300: 0x0096FA90 CAIStateGroup_SummonerCharge::IsRunnable. Respelled in place: its local CCombatManager declares the PDB
    `IsAttackerOn` (0x006E9310) and `IsDefenderDueForAttack` (0x006EA050), and targeting slot 29 `GetPBestEnemyCreatureForAttack`
    returns `CThingCreatureBase*`, so there is no cast. The stand-ins IsCreatureFightingThing / CheckSummonerChargeable are gone.
  - q12339: 0x006ED9E0 CCombatManager::DeregisterDestroyedCombatant. The last binder of 0x00662430 on `IsCombatant` now binds
    `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ` (2 spellings to 1).
  - q12262: 0x004115A0 self rename `??8CBasicStringChar@@QBE_NPBD@Z` -> `??8?$CBasicString@D@@QBE_NPBD@Z` (Ego_r.pdb 0x0040C910 + FableWin.pdb
    0x01899D90; NF202's draft, MATCH). Its three binders move with it: 0x00859040, 0x004AFE20, 0x006238A0, each ported onto fable_string.h.
    - 0x004AFE20 and 0x006238A0 use the new opt-in below with COMPARE_BODY. Their pick-any `==` and `Compare` copies equal the owners (88 / 40).
    - 0x00859040's same-TU UnassignString / ~CCharString / copy-ctor copies equal their owners 0x0099E9B0 / 0x0099EAE0 / 0x0099EC30
      (87 / 12 / 61). Their stand-in names become the header's: ReleaseData -> `?UnassignString@CCharString@@IAEXXZ`, and the counter
      becomes `g_CCharStringInstanceCount_013BD800`, the 0x0099EAE0 owner's existing spelling.
    - UnassignString stays out of line through a guarded `#ifdef _MSC_VER __declspec(noinline)` on the same-TU copy. That replaces HEAD's
      auto_inline(off) region (an established device: same-TU noinline copies, EXTRA_RULES chunk 422).
    - 0x006238A0 now binds `?NullChar@CCharString@@1DB` (0x0129AAF4) where HEAD bound an in-object `""` literal.
- Header (add-only): fable_string.h `FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO`, one new `#elif` arm in the operator== chain.
  No other landed row defines the macro, so the default path is unchanged.
- Recheck (`logs/recheck_summary.txt`, rows in `logs/recheck_rows.txt`): 4,509 rows = the direct includers of fable_string.h, the six
  rows, the callee rows, and every landed caller of 0x004115A0 / 0x00662430 / 0x006E9310 / 0x006EA050 / 0x0099E9B0 / 0x00411570. 4,507
  good; 0x00DA8510 and 0x00E0A510 are bare-CompilerFlags entries that recheck.py mis-grades, and both are RELOCATION_MATCH with their own
  `// Flags:` lines (`logs/regrade_partial_flags.txt`).
- Old-spelling census (`logs/tablecheck.txt`): no rechecked object references any old spelling externally. NF202's "about 20" binders were
  3 binders plus 13 rows that define a dead `??8CBasicStringChar` copy, whose only relocation is `.debug$F` frame data. Those 13 copies
  stay as HEAD debt and are queued.
- Link (`logs/link_check.txt`, 4,509 objects, `/FORCE`): duplicates 423 at HEAD -> 407. The only increase is `UnassignString` +1
  (0x00859040's same-TU copy against owner 0x0099E9B0, equal bytes).
- reloc_bindings.tsv: 4 stand-in witnesses removed, 2 PDB first witnesses added. 0x004115A0 goes 4 -> 3 spellings and 0x00662430 2 -> 1;
  none grows.
- Blocked (in the hand-back): q11760 / q11873, 0x006D41F0 (the operator!= form gives DIFFER(307v295); FINDINGS 2026-11-21; needs a
  fable_string.h access claim). q12265, 0x00EE2A80 (not attempted).
- Queued (tag land1121nf215): NF215's four name fixes and the family leftovers line (13 caller copies, 40 TEXT_EQ_INLINE definers, the
  other definers). The caller copies went to agent NF216.
