# NF164 notes (round 10, creature action / AI state group naming)

Resumed after a container restart (the first session left STATUS/APPLIED census lines, drafts of 0x0121E030 / 0x01229280 and a link log;
no rows/ yet). The census in work/census.txt was rebuilt from the first session's HEAD objects (pH/) with tools/cens2.sh (no recompiles).
Reading (rule slip): GOTCHAS.md was grepped (nf86 / nf88 / nf95b / nf96a / nf135 / nf140 lines and the slice's class names), ASM_RULES.md
 read by headings only (no asm used), and the batch READMEs / NAME_FIXES behind the entry tags were not read (only the nf89 landing README for q4516).

## q4516 (CCreatureActionBase::RegisteredActionEvents, data 0x013BA8BC)
- 0x0121E030 / 0x01229280: the static is defined on engine/CCreatureActionBase.h (FABLE_RETAIL_CCREATUREACTIONBASE, include pattern of the
  landed 0x006932B0). Both rows strongly define the same external data symbol
  ?RegisteredActionEvents@CCreatureActionBase@@0V?$map@...@A, exactly as the landed NF89 pair 0x0121F0E0 / 0x01229BE0 define
  CTCAIScratchpad::ActiveGossipCategories (precedent; the coordinator should treat the pair as one TU at link time, as for NF89).
  WARN RELOC_CONFLICT `_$E2` (against landed 0x01219F00) is the usual TU-local `_$E` leaf collision (NF92 notes).
- 0x006946C0: respelled in place on its existing local CCreatureActionBase (same class token; no new class token): the static's type
  is now std::map from stlport/fable_map.h and private (0V); the lookup is map::find with FABLE_RETAIL_STLP_TREE_MFIND_DECL; the
  row's local `namespace std { allocator; vector }` model was replaced by stlport/fable_containers.h (removing a local model of a
  shared-header class). Defined-symbol diff HEAD vs new: only STLport pick-any COMDATs from the shared headers (vector<CCharString>
  ctor/dtor/begin/end, allocator, _Destroy, map/_Rb_tree end/find, iterator), none naming CCreatureActionBase beyond the leaf.
  The row still models CCharString, CWorld, CThing, CMainGameComponent, CIntelligentPointer and CCreatureActionBase locally (as at HEAD;
  vland warns SHARED_HEADER_AVAILABLE): a header port is a separate job.
- Callee spellings (exe scan, tools/xr.sh; count never grows):
  - 0x00696900 _M_erase: referencers 0x00696900 (self, PDB spelling), 0x00696AF0 (?Detach_00696900@CTCCarryable), 0x00696D10 / 0x00696E20
    (?Erase@CRetailTree_erase00696900), 0x01229280. 0x01229280 moves from ?Erase_00696900@CRetailTree to the self row's PDB spelling:
    4 spellings -> 3. The three stand-in binders are in NAME_FIXES.tsv.
  - 0x00696340 _M_find: referencers 0x006946C0 only (plus the self row). 0x006946C0 moves from ?Find@CRegisteredActionEventMap to the D1
    member-template spelling ??$_M_find@VCCharString@@@... (2 -> 2). The landed self row 0x00696340 still has the non-template ?_M_find leaf:
    NAME_FIXES.tsv (its self rename is now unblocked: no other binder).
- One definition (link/, `LINK=` cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED): the new objects define, besides their leaves,
  only STLport / CCountedPointer pick-any COMDATs; two of them are landed leaves (_M_erase = 0x00696900, ~vector<CCharString> = 0x00414EA0).
  owner 0x00696900 + new 0x0121E030 (e1.log), owner 0x00696900 + new 0x01229280 (e2.log), owners 0x00696900 + 0x00414EA0 + new 0x006946C0
  (ie.log): exit 0, no LNK2005 / LNK4006. Control (ctl.log): new 0x0121E030 + new 0x01229280 together give LNK2005 on
  ?RegisteredActionEvents@CCreatureActionBase@@0V?$map@...@A: both rows strongly define the static, exactly like the landed NF89 pair
  0x0121F0E0 / 0x01229BE0 (ActiveGossipCategories). COORDINATOR DECISION: accept on that precedent (the initializer and the atexit
  destructor are one retail TU's output of one definition) or hold one of the two rows.
- No source references 0x013BA8C0 (+4) outside these rows (grep).

## q5017 (CAIStateGroup_GoToBed::FindFreeBed 0x0092D1F0, catalog-only self)
- Both PDBs IAE. 0x0092D600 / 0x0092D7C0 move FindFreeBed under `protected:` in their existing local CAIStateGroup_GoToBed (access-only,
  in place; symbol diff vs HEAD: only that callee). Note for later: 0x0092D7C0's own leaf is ?Update@CAIStateGroup_GoToBed@@QAEXXZ
  (non-virtual public); not checked against the PDBs here (outside the entry).

## q5101 / q5102 (CCreatureActionBase::PeekDefinitionManager 0x00692910 / GetWorld 0x00692940)
- Self rows right. Census (work/census.txt): many landed rows already bind the PDB spellings, so binders move one at a time (the
  address's spelling count only shrinks).
- In place (10 rows, same CCreatureActionBase token at HEAD): tools/gw.py turns `CWorld* GetWorld()/GetPWorld() [const];` into
  `protected: CWorld& GetWorld();` and the uses into `GetWorld().` / `&GetWorld()`. 0x00846100 / 0x00846410 dropped their
  `((CWorld*)(GetPWorld()))` casts (GetPWorld returned the stand-in CRandomSource*). 0x00849140 also takes
  `const CGameDefinitionManager& PeekDefinitionManager() const` on the shared engine/CGameDefinitionManager.h (FABLE_RETAIL_CGAMEDEFINITIONMANAGER,
  the row already used the shared CDefinitionManager.h). Per row symbol diff vs HEAD (tools/symdiff.sh): only the respelled callee(s).
- 0x00846100: q.py says DIFFER(687v686) at HEAD and after: the q.py oracle is cut one byte short (retail's final `jmp` eb90 straddles it);
  vland (the judge) says RELOCATION_MATCH PASS.
- 0x008B93D0: ported onto the shared headers (define block of the landed sibling 0x008CDD40 ReleaseTimeSpell::FinishAction) plus the
  NF164 retail-class opt-in FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL (hdr/README.md). Own leaf unchanged (UAE). The callee
  0x00859A20 binds ?FinishAction@CCombatActionBase@@MAEXXZ as at HEAD.
- Not attempted (remaining binders, listed in APPLIED): q5101 0x00846100 / 0x00846410 / 0x008C8180 / 0x008C89D0 (local
  CDefinitionManager + CDefPointer + CDefClassBase + def types + CCharString must move first), 0x00865C20, 0x008CB780, 0x008CB620;
  q5102 0x008C0900, 0x008F3100, 0x008C6430. A cast `(const CDefinitionManager&)PeekDefinitionManager()` through a forward-declared
  CGameDefinitionManager would reach the spelling in place, but it is a type pun added only for the name: not used (advisor).

## Locks claimed this session
0x00842BC0 0x00845900 0x00846410 (was released) 0x008491E0 0x00849380 (was released) 0x008C8180 0x008C89D0 0x008F3100 (was released)
0x0092D7C0 (was released). Row 0x0092A8A0 (NF161) untouched.

## Descriptive names
- None added.

## Flags lines
- None added.

## Decisions for the coordinator
- q4516: 0x0121E030 and 0x01229280 both strongly define CCreatureActionBase::RegisteredActionEvents (LNK2005 when linked together,
  link/ctl.log), as the landed NF89 pair 0x0121F0E0 / 0x01229BE0 does. Accept on that precedent or hold one row.
- 0x00696340: its only binder now uses the D1 `??$_M_find` member-template spelling; the self row keeps `?_M_find` (NAME_FIXES).
- The new retail-class opt-in sits in a generated header (engine/CCreatureAction_ReleasePhysicalShieldSpell.h, generated model kept in `#else`).
- The cast route for the four local-CDefinitionManager rows (q5101) was not used.
- 0x00846100: the q.py oracle is cut one byte short (DIFFER(687v686) at HEAD and after); vland says RELOCATION_MATCH.
- gate.py was not run (it needs a landing directory and pair JSON files).

## Rule slips
- One Bash call exited 2 (an `ls` of NF162's missing hdr/engine/).
- Most Bash calls did not start with the brief's env line (the compile helpers g.sh / bindchk.sh source it themselves).
- Required reading incomplete: see "Reading" above.
