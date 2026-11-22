# NF259 notes (2026-11-22, round 22b): the CCombatActionBase-family slot self rows, the CCreatureAction port debt, 0x00418DCA left open

Base: the repo tree with nf257 and nf255 landed. NF258 runs beside me (no overlap: none of its entries or locks touched; I read only its
directory listing). None of my rows is in codex_pass28 RESERVED.tsv (grepped); none of the held rows touched. Locks: evidence/locks.log
(every row and every control locked NF259 before its first compile; HEAD sources clang-prechecked too). Tools: NF257's set re-pathed (tools/;
mp.py also drops a literal `(none)` / `default` Flags text now). Read-first list: the brief chain NF259 -> NF257 -> ... -> NF215, NF214 and its
read-first list (NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS, GOTCHAS.md in full, EXTRA_RULES.md, ASM_RULES.md), the nf257 /
nf255 landing READMEs and hand-backs (NOTES, LESSONS, NAME_FIXES, evidence listing).

## Entry 1 (q12730): five vtable slot self rows -- done
- Rows (rows/): 0x0062EAC0 GetIsToFaceTargetCone, 0x0062EAD0 GetIsToFaceTargetMinDist (CCombatActionBase, MELEE prelude + MELEE_OWN_VIRTUALS,
  the define block of the landed sibling 0x00956730), 0x00692670 IsPreventingLookTurning, 0x00692880 OverrideFaceWithDefault, 0x006928B0
  CanAnimationTransitionOut (CCreatureActionBase retail branch, 0x006929A0's define block; FULL_MEMBERS + CANIMATIONENTRY_GETFLAGS (+
  CDEFSTRINGTABLE, which CAnimationEntry.h needs) for the two flag readers). All five graded on the first form: 4 RELOCATION_MATCH, 0x00692670 MATCH.
- Two sources per spelling: both PDB name sets (Ego_r + FableWin, addresses in the row headers and RENAMES) and the retail vftable slot (CCombatActionBase
  0x01259E44 slots 78 / 79; CCreatureActionBase 0x01259D24 slots 50 / 65 / 67, rvt3.py).
- Bodies: 0x0062EAC0 / 0x0062EAD0 return float literals (retail `fld [const] ; ret`): 0x01259D18 = 0x3DE38E39 (1/9) with a second reader
  0x00B4F215, 0x0122F59C = 0.5f with many readers, so both are VC7.1 pool constants (`__real@3de38e39` / `__real@3f000000`), not named globals;
  the HEAD rows read the made-up externs g_value_01259d18 / g_CombatMinDistance_0122f59c (only those rows mention them: grep of src / tests /
  include). The landed sibling 0x00956730 ThrowShotObject::GetIsToFaceTargetCone writes its literal the same way. INFERRED: the wording of the
  1/9 literal (`1.0f / 9.0f`) is unsourced; only its value is retail's (row header says so).
  0x00692670 `return IsPreventingNormalMovement() ? true : false;` (slot 49 = 0x00692A20): retail's non-tail `call [eax+0C4h] ; test al,al ;
  setne al` came out exact on the first form. 0x00692880 `(PAnimEntry->GetFlags() & 0x10) != 0`, 0x006928B0 `(PAnimEntry->GetFlags() & 1) == 0`
  give retail's `shr eax,4 ; and eax,1` / `not eax ; and eax,1`.
- Census: the five addresses have 0 code references (callers.py) and 1,796 .rdata references (absrefs.py: vtable slots of the derived action
  tables only); the landed emitters bind the PDB spellings there already. vtmap of the HEAD object of 0x00955620 with ROWS=rows/: slots 50 / 65 /
  67 / 78 / 79 flip DIFF -> SAME (12 -> 7 DIFF; evidence/vtmap_00955620_head.txt / _rows.txt). So each address loses its self stand-in
  spelling (2 -> 1 in the landed emitter's view). Other rows' local declarations of these five members (grep of rebuild/src/compiled: 10 / 9 / 46
  / 19 / 19 declarations) all read `virtual <type> Name() const;`, the PDB signature; their access in each local model (it decides U v M in the
  two protected ones) was not compiled one by one, so the census claim is -1 spelling per address, with the emitters on the PDB spelling.
  0x005DC210 (CAnimationEntry::GetFlags): 0x006928B0 moves off the made-up `?cat_probe@@YIHPAVCAnimationEntry@@@Z` (its only witness) onto
  `?GetFlags@CAnimationEntry@@QBEJXZ` (the self row's Leaf, witness 0x00663140): 2 -> 1; TABLE_CHANGES deletes the cat_probe line.
- Objects (evidence/symdiff_<row>.txt): each defines its PDB leaf; 0x0062EAC0 / 0x0062EAD0 the `__real@` constants; 0x00692880 / 0x006928B0 the
  pick-any header inline `??C?$CCountedPointer@VCAnimationEntry@@@@QBEPAVCAnimationEntry@@XZ` (0 table lines: NO-OWNER, evidence/pk_e1.tsv). No
  `??_7` (sym_n_*.txt: 0 vtables in all ten objects other than 0x006F5C10). No other landed source or header defines any of the five members (grep).
- Link (evidence/link_e1.txt; LINK unset, /FORCE /NODEFAULTLIB): the five + HEAD 0x00955620: 0 duplicates; positive control (0x00692880 and a
  renamed copy): LNK4006. (The same object named twice is deduplicated by the linker: not a control; the first line of the file shows it.)
- Tests: the HEAD tests were standalone (two printed PASS unconditionally). All five now link the real row object on the shared headers
  (raw storage + qualified call; 0x00692670's table slot 49 points at a test-only probe class's member; 0x00692880 / 0x006928B0 stub
  CAnimationEntry::GetFlags in the test and reach the protected member through a test-only derived class). PASS (evidence/vland_e1.log).
- The `// Stand-in (owner rule ...)` lines are gone with the stand-ins. Catalog names unchanged (leaf-only renames, RENAMES section 1).

## Entry 2 (q12731): nf257 port debt (5 rows) -- done
All five on the first form, leaves and callee spellings unchanged (count-neutral), HEAD tests kept (PASS):
- 0x008B7A20 SummonCreatures::Initialise: the L72 prelude (0x008B83E0's define block) + engine/CSummonDef.h + engine/CTCDParticleEmitter.h
  (CREATE). CTCAISummon has no shared header anywhere under rebuild/include (grep), so it keeps the HEAD row's local declaration (the landed
  CTCAISummon row 0x007FDE00 models it locally too). The HEAD local named the +0x44 member SummoneeEffect; in engine/CSummonDef.h +0x44 is
  SummonEffect, and the header's offsets are retail's (0x007FDE00 reads TimeBetweenSummons at the header's +0x40), so the row now reads
  SummonEffect (same bytes). New pick-any: the header inline `?Get@?$CIntelligentPointer@VCThingCreatureBase@@...` (NO-OWNER); owned copies EQ.
- 0x008C2AD0 WillHealInto::FinishAction: engine/CTCHealLife.h (NEW add-only SETAICAST declaration, hdr/) + engine/CCombatAction_WillHealInto.h
  (nf257 OVERRIDES). CCombatAction_WillHealRelease has no header: local kept. Symbol set identical to HEAD; disassembly differs only in local label
  numbers. Its GetSubDef<CSpecialEffectsDef> copy is NEQ:relocs against 0x00669580 at HEAD and after (HEAD debt, unchanged). evidence/pk_e2.tsv
  has every copy of the five objects (the 006F5C10 part unfiltered; its two NEQ copies are in the HEAD object too, which is identical).
- 0x008C0560 WillHealInto::Initialise: had no Leaf line; now `// Leaf: ?Initialise@CCombatAction_WillHealInto@@UAEXXZ` (the symbol the HEAD
  object defines, both PDBs), on the MELEE branch (INITIALISE) + WillHealInto.h. Object code identical to HEAD.
- 0x0096AE50 WhisperRetreat::UseWill: off the local CAIStateGroup_WhisperRetreat / _EnemyCreatureBase models and the banned renaming-macro
  CCreatureAction_CastAssassinRush model onto engine/CAIStateGroup_WhisperRetreat.h (nf257 UPDATE), engine/CAIStateGroup_EnemyCreatureBase.h and
  the retail engine/CCreatureAction_CastAssassinRush.h with a NEW add-only TARGETCTOR declaration (hdr/). The class's implicit destructor is now
  the header's (as at the landed owner 0x0072D990 and caller 0x008C2550): the object defines the pick-any `??1CCreatureAction_CastAssassinRush@@UAE@XZ`,
  EQ to owner 0x0072D990 (pk), and the row's call binds it under the owner's own spelling (nf212 refinement). CCreatureAction_MidAttack360: no
  header, local kept.
- 0x006F5C10 StatRegisterClass: the local CCreatureAction_SummonCreatures model -> engine/CCreatureAction_SummonCreatures.h (nf257 OVERRIDES declares
  the default ctor it calls). The object is identical to HEAD (symbol sets, code, all section bytes and relocations; the only byte difference
  is the compile-directory string in a debug section): its 31 emitted vtables are untouched, so no vtcheck was needed.

## Entry 3 (q12732): 0x008F41D0 DragonFireBeamInto::Initialise -- done
- The L92 prelude (0x009535B0's define block) + nf255 DRAGONFIREBEAMINTO_INITIALISE + CINTELLIGENTPOINTER_DEREFERENCE (for `*PCreature`,
  `*PTarget`) + CTCCreatureModeManager.h ADDMODEPOINTER, replacing the local CThingCreatureBase (an empty local class!), CBaseIntelligentPointer /
  CIntelligentPointerCreature, CCombatActionBase and class models; NCreatureMode::CModeDragonFireBeam has no header and keeps its HEAD local.
  RELOCATION_MATCH on the first form; HEAD test kept, PASS. Every UND unchanged; the HEAD-local conversion stand-in
  `??BCBaseIntelligentPointer@@QBEPAVCThing@@XZ` (0 table lines) is gone; new pick-any header inlines (`??D` / Get / GetPointer of
  CIntelligentPointer<CThing> / <CThingCreatureBase>) have no table lines (NO-OWNER); owned copies EQ (evidence/pk_e3.tsv).

## Entry 4 (q12659): 0x00418DCA -- open (not started). It needs the full 469-includer proof; the advisor's call was to leave it.

## Whole-pass checks
- Headers: two add-only declaration opt-ins (hdr/README.md). pin_asserts: 0 in CTCHealLife.h, 16 in CCreatureAction_CastAssassinRush.h.
  Whole-tree equiv_check (repo rebuild/include v a `cp -al` copy with the two files replaced): evidence/equiv_final.log.
- Recheck: evidence/vland_e1.log (5 rows), evidence/vland_e2.log (the 6 entry-2/3 rows + every landed includer of the two overlaid headers:
  0x0072D990, 0x0072CAA0, 0x007C3FB0, 0x007C4430, 0x008C2550, 0x008CD470, 0x009735D0): all RELOCATION_MATCH / MATCH + PASS. recheck_final.log:
  one run over the 11 changed rows + 0x00955620 + the 7 includers.
- Link (evidence/link_all.txt): the 11 new objects + HEAD 0x00955620 / 0x0072D990 / 0x008C2550: 0 duplicates; the HEAD set: 0.
- Funclets / folds: no `#include "../src/compiled/..."` names any of the 11 changed files (grep).
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; 0x0096AE50 keeps its HEAD `// Flags: none` text).
- Local models left (header-less classes, HEAD tokens, in place): CTCAISummon (0x008B7A20), CCombatAction_WillHealRelease (0x008C2AD0),
  CCreatureAction_MidAttack360 (0x0096AE50), NCreatureMode::CModeDragonFireBeam (0x008F41D0). None of the 11 rows now trips
  SHARED_HEADER_AVAILABLE in vland.

## Rule slips
- docs/COORDINATION.md (on CLAUDE.md's read-first list) was read late, after entry 2; nothing in it changes this pass (no B claim or
  UNMATCHED reservation names any of my rows: grep of COORDINATION_CLAIMS.tsv / UNMATCHED_ROWS.tsv).
- 0x0062EAC0's row comment was re-flowed after the first recheck (the Inferred note on its own line; same line count); recheck_final.log is
  the run after that edit.
- Two Bash calls exited non-zero during reading (a `cat` of a test path that is under rebuild/tests, not src; a `wc` over a mistyped path).
- Two GOTCHAS chunk extracts were written to my session scratchpad (outside $S/w22/agentNF259) while reading; deleted by literal path.
- One evidence line (symdiff_006f5c10.txt) was first appended unconditionally after a failed `cmp`; caught at once, removed, the objects
  recompared, and the true result (only the compile-directory string differs) recorded.
- Nothing written in the repo (work/root_before.txt v root_after.txt); no git; no foreground sleep; every grade under `timeout 900` in a call
  after its passing precheck and after its lock; no rm -rf (tools/clean.py; plain rm only on two literal file paths and on two files of my
  own hard-link tree copy before replacing them).

## Decisions for the coordinator
1. The two new add-only declaration opt-ins (CTCHEALLIFE_SETAICAST, CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR; both PDBs each). Accept?
2. 0x0062EAC0's literal spelled `1.0f / 9.0f` (value proven, wording inferred, row header marked). Accept, or prefer a decimal literal?
3. 0x008B7A20 reads the +0x44 def member as the header's SummonEffect (the HEAD local called it SummoneeEffect); same bytes. Accept?
4. NAME_FIXES: 0x0122F59C's two made-up data names (a pooled 0.5f), seven more CCombatActionBase slot rows 0x0062EA40 .. 0x0062EAB0 and
   CCreatureActionBase::AllowsCombinationAnims 0x00692890 still on stand-ins (same recipe as entry 1), and the CastAssassinRush local models
   in 0x0072CAA0 / 0x008CD470. Queue?
5. Header-based tests: entry 1's five are new header tests; the six entry-2/3 rows keep their HEAD tests (owed). Queue?
6. q12659 to the next agent.

## Post-hand-back closure (after the report was sent)
- Whole-tree equiv_check finished: DONE 0, 57,125 rows checked, 0 differing lines (evidence/equiv_final.log / .tsv).
- work/tree_new removed (tools/clean.py); both repo headers cmp-equal to orig/; repo root listing unchanged (work/root_before.txt v
  root_after.txt). The eight control locks are set back to `released` (evidence/locks.log); the 11 changed rows stay locked NF259.
