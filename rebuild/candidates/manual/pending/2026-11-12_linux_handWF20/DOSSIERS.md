## 0x00839100

# DOSSIER 0x00839100 -- CThingGameObject::GetWeaponTrailGraphicIndex (795 bytes)

## 1. Retail shape (xdis 00839100 0x32b)
- __thiscall, plain `ret` (no args), long in eax. NO frame pointer (sub esp,0xc; push ebx/ebp/esi/edi) => speed TU, no
  `optimize("y",off)`, no `optimize("s")`; no SEH frame (dtor of the def pointer is open-coded at every exit, no EH state).
- Frame 0xc: [esp+0x13] bool `special` byte; [esp+0x14] = the CDefPointer<const CWeaponDef> out slot (zeroed before the call),
  read into EDI right after the call (`mov edi,[esp+14h]`), then the SAME slot is reused for the reverse iterator `current`;
  [esp+0x18] = the GetTC key temp / iterator copy / find key (0) temp.
- Regs: ebx=this, ebp=wielder (this or carrier), edi=weapon def for the whole body.
- Exits: GetSubDef fails -> `test edi,edi; je; dec [edi+4]; jne; call [vtbl+4]` then `or eax,-1`.
  Berserk exits reload the slot (`mov ecx,[esp+14h]; test ecx,ecx`) -- null-tested release; all other exits release through
  edi WITHOUT a null test (`dec [edi+4]` / `mov eax,[edi+4]; dec eax; mov [edi+4],eax`).
- Loop alignment padding `lea esp,[esp]; lea ecx,[ecx]` before the loop head at 0x839310 (speed /O2 loop alignment).

## 2. Identity (two sources)
- Name: Ego_r.pdb 0x00813850 and FableWin.pdb 0x0184D7C6 / 0x027F0F20:
  `?GetWeaponTrailGraphicIndex@CThingGameObject@@QBEJXZ` => public, const, returns long, no params.
  Manifest label is CORRECT. Signature: `long CThingGameObject::GetWeaponTrailGraphicIndex() const;`
- Not virtual (QBE, no vtable slot). Caller: 0x0076E910 (not landed; manifest has no row file).
- engine/CThingGameObject.h does NOT declare it yet (grep) -- the row must add a declaration (or opt-in) for it.

## 3. Callees (retail addr -> landed file / landed name / PDB name)
- 0x0042B1A6 rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp
  `CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>*) const` (`??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z`).
  DRAFT WRONG: calls it `CThingBaseDef::GetWeaponTrailDef` with a local CWeaponTrailDef -- the def is CWeaponDef.
  Called as `PBaseDef->GetSubDef(&weaponDef)` (ecx=[this+0x70]).
- 0x004365B0 rebuild/src/compiled/00/43/CVectorMap_LowerBound_004365b0.cpp -- const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
  (via CThing::PeekTC/GetTC in engine/CThing.h under FABLE_RETAIL_CTHING_GETTC_EXPANDED). Draft's local CInterfaceMap = wrong spelling.
- 0x00705170 rebuild/src/compiled/00/70/CTCCarryable_GetPCarriedByThing_00705170.cpp `CTCCarryable::GetPCarriedByThing() const`
  (Ego_r+FableWin `?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ`); engine/CTCCarryable.h declares it. Draft: `GetCarrier`/`Carrier` -- wrong; member is PCarryingTC +0x10.
- 0x00787C18 (unaligned, 3 bytes `mov al,1; ret`) landed rebuild/src/compiled/00/78/global_operator_00787c18.cpp as
  `ReturnTrue_ret0_00787c18` (`?fn_operator@@YI_NXZ`, fastcall). Called here on the TCI_TRAP (125) component: the method is
  CTCTrapBase::IsCurrentTrapToCauseKnockdown() const (Ego_r `?IsCurrentTrapToCauseKnockdown@CTCTrapBase@@QBE_NXZ` at Ego_r 0x0078EF7D,
  a 3-byte body before IsTriggered 0x0078EF80 = same `mov al,1;ret`; declared in engine/CTCTrapBase.h line 32; TCI_TRAP in
  CTCTrapBase::StatGetInterfaceType). Likely an ICF-shared stub -> candidate NAME_FIXES row (descriptive landed name). Draft: `CTCSpecialMove::IsSpecialMoveActive` -- made up.
- 0x0078BED0 / 0x0078BEE0 landed as `CountReferences::CountReferences` (`?M@C_0078bed0@@QAEKXZ` / `?M@C_0078bee0@@QAEKXZ`) -- WRONG.
  Real: CTCBerserk::GetWeaponTrailAttack() const / GetWeaponTrailKnockDown() const (Ego_r `?GetWeaponTrailAttack@CTCBerserk@@QBEJXZ` 0x00792F30,
  `?GetWeaponTrailKnockDown@CTCBerserk@@QBEJXZ` 0x00792F40, consecutive as retail; bodies read PDef_Object(+0x68)->+0x148/+0x14c).
  Interface 0x84 = TCI_BERSERK. NAME_FIXES candidates. Draft: CTCCharge::GetTrailGraphic/GetSpecialTrailGraphic -- made up.
  special -> KnockDown (0x78BEE0), else Attack (0x78BED0).
- 0x00733EE0 rebuild/src/compiled/00/73/CTCCombat_IsMeleeFlourishValid_00733ee0.cpp `CTCCombat::IsMeleeFlourishValid() const`
  (Ego_r `?IsMeleeFlourishValid@CTCCombat@@QBE_NXZ`). Interface 0x30 = TCI_COMBAT. Draft: CTCHand::IsActive -- wrong.
- 0x00766050 rebuild/src/compiled/00/76/CTCObjectAugmentations_HasCombinationOfAugmentations_00766050.cpp
  `CTCObjectAugmentations::HasCombinationOfAugmentations(EObjectAugmentationType) const` (engine/CTCObjectAugmentations.h line 41;
  NOTE the header also has an inline body at line 89 -- retail calls it OUT OF LINE, so the row must not let it inline). Interface 0xBF = TCI_OBJECT_AUGMENTATIONS.
- 0x0083AEA0 rebuild/src/compiled/00/83/CRetailTreeIterator_Decrement_0083aea0.cpp (`CRetailTreeIterator::Decrement`) -- STLport
  _Rb_tree_iterator::operator--() emitted out of line thiscall (ecx=&iter, returns this). Called 3 ways: reverse_iterator operator*
  (copy current to [esp+18], --copy, deref), operator++ (--current at [esp+14]).
- 0x0083B170 rebuild/src/compiled/00/83/stdRbtree...find_0083b170.cpp -- `_Rb_tree<...>::find(const key&)` non-const?/const, hidden
  iterator return (push &key; push &result), key placeholder CUnprovenKey_0083b170. Real type: the WeaponTrails map
  `map<EObjectAugmentationType,CWeaponTrailGraphicSet>` (Ego_r CWeaponDef.WeaponTrails) -- the placeholder could be respelled.
- CCreatureActionBase virtuals via [wielder+0x100]: +0x4c (slot 19), +0x48 (slot 18), +0xd8 (slot 54), called in that order, `||`.
  Counting engine/CCreatureActionBase.h virtual decls from 0: 18=IsKnockdownStrike, 19=IsBlockCounter, 54=PreventSneaking -- UNVERIFIED
  (rtti-vtables.tsv only lists slot 0). Semantically more plausible with a +1 shift (18 IsBlockCounter, 19 IsMeleeFlourish,
  54 IsDecapitatingAttack). Author: verify slot indices against the header's actual vtable (offsetof asserts / another landed caller)
  and use the header's class rather than the draft's v00..v53 stand-in.
- Release: def vtable slot 1 (+4) through CDefPointer dtor.

## 4. Members
- this/wielder (engine/CThing.h): UsingComponents bitset +0x20 (bits 71 [ebx+28] byte sign, 125 [ebx+2c]&0x20000000,
  132 [ebp+30]&0x10, 48 [ebp+24]&0x10000, 191 [ebx+34] sign); Components (CVectorMap) +0x44; ThingTypeFlags +0x6c (&8 = creature);
  PBaseDef +0x70 (CDefPointer<const CThingBaseDef>). Use FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC/GetTC out-param form, LowerBound out of line).
  Note which thing: 0x47 (carryable), 0x7d (trap), 0xbf (augmentations) are looked up on THIS (ebx); 0x84 (berserk), 0x30 (combat),
  and the +0x6c/+0x100 reads on the WIELDER (ebp).
- wielder +0x100: CThingCreatureBase::PCurrentAction (CCountedPointer<CCreatureActionBase>; engine/CThingCreatureBase.h
  PCurrentAction_Data +0x100 / Info +0x104, under FABLE_RETAIL_CTHINGCREATUREBASE). Cast wielder to CThingCreatureBase when flag 8.
- CTCCarryable PCarryingTC +0x10 (engine/CTCCarryable.h, FABLE_RETAIL_CTCBASE): `if (carryable->PCarryingTC) wielder = carryable->GetPCarriedByThing();`
- CTCCombat IsInMiddleOfFlourishAction +0x94 (bool; engine/CTCCombat.h line 148, Ego_r CTCCombat +0xa8 'IsInMiddleOfFlourishAction').
- CWeaponDef (engine/CWeaponDef.h, needs FABLE_RETAIL_DEF_CLASS_BASE): FlourishSpecialWeaponTrail +0x7c `long[2]` {Attack,Knockdown}
  (Ego_r CWeaponTrailGraphicSet{long Attack; long Knockdown;}); WeaponTrails +0x84 opaque `unsigned char[0xc]` =
  map<EObjectAugmentationType,CWeaponTrailGraphicSet> (header ptr +0x84, node key +0x10, Attack +0x14, Knockdown +0x18).
  The header models the map as bytes: the row needs a typed view (or an opt-in giving the real std::map type from stlport/fable_map.h).
  CWeaponDef.h opt-ins: only FABLE_RETAIL_CWEAPONDEF_TOOHEAVY (irrelevant).
- Local models in draft that have shared headers: CInterfaceMap (CVectorMap.h), CDefBase/CDefPointer (CDefPointer.h, CDefClassBase.h),
  CWeaponTrailDef (CWeaponDef.h), CThingBaseDef (CThingBaseDef.h), CTCCarryable, CTCCombat (as "CTCHand"), CTCObjectAugmentations,
  CTCBerserk (as "CTCCharge"), CTCTrapBase (as "CTCSpecialMove"), CCreatureActionBase, CThing, CThingGameObject, STLport tree node/iterator (stlport/fable_map.h).

## 5. Callee visibility
- KEY: GetSubDef<CWeaponDef> 0x0042B1A6 is an unaligned template COMDAT => body visible to retail. Opt-in
  FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + #include "engine/CDefClassBase_GetSubDef.h": per its comment (row 0x0066BC10) the
  visible noinline body lets VC7.1 keep the def read back in a register (`mov edi,[esp+..]` after the call, release through edi,
  no reload) -- EXACTLY this row's residue (draft's CDefPointer stayed address-taken). Same pattern landed in DropWeapon 0x00845CE0
  ("weapon def held in edi"); 41 landed rows use the macro.
- CVectorMap::LowerBound const 0x004365B0: out of line (via CVectorMap.h; GETTC_EXPANDED form).
- STLport tree: operator-- (0x83AEA0) and find (0x83B170) are COMDATs at aligned addresses after this TU's functions (0x83ADA0..0x83B170
  are this TU's tree helpers: operator*, end, iterator ctors, ==, _S_value, _Select1st at 0x83ADA0-0x83AE20 all out of line too).
  fable_map.h: iterator::operator-- is inline calling static _M_decrement (cdecl) -- retail instead calls a thiscall operator-- out
  of line; needs a noinline/out-of-line operator-- for this instantiation. find: FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE (decl-only member
  template, hidden-iterator call) / FABLE_RETAIL_STLP_TREE_CONSTFIND(_TEMPLATE_BODY) / FABLE_RETAIL_AUGMENTATION_PARTICLES (noinline
  const find for an EObjectAugmentationType-keyed map, CTCObjectAugmentations rows 0x007677E0) -- check which matches `cmp eax,[esi]`
  against header after the call. Reverse iteration: STLport reverse_iterator (*it copies current and decrements the copy) --
  see rebuild/src/compiled/00/7a/CTCEntityEvents_GetEvent_007afb90.cpp lines 60-96 for the idiom.
- HasCombinationOfAugmentations has an inline body in CTCObjectAugmentations.h:89 -- must stay a call.
- 0x787C18 / 0x78BED0 / 0x78BEE0 / 0x733EE0 / 0x705170 are separate TUs -> declaration only.

## 6. Siblings
- rebuild/src/compiled/00/83/CThingGameObject_GetMeleeHitParameters_00838bc0.cpp (auto_inline(off) islands, pack(1))
- rebuild/src/compiled/00/83/CThingGameObject_GetShotHitParameters_00838ec0.cpp (`// Flags: none`, FABLE_RETAIL_BASE_OBJECT,
  FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND)
- rebuild/src/compiled/00/83/CThingGameObject_OnCollision_00839080.cpp (CVectorMap.h), CThingGameObject_UpdateStandingOnThing_00838aa0.cpp
- rebuild/src/compiled/00/84/CCreatureActionDropWeapon_DropWeapon_00845ce0.cpp (GetSubDef<CWeaponDef> + GETSUBDEF_NOINLINE, def in edi)
- rebuild/src/compiled/00/73/CTCCombat_IsMeleeFlourishValid_00733ee0.cpp (FABLE_RETAIL_CTHING_GETTC_EXPANDED usage)
- None use optimize pragmas (speed TU, no /Oy-).

## 7. Draft model errors
- The address-taken CDefPointer (residue): fix with the visible noinline GetSubDef body (section 5), not with release-form tricks.
- Draft wraps `special` in GetInterface(&move,0x7d) on THIS -- correct object, wrong class/method (CTCTrapBase::IsCurrentTrapToCauseKnockdown).
- Flourish branch: `if (combat && combat->IsMeleeFlourishValid() && !combat->IsInMiddleOfFlourishAction)` then
  `graphic = special ? def->FlourishSpecialWeaponTrail[1] : [0]; if (graphic > 0) return graphic;` (jg) -- shape right, names wrong.
- Augmentation loop: retail iterates rbegin..rend with STLport reverse_iterator calling out-of-line operator-- (draft hand-rolled ok
  in shape); retail does NOT skip key 0 (draft adds `key &&`? -- retail tests the key `test eax,eax; je` before calling HasCombination,
  so the skip is real) ; key read from node+0x10 via operator* after a decrement of a copy.
- After loop: find(EObjectAugmentationType(0)) on WeaponTrails; `== end()` -> -1 path (shared with no-def exit at 0x839400:
  null-tested release + or eax,-1); else special ? Knockdown(+0x18) : Attack(+0x14).
- Draft uses local CThing stand-in with m_presentBits etc. -- use engine/CThing.h (GETTC_EXPANDED).

## 0x00AD9F10 (not landed)

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

## 0x008C12D0 (not landed)

# DOSSIER 0x008C12D0 -- CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise (808 bytes)

## 1. Retail shape (xdis 008c12d0 0x338)
- `sub esp,3Ch ; push ebx ; push ebp ; push esi ; mov esi,ecx ; lea ebp,[esi+8] ; push edi` ... `pop edi/esi/ebp/ebx ; add esp,3Ch ; ret` (no arg: `ret`).
  thiscall, no frame pointer (/Oy default, no optimize("y",off)), no SEH/EH frame, /O2 speed idioms (no optimize("s")). Frame 0x3C.
- Register plan: ebp = &PCreature (+8) for the whole body; esi = this; edi = map/boss -> def; ebx = 0 (xor) then hitter.
- Flow: GetTC(0xAD) inline (`test ch,20h` = UsingComponents bit 173 at +0x34; key 0xAD stored at [esp+18h] INSIDE the test; LowerBound 0x40F020;
  `cmp eax,end ; je ; cmp [eax],0ADh ; jle ; mov eax,end` then `mov edi,[eax+4]`); absent path reads stale [esp+18h] (boss never null-initialised).
  def slot [esp+14h] = ebx(0); GetPItem; `push &def ; mov ecx,[eax+70h] ; call 0x8C33E0` (creature fetched before &def pushed).
  hits=boss->GenericCounter+1 compared with def[+40h]: `jg -> 0 ; jne -> 0 ; else 1` stored in [esi+148h], then `cmp [esi+148h],bl` (re-read).
  !CauseRush: `call 0x865F40 ; jmp release` (release with def still in edi). Rush path: GetFacingVector (slot 70 +0x118, hidden ret ptr [esp+40h]),
  PointOfHit(+11C) - GetPos; IS-alive test on PThingThatHit (+114, `test byte [eax+91h],1`), hitter delta; away=(0,0,?) zeroed X,Y only;
  inline fast-inv-sqrt Normalise (0x17C, table 0x013CE558, 3.0f 0x122DED4, 0.5f 0x122F59C); dot; `fld qword [1277128h] ; fcos ; fxch ; fxch ; fcompp ; test ah,41h`;
  two CCharString temporaries (0x12802FC back / 0x12802DC front) tail-merged into one SetCombatAnimation call + one ~CCharString;
  fpatan*1/2pi (0x1237F08), WrapAngle via __CIfmod 0xBFEB28 (fmod intrinsic), PhysicsTC slot 66 (+0x108) SetFacingAngleXY;
  `call 0x857520` (CCombatActionBase::Initialise, direct); then `mov edi,[esp+14h]` (def reloaded) and release `dec [edi+4] ; jne ; call [vft+4]` -- NO null test.

## 2. Identity
- Name CONFIRMED: `?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush@@UAEXXZ`, `virtual void Initialise()` (public virtual, void, no params, non-const).
  Sources: (a) rtti-vtables.tsv: CCombatAction_StrikeResponseJackOfBladesIntoRush vtable 0x012808EC slot 12 = 0x008C12D0; slot 12 is Initialise in
  CCombatActionBase (0x01259E44 slot12 = 0x00857520 ?Initialise@CCombatActionBase) ; (b) Ego_r.pdb egor_pdb_names.tsv has the exact leaf
  (Ego_r address 0x008F94F0, a different binary). xref: only reference is the vtable (.rdata 0x0128091C). Manifest label is right.
- Leaf as in the drafts: `// Leaf: ?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush`.

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | landed/PDB name | draft spelling | flag |
|---|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | local CBaseIntelligentPointer returning CThing* | use engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER) + cast, as siblings |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) | CInterfaceMap::LowerBound(const long*) EXTERN | WRONG spelling; also siblings define it same-TU noinline (engine/CVectorMap.h) |
| 0x008C33E0 | 00/8c/CDefClassBase_GetSubDefCJackOfBladesBattleDef_008c33e0.cpp | ??$GetSubDef@VCJackOfBladesBattleDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCJackOfBladesBattleDef@@@@@Z (const, CDefPointer<const T>*) | non-const, CDefPointer<CJackOfBladesBattleDef>* | const + const T missing |
| 0x00865F40 | NOT landed (parked 2026-10-23_linux_hand314, DIFFER 1106v1161) | ?Initialise@CCombatAction_GenericStrikeResponseBase@@UAEXXZ (Ego_r) | same | ok (other TU) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition | WRONG -> GetPos |
| vslot 70 (+0x118) | CTCPhysicsBase 0x006B1410 | ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ (const) | GetFacing() non-const | WRONG -> GetFacingVector() const |
| vslot 66 (+0x108) | CTCPhysicsBase 0x006B1260 | ?SetFacingAngleXY@CTCPhysicsBase@@UAEXM@Z | SetFacingAngleXY | ok |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*,long) / ~CCharString (fable_string.h) | local | use fable_string.h |
| 0x008574D0 | 00/85/CCombatActionBase_SetCombatAnimation_008574d0.cpp | ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z | SetAnimationName | WRONG -> SetCombatAnimation |
| 0x00BFEB28 | 00/bf/global_CIfmod_00bfeb28.cpp | __CIfmod (toolchain lib; fmod intrinsic) | fmod intrinsic | ok |
| 0x00857520 | 00/85/CCombatActionBase_Initialise_00857520.cpp | ?Initialise@CCombatActionBase@@UAEXXZ | ok | ok (engine/CCombatActionBase.h FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE declares Initialise + SetCombatAnimation(const CCharString&)) |
| def vslot 1 | -- | CDefPointeeBase::Delete (engine/CDefPointer.h names it Delete, count IVRefCount) | Destroy / RefCount | respell |
| global 0x013CE558 | -- | ?GFastOneOverSqrtTable@@3PAKA (naming wave 18, sibling 0x008C2550) | g_InvSqrtMantissaTable | WRONG -> GFastOneOverSqrtTable |

## 4. Members
- this +0x08 PCreature (CCreatureActionBase, CIntelligentPointer<CThingCreatureBase> in Ego_r; landed 0x857520 models it so).
- this +0x114 PThingThatHit (CIntelligentPointer), +0x11C PointOfHit (C3DVector): engine/CCombatAction_GenericStrikeResponseBase.h has them only as
  opaque `unsigned char[8]/[12]`, PROTECTED, base CCombatActionBase sizeof 0x148 asserted. No opt-in macros.
- this +0x148 CauseRush (bool): engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h is generated opaque padding, not derived from the base
  (PDB +0x160). Siblings 0x008C2550 / 0x008C1600 / 0x008C2810 model the class LOCALLY for that reason (accepted).
- CThing (engine/CThing.h, asserts): UsingComponents +0x20 (bitset; HasTC), Components +0x44 (CVectorMap), PhysicsTC +0x60 (GetPhysicsTC()),
  PBaseDef +0x70 (CDefPointer<const CThingBaseDef>; retail reads the word and calls GetSubDef on it), Dead bit0 of +0x91 (IsDead/IsAlive).
  Opt-in: FABLE_RETAIL_CTHING_GETTC_EXPANDED gives GetTC(ETCInterfaceType, T**) with exactly retail's in-test key store + conditional-expression end test (used by landed 0x857520).
  Draft names m_presentBits/m_interfaces/m_pPhysics/m_pDef/m_flags91 are all made up.
- CTCBoss (engine/CTCBoss.h, retail size 72): GenericCounter +0x10 (long, PRIVATE, no accessor); StatGetInterfaceType()=173 (0xAD). Opt-ins
  in header: FABLE_RETAIL_JACK_STAGE1_UPDATE / _CTCBOSS_EFFECTS / _SETHELPER / _KILLHELPERS / _LIVINGHELPERS (none needed; private access is the issue).
- CJackOfBladesBattleDef (engine/CJackOfBladesBattleDef.h, sizeof 0x80 retail): NumGetHitsBeforeRush long at retail +0x40 (PDB +0x44). Needs StatGetTypeName (landed GetSubDef row defines it inline).
- CDefPointer (engine/CDefPointer.h, requires FABLE_RETAIL_DEF_CLASS_BASE): Object +0, Release() null-tests (all variants: plain, _RELEASE_COUNTREF, _RELEASE_POINTEE,
  _SELFDELETING). Retail has NO null test -> shared header doesn't give it; siblings use a local no-null-test CDefPointer stand-in.
- Classes the old draft models locally that have shared headers: C3DVector, CCharString(fable_string.h), CThing, CBaseIntelligentPointer,
  CVectorMap(CInterfaceMap), CDefPointer, CDefClassBase(CDefClassBase_GetSubDef.h), CJackOfBladesBattleDef, CTCBoss, CTCPhysicsBase,
  CCombatActionBase, CCombatAction_GenericStrikeResponseBase, CCombatAction_StrikeResponseJackOfBladesIntoRush.

## 5. Callee visibility
- Same TU (retail neighbours 0x008C1600 GetValidRushPosition, 0x008C2530 ctor, 0x008C2550 FinishAction, inline IsTo* at 0x8C3A70..): the GetSubDef<CJackOfBladesBattleDef>
  COMDAT 0x008C33E0 sits after the TU's functions -> body visible (template). Landed siblings use a same-TU __declspec(noinline) GetSubDef STAND-IN
  (body only writes out->Object), which also drops the null test. engine/CDefClassBase_GetSubDef.h has FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (real body, noinline).
- CVectorMap::LowerBound 0x0040F020: both landed siblings in this TU define it SAME-TU NOINLINE (engine/CVectorMap.h default branch; "VC7.1 then knows the callee does not capture the key").
  The old draft declared it EXTERN, and NOTES' tried-list never lists a visible LowerBound -> UNTRIED lever for the &PCreature ebp vs edi register gap
  (an escaping key address changes what is live/aliased across GetSubDef). Alternative: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (inline) -- wrong here (retail calls it).
- Other callees (GetPItem 0xA01B50, GetPos 0x4C73D0, CCharString, SetCombatAnimation, the two base Initialise, physics virtuals) are other-TU, out of line.
- STLport: none called.

## 6. Siblings to copy idioms from
- 00/8c/CCombatActionStrikeResponseJackOfBladesIntoRush_FinishAction_008c2550.cpp (same class, same TU, same GetTC(0xAD)+GetSubDef prologue):
  fable_compiler.h, engine/CVectorMap.h (same-TU LowerBound), FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h, local
  CDefPointer_CJackOfBladesBattleDef, noinline GetSubDef stand-in, FABLE_FORCEINLINE CThing::GetSubDef wrapper "so the creature is fetched before &def is pushed",
  FastInvSqrt with GFastOneOverSqrtTable, `#pragma intrinsic(sqrt)`. No // Flags, no optimize pragma.
  NAME_FIX candidates there: CJackOfBladesBattleDef "MaxRushCount" +0x40 is PDB NumGetHitsBeforeRush; CTCRushCounter::RushCount +0x10 (iface 0xAD) is CTCBoss::GenericCounter.
- 00/8c/CCombatActionStrikeResponseJackOfBladesIntoRush_GetValidRushPosition_008c1600.cpp (same class/TU): local CDefPointer without null test, same-TU noinline LowerBound
  and GetSubDef stand-ins, def declared BEFORE the boss lookup there (order differs from this row: here boss first).
- 00/8c/CCombatActionStrikeResponseMazeDoForcePush_FinishAction_008c2810.cpp (sister strike response; fable_compiler.h, math.h, engine/CVectorMap.h, CIntelligentPointer).
- 00/85/CCombatActionBase_Initialise_00857520.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CThing.h + engine/CVectorMap.h + CIntelligentPointer; GetTC(TCI_x,&p).
- 00/8c/CCreatureActionJackOfBladesUnblockableMidAttackNoBlend_Initialise_008c0460.cpp (CCharString temp + direct CCombatActionBase::Initialise).
- No row in 0x008C0000-0x008C3FFF carries `// Flags:`; 5 rows use `#pragma optimize("t", on)` (none needed here: shape is /O2).

## 7. Old draft model errors vs disassembly
- Control flow / x87 / strings: already in retail order (DIFFER 809v808); residue is ONLY register allocation (&PCreature edi vs retail ebp; def in ebp; frame 0x38 vs 0x3C because the def slot is reused for the angle).
  Retail keeps the def slot [esp+14h] alive to the end (reload `mov edi,[esp+14h]` after 0x857520) and the angle in its own slot [esp+10h].
- LowerBound extern (escaping key) instead of the sibling same-TU noinline body -- untried; GetInterface uses `if` re-assignment rather than the
  GETTC_EXPANDED conditional expression (retail bytes fit both; header form is the lane default).
- GetSubDef non-const with CDefPointer<CJackOfBladesBattleDef> (PDB: const member, CDefPointer<const T>*); GetDef() accessor invented (PBaseDef +0x70).
- Wrong callee spellings: SetAnimationName->SetCombatAnimation, GetPosition->GetPos, GetFacing->GetFacingVector() const, Destroy/RefCount->Delete/IVRefCount, g_InvSqrtMantissaTable->GFastOneOverSqrtTable, CInterfaceMap::LowerBound(const long*)->CVectorMap<..>::LowerBound(const ETCInterfaceType&).
- Local C3DVector/CThing/CTCBoss/CJackOfBladesBattleDef models where shared headers exist (CTCBoss::GenericCounter is private: needs a local model or friend).
- The CauseRush computation `if (hits > N) false; else if (hits == N) true; else false;` matches retail's `jg/jne` pair; keep it.

