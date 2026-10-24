# Chunk 381 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The CTCInventoryAbilities STL TU (0x005F0000..0x005FA4xx) is a size TU: `#pragma optimize("s", on)`. Bodies that keep a frame pointer add `("y", off)` first. The giveaways: `__copy` bumps the result in its argument slot (`add dword ptr [esp+8],4`), and `__median` shares one epilogue. The CTCInventoryMap code from 0x005FA5xx on, and the 0x0061xxxx persist code, are speed TUs (no pragma).
- In the size TU, small STLport helpers are emitted out of line unless marked `__forceinline`: iter_swap/__iter_swap_aux (0x5F8C9B), copy_backward (0x5F9A98), _Copy_Construct, allocate, max, _M_set, and the counted CDefPointer operator= / copy ctor.
- A callee's body matters, not just its declaration. VC7.1 allocates the caller's registers using the clobber set of callees compiled earlier in the same TU.
  - 0x5F8EC0: the allocator temporary lands in the live n slot only when the `_Vector_base(n, a)` body is defined above under `#pragma auto_inline(off)` (declaration only: 48v47).
  - 0x5F9633: the `__uninitialized_copy` stand-in must clobber edx (count down `for (int n = last - first; n > 0; ...)`), or the caller keeps a value in edx across the call (222/226 v 225).
- A record that VC7.1 copies implicitly needs a user copy constructor on its C2DVector member (`X(o.X), Y(o.Y)`). With a POD C2DVector, the trailing words merge into one block copy and two extra cursors get strength-reduced (`lea esi,[ebx+8] ; add edi,8`; 0x613DD0 went 111 v 84).
- Out of line, STLport 4.5 set/map find is `_M_find(k)`: it returns the node pointer in eax with `ret 4`. A find that returns an iterator object uses a hidden pointer and `ret 8` (0x614A10).
- The STLport `_Rb_tree::_M_insert` model from 0x004ABC70 carries over to any value type. Swap the value/KeyOfValue, and the explicit pair copy ctors give the three-word edx copy (0x616F90). `_Rb_global<bool>::_Rebalance` is `static __cdecl`.
- In the CTCInventoryMap TU, when for_each + clear run on several member vectors, write them directly in the body. Routing them through an inline helper that takes the vector by reference swaps esi/edi (0x5FC510).
- `CPersistTraits<T>` for a record type holds an uninitialised `T DefaultVal`: GFSerialiseVectorBinaryOut<CUIRegionInfo> (0x614700) has a 40-byte frame. The CPersistContext transfer arms give each GFSerialiseVector* element type: 0x6192F0 u64, 0x6197D0 CUIRegionInfo, 0x619890 long.
- Retail STLport lists are 4 bytes and vectors 12, so PDB (Dinkumware) member offsets run high. CTCVillage::PlayersCrimes sits at +0x64, after 15 lists from +0x28 (PDB 220). CTCInventoryMap's primitive-handle vectors sit at +0x1AC/+0x1B8/+0x1C4/+0x1D0 (PDB 0x1E4..).
- Free STLport templates in these /Gr TUs are `__fastcall`. Their leaf is the mangled `??$name@` prefix. Member functions of class templates use `?name@`, and member templates use `??$name@`.

Review at landing:
- 00618de0 (map::operator[]) is parked on store scheduling alone. Its 5.1 retry should try `pair()` with body assignments or a named `_Tp` local before calling it a lane gap.
- 00619560: try the `T DefaultVal` traits member (as 0x614700) with a POD CUIRegionInfo. The parked best uses a zeroing default ctor, which is only a probe lever.
- Sweep the landed 0x00624B90: it is named `vector<pair<int,CWideString>>`, but the element is CQuestCardItem (Ego_r.pdb, CTCInventoryQuests::QuestCardItemList).
- Check the 0x004FB490 label (GetCategoryWithIndex returns CCategory*, yet the 0x00606300 site stores its results as const CRegion* pairs). Check 0x0065914B, which is labelled CBankFileAsyncData but touches CTCVillage+0x64.
- 005fc510 keeps a local CTCInventoryMap with a 0x1AC-byte leading block. No engine/CTCInventoryMap.h exists, and the Ego_r offsets do not match retail.
