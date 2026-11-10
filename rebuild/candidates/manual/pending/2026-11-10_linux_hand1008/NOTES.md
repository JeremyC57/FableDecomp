# Chunk 1008 (CTCInventoryAbilities) -- notes

**Header overlay:** every src/ row was graded with `FABLE_HDR_OVERLAY=$A/hdr` (see hdr/README.md: two additive opt-ins in
engine/CTCInventoryAbilities.h, one in engine/CTCHeroStats.h). `gall.sh` does not export it: run
`FABLE_HDR_OVERLAY=$A/hdr bash $A/gall.sh` (all 9 RELOCATION_MATCH that way).

## Result
9 RELOCATION_MATCH (0x005EE77C, 0x005F077F, 0x005F0C5F, 0x005F0DC5, 0x005F1320, 0x005F1526, 0x005F3F7B, 0x005F45B5, 0x005F4C8F),
all on shared headers, each with a self-contained C++03 behaviour test (g++ and VC7.1 /Od both PASS via tall.sh / tall.py).
2 parked with a differ/ source (0x005F11A9 358v375, 0x005F59C7 2649v2652), 5 not reached (0x005EE49A, 0x005F64DD, 0x005F6793,
0x005F6BD6, 0x005F6E40).

Call targets: tools/calls.py pairs each matched object's call relocations with retail's direct call targets in order; every
retail address binds one symbol across the 9 rows (overloads such as the two CDeletion ctors / CWideString += keep distinct
addresses). Output: probe/calls_all.txt.

## Identities proven
- 0x005F3F7B is **ConstructExpressionCategoryList** (private, console), not ConstructPCExpressionCategoryList: called 4x by the
  console GetExpressionsListUIComponent 0x005F4C8F, calls the console ContructItemLists 0x005F077F, builds UI_* templates. The PC
  name belongs to 0x005F45B5 (called 4x by ConstructPCExpressionsList 0x005F59C7, calls ConstructPCItemLists 0x005F1526, PC_*
  templates). Both byte-match under those signatures (NAME_FIXES.tsv).
- 0x005F077F ContructItemLists: FableWin.pdb's own spelling ("Contruct"), private (AAE).
- 0x005EDCF0 is CTCInventoryAbilities::GetNumAssignableSpecialMoveButtons (QBEJXZ); landed rows call it GetNumAbilitiesPerRow,
  which the PDB does not have (NAME_FIXES.tsv lists the callers to respell).

## Callee names bound by the matched objects
CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494; map<EHeroAbility,CComponent*>
operator[] 0x005F9EA5, _Rb_tree::_M_find 0x005F8444, erase(iterator) 0x0057926B; CDeletion ctors 0x0042BED4 (default) /
0x0042CD84 (copy), _List_base<CComponent*> dtor 0x0042ABCA (CDeletion's list member); CTCInventoryItem::GetDescription 0x005D95C0
/ GetDetails 0x005D9630 / GetEngineGraphic 0x005D96A0; GetDef<CThingBaseDef> 0x0042AF3C; GetSubDef<CExpressionSubDef> 0x005DA240
and GetDef(long, CDefPointer<CExpressionDef>*) 0x005F81BE (both identical-code-folded with other instantiations: BSim names
0x005DA240 GetSubDef<CAICreatureWillPowerIndicatorDef>, 0x005F81BE GetDef<CSpecialAbilitiesDrunkennessDef>; not labelled here);
TITLE_NAME 0x013B8C60; CTCInventoryStats::GetXString 0x005CC772; GFIntToWideString 0x00405F00; CTCHeroStats::GetMoralityMax
0x0057A67E.

## Header debt
- hdr/ opt-ins to land: FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN, _ITEMLISTS, FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX.
- 0x005F4C8F needs same-TU bodies of GetDef / GetSubDef and an MSVC-only stand-in for ConstructExpressionCategoryList (in the row).
- Not reached rows need: CSpecialAbilities{Fireball,UnholyPower,DivineWrath}Def headers + hero-stats will energy + the interface
  0x91 cost component (0x005EE49A); CSoundInit / game-state sound player / GetPrioritisedAbilityDefs on the shared headers
  (0x005F64DD, 0x005F6793); player-manager controller GetNumButtons + AbilityButtons CVectorMap find/erase (0x005F6BD6).

## Unmatched rows: what next
- **0x005F11A9 UnAssignAbility** DIFFER(358v375), 6 counted grades + ~16 free probes. Only the head differs: retail loads
  A[slot] and S[slot] into ecx/eax, `cmp ecx,eax ; mov edx,ecx ; mov [ebp-4],edx`, then each arm reloads
  `AssignedAbilities._M_start` ([esi+17Ch]) before its store. Ours computes `&AssignedAbilities[slot]` once (`add eax,edi`) and
  stores through it. Tried: ability/removed/saved copies in every order, `bool restore`, removed assigned inside both arms,
  `==` with swapped arms, compare through the vectors (g3 371v375). Next: something between the compare and the arms that kills
  the address CSE in retail -- e.g. an out-of-line or inline helper per arm (SetAssignedAbility-like), or the restore written via
  a separate member call; check the matched sibling 0x005F1320's head (`add eax,edi` CSE there IS retail's).
- **0x005F59C7 ConstructPCExpressionsList** DIFFER(2649v2652), free probes only (0 counted). Frame 0xD4 v 0xC8; retail homes the
  text-bank reference in memory and gives ebx the tail's -1; inventory/zero swapped in ebx/edi. Next: the 0x005F4C8F lever
  (everything after the sub-list creation in a {} block) applied to the PC body, plus the PC twin of 4c8f's same-TU def bodies.
- Unreached rows: start from the priors listed in STATUS.tsv, rewrite on shared headers first (the matched rows show the
  shared-header form with the same-TU def bodies is what retail's TU looks like).

## Tools left in tools/
gr.sh (counted grade, logs ATTEMPTS.log), pb.sh / probe.py (free probe; `-p` = plain cl compile without c2prio, use it when the
c2prio gdb attach stalls), mkv.py (swap a function body into a variant), calls.py (call-target pairing), rdis.py (retail
disassembly; run with `python3 -I`), fr.sh / fr.py (frame packer trace), cp.sh (c2prio), tests/ (test generator: prelude.inc +
model.inc + per-row .drv, `python3 tools/tests/gen.py` rewrites test/<addr>.cpp from src/).
