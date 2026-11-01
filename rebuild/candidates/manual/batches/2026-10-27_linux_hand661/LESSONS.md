# Chunk 661 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **The ??1 / ??_G / _Destroy triple**: for a class that a TU destroys by name and keeps in an STLport container, VC7.1 emits
  three COMDATs from one `_Destroy<T>` instantiation -- the implicit `??1`, a non-virtual `??_G`, and `std::_Destroy<T>` -- and
  retail kept all three, in that address order inside the TU (??1 with the special members, ??_G after, _Destroy among the
  STLport helpers). Four masked-identical "_Dest_val" rows were four different instantiations: pair<CCharString,CPersistInfo>
  0x004B6A80/0x004B77A0/0x004B86B0, pair<CCharString,CSymbolInfo> 0x006FB460/0x006FB730/0x006FB8D0, NFire::CFirePoint
  0x007803D0/0x007811D0/0x00781540, AttackParticleAndEndPoints 0x00768BF0/0x007692A0/0x007699C0. Find the triple by scanning
  the TU for the body prefix, then name the element from the TU's other STLport instantiations / the Ego_r layout.
- **A same-name COMDAT exists once per image**: two byte-identical bodies can never share a mangled name (the linker keeps
  the first); the landed 0x004B86B0 and 0x006FB460 both carried `??1?$pair@VCCharString@@VCSymbolInfo..` -- one was wrong. When
  the element layouts coincide (pair<CCharString,CPersistInfo> 12 bytes vs pair<CCharString,CSymbolInfo> 16 bytes destroy the same
  +8/+4/+0 strings), the TU decides the element.
- **Dynamic-initializer naming from the .CRT$XCU order**: walk the retail XCU table (0x01374000..) next to the PDB
  `dynamic initializer for` sequence: anonymous-namespace header constants (REACTION_MANAGER_DEF_INSTANTIATION_NAME, 32 retail
  copies of "OPINION_REACTION_MANAGER_INSTANCE") anchor each TU; per-TU constants then match literal for literal (STrap x4,
  BOASTING_CROWD x3, TITLE_NAME / ITEM_* x3). Write them `namespace { CCharString X("lit", -1); }` with Leaf `_$E1`; line 1
  `// $E_X() -- ...` (finish.py module _global). An unreferenced global with a ctor is still emitted (no reference needed).
- A static STLport tree that retail never reads (CThing::TCsToBeRemoved 0x013B8A8C) is named from the initializer order and the
  TU's .bss block; its element type is unprovable -> the CUnproven K/V of the instantiation its atexit _M_erase belongs to.
- engine/CParentDefClassBase.h's out-of-line `virtual ~CParentDefClassBase();` makes derived implicit dtors call 0x00430300; retail
  /O2 TUs inline the (implicit) parent dtor (0x0067C370, 0x004304A0). Use the engine/CDefClassBase.h opt-in chain plus a local
  CParentDefClassBase without a dtor declaration until the header is fixed.
- Audio-library output vtables (DirectSound 0x012B8D98, OpenAL 0x012B8ED4, waveOut 0x012B8FCC, silent 0x012B9174) share one
  slot order: 1 Initialize, 2 Uninitialize (DirectSound releases the device; the others only call slot 5), 3 CreateAndPlay,
  4 CreateLoopingBuffer (0x00C572C0 `return false` default), 5 ReleaseBuffer, 6 IsChunkReady, 7 LockChunk, 8 UnlockChunk/Write.
  A slot-N body `this->slotM()` is `mov eax,[ecx] ; jmp [eax+4*M]`.

Review at landing:
- NAME_FIXES.tsv (51 rows): the triple neighbours 0x004B86B0 / 0x004B77A0 / 0x006FB730 / 0x00781540 / 0x007811D0 / 0x007699C0 /
  0x007692A0 / 0x00768BC0 (each lane-verified RELOCATION_MATCH under its new spelling except the ctor 0x00768BC0, not compiled),
  0x00527950 = STLport _M_erase (lane-verified), the data global 0x013B8A8C (binding `_g_StaticTree013B8A8C` from the landed
  thunk 0x01228DF0), and 41 anonymous-namespace CCharString globals. Their `?A0x` hash is per TU and unknowable for retail: the
  respell keeps the lane's hash (as the landed anonymous-namespace rows do).
- The landed 0x00528E90 (`?Erase@CRetailTree_erase00527950`, "LTextTreeWalkThrough_Destructor") is ~_Rb_tree of the same
  CUnproven_00527950 instantiation (calls _M_erase 0x00527950); its own leaf was not re-proven here.
- 0x0121F160 (the 0x0121A670 twin) and its 14 siblings still use the invented `CRetailTree_node0x20` stand-in; the
  STLport _Rb_tree text of 0x0121A670 is the drop-in replacement (each needs its own instantiation / global name).
- Header debt: engine/CParentDefClassBase.h dtor (above).
- 0x0067C370's local CParentDefClassBase keeps the header's `#pragma pack(push, 1)` (retail 0x39-byte layout, CVectorMap at +0x29);
  no new packing was introduced.
