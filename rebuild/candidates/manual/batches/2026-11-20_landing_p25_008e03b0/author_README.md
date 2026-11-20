# 0x008E03B0 NCreatureMode::CModeGuildmasterPowerUpSpeed::OnActivate

**Grade:** RELOCATION_MATCH (811/811). No flag, no pragma, no header change.
- **Strict vland:** passes. LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`, fresh LANDVERIFY_WORK (`vland.log`).
  - Its two warnings are RELOC_NAME: the labels DB has placeholder names for the two STLport `_M_insert_overflow` COMDATs (0x007AFD10,
    0x004AC940).
  - Both go in NAME_FIXES.tsv as labels-DB respells. The landed Shield sibling 0x008E0060 binds the same two symbols.
- **Behaviour test:** passes on the real object. 6 runs drive the object's own thing-map walk, GetSubDef and GetEffectFromString; every
  undefined callee is a recording stub.
- **Mutants:** 17/17 KILLED (`mutants/008e03b0.out`).
  - One byte- and behaviour-neutral mutant SURVIVES by design: the Ego_d `ptc_multi_strike` block against a bare AddTC statement
    (`.dead.out`).
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.
- **Name:** Ego_r.pdb 0x00820330 and FableWin.pdb creature_modes.obj #282 both spell it
  `?OnActivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ`. FableWin's copy is 2481 B, the same size as Ego_d's.

## What changed against the draft (pending/2026-11-11_linux_handQ8/differ/008e03b0.cpp, DIFFER(809v811))
- **Rewrite:** the draft's local models are gone. The source is a copy of the landed Shield sibling 0x008E0060, which is on the shared
  headers (same TU, creature_modes.obj), with the Speed differences:
  - no CastPhysicalShieldSpell action;
  - `if (!guards[random_guard]->HasTC(TCI_MULTI_STRIKE)) { CTCMultiStrike* ptc_multi_strike = ...AddTC("CTCMultiStrike", false, 0); }`,
    the Ego_d block and local.
- **The draft's residue was the search filter.** Retail calls the 0x008E4100 instance,
  `CFilter_Bind2<CThingFilter_HasDefGlobalIndex, `anonymous namespace'::CThingFilter_IsHoldingMeleeWeapon>`.
  - The source builds it with GFFilter_Bind2. FableWin.pdb has that instance, and retail expands it: the two pointer words are stored at
    0x008E0413 / 0x008E0422.
  - The anonymous class is the landed 0x008E4100 text plus its user default constructor. FableWin.pdb has
    `??0CThingFilter_IsHoldingMeleeWeapon@?A0x..@@QAE@XZ` at 0x02863170, directly after this function in creature_modes.obj, and Ego_d has
    it too.
  - Without the constructor, the temporary is value-initialised (`mov byte ptr [esp+17h],bl`).

## Ablation (each item removed singly from the final source; `evidence/ablations/`)
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| no CTHINGSEARCHTOOLS_ROUGHAREABODY (the instance body is opaque) | DIFFER(823v811) |
| no CDEFCLASSBASE_GETSUBDEF_BODY | DIFFER(813v811) |
| no CINTELLIGENTPOINTER_COPY (the implicit member-wise copy) | DIFFER(784v811) |
| no user ctor on CThingFilter_IsHoldingMeleeWeapon | DIFFER(827v811) |
| the plain HasDefGlobalIndex filter (the Shield / 0x008E3EB0 instance) | DIFFER(795v811) |
| the explicit CFilter_Bind2<..>(..) temporary instead of GFFilter_Bind2 | RELOCATION_MATCH (GFFilter_Bind2 kept: both PDBs have the instance) |

- **Line 2:** names the three opt-ins with these grades.
- **Define sweep:** the Shield define set was swept singly (`define_sweep_on_v4.txt`). 16 defines grade RELOCATION_MATCH when removed and
  are dropped from the source.
  - CMODESTANDBASE_ACTIVATE was kept although it is byte-neutral. Dropping it rebinds the base call to
    ?OnActivate@CModeBase@CTCCreatureModeManager@@.
  - The final object's undefined-symbol set is identical to the full-define build's (checked).

## Notes for the coordinator
- **Ego_d `duration_in_frames`:** Ego_d has this local (line 4242, 227 B, the same size as Shield's line 4091). Retail has no trace of it:
  the statement is dead, and RELOCATION_MATCH with it (`evidence/alt_with_duration_in_frames.cpp`, 12.0f borrowed from Shield).
  - I left it out because its constant is unknown. It is coordinator-overridable.
- **Fix for 0x008E4100:** the landed 0x008E4100 should get the same ctor so the anonymous class reads the same in both places. With it,
  0x008E4100 is still RELOCATION_MATCH (`evidence/008e4100_with_pdb_ctor.cpp`).
