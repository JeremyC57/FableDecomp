# Landing nf257 (Account A, 2026-11-22): CCreatureAction_SummonCreatures and CCombatAction_WillHealInto on their 81-slot vtables, CAIStateGroup_WhisperRetreat::Update (9 rows)

Agent NF257 (q12722, q12723, q12724 done; q12659 open). Hand-back: `pending/2026-11-22_nf257_handback/` (evidence/ = vtmap, slot owners,
equiv_final.log).
- **L72 CCreatureAction_SummonCreatures (5 rows):** ctors 0x008B83C0 (leaf unchanged), 0x008B83E0 (PAV -> `AAV@@0_N`), 0x008B8440 (PAV ->
  AAV), Clone 0x008B89A0 (QAE stand-in -> UBE), `??_G` 0x008B87A0 (free `__fastcall` stand-in -> the member); both PDBs throughout. All
  four ctors / Clone store vtable 0x0127ADDC (HEAD: one shared stand-in table): own slots 0 / 11 / 12 (Initialise 0x008B7A20 already
  landed). vtmap identical across the five objects; all 81 slot symbols name their retail owners and are in a PDB name set; 77 / 77
  shared slots equal 0x00955620's table; the `??_G` copy EQ 0x008B87A0; 0 link duplicates.
- **L100 CCombatAction_WillHealInto (3 rows):** ctor 0x00956870 PAV -> AAV, Clone 0x00956A20 stand-in -> EBE (private virtual const), `??_G`
  0x00956940; own slots 0 / 4 (FinishAction 0x008C2AD0) / 11 / 12 (Initialise 0x008C0560); 0 owner errors, 76 / 76 shared slots equal
  0x00955620's, 0 link duplicates; the header's includers 0x00955620 / 0x006F8010 rechecked.
- **q12724 0x0096B600** CAIStateGroup_WhisperRetreat::Update: ten row-local class models -> the shared headers (form 2: creature and target
  named before the two GetPos calls). 0x006649C0 onto `?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ` (2 -> 1), 0x0096AE50 UseWill QAE
  -> IAE (2 -> 1), 0x0096AC20 JumpToSafety QAE -> IAE (both PDBs). Table: 3 stand-in lines deleted, 2 IAE lines added.
- **Headers (add-only, declarations only):** CCombatActionBase.h `MELEE_PRIORITYCTOR` (protected `CCombatActionBase(long)`, `IAE@J` both PDBs),
  CCreatureAction_SummonCreatures.h `SUMMONCREATURES_OVERRIDES`, CCombatAction_WillHealInto.h `WILLHEALINTO_OVERRIDES`,
  CAIStateGroup_WhisperRetreat.h `WHISPERRETREAT_UPDATE` (one assert pinned). Whole-tree equiv_check with all four: 0 of 57,125 differ.
- **Recheck** (`logs/recheck.log`, landed tree): the 9 changed rows + the includers / binders 0x00955620, 0x006F8010, 0x008B9010, 0x0091C6A0,
  0x008B7A20, 0x008C2AD0, 0x008C0560, 0x0096AE50, 0x00664A50, 0x006649C0 (19 rows): 19 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF257's questions
1. L72 as a five-row unit (all three ctors and Clone store 0x0127ADDC and shared one stand-in table at HEAD; 0x008B83C0 is the `??_7`
   witness): accepted, 0x008B83C0 and 0x008B8440 included.
2. The new `MELEE_PRIORITYCTOR` macro (landed 0x008B9010 defines both MELEE and PRIORITYCTOR): accepted.
3. Header-based tests for the 9 rows: queued.  4. Port debt (0x008B7A20, 0x006F5C10, 0x008C2AD0, 0x008C0560 (no Leaf line), 0x0096AE50):
   queued.  5. q12659 to the next agent.
Rule slips (0x0096B600's form-2 grade launched in the same call as its clang precheck, ungated; one recheck started on a bad payload,
stopped by PID): noted.
