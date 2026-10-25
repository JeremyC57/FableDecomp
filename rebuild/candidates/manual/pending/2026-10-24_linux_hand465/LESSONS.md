# Chunk 465 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `__unguarded_insertion_sort_aux` over any element is one template that matches on the first grade. It is
  `__fastcall`, with `__unguarded_linear_insert(__i, _Tp(*__i), __comp)` declared out of line and operator() left
  declaration-only. The by-value element is built in its outgoing slot, so the element's real copy semantics decide the
  bytes (0x00AF3240, 0x00624330, 0x00A45F90, 0x00A6F8B0, 0x00CB6860, 0x00A32340, 0x0071FCF0, 0x0080AB40: eight of eight).
- A function-pointer comparator in that helper goes in edx, because the by-value element does not fit a register: write
  `typedef bool (__fastcall* F)(const T&, const T&)` and instantiate the helper with F (0x00A45F90).
- `__insertion_sort(first, last, comp)` with `ret 4` and an element copy before each call is STLport's
  `__linear_insert(first, i, *i, comp)` taking the element BY VALUE. It is not the aux helper (0x00A415D0).
- Byte-by-byte copies stored in R, G, B, A order (with byte spills to the stack) come from CRGBColour's user copy
  constructor `Set(o.R, o.G, o.B, o.A)` inside an otherwise implicit copy (pair<long, CTCHeroMorph::CTextureLayer>, 0x0071FCF0).
- A by-value comparator copied memberwise (a bool moved as one byte, not a dword block) means some member has a user copy
  constructor. Here it is C3DVector's, which makes CThingCompare_BestForTargeting2's implicit copy memberwise (0x0080AB40).
- An inline CIntelligentPointer<CThing> copy is the 0xA01B10 ctor, then the vtable 0x01238C6C store, then
  `SetPItem(other.GetPItem())` (0xA01B50 / 0xA01B90). It is the same model as the landed 0x008C7D80 (NPlayerTargeting::CTargetPoint,
  0x0080AB40).
- The STLport `vector::resize(n)` TU emits both `resize(n)` and `resize(n, x)`, so the Leaf prefix `?resize@?$vector@` matches
  two symbols. Pin the full mangled `...QAEXI@Z` name (0x00AE7AA0, 0x00AE7B10, 0x00BBDD40, 0x00BCC3B0, 0x00BE0140).
- Catalog `vector<unsigned __int64>::_Ufill` rows come in two kinds:
  - `__uninitialized_fill_n`: a `test esi,esi ; je` placement-new null test and `ret 8` (tag on the stack).
  - `fill_n`: plain assignment and `ret 4`.
  (0x004B81D0 vs 0x006149B0.)
- A `pair<K, list<T> >` copy inside `__uninitialized_fill_n` inlines the list copy constructor:
  - a `malloc(0x18)` (0x00BFEA0E) header, self-linked;
  - then `_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&)`;
  - the iterator has a user copy constructor (built in its slot), the const_iterator an implicit one (0x0051A880, 0x00A830A0).
- `vector<T*>::resize(n, x)` with erase inline expands `__copy_trivial`. Its memmove is the direct call 0x00BFEAE6 (no
  dllimport). `vector<4-byte class>` instead expands the `__copy` loop with a `long` distance (0x00A2D0E0 vs 0x0085E8D0).
- An unaligned entry address (0x00467686, 0x00486189, 0x005F9074) predicts `#pragma optimize("s", on)`: the STLport bodies
  then match verbatim.

Review at landing:
- 0x00AB44D0 and 0x00B6CC90 are parked as a scheduling-only diff: the first zero store of an 8-byte default element, and
  edx vs eax for the erase start. I tried five element spellings (4 and 3 grades). The element type is unconfirmed (no callers).
- 0x007D2370 uses a local CDefClassBase model (virtual dtor, 0x18 base bytes, PDefManager +0x1C, GlobalIndex +0x20, a bitfield
  at +0x24, copy ctor 0x0046644C). The generated engine header is pack(1) at 0x25 bytes, which does not fit the retail
  layout (Start +0x28).
- 0x00CD4EA0 CMarkerDataEntry is a descriptive name (taken from its parser 0x00CD5170). No PDB name was found.
- 0x0086D8C0 took 2 grades: the second only renamed the element to the PDB's nested CRandomAppearanceMorph::CBodyPartMesh.
- Already landed:
  - 0x0071DE80 CTCHeroMorph::UpdateTextureMorphs models the composite sort element as {float Priority; long Key; Data[0x10]}.
  - 0x007218E0 calls the same element `SElement24`.
  - Ego_r.pdb says it is pair<long, CTCHeroMorph::CTextureLayer> (the first member is a long key, the rest TextureID +
    CCompositeBlendDesc).
  - This affects names only; the bytes are unaffected.
- The manifest labels the function holding 0x0080B196 `_Insertion_sort<CThing**, CThingCompare_BestForTargeting2>`. It sorts
  NPlayerTargeting::CTargetPoint.
