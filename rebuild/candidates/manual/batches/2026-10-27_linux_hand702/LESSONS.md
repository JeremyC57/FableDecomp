# Chunk 702 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CTCBase vtable slot 12 is `virtual void Copy(const CTCBase& other)`: a 19-25-byte slot-12 body that adds the same offset to
  `this` and the argument and tail-jumps is `Derived::Copy` = `const D& o = (const D&)other; Member = o.Member;` over an
  out-of-line STLport `vector<T>::operator=` (0x00778B10, 0x00722570); there is no CTCBase::Copy call in such overrides.
- A 21-byte NUISystem override `mov eax,[esp+4]; mov edx,[eax]; mov [ecx+N],edx; mov [esp+4],eax; jmp CComponent::AddChild` is
  `PMember = child.Get(); CComponent::AddChild(child);` (qualified base call skips the class's own AddChild; 0x00544310).
- A const virtual that tail-jumps an interface call after an out-of-line accessor needs the interface pointer in a NAMED local;
  the one-expression chain pushes the argument before the accessor call and loses the tail jump (0x00657200, as 0x00643FC8).
- An implicit destructor of a plain struct whose only non-trivial member is a CEnginePrimitiveHandle inlines the handle's virtual
  dtor only with `__forceinline` (the fleet's model); plain inline gives an 8-byte `add ecx,44h; jmp ~CEnginePrimitiveHandle`, with
  or without a user ~T (0x00BFBBF0).
- Two adjacent identical 8-byte `mov ecx,[ecx+8]; jmp CPlayerManager::GetMainPlayer` bodies (0x00643D7C / 0x00643D84) are the
  non-const / const NPlayerGui::CDrawBase::GetPlayer pair (Ego_r 0x00686C81 / 0x00686C89, same 8-byte spacing); a const caller
  binds the second.

Review at landing:
- NAME_FIXES.tsv carries respells for landed rows / bindings: 0x00643D7C, 0x00643D84, 0x00643FC8, 0x00445EE0, 0x00AEE8C0,
  0x00569A80, 0x00566DC0, 0x00566E10, 0x00562E10, 0x007305A0, and the PROBABLE 0x00562E70 Close (donor order + body only).
- 0x00BFBBF0 relies on `virtual __forceinline ~CEnginePrimitiveHandle()` (same as the landed includers 0x005424B0, 0x005483C0,
  0x00AFBBD0, ...); FableWin.pdb (debug) has an out-of-line ??1CEnginePrimitiveHandle, Ego_r does not.
- 0x008D7580 uses `#pragma pack(push, 1)` for CModeOutOfActionBase (PEmitter at the PDB's offset 109 = +0x6D; the landed ctor
  0x00740160 does the same).
- Local base models next to existing (flattened) engine headers: CTCBase, CModeStandBase, CObserver (NUISystem), CParticleEmitter,
  CEnginePersistentPrimitiveBase, CGamePlayerInterface (see NOTES.md Header debt).
