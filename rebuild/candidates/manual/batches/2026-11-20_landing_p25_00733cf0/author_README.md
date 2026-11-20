# 0x00733CF0 CTCCombat::UpdateGetHitCounterAttack (p25N, 2026-11-20)

**Grade:** RELOCATION_MATCH (288/288). No flag, no pragma, no inline device, no Inferred line. Shared headers only (no local models).
- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, fresh LANDVERIFY_WORK, behaviour PASS. One
  warning, RELOC_NAME for 0x00738A80 (garbled label-db name of the GetEvent instance; `NAME_FIXES.tsv`).
- **Base:** a new port. The prior drafts (pending/2026-10-30_parked_retry2_sec2b, 2026-11-10_linux_hand1017, best DIFFER(285v288)) were
  local models with an invented free function `GetWorld_00686a80()`.

## The residue and its fix
Retail calls CTCBase::PeekWorld (0x00733D1C) without loading ecx. The drafts called this an LTCG this-less call. That reading was wrong:
- GetCombatAbilityData 0x00733620 never writes ecx (`mov eax,[ecx+70h]` ... `ret 8`; ecx only read).
- With its body compiled earlier in the same TU, VC7.1 knows `this` is still in ecx after the call and drops the reload. This is the
  GOTCHAS nf96a mechanism (precedent 0x005C150B).
- Declaration-only GetCombatAbilityData: DIFFER(297v288).

The port then matched on its first grade, with IsValid / SetAsActive in the TU as Ego_d places them (see Same-TU bodies).

## Same-TU bodies (all `// Fidelity:`; each compiles to its retail out-of-line copy, cmpcomdat SAME)
Ego_d.pdb's line table puts CCombatAbility::IsActive / IsValid / SetAsActive in tc_combat.cpp: file id 3264, the same file as this row and
GetCombatAbilityData. Retail keeps out-of-line copies of all four:
| function | retail | bytes | in the row |
|---|---|---|---|
| CCombatAbility::IsActive | 0x00733090 (new row, batch 1) | 11 | expanded (inside IsValid) |
| CCombatAbility::IsValid | 0x007332F0 (re-port, batch 2) | 71 | expanded at 0x00733D2A.. |
| CCombatAbility::SetAsActive | 0x007330A0 (new row, batch 1) | 28 | expanded at 0x00733DEB.. |
| CTCCombat::GetCombatAbilityData | 0x00733620 (re-port, batch 2) | 106 | called (the ecx knowledge) |

Ablation (`evidence/construct_ablation.txt`):
- GetCombatAbilityData declaration only: DIFFER(297v288).
- IsValid declaration only: DIFFER(244v288).
- SetAsActive declaration only: DIFFER(286v288).

- **Landing order 3 of 3.** The four copies are the plain `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)` form, verbatim
  from the companion batches:
  - `batches/2026-11-20_ccombatability_newrows`: new rows 0x00733090 / 0x007330A0 (+ 0x007330C0 SetAsFinished, 0x007330E0 Reset).
  - `batches/2026-11-20_tc_combat_reports`: re-ports of landed 0x007332F0 / 0x00733620 onto the shared-header text.
  - All seven files share one define block; a script built the copies from the same body strings as the companion row files.

## Text oracle (Ego_d)
- **Locals:** this, hit_by, it { pevent_hit, ptc_entity_events { attacker_creature } }. The source follows them.
- **Line sizes at /Od** (`evidence/od_lines_row.txt`, made with `evidence/odlines_coff.py` from the COFF line table of a /Od /Z7 compile):

| Ego_d line | Ego_d bytes | ours | note |
|---|---|---|---|
| 1117 | 15 | 8 | `it`: Ego_d is a Dinkumware build |
| 1118 | 89 | 89 | |
| 1122 | 100 | 100 | |
| 1127 | 20 | 20 | |
| 1129 | 46 | 44 | |
| 1132 | 37 | 37 | |
| 1135 | 8 | 8 | |

- **One condition (1118)** holds both the lookup and IsValid. `if (!GetCombatAbilityData(..)) return;` + `if (IsValid(..))` is
  byte-equal in release (`evidence/d_early_return.cpp`, RELOCATION_MATCH), but at /Od it needs two more line records than Ego_d has.
- **GFFilter_Bind3(..):** FableWin.pdb has this exact factory instance. It gives line 1122's 100 bytes; the CFilter_Bind3 constructor
  spelling gives 99.
- **`attacker_creature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0)`:** FableWin.pdb has
  ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingCreatureBase@@@@.. and the landed precedents are 0x007C6460 and 0x0079C880. It gives line
  1127's 20 bytes; the plain cast gives 9 and is byte-equal in release.
- **`GetParentThing().PeekTC(&ptc_entity_events)`:** the const lookup is the one retail calls (LowerBound 0x004365B0).

## Header changes (`hdr/`, `hdr.diff`; strictly add-only)
| header | opt-in | content |
|---|---|---|
| CTCCombat.h | **new** FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK | declarations: private `GetCombatAbilityData(TCombatAbilityList::iterator&, ECombatAbilityInterfaceType)` (after the TCombatAbilityList typedef), public `UpdateGetHitCounterAttack(const CThingCreatureBase&)` |
| CCombatAbility.h | **new** FABLE_RETAIL_CCOMBATABILITY_MEMBERS | declarations: public IsActive / IsValid / SetAsActive / SetAsFinished / Reset (the same overlay as the two companion batches) |

- The only `-` lines in hdr.diff are CCombatAbility.h's four FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT pins (original lines 11, 29,
  30, 31). CTCCombat.h's asserts were already pinned.
- **newly_flagged_rows:** none (`newflags.py hdr/`).

## Tests
- **`test/00733cf0.cpp`** (payload test, passes in vland) links the REAL row object. The same-TU callees and the const LowerBound run for
  real; the out-of-line callees are `/alternatename` __fastcall stand-ins. GetEvent<..> is an explicit specialisation carrying the
  instance's exact symbol.
- **Fixtures:** a two-node STLport tree, the component map, and an action vtable.
- **14 scenarios × 3 seeds:** no type-9 ability; active (timer 10 and 0); repeat timer running; roll wins / loses / straddling chances
  18 and 19; no events component; no event; not performing; not interruptible; first node matches; negative chance (unsigned roll).
- **What it checks:** PeekWorld's `this` (must be the CTCCombat, via the ecx knowledge), seed stepping, GetEvent's component and
  filter.HitBy, the ActiveTimer store, FinishCurrentAction, and that nothing else is written.
- **Mutants:** 18/18 KILLED (`mutants/00733cf0.out`), including a header mutant (RepeatTimer / ActiveTimer swapped).

## Define ablation (`evidence/define_ablation.txt`)
- Each define was removed one at a time. 12 were byte-neutral: CCHARSTRING, CCLASS, CDEFINITIONMANAGER, CGAMECOMPONENT, CINTELLIGENTPOINTER,
  CMAINGAMECOMPONENT, CRESOURCELIST, CTHING, CTHING_GETTC_EXPANDED, CWORLD, SCOPED_PTR, STLP_LIST.
- Removed together, all seven tc_combat.cpp files still grade as before, so they are dropped from all seven.
- Every remaining define gives SRC_FAIL without it.

## NAME_FIXES.tsv
| address | fix |
|---|---|
| 0x00733CF0 | catalogue name TCCombat_ApplyBlockResponse |
| 0x00733620 | landed public `QAE ... _Rb_tree_iterator@@J` spelling -> private enum PDB spelling |
| 0x00738A80 | garbled label |
| 0x00733090 / 0x007330A0 / 0x007330C0 / 0x007330E0 | new rows (batch 1): IsActive, SetAsActive, SetAsFinished, Reset |
