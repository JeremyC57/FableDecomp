# Hand chunk 454 (_global .. list<CGraphicBankManager::CGraphicDataBankInfo>): 37 of 40 byte-exact

37 rows grade MATCH (4) or RELOCATION_MATCH (33) through gall.sh. calls.py shows every relocation going to the right
retail target (callees, `free` thunk 0x00BFEA14, operator delete 0x00BFE9BC, `__imp__free` 0x01440154, VirtualFree
import). All 37 behaviour tests PASS under g++ (tall.sh) and under VC7.1 /Od in Wine (tall.py). 3 rows are parked in
differ/. Grade counts per row are in the STATUS.tsv notes; they exclude the final gall.sh verification pass. Line 1 of
the template rows uses the plain template name (`_STL::fill(...)`, `_STL::deque::_M_pop_front_aux()`, ...), with the
instantiation in the parameters or the description, so finish.py's `::` split stays correct.

## What matched (37)

- **Dynamic initializers and atexit thunks (`.text$yc` / `.text$yd`)**. These are descriptive names; the PDB only
  gives "`dynamic initializer/atexit destructor for 'X''". VC7.1 names the per-TU pair `_$E1` / `_$E2`, and that is
  the leaf.
  - 0121aec0 NInventoryQuestsGlobals::InitQuestCardRHSet (leaf `_$E1`): `CRightHandedSet QuestCardRHSet(NEGATIVE_Z_AXIS, -NEGATIVE_Y_AXIS)`.
    - Lesson: C3DVector needs a memberwise USER copy ctor. With it, X and Y are forwarded straight off the x87 stack.
  - 01228df0 unattributed::StaticTreeDeleter013B8A8C: the atexit `~map` of a POD-valued STLport map at 0x013B8A8C.
    - Inline `clear()`, then the header free. Every header store re-reads the static header pointer.
  - 0122a20a CAIBrain::DestroyStateGroupInfo: teardown of the 203-entry `StateGroupInfo` table through the
    `vector destructor iterator` (0x00401400).
    - Lesson: a user-declared, out-of-line ~CAIStateGroupInfo plus `#pragma optimize("s", on)`. With the implicit dtor
      or /O2, VC7.1 expands an inline loop instead.
  - 0122a9a0 CMemoryAllocator::DestroyDeallocator: ~CDeallocator, which sets ShuttingDown and calls Destroy() when
    PeekNoAllocators() == 0.
    - Destroy() is `delete PAllocator` through the class `operator delete` (VirtualFree), all inline.
- **STLport containers (`_STL`)**
  - 00493c25 / 00655ca7 `deque<T>::_M_pop_front_aux`, for CSubtitleRenderer::CSetTextInfo and for
    `pair<CCharString,long>`: destroy the element, free the 128-byte buffer, step to the next map node.
    - Lessons: `// Flags: /Os` (the address is unaligned); the element is a `class` (V in the leaf); explicitly
      instantiate the member only (a whole-class instantiation inlines it into pop_front).
  - 005baf94 `set<EClothingSuitPart>::insert` (/Os): calls `insert_unique`, then returns
    `pair<iterator,bool>(__p.first, __p.second)`, which converts the tree iterator to the set's const_iterator.
  - 009e5250 CSymbolMap::Clear / 00a38550 CCRCSymbolMap::Clear: call the out-of-line `CVectorMap::Clear()`, then the
    set's `clear()` with the inline `_Rb_tree::clear` (`_M_erase` stays out of line).
  - 009f8920 / 00a0a650 `list<CGraphicDataBankInfo>::erase` and `list<CMeshDataBankInfo>::erase`: unlink the node, run
    the inline ~CCountedPointer then ~CCharString, `free` the node with no null test, return the next node.
  - 00adc790 `list<CVertexGroupBlock>::pop_back`: `erase(--end())`.
  - 00711860 `list<CAnimSet>::remove_if<CPointersEquivalent<const CAnimSet> >`: the STLport loop. The element dtor is
    two inline `CVector` dtors (`if (_M_start) free(_M_start)`).
  - 00568440 `basic_string<char>(const char*, const allocator&)`: zero-init the base, then
    `_M_range_initialize(s, s + strlen(s), forward_iterator_tag())`. Needs `#pragma intrinsic(strlen)`; the tag goes by
    const reference.
- **STLport algorithms (`__fastcall` templates)**
  - Eleven `fill` rows: 00527c10, 0086d9d0, 0086d980, 0052bca0, 0051caa0, 00a37be0, 00ad6b50, 00623110, 006fbbd0,
    007af980, 007b83d0. All share one body, `for (; first != last; ++first) *first = val;`, explicitly instantiated per
    element.
    - The element's member operators come from the PDB layouts: CCharString/CWideString `operator=`, CVector/CArray/CList
      `operator=`, `_Rb_tree::operator=`, and CIntelligentPointer `SetPItem(other.GetPItem())`.
    - 006fbbd0: CSmallVector's inline `operator=` must be written `if (&other != this) Copy(other);`. The operand order
      selects `cmp ebx,edi`.
  - 00a6f910 `__unguarded_insertion_sort_aux<pair<CCharString,C3DDummyObject>*, ..., CCompareString<C3DDummyObject> >`:
    `__unguarded_linear_insert(i, _Tp(*i), comp)` for each i, with the callee only declared.
  - 0048d490 `_Destroy<_Deque_iterator<C2DVector,..> >` and 00a0eaf0 `uninitialized_fill<_Deque_iterator<C2DVector,..>, C2DVector>`.
    Three lessons were needed:
    - `__false_type() {}` as a user ctor (otherwise VC7.1 zero-stores the tag temporary).
    - The out-of-line `__destroy_aux` / `__uninitialized_fill` defined `__declspec(noinline)` in the same TU under
      `#pragma auto_inline(off)`. This homes the tag temporary in the dead `last` argument slot, as retail does.
    - The iterator's user copy ctor, which gives the single-register member-wise outgoing copies.
- **Functors**
  - 00526120 `less<unsigned __int64>`, 00ad11a0 `less<pair<long,long> >` (with STLport's pair `operator<`), and
    0098f570 `less<CThreadedFileManager::CPendingObjectSortKey>`. The last orders by sort key, then file position, then
    address, with the early returns in source order.
  - 004aa6c0 `binary_compose<logical_or<bool>,CThingFilter_HasTC,CThingFilter_HasTC>::operator()`: `_M_op1(_M_op2(x), _M_op3(x))`,
    with both filters always evaluated.
  - 00e7e020 `NScript::CPredicate_And<CIsThingAlive,CIsVariableFalse>` ctor: base vtable, then the two member copies.
    - The CScriptThing copy ctor runs at 0x004ABE90.
    - The leaf must be the full mangled name, because the prefix also matched the implicit copy ctor.
- **libogg / libvorbis / libpk** (descriptive names for libpk)
  - 00c8d400 `ogg_sync_init` and 00c91850 `_vp_global_free` (`__fastcall`, memset intrinsic, free import).
  - 00c21750 CSoundEvent::AssignPhysicalPlayer: CheckState(3), then the channel taken into a NAMED local, then
    PlayPhysical(manager, channel, player).
  - 00c204c0 COverriddenChickenSign::SetWordA: forward to the record setter, then `m_setMask |= 0x10`.

## Parked for Fable 5.1 (3)

- 01228b70 CPlayerGui::DestroyPGuiDef DIFFER(41v40): retail decrements the def count in a register through a formed pointer (`mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`) and keeps the object in ecx, ours `dec dword ptr [eax+4]` then re-loads the static (descriptive name for "`dynamic atexit destructor for 'CPlayerGui::PGuiDef''", leaf `_$E2`) | tried: member-form inline CDefPointer dtor, named object local, count in a non-polymorphic base with inline RemoveReference, `volatile long` probe (closer, still re-loads) | next: lane gap: volatile member (volatile candidate)
- 0122ae7e NGlobalScriptDefs::DestroyPGlobalScriptDef DIFFER(37v34): retail `mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0` re-reads the count after the RMW, ours `dec [eax+4]` then re-loads the static (descriptive name for "`dynamic atexit destructor for 'pGlobalScriptDef''", /Os TU, leaf `_$E2`) | tried: member form under optimize("s") with a `__forceinline` dtor, named object local, named object + `long& count` two-statement decrement (28 bytes, folded to `dec ; jne`) | next: lane gap: volatile member (volatile candidate; the same release is inline all through the quest-script module, e.g. 0x00CBEEB0)
- 0122a550 CWideString::DestroyNullString DIFFER(75v78): retail forms the owner-count address with `add eax,0Ch` and re-loads the static before `push esi`, ours addresses `[eax+0Ch]` twice and loads the static into esi after the push (descriptive name for "`dynamic atexit destructor for 'CWideString::NullString''", leaf `_$E2`) | tried: the landed ~CWideString body inline (RTM and QFE-4035, both 75 bytes), `UnassignString(); --InstanceCount;` with UnassignString in the TU, two statement orders | next: the same `add eax,N` formation as 01228b70 points at a volatile CWideStringData owner count -> lane gap: volatile member

## Skipped

None.

## Label corrections

- 0121aec0 "$E2" -> `_$E1` (NInventoryQuestsGlobals QuestCardRHSet dynamic initializer; descriptive name).
- 01228df0 "Database_BuildTree_Deleter" is invented. It is the atexit `~map` of an unreferenced POD-valued tree at
  0x013B8A8C (descriptive name). It is possibly `CThing::TCsToBeRemoved` (from the FableWin.pdb atexit order); unconfirmed.
- 0122a20a "Init_ParticleMorphsVector" -> "`dynamic atexit destructor for 'CAIBrain::StateGroupInfo''".
- 0122a9a0 "Destroy" -> "`dynamic atexit destructor for 'CMemoryAllocator::Deallocator''" (Destroy is the inlined callee).
- 01228b70 and 0122ae7e "`anonymous namespace'::`dynamic atexit destructor for 'DEFAULT_CAMERA_MODE_DEF''" (propagated)
  -> CPlayerGui::PGuiDef (static 0x013B878C) and pGlobalScriptDef (static 0x0143E90C).
- 0122a550 "CCharString::NullString" -> CWideString::NullString (the neighbour's label was propagated).
- 00493c25, 00655ca7, 009e5250, 00a38550 "allocator<pair<CCharString const..>>::construct" (propagated)
  -> `deque<CSetTextInfo>::_M_pop_front_aux`, `deque<pair<CCharString,long> >::_M_pop_front_aux`, CSymbolMap::Clear,
  CCRCSymbolMap::Clear.
- 00c8d400 / 00c91850 "allocator<vector<pair<long..>>>::construct" -> libogg `ogg_sync_init` / libvorbis `_vp_global_free`.
- 005baf94 "bad_alloc" -> `set<EClothingSuitPart>::insert`.
- 00c21750 "basic_string<char>::compare" -> CSoundEvent::AssignPhysicalPlayer (libpk, descriptive name).
- 00c204c0 "bind2nd<greater<long>,long>" -> COverriddenChickenSign::SetWordA (libpk, descriptive name).
- 00e7e020 "deque<pair<CCharString..>>::begin" -> `NScript::CPredicate_And<CIsThingAlive,CIsVariableFalse>` ctor.
- fill element types:
  - 0086d9d0 CEntry -> CRandomAppearanceMorph::CBodyPartMesh
  - 0052bca0 CSymbolMap::CSymbolInfo -> NThingManager::CEntityRuntimePersistence::CEntityScriptInfo
  - 0051caa0 / 00a37be0 / 00ad6b50 pair<CCharString,CCharString> -> CNavigationAreaUpdate::CNavigationLayerUpdate / CClassTypeInfo / QuadricOptimiserInternals::CUniqueVertex
  - 0086d980 pair<CCharString,long> -> CVectorMap<ulong,CTextureMorph,CKeyPairCompareLess<..> >
  - 00623110 pair<float,CCharString> -> pair<float,CWideString>
  - 007af980 / 007b83d0 pair<long,CIntelligentPointer<CThing>> -> CIntelligentPointer<CThing> / CIntelligentPointer<CThingObject>
- 00a6f910 "fill<pair<CCharString,C3DDummyObject>*..>" -> `__unguarded_insertion_sort_aux<pair<CCharString,C3DDummyObject>*, .., CCompareString<C3DDummyObject> >`.
- 0048d490 / 00a0eaf0 "for_each<vector<C3DVector>.., CGetBestIciclePointForTarget>" -> `_Destroy` / `uninitialized_fill` over a
  deque of 8-byte elements (0x80-byte blocks), modelled as `deque<C2DVector>`. 0048d490 sits in the
  CInputProcessJumpingAndRolling TU, and Ego_r.pdb's CJoystickPositionTracker::RecentPositions is `deque<C2DVector>`.
  The element type is still inferred: any 8-byte class with these traits gives the same bytes.
- 00adc790 `list<CEnginePrimitiveHandle>::pop_back` -> `list<CVertexGroupBlock>::pop_back`.
- 00a0a650 `list<CGraphicDataBankInfo>::erase` -> `list<CMeshBankManager::CMeshDataBankInfo>::erase`.
- Callees seen on the way:
  - 0x00401400 is labelled `vector constructor iterator` but is `??_I`, the vector DEStructor iterator.
  - 0x0043E3D0 is labelled `_Dest_val<CMacro>` but is ~CSetTextInfo.
  - 0x0099EAE0 is labelled `_Cons_val` but is ~CCharString.
  - 0x005BAD5B is `set<EClothingSuitPart>`'s `_Rb_tree::insert_unique`.
