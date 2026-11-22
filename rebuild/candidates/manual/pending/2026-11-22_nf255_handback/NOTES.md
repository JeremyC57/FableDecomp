# NF255 notes (2026-11-22, round 20b): the blocked CCreatureAction ctor rows, one class per pass

Base: the repo tree with nf253 landed. NF254 runs beside me (no overlap: none of its locks is mine). None of my rows is in codex_pass28
RESERVED.tsv. Locks: work/locks.log (every row locked NF255 before its first compile, controls included). Tools: NF253's set re-pathed
(tools/), plus rvt3.py (retail class / CCombatActionBase / CCreatureActionBase vtable words side by side, with the slot count) and
slotchk.py (per slot: PDB name sets, table spellings at the target). Read-first list: the brief chain NF255 -> NF215, NF214 and its
read-first list (NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS), GOTCHAS.md in full, EXTRA_RULES.md, ASM_RULES.md,
COORDINATION.md (head), the nf253 landing README and hand-back (NOTES entry 4, evidence/e4, LESSONS), the nf250 drafts / NOTES / README.

## The recipe (all classes)
- Retail slot map first (no compile): rvt3.py on the class vtable against CCombatActionBase 0x01259E44 and CCreatureActionBase 0x01259D24.
  Each of the four classes runs 81 .text slots; a slot whose class target differs from CCombatActionBase's is the class's override.
- Prelude: the MELEE branch of engine/CCombatActionBase.h (the only non-PDB branch that declares the out-of-line copy constructor
  0x0062F3B0 the Clone rows call: MELEE_COPYCTOR) with the CCombatActionBase override opt-ins of the landed vtable emitter 0x00955620
  (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION, ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus SOUNDCTOR / TARGETCTOR,
  and one add-only opt-in in the class header for the class's own overrides. ONE prelude text per class, in every row of the class (ctor,
  Clone, deleting destructor, any slot self row), so the ??_7 / ??_G COMDAT copies are one (vtmap outputs byte-equal; objsyms hashes equal).
- Deleting destructor rows: the class ctor is out of line, so the L95 `new T(args)` harness emits no vtable; an anonymous-namespace helper
  that placement-copy-constructs (the implicit copy ctor stores the vtable) emits ??_7 + ??_G (the 0x009568E0 recipe).
- Slot proof (per class): vtmap with ROWS=rows/ (DIFF only where the landed Leaf is a prefix-only text or a HEAD stand-in self row);
  slotchk.py (every slot symbol in a PDB name set: Ego_r + FableWin, or FableWin alone for the header-inline CCombatActionBase bodies at
  0x0062Exxx); and the slot-for-slot comparison with the HEAD vtable of the landed emitter 0x00955620 (NF250's vt_head_00955620_006f8010.txt)
  on every slot with the same retail target: 77 of 77 equal, 0 differ (L92). So the new tables add no spelling at any target that the landed
  emitter does not already bind.

## Entry 1 (q12720): L92 CCreatureAction_DragonFireBeamInto -- done
- Rows (rows/): 0x009535B0 ctor, 0x00953CF0 Clone, 0x009539C0 ??_G. All three RELOCATION_MATCH first form, tests PASS (HEAD tests kept:
  standalone host models, no shared class model linked; owed as header tests).
- Retail slots (evidence/retail_slots_l92.txt): 81; own overrides 0 (??_G 0x009539C0), 11 (Clone 0x00953CF0), 12 (Initialise 0x008F41D0,
  landed with the PDB Leaf); CCombatActionBase targets at 3, 4, 6-8, 10, 13-15, 17-23, 27-29, 43, 44, 51, 53, 54, 56, 57, 59-62, 66, 68, 70-80.
- Header: engine/CCreatureAction_DragonFireBeamInto.h FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE (hdr/README.md; 1 assert pinned).
- vtmap (evidence/vtmap_l92.txt, identical for all three objects): 81 slots; SAME 9, NOLEAF 61, DIFF 11: slot 0 ??_E (the weak alias of the
  ??_G this hand-back lands), slots 23 / 43 / 51 / 53 / 60 (landed Leaf lines that are prefixes of the slot symbol: no second spelling),
  50 / 65 / 67 / 78 / 79 (HEAD stand-in self rows whose member spelling 0x00955620's vtable already binds at HEAD: NAME_FIXES).
- pk (evidence/pk_l92.tsv): ??_G EQ owner 0x009539C0 in the ctor and Clone objects; the <new> placement ??2@YAPAXIPAX@Z EQ its table owner
  0x00405C10; the implicit copy ctor ??0...@@QAE@ABV0@@Z (no table line, grepped; inlined everywhere in retail, FableWin 0x02BE2220 is the
  debug copy) and the hashed helper: NO-OWNER.
- Symbol diff v HEAD (evidence/symdiff_*.txt): ctor PAV -> AAV leaf, base ctor PAV -> AAV (0x00857F30), new ??_G pick-any + the 81 slot
  UNDs (HEAD: a 2-slot local vtable with the stand-in ?v00@CCreatureAction_DragonFireBeamInto@@UAEXXZ); Clone stand-in QAE -> UBE, base copy
  ctor ??0CBase_0062f3b0 -> ??0CCombatActionBase@@QAE@ABV0@@Z (0x0062F3B0: 2 table spellings, the stand-in still has other binders: no growth);
  ??_G free stand-in -> ??_G member.
- Census: ctor called only by 0x00951FB0 (binds AAV already); vtable 0x01293E54 stored by the ctor and Clone rows; Clone / ??_G reached only
  through the vtable (callers.py, absrefs.py). Self spellings (symdiff): 0x009535B0 HEAD self PAV v caller AAV -> AAV only; 0x00953CF0 / 0x009539C0 HEAD self
  stand-in v the local tables' ?v00 slot stand-in -> the PDB / ??_G spelling in self, Clone slot and vtable slot alike; 0x00857F30 / 0x0062F3B0
  this row moves onto the existing PDB spelling, the stand-ins keep their other binders (no growth).
- Owner check (evidence/owners_l92.tsv): each of the 81 slot symbols names the retail owner of its target (rvt3 tag CLS / CAB / CRB): 0 errors.
- Link (evidence/link_l92.txt, LINK unset, /FORCE /NODEFAULTLIB; control: the same object twice gives LNK4006): new three + HEAD 0x008F41D0 +
  HEAD 0x00951FB0: 0 duplicates; HEAD set: 0. lnk.py sees only LNK4006 / LNK2005, so the controls' objects were read too
  (objdump -t): 0x008F41D0 defines only its leaf of this class; 0x00951FB0 defines a pick-any implicit ??1CCreatureAction_DragonFireBeamInto@@UAE@XZ
  (HEAD debt, no retail owner: inlined into the ??_G in retail; none of my objects defines ??1); no control defines ??_7 / ??_G / the copy ctor.
- Recheck: vland --recheck-landed over the three rows and the header's three includers 0x00951FB0 / 0x009539A0 / 0x00953A00: 6 / 6
  RELOCATION_MATCH + PASS (evidence/vland_l92.log). Non-blocking STANDIN_LEAF on the three ??_G rows (catalog text `scalar_deleting_destructor'
  as the siblings; nf250 ruling 6).

## Entry 2 (q12721): L74 CCreatureAction_NymphSummonOutOf -- done
- Rows (rows/): 0x008CA600 ctor (NF250's draft on the L92 prelude; its `GetSoundPair(creature, (const char*)0)` is the HEAD row's own spelling), 0x008CAA70 Clone, 0x008C9E40 FinishAction, 0x008CA9A0 ??_G. All four
  RELOCATION_MATCH first form, tests PASS (HEAD tests kept: standalone host models; owed as header tests).
- Retail slots (evidence/retail_slots_l74.txt): 81; own overrides 0 (??_G 0x008CA9A0), 4 (FinishAction 0x008C9E40), 11 (Clone 0x008CAA70);
  slot 12 is CCombatActionBase::Initialise 0x00857520; every other CCombatActionBase target as L92.
- Header: engine/CCreatureAction_NymphSummonOutOf.h FABLE_RETAIL_CCREATUREACTION_NYMPHSUMMONOUTOF_OVERRIDES (Clone + FinishAction; hdr/README.md).
- FinishAction body: `PCreature->GetPhysicsTC()->EnablePhysics(false); CCombatActionBase::FinishAction();` (retail 32 bytes: GetPItem
  0x00A01B50 on PCreature at +8, PhysicsTC at +0x60, CTCPhysicsBase slot 36 = EnablePhysics(bool) per engine/CTCPhysicsBase.h, tail jmp to
  0x00859A20). The sibling 0x008CA220 (NymphGetHit::Initialise) writes the same call (`PCreature->GetPhysicsTC()->EnablePhysics(false)`) on a
  local model. Its object also defines the header inlines CIntelligentPointer<CThingCreatureBase>::Get / operator-> and CThing::GetPhysicsTC
  pick-any (column 1 of reloc_bindings.tsv grepped for each: no line, so no retail owner: NO-OWNER in evidence/pk_l74.tsv).
- vtmap (evidence/vtmap_l74.txt, identical for the three objects that emit the table; the FinishAction object emits none): SAME 9, NOLEAF 61,
  DIFF 11, the same eleven as L92 (slot 0 the ??_E alias; prefix-only landed Leafs; the five HEAD stand-in self rows in NAME_FIXES).
  Against 0x00955620's HEAD vtable on the 76 slots with the same retail target: 76 equal, 0 differ.
- pk (evidence/pk_l74.tsv): ??_G EQ owner 0x008CA9A0 in the ctor and Clone objects; placement ??2@YAPAXIPAX@Z EQ 0x00405C10; the rest
  NO-OWNER (implicit copy ctor and the three header inlines: no table line; the hashed helper).
- Census: ctor called only by 0x0091C6A0 (binds AAV already); vtable 0x01282354 stored by the ctor and Clone; Clone / FinishAction / ??_G
  reached only through the vtable. 0x00859A20: 0x008C9E40 moves off its own stand-in `?base_FinishAction_859a20@@...` (witness 0x008C9E40, no
  other binder) onto the PDB `?FinishAction@CCombatActionBase@@MAEXXZ` (already a table line): one table line deleted (TABLE_CHANGES), 0x00859A20
  goes down one spelling (its other stand-ins are other rows' debt). 0x00A01B50: unchanged (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
  before and after).
- Link (evidence/link_l74.txt): new four + HEAD 0x0091C6A0: 0 duplicates; HEAD set: 0. objdump -t of the control: 0x0091C6A0 defines only a
  pick-any implicit ??1CCreatureAction_NymphSummonOutOf@@UAE@XZ of this class (HEAD debt, no retail owner; none of my objects defines ??1).
- Recheck: 5 / 5 RELOCATION_MATCH + PASS (evidence/vland_l74.log; the RELOC_NAME warning on 0x0091C6A0 is its pre-existing 0x0091C080 label).

- Owner check (evidence/owners_l74.tsv): each of the 81 slot symbols names the retail owner of its target (rvt3 tag CLS / CAB / CRB): 0 errors.

## Entries 3-6 (q12722 L72, q12723 L100, q12724 0x0096B600, q12659 0x00418DCA) -- open (not started)
Handed back early at a class boundary (context). L72 (0x008B83E0: Clone UBE 0x008B89A0, Initialise UAE 0x008B7A20) and L100 (0x00956870:
Clone EBE 0x00956A20, FinishAction 0x008C2AD0, Initialise 0x008C0560; 0x008C9E40 is NOT L100's, it is L74's FinishAction and is done here)
should script cleanly on the same recipe: rvt3.py for the slot owners, the L92 / L74 prelude with the class's opt-in, one prelude per class,
the copy-ctor ??_G harness where the ??_G row is a stand-in. L72's and L100's binders (0x0091C6A0, 0x00955620) were not touched; 0x00955620
includes engine/CCombatAction_WillHealInto.h, so an L100 opt-in there needs its own equiv_check over that row. Locks of entries 3-6 are not held.

## Whole-pass summary
- 7 rows ported (0x009535B0, 0x00953CF0, 0x009539C0, 0x008CA600, 0x008CAA70, 0x008C9E40, 0x008CA9A0), 4 not-a-binder includers / binders
  rechecked (0x00951FB0, 0x009539A0, 0x00953A00, 0x0091C6A0), 1 link control (0x008F41D0). recheck_final.log: one vland --recheck-landed run
  over all 11: 11 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT.
- Two headers, one add-only opt-in each (hdr/README.md). equiv_check: whole tree with the L92 header (evidence/equiv_l92.log): 0 of 57,125
  rows differ (one transient stub-race line on 0x0092C070, which includes neither header, rerun identical); both headers over every includer
  (0x008C9E40, 0x0091C6A0, 0x00951FB0, 0x009539A0, 0x00953A00, tests included; no fold / funclet row includes them): 0 differ
  (evidence/equiv_l74.log). Descriptive names added: none (the two hashed `Reference...Vtable` helpers are
  per-TU scaffolding, as 0x009568E0's). Flags lines added: none (no row has CompilerFlags; 0x0091C6A0's `// Flags: default` was cleared in my
  payloads, its catalog has none). Funclets: no EH funclet or fold row includes any changed file (grep of `#include "../src/compiled` for the
  seven file names: 0 hits).
- No new split: every touched address ends on one spelling or fewer (census per class above); one table line deleted.

## Rule slips
- One Bash call exited non-zero (a trailing `kill -0` test after its job had finished). No git, no foreground sleep, every grade under
  `timeout` (q.py runs inside timeout 900 / 300; vland inside timeout 900), every compile after its lock and its clang precheck (controls
  included: 0x008F41D0, 0x00951FB0, 0x009539A0, 0x00953A00, 0x0091C6A0 locked NF255 first and prechecked), no rm -rf (tools/clean.py), nothing
  written in the repo (work/root_before.txt v root_after.txt).
- Control / includer rows locked for compiles only (released at hand-back): 0x008F41D0, 0x00951FB0, 0x009539A0, 0x00953A00, 0x0091C6A0.

## Decisions for the coordinator
1. The MELEE-branch prelude (0x00955620's CCombatActionBase opt-in set + SOUNDCTOR / TARGETCTOR) for the DragonFireBeamInto and
   NymphSummonOutOf class rows, although their callers 0x00951FB0 / 0x0091C6A0 compile CCombatActionBase in other branches: only the class rows
   emit these vtables, and only the MELEE branch declares the copy ctor the Clone rows call. Accept?
2. The copy-construction harness for the two ??_G rows (placement copy of the implicit copy ctor; `// Fidelity: scaffolding, not retail`,
   the 0x009568E0 pattern with a copy instead of the out-of-line ctor). Accept?
3. HEAD tests kept for all seven rows (standalone host models, PASS); header tests owed. Queue?
4. NAME_FIXES: the five CCombatActionBase / CCreatureActionBase slot self rows on free stand-ins (0x0062EAC0, 0x0062EAD0, 0x00692670,
   0x00692880, 0x006928B0; HEAD debt the new tables share with 0x00955620) and 0x008F41D0's local-model port. Queue?
5. ENTRIES q12723 lists 0x008C9E40 as L100's stand-in; it is L74's FinishAction (retail 0x01282354 slot 4; REJECTED.tsv) and is done here.
6. Entries 3-6 to the next agent (same recipe; L100's opt-in needs an equiv_check over 0x00955620).
