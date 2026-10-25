# Chunk 457 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport copy_backward tag: POD `__false_type`.** Give `_OKToMemCpy<T1,T2>::_Answer()` a body of
  `return __false_type();`, called through `_IsOKToMemCpy(first, result)._Answer()`, with `__false_type` an empty POD.
  This produces retail's zeroed byte tag, `xor eax,eax ; lea edi,[ebp-1] ; stosb ; mov al,[ebp-1] ; mov [ebp-1],al`,
  pushed as `&tag` to an out-of-line `__copy_backward_ptrs`. A user constructor on `__false_type` drops both stores.
  (0x00470E08 / 0x004712B2, sound-map `__linear_insert`.)
- **STLport iterator tags: model the real hierarchy.** `struct random_access_iterator_tag :
  bidirectional_iterator_tag : forward_iterator_tag : input_iterator_tag`, all empty. Because they have bases they are
  non-POD, so `random_access_iterator_tag()` emits no store. When __copy_backward_ptrs is inlined, its tag temporary is
  homed in a dead argument slot (`lea eax,[esp+1Ch]` = the comparator slot). This is the reason the "tags need a user
  ctor" rule works; the hierarchy is the faithful form. (0x004BAB30.)
- **Size TU (0x45..0x47 def/sound modules): frame or no frame.** A body with an ebp frame needs `optimize("y", off)`
  plus `optimize("s", on)`; a frameless body needs "s" alone. Adding "y" off to a frameless body adds `push ebp ; mov
  ebp,esp` (verified on 0x004713F2). Under "s", the STLport inline helpers must be `__forceinline` or they are called
  out of line: make_heap, __pop_heap, sort_heap, pop_heap, __lg, iter_swap / __iter_swap, `__copy_backward`, allocate,
  fill_n, uninitialized_fill_n, _Construct, __uninitialized_copy.
  (0x00470FA5 / 0x0047142B / 0x00470CF1 / 0x00451C3E.)
- **Under "s", `fill_n` over long becomes `rep stosd`** once allocate, fill_n and uninitialized_fill_n are all
  `__forceinline`. (0x00451C3E.)
- **Read a member into a named local before a virtual call** (`long created = WorldFrame; return frame > created +
  GetLifeTime();`). This keeps retail's load-before-call order; the inline expression loads after the call.
  (0x00812030.)
- **Call through an inline const accessor to pick edx.** `thing->GetPhysicsTC()->GetPhysicsMeshInfo()` through
  `const CTCPhysicsBase* GetPhysicsTC() const` puts the vtable in edx, as retail does. The bare member chain reuses eax.
  (0x008FBDD0.)
- **Write a same-instantiation for-loop erase as the member function itself.** `CTCEntityEvents::RemoveEvent` and
  `CNavRegion::RemoveConnectionFromConnectionList` are `for (it = begin(); it != end(); ++it) if (it->Data == x) {
  erase(it); return; }`. The catalog had them as propagated remove_if rows. FableWin's `AAE` means the CNavRegion
  member must be `private`. (0x008112D0 / 0x00A77710.)
- **const_mem_fun1_t over a by-value class argument.** The copy is built in the pushed slot (`push ecx ; mov ecx,esp ;
  push &x ; call copy-ctor`), the bool result is parked in bl across the parameter's destructor, and the pointer to
  member is a 4-byte code pointer: the class must be complete with single inheritance. (0x0048B2F0.)
- **STLport list range insert.** `_M_insert_dispatch(pos, first, last, const __false_type&)` is a thiscall that never
  touches `this` (stateless allocator), with `ret 10h`. The input iterator is the `_Const_traits` iterator when the
  list copy constructor shares the instantiation. The placement-new null test lands on `&node->_M_data`.
  (0x004B92D0.)
- **Check the leaf's U/V letter.** STLport `_List_iterator` and `pair` are `struct` (U), PDB def classes are V. A wrong
  letter still graded through the largest-function fallback (0x004B92D0, 0x00470E08); confirm with `objdump -t`.

Review at landing:
- Local class models sit next to existing engine headers whose Ego_r (Dinkumware) layouts do not fit the retail
  STLport members:
  - CTCEntityEvents.h (0x008112D0)
  - CMeshBankManager.h (0x00A0A720)
  - CMarkerInfo.h / CMiniMapDisplay.h (0x0082C6E0, 0x0082C770)
  - CQuestManager.h (0x004BA390, 0x004B92D0, only a forward nested CActiveQuest)
  - CTCPhysicsBase.h (0x008FBDD0, a local 94-slot vtable model)
  - CDefClassBase.h (0x0046EECF / 0x0046EF33, only the nested CSubDefInfo)
  - 0x008FBDD0 also uses a local `class CThing { unsigned char _pad00[0x60]; CTCPhysicsBase* PhysicsTC; }` (no
    engine/CThing.h exists).
- Element types disagree across the landed 0x47xxxx sort family. 0x00471150 (CSoundPair `__pop_heap_aux`) is landed with
  a CTextureMorph element, and 0x00471576 (CDefString `__pop_heap_aux`) with pair<long, EHeroAbility>. Their callers
  0x00470FA5 / 0x0047142B prove the element. The bytes match either way, so only the names and models are wrong.
- Landed placeholder names that should get their real names:
  - 0x0047130E is `__copy_backward_ptrs<pair<unsigned long,CDefString>*>`.
  - 0x00471239 is `__unguarded_insertion_sort_aux`.
  - 0x0047119A is `CVectorMap<unsigned long,CDefString,..>::Sort`.
  - 0x00470CD8 is `CVectorMap<unsigned long,CSoundPair,..>::Sort`, landed as global_TransferToGui with a
    CGuiVarTransferBool model.
  - 0x00470DBC models CKeyPairCompareLess as a `void*` context struct.
- The landed 0x00470E9E / 0x00471338 `__introsort_loop` rows use namespace `std` rather than `_STL`, so their mangled
  names differ from the STLport rows here. 0x00471338's callee comments name the 12-byte addresses (0x470F32/0x470F5F/
  0x470FA5) instead of its own 0x4713C5/0x4713F2/0x47142B.
- The landed CTCEntityEvents_FrameUpdate_00811450.cpp is a weak model of the same class; recheck it against this row's
  CTCEntityEvents (Events list at +0xC).
- No row needed more than 4 attempts. 0x00470E08 took 4 grader runs: 2 source forms, plus a leaf-letter fix and the
  promote regrade.
- 0x0048B2F0 has no call or absolute reference in retail (an unreferenced COMDAT); the identity comes from the FableWin
  name set (only `const_mem_fun1_t<bool,CThing,CCharString>`, only `CThing::IsSameScriptName`).
