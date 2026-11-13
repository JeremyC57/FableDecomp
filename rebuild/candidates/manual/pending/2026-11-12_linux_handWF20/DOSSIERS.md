## 0x00AD9F10

# DOSSIER 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (806 B = 0x326; draft header's "0x324" is wrong)

## 1. Retail shape (xdis 00ad9f10 0x336)
- __thiscall, `ret 4` (one arg), 4 exits all `pop..; add esp,40h; ret 4`. No frame pointer (/Oy), no SEH (no /EHsc state
  despite a local vector with a dtor), speed idioms (`lea esp,[esp]`/`lea ebx,[ebx]` loop-alignment nops) -> default /O2, no pragmas.
- Prologue `sub esp,40h; push ebx; push esi; push edi`; **ebp pushed late** (0xAD9F7C, after the two early-return tests,
  shrink-wrapped) and holds the hoisted constant 0. The first early-exit path pops only edi/esi/ebx (0xADA22D).
- `this` in esi throughout; ebx = &OptimalBlockList (this+0xC), also spilled to [esp+24h] (later [esp+28h] after push ebp).
- `start` arg slot [esp+54h] is reused: InsertBlock's `added` out-ref, then the per-candidate new-group counter (draft does this - correct).
- Recursion: 4 self-calls (0xAD9FEE, 0xADA1B7, 0xADA1FE) + caller Process 0x00ADA385.

## 2. Identity (two sources -> manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00c5d610) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x0183c6c9,
  naming_stragglers/FableWin.exe.signatures.tsv): `?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z`
  = **private: void __thiscall CVertexGroupBlockOptimiser::AddBlocks(unsigned long)** (non-const). Mangled leaf `?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z`.
- Caller: Process 0x00ADA240 (landed rebuild/src/compiled/00/ad/CVertexGroupBlockOptimiser_Process_00ada240.cpp; PDB ?Process@...QAEXAAVCVertexGroupBlockList@@_N@Z). Not virtual (no vtable).

## 3. Callees (landed file | landed name | PDB name | draft spelling)
| addr | landed file (rebuild/src/compiled/00/..) | landed / PDB name | draft | flag |
|---|---|---|---|---|
| 0x00AD8FE0 | ad/CVertexGroupBlockOptimiser_CalcCurrentBestTheoreticalResultNewBlock_00ad8fe0.cpp | CalcCurrentBestTheoreticalResultNewBlock() `AAEKXZ` (private, non-const, ulong) | MinimumBlocksForCurrent | **WRONG** |
| 0x00AD9D10 | ad/..._CalcCurrentBestTheoreticalResultNewBlock2_00ad9d10.cpp | CalcCurrentBestTheoreticalResultNewBlock2() `AAEKXZ` | MinimumBlocksForRemaining | **WRONG** |
| 0x00AD94F0 | ad/CVertexGroupBlockOptimiser_FindBestStartingBlock_00ad94f0.cpp | FindBestStartingBlock() `AAEKXZ` (proven: builds map<ulong,CGroupInfo>; labels_chatgpt_pass9) | FindLargestUnusedBlock (a different PDB fn, ABEKXZ const) | **WRONG** |
| 0x00AD8DD0 | ad/CVertexGroupBlockOptimiser_CanInsertMoreBlocks_00ad8dd0.cpp | CanInsertMoreBlocks() const `ABE_NXZ` | CanInsertMoreBlocks() non-const | const missing |
| 0x00AD92E0 | ad/CVertexGroupBlockOptimiser_InsertBlock_00ad92e0.cpp | InsertBlock(unsigned long, unsigned long&) `AAEXKAAK@Z`, ret 8 | same | ok |
| 0x00AD9470 | ad/CVertexGroupBlock_RemoveLast_00ad9470.cpp | CVertexGroupBlock::RemoveLast(unsigned long) `QAEXK@Z` | same | ok |
| 0x00AD9030 | ad/CVertexGroupBlock_CVertexGroupBlock_00ad9030.cpp | ??0CVertexGroupBlock@@QAE@XZ | ctor | ok |
| 0x00A96DA0 | a9/CEntryListBuffer00a96da0_..._00a96da0.cpp (landed under a propagated class name; manifest says ~CVertexGroupBlock) | ??1CVertexGroupBlock@@QAE@XZ | dtor | ok (landed file misnamed) |
| 0x006699C0 | 66/CRetailTreeulongkey_find_006699c0.cpp | map<unsigned long,long>::find (folded COMDAT, 12 bodies), hidden-result `ret 8` | CGroupMap::find | ok |
| 0x00ADB570 | ad/CGuiControlTreePane_SortTreeRecursively_00adb570.cpp (**mislabel**: body is `mov edx,[ecx]` thiscall list distance loop) | really list<CVertexGroupBlock>::size() const | CBlockList::size | NAME_FIXES candidate |
| 0x00ADD020 | ad/STLlistCVertexGroupBlock_operator_00add020.cpp | ??4?$list@VCVertexGroupBlock... operator= | operator= | ok |
| 0x00ADCE10 | ad/STLlistCVertexGroupBlock_pushback_00adce10.cpp | list<CVertexGroupBlock>::push_back, ret 4 | ok | |
| 0x00ADC790 | ad/STLlistCVertexGroupBlock_popback_00adc790.cpp | list<CVertexGroupBlock>::pop_back (landed says __fastcall, plain ret; ecx=list) | ok | |
| 0x00ADB960 | ad/stdvector_reserve_00adb960.cpp | vector<CVertexGroupBlockOptimiser::CBlockInfo>::reserve, ret 4 | CCandidateArray::reserve | type name wrong |
| 0x00ADBFB0 | ad/stdvector_pushback_00adbfb0.cpp | vector<CBlockInfo>::push_back(const CBlockInfo&) | CCandidateArray::push_back | type name wrong |
| 0x00ADC890 | ad/STL_sort_00adc890.cpp (`// Flags: /Gr`) | std::sort<CBlockInfo*>(first,last) fastcall, uses CBlockInfo::operator< (Cost) | SortCandidates | name wrong |
| 0x00BFEA14 | bf/global_free_00bfea14.cpp | free | free | ok (vector dtor inline) |

## 4. Members (retail = STLport sizes; Ego_r offsets differ: Dinkumware 16-B vectors, 12-B lists)
No shared header exists for CVertexGroupBlockOptimiser, CVertexGroupBlockList, CBlockInfo (grep rebuild/include: none).
Shared header exists ONLY for CVertexGroupBlock: rebuild/include/engine/CVertexGroupBlock.h, opt-in
`FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS` (pulls engine/CArray.h + stlport/fable_map.h; gives private Groups CArray<unsigned long> +0,
GroupToIndexMap std::map<unsigned long,long> +0xC, inline const GetGroupCount()/GetGroup(i)/HasGroup(g) -- PDB QBEKXZ/QBEKK/QBE_NK).
The draft models CVertexGroupBlock LOCALLY although this header exists (verifier warns).
Optimiser members used (struct_layouts_egor.tsv names; retail offsets as all landed siblings):
- +0x00 InputList : CVertexGroupBlockList { CArray<CVertexGroupBlock> Blocks } (24-B elements, `imul 2AAAAAABh; sar 2`); PDB accessor GetBlockCount() const / GetBlock(i) const.
- +0x0C OptimalBlockList, +0x10 Blocks : std::list<CVertexGroupBlock> (4 B, node ptr).
- +0x14 CurrentBlock : CVertexGroupBlock*.   +0x18 RemainingBlocks : vector<bool> (retail one byte/element, CArray<bool> in PDB sig of CalcMinimumBlocks).
- +0x44 MaxN ulong, +0x4C MaxIterations ulong, +0x50 Greedy bool, +0x54 Iterations ulong. (Unused here: +0x24 CurrentBlockSegmentGroups,
  +0x30 CurrentBlockSegmentIndices, +0x3C CurrentBlockSegmentCount, +0x40 MaxGroupIndex, +0x48 OptimalSize, +0x58 Debug.)
- CBlockInfo (nested, Ego_r 12 B): BlockIndex ulong +0, Cost long +4, Benefit long +8; `bool operator<(const CBlockInfo&) const` (??M, Cost<).
  Retail fills {j, newGroups, groupCount} at [esp+38h..40h] -> BlockIndex=j, Cost=new groups, Benefit=block group count.
STLport opt-ins available: fable_containers.h `FABLE_RETAIL_STLP_LIST` (+ `FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE`, back() = end()-- -> `mov edx,[ecx+4]; add edx,8` as retail 0xADA1AA),
fable_map.h find variants (`FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE`, `..._CONSTFIND_TEMPLATE_BODY`, `FABLE_RETAIL_STLP_TREE_MFIND_DECL`),
CArray.h `FABLE_RETAIL_CARRAY*`. Landed CVertexGroupBlock rows use local models + `#pragma auto_inline(off)` around a same-TU noinline find body.

## 5. Callee visibility (same TU = vertex-group-block-optimiser .cpp, 0x00AD8C30..0x00ADA3F0, all landed)
- Same-TU, defined earlier, called out of line: CanInsertMoreBlocks 8DD0, NewBlock 8FE0, InsertBlock 92E0, RemoveLast 9470,
  FindBestStartingBlock 94F0, NewBlock2 9D10 (all aligned, real members). VC7.1 saw their bodies but did not inline (large / called recursively-adjacent).
- **Inline in AddBlocks**: HasMoreBlocks (loop at 0xAD9F87, PDB ?HasMoreBlocks ABE_NXZ const), the first OptimalBlockList.size() (loop at 0xAD9F50),
  CVertexGroupBlock::GetGroupCount/HasGroup (HasGroup inlined but its map find stays a CALL to 0x006699C0), vector<CBlockInfo> ctor + dtor (free at 0xADA224),
  list::back(), CBlockInfo copy.
- **Out of line although template/inline**: list::size() (0xADB570, called 3x later in the same function while the first use was inlined -- inline budget),
  map::find 0x006699C0 (IsProperSubsetOf 0x00AD8CA0 models it as a same-TU `__declspec(noinline)` body under `#pragma auto_inline(off)`),
  list push_back/pop_back/operator=, vector reserve/push_back, std::sort.
- The find call passes (result*, key*) with key in a frame local [E+1Ch] and the iterator result [E+20h] compared with [ebx]=map header (`it == GroupToIndexMap.end()` -> ++count).

## 6. Sibling landed rows (all rebuild/src/compiled/00/ad/)
- CVertexGroupBlockOptimiser_CanInsertMoreBlocks_00ad8dd0.cpp -- `// Flags: none`, no pragmas; full STLport vector/map/list model, same-TU HasGroup auto-inlined; indexed loops (`InputList.Blocks[i]`, `block.Groups[j]` count hoisted) -> retail count-down strength reduction.
- CVertexGroupBlockOptimiser_CountRemainingGroups_00ad8ed0.cpp -- class layout donor (InputList, lists, CurrentBlock, vector<bool> RemainingBlocks, ...); no pragmas.
- CVertexGroupBlockOptimiser_CalcCurrentBestTheoreticalResultNewBlock_00ad8fe0.cpp -- CBlockList::size() for-loop model.
- CVertexGroupBlock_IsProperSubsetOf_00ad8ca0.cpp -- `#pragma auto_inline(off)` + noinline same-TU find (0x006699C0), FABLE_FORCEINLINE HasGroup, named iterator local.
- CVertexGroupBlockOptimiser_Process_00ada240.cpp -- the caller; STLport list/map/vector model incl. CBlockList, CVertexGroupBlockList.
- STL_sort_00adc890.cpp (`// Flags: /Gr`), stdvector_pushback_00adbfb0.cpp -- carry the CBlockInfo class (nested in CVertexGroupBlockOptimiser).

## 7. Draft model errors vs disassembly
- Callee names wrong (MinimumBlocksForCurrent/Remaining, FindLargestUnusedBlock, SortCandidates, CCandidate*, CGroupMap) -- see table; CanInsertMoreBlocks must be const; CBlockInfo field names BlockIndex/Cost/Benefit.
- Local CVertexGroupBlock while engine/CVertexGroupBlock.h exists (FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS: uses GetGroupCount/GetGroup/HasGroup).
- HasMoreBlocks: retail reuses its count (eax) as the reserve() argument (`push eax` at 0xADA011) -- InputList size computed once and live across the CurrentBlock test; draft calls InputList.size() again for reserve (CSE may or may not merge).
- Order of evaluation: at 0xADA157.. retail computes Blocks.size(), Optimal.size(), then Optimal.size() AGAIN, THEN calls NewBlock2 -> spelling `OptimalBlockList.size() <= ...NewBlock2()` or `size >= ...` both must evaluate size first; top test likewise computes inline Optimal size before calling NewBlock (draft's explicit `optimal` local is fine there).
- Candidate loop: retail inner loop is pointer + count-down ([esp+14h] counter, ebp = group ptr, ebx = &CurrentBlock->GroupToIndexMap reloaded per j), skips the `<= space` compare when count==0 (jbe to push) -- natural from `for k<count` + unsigned compare; previous attempts lost because the zero went to ebx.
- Candidate vector: `CArray`/`std::vector<CBlockInfo>` default-ctor (3 zero stores from ebp), first/last read (edi=end, ebp=begin) BEFORE the Greedy test; dtor frees ebp=begin.
- Empty-candidate path: `CVertexGroupBlock* saved=CurrentBlock` read before temp ctor; push_back(CVertexGroupBlock()) temp dtor then `CurrentBlock=&Blocks.back()` -- draft order matches.
- Non-empty path: `next = Greedy ? 0 : BlockIndex+1` (retail `xor eax,eax; test cl,cl; jne; mov eax,[edi]; inc eax`).
- Prior residue (hand310 NOTES): 826 v 806, only regalloc: zero in ebx not ebp; 9 variants tried (see rebuild/candidates/manual/pending/2026-10-23_linux_hand310/NOTES.md).

