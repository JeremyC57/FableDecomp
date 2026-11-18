# NF88 notes (round 7, queue q11150..q11162)

Counts come from STATUS.tsv / APPLIED.tsv / RENAMES.tsv / NAME_FIXES.tsv in this directory. 36 rows (35 locked + 1 claimed through the lock:
0x004DD1DB): 21 ported, 13 not-a-binder, 2 blocked (port: 0x008E74F0, 0x004C9D60), 0 deferred. 13 entries: 10 done, 1 done-already (q11150, a Ghidra label fix), 2 blocked (q11161:
0x008E74F0 left; q11162: 0x004C9D60 left). recheck_final.log: all 21 ported rows in one vland run, 19 RELOCATION_MATCH + 2 MATCH, every test
PASS. RENAMES: 16 section-1 lines, 32 section-2 lines. NAME_FIXES.tsv: 12 lines. REJECTED.tsv: no claim rejected (every claim held from two
sources). Header opt-ins: 4 macros (+1 nested body macro) in 4 overlays (hdr/README.md).

## q11162: one definition (coordinator ruling) -- what was done and proven
The three owning rows no longer define their header's `_DEFINE` macro; each includes the inline body like its includers and emits it as a
pick-any COMDAT through an anonymous-namespace function marked `// Fidelity: scaffolding, not retail` (the 0x00860150 pattern). The `_DEFINE`
branches stay in the headers, unused (brief).
- **Address reference, not a call.** The helper takes the function's address instead of calling it: a call to a plain `inline` body can be
  expanded in the helper, which would emit no COMDAT. For the protected members (GetValidRushPosition, GetStrongestEngineThemeAt) the address
  is taken in an out-of-class static member of an anonymous derived class that is never instantiated. A first form with the static defined
  in the class (implicitly inline, never used) emitted nothing (0x008C1600 grade 1, OBJDUMP_ERR: a 1 KB object without the function);
  defined out of class it is an ordinary function and VC7.1 compiles it. **The derived-class helper is a scaffolding variant the coordinator
  has not seen before; flag it if only a free function is accepted** (a free function cannot name a protected member).
- **symsel.py:** each owning object had the function as selection 1 (nodup); now selection 2 (pick-any), and the includers carry it as 2.
- **Link proof, `LINK=` cleared** (work/link/*.log; `link /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED`):

  | Owner | Includer | Landed owner + includer (control) | New owner + includer |
  |---|---|---|---|
  | 0x00686F10 AllocateTC | 0x00686FA0 CTCBase::Clone | LNK2005 + LNK1169 | no LNK2005, exit 0 |
  | 0x008C1600 GetValidRushPosition | 0x008C2550 FinishAction | LNK2005 + LNK1169 | no LNK2005, exit 0 |
  | 0x0081F910 GetStrongestEngineThemeAt | 0x0081FA20 GetThemeSizeZOffsetAt | LNK2005 + LNK1169 | no LNK2005, exit 0 |

  The only other diagnostics are the expected unresolved externals (forced).
- Grades: 0x00686F10 1 (RM), 0x008C1600 2 (RM; relocations identical to the landed row), 0x0081F910 1 (MATCH). No row stopped matching.
- The binder 0x004C9D60 (CThing::AddTC) binds AllocateTC as a free `CreateTC` stand-in inside a 504-byte all-local model: blocked/port
  (STATUS names what must move first; NAME_FIXES has the line).

## Inventory chain (q11153..q11159): how the PASS22 header chain was fixed
- NF86's form A failed only because engine/CDefPointer.h needs `FABLE_RETAIL_DEF_CLASS_BASE`. With it, `FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22`
  alone (without `_ORIGINAL_COMPLETE_METHODS_PASS22`, which stays broken) works in the default branch of engine/NInventory_CTCInventoryBase.h:
  it types `Categories` as `std::vector<CCategory>` and includes the PASS22 NInventory::CCategory from engine/CCategory.h. The only header
  addition needed was the NF88 opt-in `FABLE_RETAIL_CCATEGORY_ITEMQUERIES` (member declarations + the two predicates) and the random-access
  `__find_if` in stlport/fable_algorithm.h.
- Ported: the three CCategory queries, the four CTCInventoryBase wrappers (0x005BF2ED was a hidden binder of 0x005C08C0: the earlier slice
  recorded it before 0x005C08C0 was an entry), find_if 0x005C08C0 and __find_if 0x005C0653 (leaf: the made-up predicate
  CIsCategoryWithDefIndex -> NInventory::CIsInventoryCategory), and the binders 0x005D8E70 (own leaf QAE -> PDB QBE) and 0x005BDF08 (nine
  local def / inventory classes gone; 8 forms after the probe, 9 compiles with the final regrade).
- The const (`PBV`) find_if is proven by its five callers, all QBE in Ego_r; the non-const twin is 0x005C08D3 -> 0x005C06C3.
- 0x005C08C0's test linked the old extern "C" stand-in; it is rewritten (rows/005c08c0_test.cpp) to supply __find_if and check the forwarded
  range and predicate; PassPattern UMOVE_005c08c0_OK kept.

## Descriptive names (no PDB spelling exists)
- **Descriptive by library (STLport 4.6; both PDBs are Dinkumware):** 0x004508D6 `_M_erase` (map<EObjectAugmentationType,float>), 0x004DB853
  `clear` and 0x004DD1DB `~_Rb_tree` (map<EHeroExperienceStatCategory,float>), 0x005C08C0 `find_if` and 0x005C0653 `__find_if` (const
  NInventory::CCategory* with NInventory::CIsInventoryCategory). Each is the STLport source name; the second source is the landed caller or
  callee binding (RENAMES s1 evidence per line). NAME_FIXES 0x004E6E43 / 0x004F9960 / 0x006DCE90 are descriptive by library the same way.
- Every other spelling is an Ego_r.pdb and/or FableWin.pdb spelling (cited per line).

## Other judgements
- **0x004DD1DB** was claimed through the lock (`mkdir`, owner NF88): it is the only caller of the renamed clear 0x004DB853 (a self rename's
  caller must follow). Its own caller 0x004E3644 is a `??_G` stand-in (NF86 lesson: needs its ctor TU); NAME_FIXES.
- **0x006DC450 / 0x006DD1C0** (q11152 binders) were the CHeroStatIncreaseData map serialisers under the catalog names
  CHeroExperienceDef::WriteToStream / ReadFromStream; both are self renames to the PDB GFSerialiseMapBinaryOut / In (module `_global`, name as
  the 0x006DC350 / 0x006DCFA0 sibling catalog lines). The writer uses a class specialisation of CPersistTraits<CHeroStatIncreaseData> (the NF81
  CComboMultiplierData precedent); the reader uses the primary template with a declared member specialisation (the 0x006DCFA0 precedent).
- **0x0091D190** (q11160): ported in 4 forms (5 compiles after the probe, with the final regrade); the event pointer is copied out of GetEvent's out slot in an inner scope (retail keeps it in
  edi; a plain out-slot local is reloaded through eax). The event / filter classes and CAIStateGroup_MinionReactToApology have no shared
  header and stay declared in the row; CEventBase comes from engine/NEntityEvents_CEventHitBase.h. Own leaf QAE -> the PDB UAE.
- **0x008E74F0** (q11161 binder) and **0x004C9D60** (q11162 binder): blocked/port on the probe alone (each is an all-local model of 8+
  shared-header classes; STATUS lists what must move first). 0x008E74F0 has a concrete stopper: retail expands CModeBase::OnDeactivate
  inline, and that body is the separately landed strong row 0x00739E60, so a port would need an inline opt-in that recreates the q11162
  one-definition clash (coordinator ruling, or 0x00739E60 re-authored as a pick-any COMDAT first). Its slot / member mapping is in STATUS.
  0x004C9D60 has ten stand-in bindings and an unnamed global (0x013B8A24); not attempted.
- **0x005BE8B1:** CCountedPointer's public `Data` is read directly; `Get()` makes VC7.1 materialise the member address (36 v 34 bytes).

## vland warnings in recheck_final.log (triaged)
- STANDIN_LEAF on 0x004508D6, 0x004DB853, 0x006DC450, 0x006DD1C0, 0x005C08C0, 0x005BE8B1, 0x005BEAD5, 0x005BE8D3, 0x005BF30D, 0x00664670:
  compares against the OLD catalog names, which RENAMES section 1 replaces.
- RELOC_NAME: each target's spelling is proven here: 0x004508D6 / 0x004DB853 / 0x005C08C0 / 0x005BEAD5 (RENAMES s1); 0x004E6E43 / 0x004F9960 /
  0x005BF19C / 0x005C1921 (NAME_FIXES). Checker artifacts (label in demangled form, binding unchanged): 0x013BA89C RegisteredTCs (inherited
  from the landed 0x00686F10) and 0x0091D0A0 GetEvent (the landed self leaf is exactly the bound symbol).

## Housekeeping
- Grade counts in STATUS count new forms graded (the probe of the landed row and a final regrade after comment-only edits are not counted).
- No EH funclet row includes any changed row (grep of rebuild/src/compiled for every changed row's file name).
- The header comments in engine/CTCBase.h, engine/CMap.h and engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h still
  describe the `_DEFINE` scheme; left as is (the brief keeps the macros), the coordinator may reword them.
- NF87's landing (coordinator note) changed engine/NInventory_CTCInventoryBase.h, stlport/fable_containers.h and others in the repo. NF88 overlays
  none of those headers (its four overlays still equal the repo copies byte for byte), uses none of NF87's new macros, and duplicates none of
  NF87's accessors (FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS sits in the !ALL_ITEMS_CHAIN branch; NF88's CCategory members are different ones).
  recheck_final.log was rerun after that landing, against the current repo headers.
- rows.txt was appended with the claimed 0x004DD1DB (rows_all.txt left as given).
- Follow-up not done: the non-const twins 0x005C08D3 (find_if forwarder) / 0x005C06C3 (__find_if) likely carry the same `_Umove`-style stand-in
  names; not probed.
