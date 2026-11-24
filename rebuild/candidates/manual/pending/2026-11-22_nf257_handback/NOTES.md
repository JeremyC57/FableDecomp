# NF257 notes (2026-11-22, round 21b): L72 SummonCreatures, L100 WillHealInto, 0x0096B600; 0x00418DCA left open

Base: the repo tree with nf255 landed. NF256 runs beside me (no overlap: its rows 0x0063E480 / 0x0049C490 / 0x0049BE90 are not mine; I read
none of its files beyond rows.txt / OPEN_LINES.tsv headers). None of my rows is in codex_pass28 RESERVED.tsv; none of the held rows touched.
Locks: evidence/locks.log (every row and every control locked NF257 before its first compile; HEAD sources clang-prechecked too).
Tools: NF255's set re-pathed (tools/), plus owners.py (per-slot owner check from rvt3 tags) and vs955.py (slot-for-slot against the landed
emitter 0x00955620's HEAD table; it reproduces NF255's 76 / 76 for L74 as a check). Read-first list: the brief chain NF257 -> NF215, NF214 and
its read-first list (NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF sections, NF88-90 LESSONS), GOTCHAS.md in full, EXTRA_RULES.md, ASM_RULES.md,
COORDINATION.md (head), the nf255 landing README and hand-back (NOTES / LESSONS / evidence), the nf250 blocked drafts, nf253 NOTES entry 4.

## The recipe (NF255's, unchanged)
Retail slot map first (rvt3.py: class v CCombatActionBase 0x01259E44 v CCreatureActionBase 0x01259D24); one MELEE-branch prelude text per
class in every row that emits the class table; the placement-copy `??_G` helper; vtmap of every emitting object (byte-equal outputs);
owners.py (each slot symbol names the retail owner of its target); slotchk.py (every slot symbol in a PDB name set); vs955.py; pk.py;
link with the HEAD controls (LINK unset, /FORCE /NODEFAULTLIB; the same object twice as the positive control); vland --recheck-landed.

## Entry 1 (q12722): L72 CCreatureAction_SummonCreatures -- done
- Rows (rows/): ctors 0x008B83C0 (default), 0x008B83E0 (creature, target, bool), 0x008B8440 (creature), Clone 0x008B89A0, ??_G 0x008B87A0.
  All five RELOCATION_MATCH on the first form, tests PASS (HEAD tests kept: standalone host models; owed as header tests).
- Why all three ctors (not only the entry's rows): retail 0x0127ADDC is stored by all three ctors and Clone (absrefs), and the table's
  ??_7 witness is 0x008B83C0. At HEAD all four emitted the same 1-slot local stand-in table (slot 0 ?v00@...); porting some would leave copies
  of one COMDAT with different contents that matched at HEAD (advisor). So the unit is the five emitters on one prelude.
- Retail slots (evidence/retail_slots_l72.txt): 81; own overrides 0 (??_G 0x008B87A0), 11 (Clone 0x008B89A0), 12 (Initialise 0x008B7A20,
  landed, PDB Leaf). The rest are CCombatActionBase / CCreatureActionBase targets as L92.
- Headers: engine/CCreatureAction_SummonCreatures.h SUMMONCREATURES_OVERRIDES (default + one-argument ctors, Clone, Initialise; both PDBs) and
  engine/CCombatActionBase.h MELEE_PRIORITYCTOR (the protected `CCombatActionBase(long)` 0x00857E40, `IAE@J` in Ego_r 0x00838AC0 + FableWin
  0x02823960, the spelling binder 0x006F5C10 already uses; new name because landed 0x008B9010 defines MELEE and PRIORITYCTOR together). hdr/README.md.
- vtmap (evidence/vtmap_l72.txt; md5 identical for all five objects, vtmap_l72_md5.txt): SAME 9, NOLEAF 61, DIFF 11, the same eleven as L92 /
  L74 (slot 0 the ??_E alias of the ??_G landed here; 23 / 43 / 51 / 53 / 60 prefix-only landed Leaf lines; 50 / 65 / 67 / 78 / 79 the HEAD
  stand-in self rows already in NF255's NAME_FIXES). Owners (owners_l72.tsv): 0 errors over 81. slotchk: 81 / 81 in a PDB name set (69 rf,
  12 f). vs955 (vs955_l72.txt): 77 shared targets, 77 equal, 0 differ.
- pk (pk_l72.tsv): ??_G EQ owner 0x008B87A0 in all four other objects; placement ??2@YAPAXIPAX@Z EQ 0x00405C10; the implicit copy ctor
  ??0...@@QAE@ABV0@@Z (no table line; inlined in retail, FableWin 0x029FF230 is the debug copy) and the hashed helper: NO-OWNER.
- Symbol diffs v HEAD (evidence/symdiff_<row>.txt): 0x008B83C0 base ctor QAE@J -> IAE@J; 0x008B83E0 self PAV/PAVCThing -> AAV@@0_N, base
  ctor 0x00857F30 PAV -> AAV; 0x008B8440 self PAV -> AAV; Clone stand-in QAE -> UBE, base copy ctor CBase_0062f3b0 -> CCombatActionBase; ??_G
  free stand-in -> member; every emitter: the 1-slot local table (?v00 stand-in) -> the 81-slot table.
- Census: 0x008B83C0 called only by 0x006F5C10 (QAE@XZ, unchanged), 0x008B83E0 only by 0x0091C6A0 (AAV@@0_N already), 0x008B8440 by no
  landed row; Clone / ??_G / Initialise reached only through 0x0127ADDC. 0x008B87A0: HEAD {?v00 slot stand-in (4 rows), free self stand-in}
  -> {??_G / ??_E} (2 -> 1). 0x00857E40 / 0x00857F30 / 0x0062F3B0: these rows move onto spellings already in the table (IAE@J witness
  0x006F5C10, AAV witness 0x00906380, PDB copy ctor witness 0x008F57F0); the stand-ins keep other witnesses (no growth). Table: no change
  (??_7 witness 0x008B83C0 still emits it; the ctor line stays).
- Link (link_l72.txt): new five + HEAD 0x008B7A20 / 0x0091C6A0 / 0x006F5C10: 0 duplicates; HEAD set: 0; positive control: LNK4006.
  objdump -t of the controls: 0x0091C6A0 defines only a pick-any implicit ??1CCreatureAction_SummonCreatures@@UAE@XZ (HEAD debt, no retail
  owner); 0x008B7A20 only its leaf; 0x006F5C10 nothing of the class.
- Recheck (evidence/vland_l72.log): the five + 0x0091C6A0 (the header's only includer) + 0x006F5C10 + 0x008B7A20: 8 / 8 RELOCATION_MATCH
  PASS. Warnings pre-existing (STANDIN_LEAF on the ??_G catalog text as the siblings; RELOC_NAME / SHARED_HEADER_AVAILABLE on the controls).
- Equiv (evidence/equiv_l72.log): whole tree with the two L72 headers: 0 of 57,125 rows differ.

## Entry 2 (q12723): L100 CCombatAction_WillHealInto -- done
- Rows: ctor 0x00956870 (NF250's draft text on the prelude), Clone 0x00956A20, ??_G 0x00956940. All three RELOCATION_MATCH first form,
  tests PASS (HEAD tests kept).
- Retail slots (retail_slots_l100.txt): 81; own overrides 0 (??_G 0x00956940), 4 (FinishAction 0x008C2AD0, landed PDB Leaf), 11 (Clone
  0x00956A20, EBE), 12 (Initialise 0x008C0560, landed, no Leaf line; its object defines the PDB spelling).
- Header: engine/CCombatAction_WillHealInto.h WILLHEALINTO_OVERRIDES (FinishAction, Initialise public; Clone private; both PDBs).
- vtmap (md5 identical for the three objects): SAME 8, NOLEAF 62, DIFF 11 (the same eleven). Owners: 0 errors. slotchk 81 / 81.
  vs955: 76 shared targets, 76 equal.
- pk: ??_G EQ 0x00956940 in the ctor / Clone objects; placement ??2 EQ 0x00405C10; copy ctor + helper NO-OWNER (no table line).
- Census: ctor called only by 0x00955620 (AAV already); 0x012948AC stored by the ctor and Clone; Clone / ??_G / FinishAction / Initialise
  only through the vtable. 0x00956940: HEAD {?v00 slot stand-in (2 rows), free self stand-in} -> {??_G / ??_E}. Table: no change.
- Link (link_l100.txt): new three + HEAD 0x008C2AD0 / 0x008C0560 / 0x00955620 / 0x006F8010: 0; HEAD set: 0; positive control LNK4006;
  L72 + L100 new objects together: 0. 0x00955620 defines a pick-any implicit ??1CCombatAction_WillHealInto@@UAE@XZ (HEAD debt).
- Recheck (vland_l100.log): the three + includers 0x00955620 / 0x006F8010 + slot rows 0x008C2AD0 / 0x008C0560: 7 / 7 RELOCATION_MATCH PASS.
- Equiv: the header's includers are 0x00955620 (+ test) and 0x006F8010 (the brief named only the first). Whole tree with the three L72 /
  L100 headers (evidence/equiv_l72_l100.log): 0 of 57,125 rows differ; the final four-header run is evidence/equiv_final.log.

## Entry 3 (q12724): 0x0096B600 CAIStateGroup_WhisperRetreat::Update -- done
- Claim verified: ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ (Ego_r / FableWin / Ego_d, NF253; engine/CThingCreatureBase.h
  UNSHEATHEMELEEWEAPON); 0x0096B600 was its last stand-in binder (`?UnsheatheMeleeWeapon@CThing@@QAEXXZ` on a local CThing model, table
  witness 0x0096B600 only).
- Port: the row moved off its local CThing / CBaseIntelligentPointer / CIntelligentPointer / IS_THING_ALIVE / CAIStateGroupBase / CAIBrain /
  CTCBoss / CTCStealth / CTCCarrying / CAIStateGroup_WhisperRetreat models onto the shared headers (FindHitEvent 0x0096A260's
  WHISPERRETREAT branch + the callee headers). Form 1 DIFFER(351v355) (GetPos read through the accessors at each use); form 2 (named creature
  reference and target pointer before the two GetPos calls, as the HEAD row and retail's load order have) RELOCATION_MATCH
  (evidence/forms_0096b600.txt). Test: HEAD test kept (standalone behaviour copy), PASS.
- Header: engine/CAIStateGroup_WhisperRetreat.h WHISPERRETREAT_UPDATE (Update public virtual; JumpToSafety / UseWill protected, both PDBs).
- Binder moves (symdiff_0096b600.txt): 0x006649C0 CThing stand-in -> CThingCreatureBase PDB (table spellings 2 -> 1; the forwarder 0x00666820's
  `_ForwardTarget_006649c0` text has no table line and stays NF253's NAME_FIXES item); 0x0096AE50 QAE -> IAE (its self row's leaf: 2 -> 1);
  0x0096AC20 QAE -> IAE (no landed self row; 1 -> 1, now the PDB spelling). Every other callee spelling unchanged.
- pk (pk_0096b600.tsv): the owned pick-any copies all EQ (LowerBound 0x0040F020, IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase>>
  0x006961F0, bitset<274>::test 0x00410DE0, HasTC 0x004118C0); every NO-OWNER symbol (GetTC x4, IsDead, the two GetCreature, Get,
  operator->, the two StatGetInterfaceType, operator!=, the bitset helpers, __stl_throw_out_of_range, sqrt) has 0 lines in column 1 of
  reloc_bindings.tsv (grepped one by one), so none has a retail owner; the leaf calls none of them out of line. Same check for the two
  implicit copy ctors of L72 / L100: 0 lines.
- Link (link_0096b600.txt): new 0x0096B600 + HEAD owners 0x006961F0 / 0x00410DE0 / 0x0040F020 + HEAD 0x0096AE50 / 0x0096A260: 0 duplicates.
- Recheck (vland_q12724.log): 0x0096B600 + 0x0096AE50 + the header's includers 0x008FE547 / 0x0090188E / 0x0096A260 / 0x0121F86F: 6 / 6
  (5 RELOCATION_MATCH, 1 MATCH) PASS. The RELOC_CONFLICT `_$E2` on 0x0121F86F is the usual TU-local `_$E` collision (NF92 notes), pre-existing.
- Equiv over the header's includers: evidence/equiv_q12724.log (0 of 6 differ); whole tree with all four headers: evidence/equiv_final.log, 0 of 57,125 rows differ.
- Self leaf unchanged (?Update@CAIStateGroup_WhisperRetreat@@UAEXXZ); no RENAMES section 1 line; section 2 has the three callee moves.

## Entry 4 (q12659): 0x00418DCA -- open (not started; context). It needs the full 469-includer proof, as every earlier round said.

## Whole-pass summary
- 9 rows ported (0x008B83C0, 0x008B83E0, 0x008B8440, 0x008B89A0, 0x008B87A0, 0x00956870, 0x00956A20, 0x00956940, 0x0096B600); not-a-binder
  includers / binders / controls rechecked: 0x0091C6A0, 0x006F5C10, 0x008B7A20, 0x00955620, 0x006F8010, 0x008C2AD0, 0x008C0560, 0x0096AE50,
  0x008FE547, 0x0090188E, 0x0096A260, 0x0121F86F; link-only controls 0x006961F0, 0x00410DE0, 0x0040F020. recheck_final.log: one vland run.
- Descriptive names added: none (the hashed `Reference...Vtable` helpers are per-TU scaffolding, as 0x009568E0's).
- Flags lines added: none; no row has CompilerFlags. 0x0091C6A0's `// Flags: default` and 0x00955620's empty `// Flags:` were cleared in my
  payloads (my mp.py copy also had a regex bug that read the next line after an empty `// Flags:`; fixed, LESSONS).
- Funclets / folds: no `#include "../src/compiled/..."` names any changed file.
- No new split: every touched address ends on one spelling or fewer (census per entry); TABLE_CHANGES.tsv has the q12724 lines.

## Rule slips
- Two Bash calls exited non-zero, both ending on a `diff` that found differences (the L72 prelude against L74's; orig v hdr of
  CCombatActionBase.h); a harmless `mv` of a file onto itself printed an error mid-call. No other slip: no git, no foreground sleep, every grade under `timeout` (q.py inside timeout 900 / 300, vland inside timeout 900), every
  compile after its lock and its clang precheck (controls included), no rm -rf (tools/clean.py), nothing written in the repo
  (root_before.txt v root_after.txt). One near-miss: the 0x0096B600 form-2 grade was launched in the same Bash call as its precheck
  (the precheck printed no error before the launch, but the launch was not gated on it); later launches were gated.
- One vland run (r100) was started with a wrong payload (the empty-Flags regex bug) and stopped by PID before any grade finished.

## Decisions for the coordinator
1. L72's unit is five rows, not the entry's two: all three ctors and Clone store 0x0127ADDC and emitted one shared stand-in table at HEAD.
   Accept the two extra ctor rows (0x008B83C0 leaf unchanged, 0x008B8440 PAV -> AAV, no landed caller)?
2. The new add-only FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_PRIORITYCTOR in the widely included CCombatActionBase.h (declaration only; whole-tree
   equiv 0 differ). Accept, or prefer PRIORITYCTOR itself in the MELEE branch (that would change 0x008B9010's preprocessed text)?
3. HEAD tests kept for all nine rows (standalone host models, PASS); header tests owed. Queue?
4. NAME_FIXES: port debt rows 0x008B7A20, 0x006F5C10 (a local model of CCreatureAction_SummonCreatures), 0x008C2AD0, 0x008C0560 (also no Leaf
   line), 0x0096AE50 (local CAIStateGroup_WhisperRetreat model). Queue?
5. q12659 to the next agent.
