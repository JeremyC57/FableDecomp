# NF317 notes (2026-11-23, w22): the in-place rejects of today's scripted routes (112 entries)

Base: the repo working tree of today. No repo writes, no git. Reading done in full before the first compile: BRIEF.md, BRIEF_NF_BATCH.md
(all rulings incl. the 2026-11-23 switch-route ruling), GOTCHAS.md (514 lines), the 2026-11-23 naming finish plan, the landing READMEs
access1..4 / binders1, the NF316 hand-back; skimmed EXTRA_RULES (naming sections), COORDINATION.md, access_fix.py.

## Result: 112 / 112 entries have a STATUS
- **47 done** (53 changed rows: 47 entry rows + 6 unit binders), **56 skipped-port**, **9 skipped-held**. REJECTED.tsv is empty (every
  non-landing entry is a STATUS line with its reason).
- Every changed row: precheck (clang, or `cl /Zs` per D1 for the four unchanged tests that fail only in VC7.1 headers / on MSVC-only
  syntax: work/pre_zs.log), lock held, HEAD v new graded at ONE path (nf.py grade), objsec ALL_EQ under the renames with `.drectve`
  included and the COMDAT / DEF sets SAME (evidence/proof_g1 / g23 / g4 / g5 / g6.txt; the `tables` column lists only vftables HEAD
  already emitted), the graded symbol is the PDB symbol (full mangled Leaf written on every self), vland --recheck-landed MATCH /
  RELOCATION_MATCH PASS (recheck_final.log: 5 MATCH + 48 RELOCATION_MATCH on the 53 rows; two more rows were rechecked and then
  withdrawn, see below), and the link check (evidence/link_final2.txt: the 53 rows; link_all / link_final still contain the two withdrawn rows).
- **Link check** (link_final2.txt): HEAD set (53 HEAD objects + 60 compiled census controls + 4 callee self rows) leaves the PDB spellings unresolved; the new
  set resolves every one whose definer is in the set; no reference HEAD resolved is left unresolved; the duplicate set is unchanged
  (5 pre-existing pick-any LowerBound / GetSubDef lines in both).

## What the script's rejects really were
- **8 objsec "relocs False" rejects were not differences** (19, 20, 25, 34, 35, 49, 51, 107): the coordinator's vland compiled HEAD and new
  in different directories, so each anonymous-namespace symbol got a different `?A0x` path hash. nf.py now compiles HEAD and new at one
  path; the script's own attempts were ALL_EQ. Entry 57 likewise: the static `__ehhandler$<self>` name follows the self rename (nf.py proof
  now maps derived names).
- **16 access1 Q->U / Q->M SKIPs** were never attempted with `virtual` (access1 ran without --virtual). 13 landed (NF299 Q3: no vftable
  emitted, offsets kept): a placeholder slot (`v09`) or a data vptr stand-in (`void* __vftable`, `m_unused0`, `_pad00[..]`) became the
  real slot at the rtti-vtables index; 0x00995D30's explicit `vtable[2]` __fastcall cast became a virtual call through a modelled
  polymorphic CDataInputStream base (slot 2 = 0x00405FA0 GetPosition const, labels_rtti_port). 0x00BAD670 is a port (flat shared base
  header), 0x006A4E90 / 0x008F1720 are held (other T3 lines).
- **LINK_FAIL (23, 38, 66, 85, 99, 109)**: the landed test declares the member and links the row object; the test model moves with the row
  (rows/<addr>_test.cpp; a protected self is reached with `friend int main();` in the test model).
- **SRC_FAIL after const**: landed where the const is free of symbols or onto the PDB:
  - STLport const `operator[]` / `begin()` added to the local inline container models (7 rows; the helpers are unreferenced pick-any
    COMDATs that follow the rename, no relocation names them; other landed rows already define the same const helper names with the same
    element-load body);
  - the callee declaration takes its PDB const (0x005DC070 -> 0x005DB630 QBE, whose self defines it: now resolved; 0x00958E20 -> 0x00958C00
    ABE and 0x0096D380 -> 0x0096D2D0 IBE, whose selves define free __fastcall stand-ins: NAME_FIXES);
  - R1 const slots for virtual calls (0x005FAA60 slot 3, 0x0096D190 slots 14 / 15);
  - a local pointer to a member becomes a pointer to const (0x009D4A50, 0x008D0BA0);
  - the force-inline GetTC / GetInterface lookups of the local CThing model become const (0x004C8C90, 0x00665E90).
- **Ports (56)**: 23 CCreatureAction_*::Clone (the TU emits the class vftable through the inline copy ctor, `virtual Clone() const` grows
  the stand-in ??_7: a vtable change, NF299 Q3 fails: decision / port onto engine/CCreatureActionBase.h); 14 return-type edits (the
  row returns the full-width / byte value retail's bytes show; the PDB bool / void needs re-authoring); mutable needs (4); const that
  moves a callee onto another wrong form or forces const on a same-TU stand-in (NF305 Q1 / NF309 Q3: 9); flat shared-header bases (2);
  a free stand-in caller needing a friend (0x00731BF0, NF281 D1); a same-TU strong copy that would duplicate its owner (0x0058509F);
  a stand-in cast (0x00406F70).

## Units moved with an entry (no split created)
- 0x00858130 (binders1): the exe census of 0x00856E50 found three more QAE binders (0x00858400 / 0x00858590 / 0x008587B0): all four take
  the protected (IAE) declaration; QAE disappears.
- 0x00663F50 (CThingCreatureBase::OnDie): making the CThing hierarchy polymorphic moved its qualified CThingPhysical::OnDie call onto the
  PDB MAE; the other QAE binder 0x007038D0 (CThingObject::OnDie, also its own self QAE -> PDB MAE, vptr padding shrunk) moves too; and
  0x00832E60 (CThingAICreature::OnDie) bound CThingCreatureBase::OnDie QAE, which this self resolved at HEAD: it moves to UAE (its own self
  QAE -> PDB UAE, NF303 Q1).
- 0x009F4D40 (entry 56): its one exe-census binder 0x009F50A0 bound QBE (unresolved at HEAD); it takes the PDB protected IBE and now
  resolves against the fixed self.
- 0x00908B50: CCombatSequenceBase::OnCreate QAE -> UAE (PDB); 0x008B0650 already carries six spellings incl. UAE: count neutral.

## Withdrawn after compiling (binder identity doubt)
- Entry 108 (0x008C63C0) compiled ALL_EQ / RELOCATION_MATCH with `private: WhackGround` + `friend class CCreatureAction_TrollTunnel;`
  (and the second QAE binder 0x008C6430 with its own friend), but T3_PORT_QUEUE nf299 (identity research on 0x008C61C0) says the donor
  TrollTunnel::PerformAction does not call WhackGround, so the friend would rest on a doubted identity: both rows dropped (rows / orig
  removed with clean.py, locks released). The two rows' vland lines are still in recheck_final.log (marked withdrawn).

## Lessons that changed results
- VC7.1 aligns the first member after an introduced vfptr to the class's alignment: 0x00499C50 has doubles, so `_pad04[0x4c]` put
  LastTextDisplayed at +0x54 (DIFFER 129v129); `_pad08[0x48]` from +0x08 matched.
- The script's access3 runs wrote shrunk pads as `char`; kept `unsigned char` everywhere (0x008D0BA0, 0x00499C50).

## Census / stay-off
- work/offlimits.txt (T3_PORT_QUEUE except my own tags, brief holds, Codex p28 reserved), NF318's entries and rows: none compiled except
  that 0x008C61C0 (T3 nf299) was locked for a few seconds as a planned control and released before any compile.
- Controls compiled (HEAD only, then `nf.py syms` on the UND / DEF sets): the free exe-census rows of every changed target (62, incl.
  0x0068E510 for entry 23 and 0x0058509F for entry 10, added after review) + the callee self rows 0x004B0FC0 / 0x007B2D50 / 0x005DB630 /
  0x00856E50. Census rows NOT compiled (off limits) and what was relied on instead: 0x00994700 (entry 3: source declares `virtual void
  SetPosition(unsigned long)`, the PDB UAE), 0x00994780 (entry 5: `virtual void Close();`, UAE), 0x008B7CF0 (entry 38: `long
  GetDefaultWeaponDef() const;`, QBE), 0x0093E5E0 (entry 20: the table witness of the PDB AAE), 0x00736AE0 (entry 49: binds the stand-in
  ?UpdateCombatTimers@CTCCombat_Methods@@QAEXXZ, table), 0x00891870 (entry 41: NF318-locked; binds @QuestMgr_IsHeroOnQuest@4, table).
  Entries 66 / 85 / 99 have no exe-census binder (virtual slots only). Control tests are not built (failures there only in VC7.1 headers).
- Model check: the `class|struct X {` sets of orig/ and rows/ are identical for all 53 rows and 6 tests (no new local model; the first
  0x00995D30 version had added a local CDataInputStream base and was folded back and regraded before hand-back).
- Housekeeping slip: one throwaway command wrote `/tmp/x_rc` (a sorted address list) outside the scratchpad; nothing else left there.
- New symbol names: the const helper COMDATs (`??ACLongVector@@QBEABJK@Z`, `??A?$vector@M@@QBEABMJ@Z`, `??A?$CRetailVector@..@@QBE..`,
  `?begin@?$CRetailVector@..@@QBE..`, `??$GetTC@UCTCScriptControl@@@CThing@@QBE..`, `??$GetInterface@VCTCPerceiveThing@@@CThing@@QBE..`):
  grep of the landed sources: the same names are already defined by other rows with the same body text, or not at all.

## Which done rows are unconditional
- Unconditional (access / virtual / callee-PDB const / test moves / binder units): 37 entries, 42 rows.
- Ride on the rulings asked below (Q1 / Q2): entries 24, 26, 27, 28, 41, 42, 56 (inline const container overloads), 37, 45 (const
  force-inline lookups), 29 (const slot of an unproven-name slot). If the coordinator rules against, they are skipped-port and their 10
  rows (0x00439AB0, 0x00581CFD, 0x00581D4E, 0x00581FA4, 0x004B1110, 0x00581D8E, 0x009F4D40; its binder 0x009F50A0 stays valid either way (NF302 Q2:
  unresolved before and after if the self does not land), 0x004C8C90, 0x00665E90, 0x005FAA60) drop out.

## Questions for the coordinator
1. Inline const overloads on local stand-in container models (STLport const `operator[]` / `begin()` on `CLongVector`, global `vector`,
   `CRetailVector`): I treated them as R1 ("the const overloads the PDB / library gives"; nothing is called, the unreferenced helper COMDAT
   follows the rename). If NF309 Q3 ("a self fix that forces const onto same-TU stand-ins is T3") is meant to cover them, entries 24, 26,
   27, 28, 41, 42, 56 move to skipped-port.
2. Same question for const on the force-inline lookup templates GetTC<T>(long) / GetInterface<T> of the local CThing models (37, 45)
   and for the R1 const slot named `IsBlocked` (29; slot 3 of NHeroInformationScreens::CBase, 0x004D2E41, its name is unproven).
3. The 23 Clone entries: port onto engine/CCreatureActionBase.h (one family edit), or is a growing stand-in vftable COMDAT acceptable when
   no landed binder references it?
4. The 14 return-type entries: the rows return what retail's bytes return (`mov eax,1`, `setle al` without zero-extension, ...), while
   the PDBs (a later revision) say bool / void. Re-author with bool, or rule that the PDB signature is a revision difference?
