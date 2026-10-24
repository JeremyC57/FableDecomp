# Chunk 347 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `pair<long, 40-byte POD>` in `std::__uninitialized_copy` (`mov [eax],ecx ; lea edi,[eax+4] ; mov ecx,0Ah ; mov esi,ebp ; rep movsd`) DOES reproduce (plain MATCH, 0x00673D30): STLport's explicit pair copy ctor `first(o.first), second(o.second)` with a POD second, under the `for (; first != last; ++first, ++cur) new (cur) T(*first)` loop. The wave-19 note "(not reproduced)" is outdated for this form.
- A copy constructor whose member intelligent pointer gets base ctor + vtable store BEFORE the leading dword copies, then Get/SetPItem after them, is `T(const T& o) { *this = o; }` with the pointer DEFAULT-constructed and an implicit memberwise operator= (0x006C5460); memberwise mem-initialisers (or the implicit copy ctor) copy +0/+4 first.
- A bitfield member makes a user memberwise copy ctor re-merge the bit (`xor/and/xor`); a whole-dword copy of the bitfield word is the IMPLICIT copy ctor (0x004C2C70 CMeshInfo).
- A float vector inside a by-value pair stored dword by dword straight off the slot pointer needs its own memberwise user copy ctor; an implicit one block-copies via `add eax,4` (0x00769D50).
- A by-value 16-byte (unsigned __int64, bool) pair whose slot copy writes ONE byte at +8 = user copy ctor of a pair whose second is a bool (8-aligned key pads the element to 16) (0x00A7F0F0).
- `_Adjust_heap` with a polymorphic payload: the implicit operator= skips the vptr, the implicit copy stores the vtable immediate, and the compiler-generated payload dtor just calls the out-of-line base dtor on the by-value pair after `_Push_heap` (0x0076A4E0).
- Many propagated `_Cons_val<...>` labels on thiscall `ret 4` bodies returning `this` are the element's out-of-line copy constructor; `push N ; call malloc ; lea r,[eax+8] ; test r,r` + a caller that links node+0/+4 is STLport `list::_M_create_node` (`this` unused, /Os).
- A `CBase` base default-constructed through its out-of-line ctor 0x0099A2D0 inside a copy constructor = a user-written copy ctor with `: CBaseClass()` (an implicit copy inlines the base's trivial copy instead) (0x009EA2E0, 0x009EE180).
- `#pragma optimize("s", on)` alone gives the `ebp` frame + `leave` of the /O1 heap TU 0x00593E3E; no `optimize("y", off)` needed.
- In namespaced tests/sources the placement `operator new(unsigned int, void*)` must stay at global scope (a namespaced one is not found by `new (p) T`: C2660).

Review at landing:
- 0x009D3750 reuses the LOCAL retail CBankFile model of the landed 0x009D2FC0 because engine/CBankFile.h's default (Ego_r) layout puts RuntimeData/PackedDataOffset at +0x28/+0x48 (retail +0x20/+0x38); the header's opt-in `FABLE_RETAIL_BANK_FILE` layout agrees with the bytes but keeps the members opaque.
- 0x00593E3E needs `#pragma optimize("s", on)` (in the source); 0x005BAC52 and 0x00578F1A too.
- 0x009EDFE0 models CGenericVar's +0 as a vptr (vtable 0x0129C3E4, RTTI CGenericVar); the landed CGenericVar::ReadFromParser 0x00A64360 models it as a plain `long Padding`.
- The sound-bank record's out-of-line members are landed under three unrelated names (0x009922F0 SRecord::SRecord, 0x00992630 "CCamera::UpdateCleanupFlags", 0x00992510 CSoundBankRecord::~CSoundBankRecord); 0x00992B80 names them CSoundBankRecord.
- No row needed more than 4 grades; no catalog row holds several functions.
