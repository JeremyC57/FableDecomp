# NF409 hand-back (2026-11-25): T3 nf405 0x004DA6A9 identity line (the four rows that still defined the stand-in)

## Summary
The four rows 0x004DA6A9, 0x004E41A1, 0x005C1390 and 0x00656EAD each strongly defined the made-up
`?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z`. All four are compiler-generated, non-virtual scalar deleting
destructors (`??_G ... QAEPAXI@Z`, thiscall, `ret 4`, no vptr store; 31 bytes). None is in a vftable. Each is called with `push 0` from an
STLport `_Destroy` / `__destroy_aux` / `_M_erase`, or with `push 1` from a CCountedPointer DeleteData. Each row is now re-rowed in the
nf86 / nf88 pick-any form on its class's shared header, with `#pragma optimize("s",on)` kept.

| batch | rows | new self | verdict |
|---|---|---|---|
| b1 | 0x004DA6A9 | `??_G?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@QAEPAXI@Z` | ACCEPT (no table change) |
| b2 | 0x004E41A1 + 0x004E7772 (one unit: shared binder) | `??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z` + the tree's `_M_erase` | ACCEPT (TABLE_CHANGES: 1 replace) |
| b3 | 0x005C1390 | `??_GCItem@NInventory@@QAEPAXI@Z` | HOLD, R-NF409-1 |
| b4 | 0x00656EAD | `??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z` | HOLD, R-NF409-2 (TABLE_CHANGES: 1 delete) |

Every row grades RELOCATION_MATCH PASS under vland --recheck-landed, HEAD and new, at one LANDVERIFY_WORK path (work/wd_v3h / wd_v3n;
logs in batches/*/logs). There is no WARN LITERAL_DIFF, no grade drop, and the HEAD WARN SHARED_HEADER_AVAILABLE is gone in every row.
How many copies of the old stand-in remain, from the census /FORCE link (HEAD: 4 definers, 3 duplicate lines):

| landing set | definers | duplicate lines |
|---|---|---|
| b1 + b2 | 2 | 1 |
| b1 + b2 + b4 | 1 (0x005C1390) | 0 |
| all four | 0 | 0 |

## Names: two sources each
- **0x004DA6A9 `??_G?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@QAEPAXI@Z`.**
  (1) The binding table already maps this spelling here (witness 0x004DD79F). That row's object emits it, byte- and reloc-identical to b1's.
  (2) FableWin.pdb has the non-const element type: `??1?$CArray@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@@QAE@XZ`, plus its
  vector, _Destroy and allocator. Retail fits it: only `push 0 ; call` from the 12-byte-stride __destroy_aux 0x004DD79F and from the
  _Destroy copy 0x004DBEDF (rawref: unreferenced). The body runs ~CBaseIntelligentPointer 0x00A01C10 on +4, after the 4-byte key.
  STL instances have no PDB ??_G (both PDBs are Dinkumware), so the spelling is the repo STLport's.
- **0x004E41A1 `??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z`.**
  (1) Ego_r.pdb names exactly this spelling (donor 0x00504923). Ego_r types CObjectAugmentationsDef::AugmentationParticles as
  map<EObjectAugmentationType, CObjectAugmentationParticleSet>.
  (2) Retail: the callers are the tree's _M_erase 0x004E7772 and the unreferenced _Destroy 0x004E6E87. The caller of 0x004E7772 is clear
  0x004E8212, which the table binds as `?clear@?$_Rb_tree@W4EObjectAugmentationType@@...`. The only callee is the table-bound
  ??1CObjectAugmentationParticleSet 0x004E25F1, on +4.
- **0x004E7772 `?_M_erase@?$_Rb_tree@W4EObjectAugmentationType@@...@@AAEXPAU?$_Rb_tree_node@...@2@@Z`.**
  (1) Its non-recursive caller is that tree's table-bound clear 0x004E8212, which pushes the root: STLport's `_M_erase(_M_root())`.
  (2) Its value destructor is the pair ??_G above, and 0x004EC33E's object (a shared-header explicit instantiation of the same tree)
  emits the same _M_erase, byte- and reloc-identical. The bytes are 0x004E6E43's twin. The spelling is descriptive-by-library (STLport).
- **0x005C1390 `??_GCItem@NInventory@@QAEPAXI@Z`.**
  (1) FableWin.pdb names exactly this spelling, beside ??1CItem@NInventory@@QAE@XZ.
  (2) Ego_r's NInventory::CItem is 0x14 bytes with PPerItemData (CCountedPointer<NInventory::CPerItemData>) at +8. 0x14 is the stride of
  the table-bound __destroy_aux<NInventory::CItem*> 0x005C18C6, which calls it, and +8 is the member whose destructor 0x005C04BF it calls.
  The _Destroy copy 0x005C149E is unreferenced.
- **0x00656EAD `??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z`.**
  (1) FableWin.pdb names exactly this spelling, with ??1, DeleteData and ReleaseData of CCountedPointer<CQuestInfoElementCounterList>.
  (2) Retail: 0x0064FA77 news 0x28 bytes and runs the table-bound ??0CQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAE@JJM@Z
  0x00656C0A. That ctor stores vftable 0x0125A504, whose RTTI is .?AVCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@ (slot 0
  DrawAt, slot 3 GetType = 4 = QUEST_INFO_ELEMENT_COUNTER_LIST; no destructor slot, so the ??_G is non-virtual). The object then goes to
  the CCountedPointer ctor 0x00657138. The deleter that ctor stores, 0x00657016 (`if (p) delete p`), is the only reference to 0x00656EAD.
  +0x10 is CQuestInfoElement::PrimitiveHandles (Ego_r), whose ~vector is 0x00440C40 (table-bound).
- Adjacency was not used: three sibling ??_G bodies of the same shape sit at 0x00656E6F, 0x00656E8E and 0x00656EAD. The ctor chain decides.

## How each row is written (forms measured)
- The four `??_G` rows use the nf86 / nf88 form: an anonymous-namespace helper, marked `Fidelity: scaffolding, not retail`, that destroys the object
  through a typedef (`p->~T()`), or with `delete p` for 0x00656EAD (the retail DeleteData's statement). 0x004E7772 is instead one explicit
  instantiation of the stlport/fable_map.h _M_erase, the 0x004E6E43 recipe.
- Headers and macro sets are copied from landed users:
  - 0x004DA6A9: fable_containers.h + CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER (0x004DD79F).
  - 0x004E41A1 / 0x004E7772: fable_map.h + EObjectAugmentationType.h + CObjectAugmentationParticleSet.h with FABLE_RETAIL_CCHARSTRING and
    FABLE_RETAIL_CDEFSTRINGTABLE (0x004EC441).
  - 0x005C1390: NInventory_CTCInventoryBase.h with FABLE_RETAIL_CTCBASE, _CTCINVENTORYBASE_CONFISCATED and _NINVENTORY_CITEM_TRANSFERIN.
    TRANSFERIN is the existing opt-in that leaves out the out-of-line ~CItem, giving the implicit destructor retail's ??_G inlines
    (0x005C2118 precedent).
  - 0x00656EAD: NPlayerGui_CDrawQuestInfo.h with FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS and the 0x00654C05 macro set. CQuestInfoElementCounterList
    declares no destructor, so it is implicit.
- 0x00656EAD also ends with `#pragma optimize("", on)` (R-NF409-2). Without it, the header's ~vector<CEnginePrimitiveHandle> pick-any
  copy is a 30-byte size-mode body, NEQ at 0x00440C40 and reached from the leaf. With it the copy is EQ at 0x00440C40, and the leaf is
  unchanged.
- No header was edited (HEADERS.tsv is empty). Every opt-in used already exists.

## Gates
- **vland** (`runpair.sh v3`): HEAD and new under one LANDVERIFY_WORK path.
  - All five rows grade RELOCATION_MATCH PASS on both sides, with the same PassPatterns (VDD_PARITY_OK, VDD_OK, PASS_004e7772,
    PASS_005c1390, VDD_OK).
  - New WARNs on 0x004E7772 only: RELOC_NAME and STANDIN_LEAF. Both are the catalog's propagated label "LTextBinTree...BuildTreeArray" and
    "ScalarDeletingDtor_004e41a1"; the relabels are queued in T3.
  - HEAD WARN RELOC_CONFLICT (BuildTreeArray) and RELOC_NAME (?Destroy_004e41a1, ?Sub_destroy) are gone.
- **pk_all + pickany_vs_retail** (--leaf given), HEAD (repo table), new (repo table) and new (troot = repo + TABLE_CHANGES + SELF_SYMBOLS):
  - 0 NEQ and 0 reach flips in every row's object.
  - EQ copies: ~CCountedPointer<CPerItemData> at 005c04bf (HEAD too), ~vector at 00440c40 (b4), and the pair ??_G at 004e41a1 (in 0x004E7772's object).
- **Census pk** (pkcensus.py): the 85 census objects compiled with VC7.1, HEAD table v troot.
  - The only verdict changes are the three ??_GCItem copies of R-NF409-1.
    - Framed (`#pragma optimize("y", off)`): 0x005BF969 (reached) and 0x005C1ECC (already NEQ and reached at HEAD).
    - Frameless: 0x005BF91A and 0x00621A20 (unreached), whose copies call the header's ??1CItem.
  - Pre-existing debt, unchanged by b1: 0x006A3A70 and 0x006A3CD0 carry a 33-byte copy of b1's leaf name, NEQ at 0x004DA6A9 already at
    HEAD (the table line exists, reach n). Queued in T3 (R10).
  - 0x004EC33E's pair ??_G becomes EQ (resolved through the new table line).
- **Extra COMDATs** (R1 / R10; evidence/scan_v1.tsv, scan_v2.tsv, tier1_v1/, defscan_v2.txt, retail_homes.txt):
  - b1: ~CIntelligentPointer<CThing> and ~pair are TIER1 (0x004A3740, 0x006A3A70, 0x004DD79F).
  - b2: ~pair, _Destroy<pair> and deallocate<node> are byte- and reloc-identical to 0x004EC33E's copies.
    - 0x004EC33E includes stlport/fable_map.h and engine/CObjectAugmentationParticleSet.h and adds only an explicit instantiation in
      `namespace std`. tier398's shared() heuristic reads that as a local model and prints RESIDUE. I treat them as TIER1; see
      evidence/*__004ec33e.txt.
    - ~pair also has a unique retail home at 0x004E3663 (`add ecx,4 ; jmp 0x004E25F1`): unreferenced, off-catalog, after 0x004E3644's `ret 4`.
  - b3: ??1CItem is TIER1, with 8 landed definers.
  - b4: the whole vector<CEnginePrimitiveHandle> chain is TIER1 v 0x0072DB90 / 0x0072DBE0 / 0x004A3740.
    - ~CQuestInfoElement and ~CQuestInfoElementCounterList (8 B each) have four non-unique retail homes, all unreferenced, off-catalog
      `add ecx,10h ; jmp 0x00440C40` stubs: 0x006567BB, 0x006568A3, 0x006568AB, 0x006568B3.
  - The per-TU helpers are scaffolding (nf387).
- **TABLE_CHANGES** (`apply_table.py --dry`):
  - b2 replaces 1 line and b4 deletes 1; 0 misses; multi-target count 1 -> 1, nothing new.
  - b2: `?Destroy_004e41a1@CInner@@QAEXH@Z -> ??_G?$pair@$$CBW4EObjectAugmentationType@@...`, 004e41a1, witness 004e7772.
  - b4: `?Sub_destroy@@YIXPAUSub@@@Z 00440c40 00656ead` (its only user).
  - Fixed-string grep of rebuild/src, rebuild/oracles, rebuild/include and rebuild/tests: the old self has no table line and no caller,
    and appears only as the Leaf of the four rows (plus a comment in 0x004DBE39).
- **Census /FORCE link** (evidence/link_*.txt, link_summary.txt): 85 census objects plus the five rows. Duplicates by configuration:

  | configuration | duplicates |
  |---|---|
  | HEAD | 21 |
  | b1 alone | 20 |
  | b2 alone | 19 |
  | b3 alone | 20 |
  | b4 alone | 20 |
  | b1+b2 | 18 |
  | b1+b2+b4 | 17 |
  | all | 17 |

  - No duplicate symbol grew in any configuration, and nothing HEAD resolved is left unresolved.
  - Gone from the unresolved list: ?Destroy_004e41a1@CInner@@QAEXH@Z, @Sub_dtor_004e25f1@4 and ?Sub_destroy@@YIXPAUSub@@@Z.
  - The census set has 53 + 32 objects:
    - Clang prefilter (clcensus) over the header reach of the five classes: 1612 rows, reachset2.py, header `//` comments stripped.
      58 rows DEF or UND a watched name or fail in clang (the 21 failures are among the 58). Without this hand-back's five rows,
      that is 53, all compiled with VC7.1.
    - The 32 other rows that include NPlayerGui_CDrawQuestInfo.h (35 in all; 3 were already in the 53), compiled with VC7.1.

## Locks
- Kept as NF409 (ACCEPT): 004da6a9, 004e41a1, 004e7772.
- Released (HOLD): 005c1390, 00656ead.
- Census compiles were read-only HEAD compiles of other rows: no edits and no locks.

## Deviations / notes
- A first compile of 0x004E41A1 hung on a header `#error` (engine/CDefString.h needs FABLE_RETAIL_CDEFSTRINGTABLE; the NF402 gotcha).
  - The cl.exe was mine (pid 25404). The classifier refused my kill of it. It went away when its job's `timeout` expired, and no cl of mine
    is left.
  - Every later source was prechecked with clang first. I never ran wineserver -k.
- Nothing was written to the repo, and no git was run.
- The stray `/home/user/FableDecomp/D:\Documents\FableTLC` tree dates from 2026-11-19. It is not mine.
- Census tools added under tools/:
  - vccensus.py: read-only VC7.1 compiles, no lock check.
  - pkcensus.py: pk_all, HEAD v troot, for every census object.
  - defscan.py: definers of each name, grouped by body.
  - reachset2.py: reachset.py with comments stripped.
- work/troot is a full symlink root with the two table files replaced, so pk_all can import verify_and_land there.

## Descriptive names
None. Every self and callee name is a PDB spelling or the repo's STLport spelling of a PDB-typed instance.

## Follow-up after the rulings (coordinator message: b1 / b2 landed as nf409_b1 / nf409_b2)
- R-NF409-1 ACCEPT. The rawref check passed: retail's own chain is 0x005BF969 -> 0x005C1ECC (call at 0x005BF977) -> 0x005C18C6
  (0x005C1EF2) -> 0x005C1390 (0x005C18D2). The ??_GCItem copies are now two T3 lines: a `port` line (0x005BF91A, 0x00621A20) and a
  `lever` line (0x005BF969, 0x005C1ECC) that carries the link-order note.
- R-NF409-2 ACCEPT. Checks 1-3 passed:
  1. Without v with the closing pragma, only the vector<CEnginePrimitiveHandle> template chain differs.
  2. No CompilerFlags on 0x00656EAD, and neither CompilerFlags nor optimize pragmas on 0x0072DB90, 0x0072DBE0 or 0x004A3740. This is now
     cited in the row's comment (comment-only edit).
  3. `Sub_destroy` has no other user.
- Locks 005c1390 and 00656ead are re-claimed as NF409.
- Re-run on the current tree:
  - vland HEAD/new, one path per batch (wd_r3*, wd_r4*): RELOCATION_MATCH PASS, no LITERAL_DIFF.
  - pk_all: 0 NEQ / 0 flips on both the current table and the table with the changes applied.
  - Census link with b1 / b2 as the baseline: 18 -> 17 duplicates, stand-in definers 2 -> 0, nothing newly unresolved.
