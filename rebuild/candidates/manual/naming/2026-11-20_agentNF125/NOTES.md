# NF125 notes (round 10, PRIORITY slice 6 second remainder: the 8 entries NF118 / NF122 left open)

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log / selfcheck.log (summary in "Counts" at the end of this file).

## CTCHeroExperience family (q4486 q4762 q4764 q4765 q4766): handed back whole
- Claims verified from two sources each: the Ego_r.pdb and FableWin.pdb name sets (QBE const, enum arguments), the Ego_r donor order
  ForceSpendableExperienceTo 0x00587BE0 / GetExperienceSpentOnStat 0x00587C10 / GetStatExperience 0x00587DA0 / GetTrainableStatLevel
  0x00587DB0 / GetCostToUpgradeTrainableStat 0x00587E00 / ForceTrainableStatExperienceTo 0x00587E80 = retail 0x006D6470 / 0x006D64A0 /
  0x006D6510 / 0x006D6520 / 0x006D67B0 / 0x006D6830, and the bodies (13-byte getters reading +0x44 / +0x18 / +0x24 of the retail layout).
- The NF118 probe (probe_hx.tsv) still held: no family row changed in the NF118..NF121 landings. Every wrong binder it listed is moved:
  0x0057BE71, 0x0057C239, 0x005EE887 (0x006D6520); 0x0056FAD7, 0x0056E93D, 0x0056F4BA, 0x0056F597, 0x005724CD (0x006D6510);
  0x005724CD (0x006D64A0); 0x005721FA, 0x005746F7, 0x005724CD (0x006D67B0). Each graded RELOCATION_MATCH / MATCH on its first form.
- Self rows 0x006D64A0 / 0x006D6510 / 0x006D6520 were one free `__fastcall GetAbilityLevel(S*)` stand-in (?GetAbilityLevel@@YIHPAUS@@PAXH@Z,
  the same symbol at three addresses); re-authored as the out-of-line members on engine/CTCHeroExperience.h (new layout opt-in
  STATVECTORS); MATCH.
- SExpSpendUndo rows 0x0056E93D / 0x0056F4BA / 0x0056F597: their own leaves move from `PAUCTCHeroExperience_Calls` to the Ego_r PDB
  `PAVCTCHeroExperience` spelling that every referencing row (0x005785B3, 0x005786B5, 0x00576C4F, 0x00576ED1; exe scan work/side_refs.tsv)
  already binds through engine/CTCInventoryExperience.h. Their local SExpSpendUndo / CLongList models are pre-existing and kept: the
  shared SEXPSPENDUNDO_ONLY mode uses std::list<long>, whose push_back / pop_back would move the 0x0042B687 / 0x005791B5 bindings and
  may inline in the size TU (not tried; a follow-up port, not a naming item).
- Side bindings moved with the stand-in removal (each onto a spelling other landed rows already bind, or onto the only referencing row):
  0x0056FF3D GetTCHeroExperience (private AAE, as 0x005785B3 / 0x005747A3) from rows 0x005721FA / 0x005724CD / 0x005746F7;
  0x006D6470 / 0x006D6830 (0x0056F4BA; 0x006D6830 has no other referencing row); 0x006D6F60 / 0x006D8980 (0x005746F7; 0x005786B5 /
  0x0057770A / 0x005778CA bind the PDB spelling). Found in passing and applied in the same edit: 0x005EE887's GetTrainableStatMaxLevel
  0x006D6770 QAEJJ -> the PDB QBE spelling 0x00899310 already binds.
- Split check (reasoned from the exe scans + probe_hx + pdiff of every changed row): every family target ends with all its referencing
  rows on the PDB spelling; the side targets only shrink. 0x0056FF3D keeps the pre-existing CTCInventoryExperience_Methods spelling of
  0x00572672 (not in this slice). Nothing is created or grown.

## q5107 / q5151 (0x0070D1A0 CAnimSet::Init)
- Both REFS rows already bound ?Init@CAnimSet@@AAEXAAVCAnimateThing@@ABVCAnimInit@@M@Z (NF118 probe_2r). The self row (no Leaf line; HEAD
  object defines ?Init@CAnimSet@@QAEXPAVCAnimateThing@@ABUCAnimSetInitData@@M@Z on a local CAnimSet / vector / CAnimSetInitData model) is
  re-authored on engine/CAnimSet.h + CAnimInit.h with the STLport vectors (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE: the overflow
  bodies visible but out of line, as the old row's same-TU noinline stand-in); body = the landed statement order (the same body 0x0070D780
  carries as its same-TU copy). RELOCATION_MATCH on the first form.
- Its two overflow callees rebind: 0x00710F70 to the spelling its own self row defines (`vector<CTransitionBlendAlpha, allocator>`, __false_type; read from the HEAD
  object of 0x00710F70: ?_M_insert_overflow@?$vector@VCTransitionBlendAlpha@@V?$allocator@VCTransitionBlendAlpha@@@std@@@std@@IAEXPAVCTransitionBlendAlpha@@ABV3@ABU__false_type@2@I_N@Z);
  0x00710980 to the shared STLport `vector<short, allocator>` spelling. 0x00710980's referencing rows are 0x0070D1A0 and 0x0070D2F0
  (exe scan work/side2_refs.tsv): before {local vector@F@@ ..K, local vector@F@@ ..I}, after {std vector<short,allocator> ..I, local ..I};
  the count stays 2 (gate "existing"). Follow-up (not in this slice): 0x0070D2F0 CAnimSet::AddAnim and the 0x00710980 self row
  (`std::vector<short>` without its allocator argument) still model the vector locally.

## q5137 (0x006A2220 CTCCarrying::GetAllCarriedThings): blocked: port, 8 of 10 wrong binders moved
- Claim verified: Ego_r.pdb 0x006C6970 + FableWin.pdb 0x02217EE0 (QBE, STLport vector<CThing*> by reference); the 7 right REFS rows
  (0x00694430, 0x006C3CA0, 0x00785620, 0x0079ACC0, 0x007CDFE0, 0x007CEB00, 0x0089DA70) bind it through engine/CTCCarrying.h (ALLCARRIED).
  NF122 probe.tsv still held (no binder changed since).
- Ported onto std::vector<CThing*> (stlport/fable_containers.h), each binding only 0x006A2220 differently (pdiff): 0x004BFB40 / 0x004BFE20 /
  0x004C0240 (the NGraphicAppearance TU; one transformer, tools/cv.py, removes their identical CAllocator / CAllocProxy / CVectorBase /
  CRetailVector templates), 0x008EC4F0, 0x007E0BF0, 0x0079B9D0 (local CRetailVector / CThingVector stand-ins removed; the local CTCCarrying
  declaration respelled in place), 0x007CF110 (only the carried list; TintList keeps its pre-existing local CRetailVector, an explicit
  fable_containers include before the local CTCCarrying), 0x007AA6B0 (the local CTCInventory::GetAllItems stand-in replaced by
  engine/CTCCarrying.h; interface 0x46 = TCI_CARRYING).
- Left, blocked: port (not attempted; each needs a real port, not a respell):
  - 0x006A2220 (self row): local CArray / CIntelligentPointer / CTCCarrying models with a noinline InsertGrow stand-in for 0x00436AC0
    (NAME_FIXES). Not a split by the gate (self rows are not referencing rows).
  - 0x006A2370 OnSetInLimbo: carries a same-TU noinline GetAllCarriedThings whose body is NOT the owner's (placeholder callees
    GrowCarriedStorage_s7 / NextCarriedThing_s7); GOTCHAS nf96a needs the owner's body verbatim, so it should move with the 0x006A2220 port.
  - 0x006A23E0: whole row in namespace NRemap006a23e0 (about 12 local models); its own leaf is a stand-in too.
- Split check: 0x006A2220's referencing-row spellings go from 7 (PDB, CRetailVector QBE, CTCInventory::GetAllItems, GetCarriedThings on
  CRetailVector, GetCarriedThings on CThingVector, CollectCarrySlots, NRemap; NF122 probe.tsv) to 3 (PDB, CRetailVector QBE on 0x006A2370, NRemap on 0x006A23E0). Nothing created.
- Lock note: 0x004C0240's lock directory existed with owner `released`; I wrote NF125 into it (the same thing NF122 logged as a slip).

## Descriptive names
None new.

## Funclets
No EH funclet row includes a changed row (grep of rebuild/src/compiled for the changed files' names: only build_candidates.ps1).
The three self re-authors 0x006D64A0 / 0x006D6510 / 0x006D6520 change the build entry's Module (CTCInventoryAbilities -> CTCHeroExperience)
and so the file names at landing; none has funclets.

## Flags lines
0x0070D1A0 carries `// Flags: none` (its build entry has no CompilerFlags). No row needed flags its entry lacks.

## Inferred lines
None.

## Tests
Unchanged. All changed rows' tests are self-contained (they do not link the row); 0x0070D1A0's test is a verbatim copy of the old local
model and still passes as a behaviour test.

## Not changed (found in passing)
- 0x005721FA / 0x005724CD keep their local CTCInventoryAbilities_Calls view (GetTCInventoryAbilities 0x0056FFC5 public, returning the stand-in;
  0x005EE77C GetExperienceCostToUpgradeAbility / 0x005EE136 GetAbilityLevel on it): not in this slice; the PDB spellings are the private
  ?GetTCInventoryAbilities@CTCInventoryExperience@@AAEPAVCTCInventoryAbilities@@XZ (0x005785B3 binds it) and the CTCInventoryAbilities members.
- 0x0056FF3D GetTCHeroExperience: 0x00572672 still binds the CTCInventoryExperience_Methods stand-in (pre-existing).

## Decisions for the coordinator
1. 0x00710980 / 0x00710F70 (from the 0x0070D1A0 port): 0x0070D1A0's old local `vector@F@@ .. K` / `vector@CTransitionBlendAlpha@@ .. K`
   spellings are replaced by the shared-header STLport ones, while 0x0070D2F0 (AddAnim) keeps its local `.. I` spellings. The gate reads
   "existing" (2 spellings before, 2 after) and 0x00710F70's new spelling equals its own self row's, but a spelling not bound before
   appears at 0x00710980. Accept under the nf104 shared-header rule, or hold 0x0070D1A0 until 0x0070D2F0 moves too?
2. q5137 remainder: 0x006A2220 (self, local CArray) and 0x006A2370 (same-TU copy of 0x006A2220 with a non-owner body) must move together
   (GOTCHAS nf96a wants the owner's body verbatim); 0x006A23E0 is a whole-row port out of namespace NRemap006a23e0. Naming follow-up or
   Account B's?
3. 0x006D6770 GetTrainableStatMaxLevel: respelled in 0x005EE887 alongside the entry fix (outside my entries): a shrink onto the PDB QBE
   spelling 0x00899310 already binds, so the gate scan of 0x006D6770 is expected.

## Rule slips
- Required reading done only in part: ASM_RULES.md, NF92 NOTES.md and the batch READMEs behind the entry tags were not read; GOTCHAS.md was
  grepped (nf9x/nf1xx lines), EXTRA_RULES.md read with long lines truncated.
- Two Bash calls exited 1 (an awk quoting error and an objdump pipeline); several helper calls ran without the full lane preamble (the
  grade calls went through tools/g.sh / pdiff.sh, which source it).
- 0x006A2220, 0x006A2370, 0x006A23E0 are blocked "port, not attempted", not after 3 failed forms.
- 0x004C0240's lock said `released`; I wrote NF125 into the owner file instead of a fresh mkdir claim.
- Two SRC_FAIL forms (0x007AA6B0 C2248, 0x007CF110 C2653), each fixed on the second form (within budget).

## Counts
- Entries (8): 7 done (q4486 q4762 q4764 q4765 q4766 q5107 q5151), 0 done-already, 0 rejected, 1 blocked: port (q5137: 3 rows left),
  0 deferred, 0 open.
- Rows: 22 ported (3 MATCH + 19 RELOCATION_MATCH, every test PASS in recheck_final.log), 31 not-a-binder, 3 blocked (0x006A2220,
  0x006A2370, 0x006A23E0). RENAMES: 7 self renames (s1), 30 caller respellings (s2).
- Header opt-ins: 3, in one overlay header (engine/CTCHeroExperience.h: STATVECTORS, SPENTONSTAT, FORCEEXPERIENCE).
- selfcheck.log: 0 problems.
