# NF391 hand-back (2026-11-24)

## Summary
- Job 1 (T3 nf389 0x0051B720): b1 (00519970 0051af20 0051b720) ACCEPT under the coordinator's R1 tiers (follow-up below), b2 (0051a2f0,
  partial tree unification) ACCEPT, b3 (009ab770, the NF132 twin) left as handed back (coordinator lands it). The T3 line narrows (QUEUE q1-q8), it does not close.
- Job 2 (NF385's 30 no-second-source rows): 2 respelt (b4 ACCEPT under the R1 tiers: 005143e0 00615220 -> list::size); 12 more traced
  to an identity with two sources (NAME_FIXES.tsv; held for ports / spelling compiles, QUEUE q9 q11 q12); 6 zero-reference rows kept;
  2 kept with a doubtful NF385 name (00815230 / 00815240); 2 partial; 6 not traced. STATUS.tsv has all 30.
- Job 3 (nf385_b1, 21 rows): all hold (RECHECK_NF385.tsv). Job 2 found one false abs hit in NF385's data: 0x0085A680's only hit
  0x00724664 is mid-instruction (that row has no reference at all).
- Compiles: about 115 of 150 before the R1 follow-up; the follow-up used none (object reads and retail scans only). Locks held: the 7 changed rows; every census lock released.

## Job 3: recheck of landing nf385_b1 (21 rows) -- DONE
RECHECK_NF385.tsv: all 21 hold. No row's two sources rest on a false "abs" hit inside .text.
- 17 rows: the own-address abs refs are .rdata RTTI vftable slots (vt.py re-run: evidence/recheck_vt*.txt, slot names = FableWin).
- 4 rows (00618170, 0074d5d0, 007a6e20, 007a9870): E8 calls, each decoded as an instruction start by a linear sweep of the containing function
  (tools/abscheck.py); the slot-9 paths 0x007A8BE0 > 0x007A8700 > 0x007A6E20 and 0x007AB190 > 0x007AAC50 > 0x007A9870 re-disassembled.
  Note: 0x007A8700 / 0x007AAC50 are not catalog starts, so the xref index labels their sites as inside 0x007A7D20 / 0x007AA9E0 (catalog
  boundary artefact, not an abs issue).
- 00d25d20 / 00d25ef0: the only .text abs hits used (0x00D25693, 0x00D256FB in CQ_DragonBossFightScript::Main 0x00D255E0, and the vftable
  stores 0x00D25C94 / 0x00D25E64 in the Allocs) decode as IMM32 operands of `mov` (real references); literals "DBMinion" 0x012CB754 /
  "DBSummoner" 0x012CB748 pushed just before; vftable 0x012CB78C / 0x012CB7A8 slot 5 = 0x00D25D20 / 0x00D25EF0.

## Job 1: the 0x0051B720 map unit -- evidence so far
### Check 1: FableWin PopulateRegionWithVillagers destroys two locals of the map type (PASS)
FableWin ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z (world_map.obj, 24594 B): two ??0?$map<CThingBuilding*,list<..>> calls
(01C97EB2, 01C97EBE) and, at each exit, two ??1?$map<CThingBuilding*, list<CThingAICreature*> > calls on [ebp-0x70] / [ebp-0x10C]
(01C9A232 / 01C9A241, 01C9DC0E / 01C9DC1D). evidence/fw_populate_calls.txt. Retail: 0x005048C6 / 0x005048D2 call 0x0051B720 on two
locals ([esp+0B4h], [esp+0CCh]).
### Check 2: 0x00502E90's name has two sources (PASS)
- Both PDB name sets hold ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z (one each).
- Callers: retail 0x00502E90 is called from exactly 3 sites (raw full-image scan, tools/rawref.py): 0x00506598 in 0x005064C0, 0x0065C7B4
  in 0x0065C781, 0x008A1CAD in 0x008A1C50. Those three are RTTI vftable slots: CWorldMap vt 0x01244AEC slot 22 = FableWin PostRegionLoad,
  CTCVillage vt 0x0125A9AC slot 9 = FableWin CTCVillage::FrameUpdate, CGameScriptInterface vt 0x01260F0C slot 754 = FableWin
  RepopulateVillage. FableWin's callers of PopulateRegionWithVillagers are exactly those three (tools/fwcallers.py).
- Callees: retail calls AddToWorkBuildingList 0x005003E0 10 times; FableWin calls it 10 times.
- label_trust.py: not applicable (0x00502E90 is not landed).
### Derivation for 0x0051AF20 (PASS on the exe legs)
- Raw full-image E8/E9 scan of 0x00519970 (tools/rawref.py, each site an instruction start): 7 sites; containing functions
  (tools/callerbodies.py): PopulateRegionWithVillagers x2, _M_erase itself, clear 0x0051A560, erase(first,last) 0x0051AF80, and exactly two
  standalone 66-byte bodies of the destructor shape: 0x0051AF20 and 0x0051B720 (evidence/callerbodies_00519970.txt).
- 0x0051AF20 / 0x0051B720 have no other referrer (0x0051AF20: none; 0x0051B720: the two PopulateRegion calls).
### NF132 twin 0x009AB770 (exe + PDB legs PASS)
- 0x009AA830 callers: 22 sites, exactly two standalone 66-byte twin-shaped bodies, 0x009AB770 and 0x009AC310 (evidence/callerbodies_009aa830.txt).
- Both PDB name sets: the CCharString/CCharString tree appears only with map traits `$0A@` (no multimap, no set over the pair).
- Aside (queue): 0x009AA830 also has two 51-byte clear bodies (0x009AACE0, 0x009EF320) -- the same kind of pair (map::clear v _Rb_tree::clear).
### Tree-spelling unification: compile of the repo STLport (probe, work/probe/p_small.cpp, p_big.cpp)
- With FABLE_RETAIL_STLP_MAP (+ TREE_ERASE), map::find / map::erase(it) / map::insert(it, v) are inline forwarders; the out-of-line calls bind
  `_Rb_tree::find`, `_Rb_tree::erase(iterator)` and `_Rb_tree::insert_unique(iterator, const value_type&)` (40 of 40 in p_big too).
- The probe's 71-byte `?find@?$_Rb_tree@...` is EQ_MASKED to retail 0x00513420 (71 B). Canonical symbols: evidence/canonical_symbols.txt.

### Unit result (b1) and gates
- Rows: 00519970 = explicit instantiation of the STLport _M_erase; 0051af20 / 0051b720 = an anonymous-namespace holder with
  std::map<CThingBuilding*, std::list<CThingAICreature*> > and an out-of-line destructor ("Fidelity: scaffolding, not retail",
  GOTCHAS nf86 / nf88, nf387 form). The holder emits ~map, ~_Rb_tree, _M_erase, clear as Any + one 66-byte NoDuplicates holder dtor;
  no ??_G?$map (the `->~map()` recipe would emit one, outside the nf387 exception).
- vland --recheck-landed HEAD and new at one path (work/wd_u1h, wd_u1n): RELOCATION_MATCH PASS x3 both sides. The STANDIN_LEAF
  warnings compare the Leaf against the catalog name (still the stand-in label); non-blocking, the catalog label needs the new names.
- objsec: port-level COMDAT changes only (evidence/objsec/*.comdats.txt): the stand-in sections out, STLport pick-any copies + the
  holder dtor in.
- pk_all + pickany_vs_retail: HEAD, new, and new with TABLE_CHANGES applied (scratch FABLE_ROOT work/troot): ~_Rb_tree EQ at 0x0051AF20,
  ~map EQ at 0x0051B720, _M_erase EQ at 0x00519970, _List_base dtor EQ at 0x004AB4F0; no new NEQ (HEAD 00519970 had one NEQ: the
  one-argument ??1?$list copy at 0x004AB4F0; it is gone). Reach flips n->y: 0 (evidence/reachflip_all.txt).
- /FORCE link (evidence/link_b12.txt; rows + 13 tree-TU context objects): duplicates 1 -> 0; only RESOLVED_TO_GONE of removed
  definitions, and ?Erase@CRetailTree_erase00519970 (unresolved at HEAD) is gone.
- Census by compile: clang prefilter over 82 rows naming the tree / old spellings (evidence/census/clcensus_unit.txt) + VC7.1 of the
  13 tree-TU rows and 00511ce0 / 008320c0 (gr_census.log, gr_census2.log): the only binders of the old spellings are the unit's rows,
  plus 0x0051A560 (Detach_00519970, T3 flatten2, q5).
- apply_table --dry (evidence/apply_dry_b1.txt, apply_dry_all.txt): 0 misses; multi-target 1 -> 1, no new.

### b2 (0051a2f0) gates
vland RELOCATION_MATCH PASS both; objsec ALL_EQ_PERM renames=5 (PBV -> QAV on erase, ~pair, ??_G pair, _Destroy, deallocate); pk no NEQ;
link in link_b12.txt; no table line names the PBV spelling (census as b1). No TABLE_CHANGES.

### b3 (009ab770) gates
VC7.1 census of every ~_Rb_tree<CCharString> definer (evidence/census/b3_rbtree_copies.txt): 22 copies (19 clang DEF rows + 009ac310
00dbb310 00f0fb70) all Any, 66 B, bytes + relocations EQ to this row's copy; 00e55ce0 (T3 nf294) not compiled (stay-off).
vland RELOCATION_MATCH PASS both; pk with the changed table: _M_erase EQ at 0x009AA830, ~map EQ at 0x009AC310, ~_Rb_tree EQ at 0x009AB770.
/FORCE link (evidence/link_b3.txt; with 009a7b70 009c46b0 00da9c70 00daa260 00daa700 00dbb310 00df4be0 00f0fb70 009aa830 009ac310):
duplicates 1 -> 0 (the stand-in dtor was defined by both 009ab770 and 009a7b70), nothing resolved -> unresolved. VC7.1 census
(gr_census3.log, gr_census6.log, census/comdats_*.txt): every ~_Rb_tree<CCharString> copy is Any 66 B; 009a7b70 is the only remaining
definer of ~CRetailTree_erase009aa830 and binder of ?Erase@CRetailTree_erase009aa830 -> the Erase line is rewitnessed 009ab770 -> 009a7b70
in HAND_TABLE_CHANGES.tsv (witness not a row of the run). TABLE_CHANGES adds the ~_Rb_tree line. apply_table --dry: 0 misses, no new
multi-target.

### b4 (005143e0, 00615220) gates
vland MATCH PASS both; objsec NOT_EQ at port level (the NoDuplicates ?SortTreeRecursively@@YIHPAUTree@@@Z section replaced by pick-any
list members); pk no NEQ, reach flips 0. /FORCE link (evidence/link_b4.txt, with the 6 census rows + 0058578b): duplicates 1 -> 0, only
RESOLVED_TO_GONE ?SortTreeRecursively (nobody binds it). Census: clang over 417 rows; VC7.1 of 005029a0 00502b00 005fc2d0 005fc8a0
005fcbe0 005fddd0 (define the new size names, Any, 24 B, section EQ to the new rows' copies; none calls them from its leaf) and 0058578b /
00511ce0 (no size of these lists). 007fdf60 (T3) also defines the CIntelligentPointer size per clang; not compiled (stay-off).
004b87d0 006cd240 00da0ed0 still define ?SortTreeRecursively (q10).

## Job 2 notes
Traced identities and the reason each is held are in STATUS.tsv / NAME_FIXES.tsv. Both PDBs are Dinkumware for std containers, so for STL
internals the PDB gives the container + element type (the identity) but not the STLport mangled spelling; that spelling must come from a
compile of the repo STLport at the port (q11). Class members with a PDB spelling: 00a73cf0 / 00a127c0 (q9), 005c1390 / 00656ead (q12).

## Rulings needed
- R1 (b1, b3, b4; q13): the holder / explicit-instantiation forms emit pick-any COMDATs beyond the leaf and one per-TU helper: the
  STLport special-member chain (allocator / alloc_proxy / _Rb_tree_base / _List_base dtors, deallocate, clear, _Destroy, ??_G?$pair from
  _Destroy's explicit p->~T(), distance / __distance for list::size). The nf387 GOTCHAS line accepts only the leaf's selection + one small
  per-TU helper (and EQ-at-retail PDB-named copies); NF390 R7 says no blanket exception for unreferenced NOADDR pick-any chains: "pilot,
  bin each new COMDAT, bring the residue back". The bin is evidence/comdat_bins.tsv (per row: LEAF, HELPER = the 66-byte NoDuplicates
  holder dtor, EQ = copies pk places EQ at a retail address, NOADDR split into ALREADY_IN_BUILD (another compiled object already
  defines the same symbol) and NEW_TO_BUILD (relative to my compiled set only)). Residue to rule on: b3 none beyond the helper (every
  NOADDR copy is already defined by landed rows 009aa830 / 009ac310 / 00dbb310 / 00f0fb70); b1 7 NOADDR copies of the CThingBuilding
  tree chain not defined by any compiled object (the old rows used local models); b4 2 per row (distance / __distance). Asked: accept
  ALREADY_IN_BUILD NOADDR copies and the holder as the per-TU helper (then b3 ACCEPT), and rule on the NEW_TO_BUILD residue (b1, b4).
  Precedent to weigh: NF39 0x00ADB570 landed list::size in the same explicit-instantiation form.
- R2: canonical _Rb_tree::find form (q1): 23 member-template ??$find@ lines v 14 plain ?find@ lines; the repo STLport emits plain ?find@.
- R3: 0x005003E0 / 0x0051BF90 binders: a respell moves the call onto a different wrong body (NF305 Q1), so they are ports (q2 / q3).

## Other notes
- b4 adds no table line: no landed row calls 0x005143E0 / 0x00615220 (callers 0x00502E90 and 0x0093AA40 are not landed); the six census
  copies are section-EQ to the rows' copies, which MATCH retail. (Table lines for list::size exist only where a landed caller binds them,
  e.g. 00adb570 witness 00ad9f10.) Two of the other SortTreeRecursively selves already have caller-bound size names in the table:
  ?size@?$list@VCBoast@CQuestManager@@...@ -> 004b87d0 and ?size@CRetailList_CScriptThing@@QBEIXZ -> 00da0ed0 (q10).
- Tests are unchanged. The 00519970 test is self-contained (its own local model class, still named CBuildingCreatureMapTree, never links
  the row) and PASSes against the new row in vland; renaming that test-local model is cosmetic and left to the coordinator.
- The catalog labels of all 7 changed rows still carry the stand-in names (vland STANDIN_LEAF warnings, non-blocking).

## Deviations
- While re-running my own assembler I deleted its generated batch subfolders once with rm -rf (my own scratch output only, no repo
  file); later re-runs used clean.py.
- The six b4 census rows (005029a0 00502b00 005fc2d0 005fc8a0 005fcbe0 005fddd0) were locked after their tests failed the clang precheck
  (only inside VC7.1 headers) and before a cl /Zs of those tests; their sources then passed the precheck and they compiled
  RELOCATION_MATCH. Locks released. The later census sets (0058578b, 009aa830, 009ac310, the 19 ~_Rb_tree<CCharString> definers)
  followed the order: test precheck (clang, else cl /Zs) PASS -> lock -> source precheck -> compile.

## R1 follow-up (coordinator ruling R1, 2026-11-24)
Tool: tools/tier391.py (writes evidence/scan_b1.tsv, scan_b4.tsv and the comdat_cmp.py proofs in evidence/tier1_b1/, tier1_b4/).
Landed definer set = work/hobj/* (VC7.1 HEAD objects of landed census/context rows) minus the batch rows' own HEAD objects and 0051a2f0's
PBV HEAD, plus b2's new 0051a2f0 object (treated as landed, as asked). Per new COMDAT (leaf and the per-TU holder excluded):
- Tier 1: byte-and-reloc identical (comdat_cmp.py load) to the same-named COMDAT of a definer; the comdat_cmp output for that pair is saved
  and shows EQ for the symbol. b1: 10 per holder row, 9 for 00519970; three of them (_Destroy, ??_G?$pair, the tree-node deallocate) match
  only b2's new 0051a2f0, as predicted. b4: 7 per row.
- Tier 2: reloc-masked scan of every executable retail section; a hit counts at a catalog start or a 16-aligned address after 0xCC. Where
  every reloc symbol has one table home (work/troot = repo table + this run's TABLE_CHANGES) the scan also requires the retail reloc to
  resolve there. Results:
  - _Rb_tree<CThingBuilding...>::clear (51 B, 1 reloc): masked 158 function starts, resolved (call -> 0x00519970) exactly 1: 0x0051A560
    (unique home, recorded). It carries the landed self OnKill (T3 flatten2) -> NAME_FIXES item.
  - _Rb_tree_base dtor (14 B, call free): 1117 function starts, all resolving to free -> home exists, not unique, no address (R9).
  - distance / __distance for both b4 lists (26 B, reloc-free, byte-identical bodies): 99 function starts -> home exists, not unique.
  - allocator / _STLP_alloc_proxy dtors of the tree node (1 B, reloc-free `ret`): tier-2 small, accepted, nothing recorded.
  My reading of the ruling: a non-unique tier-2 match is still a retail home (accepted, no address recorded, R9). If the coordinator
  meant "unique only", the residue is: b1 _Rb_tree_base dtor (14 B) on 0051af20 / 0051b720; b4 distance and __distance (26 B each) on both rows.
- UNIT_LEAF: 0051af20 / 0051b720 also emit the unit's other leaves (~map / ~_Rb_tree / _M_erase); they are placed by the b1 TABLE_CHANGES
  self lines, pk EQ at 0x0051B720 / 0x0051AF20 / 0x00519970.
- No residue -> b1 and b4 RESULTS rewritten as ACCEPT; every old gate is unchanged (same row objects).
- Check (d): see batches/b1/T3.txt (grep of 00519970, 0x00502E90 -> 0x0051B720 calls, the 009ac310 ~map line's caller witness 00e55ce0).
- R2 guidance noted (decide the find spelling per class from a VC7.1 compile of the repo header; map::find and _Rb_tree::find are
  different functions); no action taken. R3: queue only.
