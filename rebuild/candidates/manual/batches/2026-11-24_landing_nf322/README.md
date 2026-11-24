# Landing nf322 (Account A, 2026-11-24): 60 flat-header rows onto polymorphic class models (40 add-only header opt-ins)

Hand-back: `pending/2026-11-24_nf322_handback/`; helpers under `naming_batch/agent_tools/nf322/`.
- **Entries:** 144 RTTI-named virtual members of classes whose shared header model is flat (`void* __vftable`, no virtuals).
- **Outcome:** 60 done, 28 skipped-held, 56 skipped-port.

## What landed
- **Headers.** Each of 40 headers gets one new opt-in: `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` (polymorphic model) / `#else` (flat
  model, text unchanged) / `#endif`.
  - `genmodel.py` generated each model from the class's retail vftable, found through RTTI, and the RTTI-port labels.
  - **8 CInputType\* classes:** no base, so `__vftable` becomes the virtual declarations in slot order.
  - **32 classes:** derive from their real base and declare only the slots RTTI names for the class.
  - Members are unchanged, and static asserts inside the opt-in pin sizeof and every offset.
- **Rows.** 60 rows define the opt-in and `X::M` in the exact PDB spelling: 26 CInputType, 13 CAIStateGroupBase-derived, 10 Def
  (Transfer / GetSizeofClass), 2 CGroundNavigatorExternalInterface, 6 hover / WhisperPotionCombat, 3 others. No test changed.

## Proof
- **Per row:**
  - precheck;
  - vland HEAD and new compiled under one work path, all MATCH / RELOCATION_MATCH PASS;
  - libcp.py: one definition rename, equal UND / DEF sets, no COMDAT added or dropped, no `??_7` / `??_R`, all sections equal;
  - `.drectve` equal on 58 rows; 0x006CD9B0 and 0x006CDAB0 gain only `/DEFAULTLIB:"libcp"` (NF318 Q1, NF322 Q3);
  - per-batch links with 0 duplicates;
  - no landed row or table line binds these addresses.
- **Headers:**
  - removing each opt-in gives the original header byte for byte (the only edited existing lines are 86 assert pins);
  - equiv_check over 57,146 rows and tests: 0 differing;
  - the planted control flags 126 lines, including all 60 rows' HEAD sources.
- **Slot-order probe:** per class, VC7.1 emits the vftable, and it matches retail slot for slot. The only differences are the `??_E` /
  `??_G` alias at slot 0 and the unlabelled CInputType `operator==`.
- **At landing:**
  - all 160 orig/ copies equal HEAD;
  - recheck of the 60 rows plus every direct includer of the 40 headers: 114 of 114 PASS (45 MATCH, 69 RELOCATION_MATCH);
  - every row's graded symbol equals its RENAMES spelling.

## Rulings on NF322's questions
- **Q1 (allowed):** widening CCombatSequenceBase's `#if defined(..._PASS27)` guard with `|| defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE)` is in
  effect an opt-in, because no TU defines the new macro. Conditions:
  - the widened region holds only the class model (no inline bodies, no pass-specific members), and every other `_PASS27` use under
    rebuild/include is checked;
  - model proof is the compiler-emitted `??_7` probe against retail, slot for slot; declaration order alone is not proof;
  - equiv_check plus a planted control inside the block that flags the `_PASS27` definers.
  The same ruling covers the `_PASS24` hold (0x00A85FB0 / 0x00A86030). Once landed, the switch joins `SWITCHES` in flatten_pilot.py.
- **Q2 (part of the port):** a new base header is part of the class-model port when:
  - the base comes from `rtti_bases.tsv` and `gen_class_headers.py` produces the flat header from the PDB layout;
  - it then takes the opt-in model, the probe and the asserts, and lands with or before its rows.
  A class absent from the layout TSV is a separate authoring unit.
- **Q3:** NF318 Q1 covers libcp on header-route rows.

## Queued
- 84 T3 lines `nf322` (28 held, 56 port).
- Two class-model defects found by the probe:
  - CEngineInternalPrimitiveBase SetChildPrimitive overloads at slots 7/8 are reversed against retail, and
    CEngineInternalPrimitiveParticleGroup lacks GetRenderLayerMask at slot 28. Landed calls through slots 7/8 must be checked for a misbound
    overload name.
  - CAIStateGroup_EnemyCreatureBase is missing its override declarations at slots 1, 2, 9 and 11.

## Recheck
`logs/recheck.log` (landed tree, 114 rows): 114 PASS.
