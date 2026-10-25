# Chunk 459 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `vector::_M_insert_overflow` (non-POD arm): retail homes BOTH `__false_type()` temporaries in the `atend`
  argument slot (`lea ecx,[esp+28h]`) although atend is read later; that only happens when the out-of-line
  `__uninitialized_copy` / `__uninitialized_fill_n` (and `__destroy_aux`, when the element has a destructor) have same-TU
  `auto_inline(off)` + `__declspec(noinline)` bodies that never read the tag. Declarations only give the tag a byte of
  its own (`sub esp,8`, 230v226) -- 0x00721060, 0x00728D10.
- `vector::erase(first, last)` over strings: `__copy_ptrs` -> out-of-line counted `__copy(..., random_access_iterator_tag(),
  (int*)0)` with the tag homed on the LIVE `first` slot (returned at the end); a same-TU `__copy` stand-in body matches
  first grade; the `_Destroy` loop and the string destructor stay inline -- 0x00841FC0.
- An element holding a C3DVector next to a user-copyable member (CIntelligentPointer): C3DVector needs a user copy ctor
  (`X = v.X; Y = v.Y; Z = v.Z;`); with the implicit one VC7.1 copies through two extra register moves (109v105) --
  0x00723270.
- A catalog `find<char*,char>` whose value is read straight from the argument slot (`mov bl,byte ptr [esp+8]`, `ret 8`)
  is STLport `__find_if(first, last, _Eq_char_bound<char_traits<char> >(c), random_access_iterator_tag)`: the 1-byte
  predicate travels by value; callers build it with `mov [esp+N],al ; push dword [esp+N]` -- 0x00841640.
- STLport's default `__type_traits` call every user class non-POD: a vector copy of a {long, bool} element is the
  null-tested placement loop with a two-dword copy, never memmove -- 0x00708830.
- The same verbatim `__unguarded_partition` spills iter_swap's temporary into the by-value pivot's dead second-member
  slots for a 12-byte pair (no frame, 0x0086F820) and into a `sub esp,10h` frame for a 16-byte pair (0x0086B3B0): no
  source tricks, both first grade.
- A vector stride of 9 with a vptr-bearing member (`add dword ptr [edi+4],9`) is a `#pragma pack(1)` element in retail
  ({CIntelligentPointer<CThing>, bool}); the inline intelligent-pointer copy ctor is `: CBaseIntelligentPointer() {
  SetPItem(other.GetPItem()); }` (ctor call, vtable store, GetPItem, SetPItem) -- 0x006A3C70.
- An introsort-labelled row with a `< 15` threshold, two self-calls on halves and a 6-argument tail call is STLport
  `__inplace_stable_sort` -> `__merge_without_buffer` (stack: last, len1, len2, comp) -- 0x00711640.
- STLport list copy ctor inside `__uninitialized_fill_n`: `_List_base(a)` (proxy zero, malloc(0x24) sentinel,
  self-link) + `insert(begin(), x.begin(), x.end())` -> `_M_insert_dispatch(iterator, const_iterator, const_iterator,
  const __false_type&)` matches first grade once `_List_iterator` has the `_Nonconst_traits` copy ctor and the
  `_Const_traits` iterator an implicit one -- 0x008656B0.

Review at landing:
- Same-TU noinline stand-in bodies (their calls bind to the stand-ins, relocation-masked; calls.py confirmed the
  retail targets): 0x006251C0 (comparator), 0x006562E7 (3 helpers), 0x006860F0 (2 ctors), 0x006984A0 (__find),
  0x00721060 (2), 0x00728D10 (3), 0x00841FC0 (__copy).
- 0x006984A0 uses a local CGameCameraManager model (retail NewAutoMode +0xA8, AvailableCamPoints +0xAC) next to the
  existing rebuild/include/engine/CGameCameraManager.h, which carries the Ego_r offsets (+0xAC / +0xB0).
- 0x006A3C70 uses `#pragma pack(push, 1)` for its element: that IS the retail 9-byte stride (the landed caller
  0x008A99F0 models it the same way), not a layout trick -- confirm against the purity rule. The PDB's
  CScreenFilterSThingByPass holds a CScriptThing (16 bytes); TLC's element holds the intelligent pointer.
- 0x0086F090 / 0x0086F820 / 0x0086FEB0 identify the landed descriptive pair<long, CLongMapValue8> map as
  CVectorMap<EExpressionType, CExpressionSet::CEntry> (CCreatureDef::Transfer 0x00675E20 calls its reader / writer, the
  manifest names its __adjust_heap 0x0086F450 so, Add 0x0086F600 keys by the value's first word); rename the landed
  relatives (0x0086EFF0, 0x0086F3E0, 0x0086FA00, 0x0086F600, 0x008700A0) or keep them descriptive -- decide once.
- 0x00728D10's element identification (CMeshMorphSetParts) rests on the layout chain (0x00722130 destroy, 0x005BBDF1
  copy = CArray<CEngineTextureMorph>) plus the CTCRandomAppearanceMorph neighbourhood; no PDB-named caller.
- Already-landed names to fix: 0x0065572D "_Destroy_range" = `_STL::fill`; 0x0086C620 "CCompressorZlib::SetupStream" =
  skeletal-morph `__linear_insert`; 0x00841DE0 / 0x00865650 / 0x00722130 / 0x00728A40 / 0x00728B10 / 0x00728B50 /
  0x005BBDF1 descriptive names (see NOTES.md); 0x0086AF60's CSkeletalMorph members differ from the PDB.
- No row needed more than 3 grades (0x00534F20 and 0x006562E7: 3); no catalog row held two functions.
