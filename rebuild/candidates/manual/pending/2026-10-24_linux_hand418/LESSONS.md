# Chunk 418 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport 4.6 sorts over CCharString pairs match first try when written as verbatim `_algo.c` / `_heap.c` `__fastcall`
  templates in `_STL`. The model needs the inline CCharString operator< over a same-TU noinline
  `CBasicString<char>::operator<` and STLport's explicit pair copy constructor. Out-of-line callees are only declared.
  15 of the 16 rows took 1 grade (0x00A40370 .. 0x00A41CD0).
- A by-value comparator slot that is dead after the comparison absorbs the `copy_backward` random_access_iterator_tag
  with a declaration-only `__copy_backward`: no stand-in is needed (0x00A40730, 0x00A40F50).
- `vector::erase(first, last)` homes `copy()`'s random_access_iterator_tag in the LIVE `first` slot (`lea eax,[esp+18h]`
  after `push 0`). Give `__copy` a same-TU noinline body that never reads the tag (0x00A40C10).
- `CVectorMap::LowerBound`: `int count = _M_finish - _M_start;` BEFORE `first = _M_start;` gives retail's
  `mov esi,[ecx]` ... `mov ebp,esi` (count first, then first). First-then-count loads _M_start straight into ebp (216v217).
  Lionhead's loop halves with `count / 2` (`cdq; sub; sar`), not STLport's `>> 1` (0x00A40AF0, as landed 0x00447C80).
- The `pair<CCharString, CCharString>` operator< is `__cdecl` even inside /Gr code (two pushes, `add esp,8`,
  0x00A40250). It is inline in 0x00A40820 and 0x00A40AF0 and called out of line in 0x00A3EE50, 0x00A40D80,
  0x00A40F50 and 0x00A41CD0: declare it, or give it the STLport body, to match each row.
- The same `__partial_sort` source inlines `__pop_heap` for 8-byte entries (0x00A41B60, `and esp,-8` ebp frame) and
  calls it out of line for 12-byte entries (0x00A41CD0). Declare `__pop_heap` only in the row that calls it; it is
  inline (non-`__fastcall`) in the other. This is the inliner's size call, not a source difference.
- `CVectorMap::Insert` = `Data.push_back(value_type(key, value)); Dirty = true;`. The pair temporary's destructor
  runs before the Dirty store on both paths, and the `__false_type` tag lands in the dead key slot (0x00A41AD0).
- `??_G` of a polymorphic class: define the destructor OUT OF LINE in the TU (`T::~T() { }`). The in-class
  `virtual ~T() {}` made every member destructor an out-of-line call (54v78 -> match, 0x00A38080).
- `CVectorMap::Find`: hold `e = Data.end()` in a local (`if (it == e || Comp(key, *it)) return e; return it;`), or
  end() is reloaded in each arm (0x00A3ED30, 0x00A3EE50).
- Named size locals in `CCompressorZlib::Compress` swapped esi/edi. Repeating `count * elementSize * 2 + 13` inline
  matched (0x00A3AD20).
- `if (cond) { ...; return x; } return 0;` keeps retail's block order where the `if (!cond) return 0;` early return
  did not (0x00A3ECB0).
- A same-TU noinline stand-in for an out-of-line CVectorMap::LowerBound keeps the key register live across the call,
  as retail's LTCG did (0x00A38420).
- guiddef.h inlines emitted out of line: write `__fastcall` and add `#pragma intrinsic(memcmp)` with the stock
  `!memcmp(&a, &b, sizeof(GUID))` body (0x00A3AEF0, 0x00A3AF10, 0x00A3AF30).

Review at landing:
- No row needed more than 4 grades. 00a38080 took 3.
- 7 rows (0x00A3CB90 .. 0x00A3D380) use a LOCAL polymorphic model
  (CIVCountedPointeeBase / C3DAnimationState / C3DAnimationStateAlpha) next to the existing
  engine/C3DAnimationStateAlpha.h. That header flattens the vptr into a data member, which cannot produce the
  per-level vtable stores of the implicit copy constructor.
- Same-TU stand-in bodies are oracle helpers, not reconstructions: `_M_insert_overflow` in 00a3d380, 00a418a0 and
  00a41ad0, and `LowerBound` in 00a38420. The real `__copy` (0x00A40040) is reproduced in 00a40c10, and the real
  `CBasicString<char>::operator<` in all 16 sort rows. 00a38080's TU also defines the CClassFactory constructor
  (0x00A373B0, landed separately) only to emit the vtable; the leaf selects `??_G`.
- Already-landed errors:
  - The CSoundEmitterLH constructor 0x00AAC580 takes the owning CSoundManagerLH* as its first parameter (the landed
    source calls it CGameScriptThing*). 0x00A3E420 is a CSoundManagerLH method, not DecrementScriptCounter.
  - The landed CCRCSymbolMap::ParseEnum 0x00A38670 calls 0x00A385A0 "AddEnumValue"; the FableWin.pdb name is
    AddSymbolDirty.
  - The landed CSoundManagerLH vfunc_38 / vfunc_40 declare 0x00A3ED30 / 0x00A3EE50 with long keys; both take
    `const CCharString&`.
  - The landed CClassFactory constructor 0x00A373B0 models CClassTypeInfo as 0x20 bytes; retail is 0x14 (4-byte
    STLport list).
  - The landed vfunc_6 0x00A3EC70 returns long, but its caller (0x00A3ECB0) tests al, so it is bool.
- Manifest: 0x00A40250 is labelled `_Med3<pair<CCharString,CCharString>*,...>`, `__fastcall`, but it is the `__cdecl`
  pair operator<. The landed 0x00447C80 LowerBound is one folded body for several 12-byte-entry maps (already noted
  in that source).
