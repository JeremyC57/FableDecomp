# Hand chunk 351 (_Copy_impl<NPlayerTargeting::CTargetPoint*> .. _Copy_impl<std::pair<unsigned long,C3DDummyObject>*>): 39 of 40 byte-exact

All 40 rows carry propagated Dinkumware `_Copy_impl<...>` catalog labels. The bodies are the retail STLport 4.x
algorithm helpers (`/Gr` TUs, `__fastcall`) for many different element types, plus four non-copy functions (two
insertion sorts, a list range erase, an SSE DSP kernel). Every matched source sits in `src/` with a behaviour test in
`test/` (PASS under g++ and under the lane's VC7.1 cl.exe); `grades.txt` holds the final `gall.sh` run (9 MATCH,
30 RELOCATION_MATCH).

## What matched (39)

### STLport `__copy(first, last, result, const random_access_iterator_tag&, ptrdiff_t*)` -- `ret 0Ch` (17)
- 006314c0 CConfigOptionsDefaultsDef, 006b65c0 CThemeChannel, 00719f00 CSubtitledMessage, 0075f130 CKeyCamera,
  00889830 CBlendedEnvironmentTheme, 00a2bbb0 C3DMeshInfo, 00770230 / 00a82650 (4-byte records): one out-of-line
  `operator=` per element. Shape: `for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
  return result;` with the element as `unsigned char _body[N]` + the declared assignment (the reciprocal-multiply magic
  gives N). The element names come from the callees' landed sources.
- 00517200 {CCharString, 12-byte vector}, 0051cae0 {vector, vector}, 00a40040 {CCharString, CCharString, long},
  00adc0d0 / 00adc120 {vector, CRetailTree_0045c811}: the element's IMPLICIT operator= inlines memberwise (each member
  with its own assignment a `push src ; mov ecx,dst ; call`).
- 00806e10 / 00806ea0 NPlayerTargeting::CTargetPoint (PDB layout, the landed 0x0080AA60 model): the forceinline
  CIntelligentPointer assignment (`SetPItem(src.GetPItem())`), Index, the C3DVector by its implicit assignment
  (re-addressed `mov ecx,esi ; mov edx,edi`), FrameRejected.
- 00748af0 {CIntelligentPointer<CThing>, 8-byte POD}: matched on attempt 2 -- the two words at +8/+0xC must be ONE
  nested 8-byte record; that is what produces retail's strength-reduced src+8/dst+8 pointer pair (two plain members
  compile to `[esi+8]`/`[edi+8]` off the element pointers, 72 bytes).

### STLport `__copy_ptrs(first, last, result, const __false_type&)` with `__copy` inlined -- `ret 8` (2)
- 004e9bda / 004ed102 CShopItemDef (both in vector<CShopItemDef> helpers: operator= and erase; the latter's
  _M_fill_insert @ 0x004ED161 stores the CShopItemDef vtable). Size-optimised frame TU: `#pragma optimize("y", off)` +
  `("s", on)`, `push 48h ; pop edi ; idiv`, `push 7 ; pop ecx ; rep movsd` for the 28-byte CShopItem. Matched on
  attempt 2 once `__copy` was `__forceinline` (a plain `inline` __copy stays a call under "s").

### STLport `__uninitialized_copy(first, last, result, const __false_type&)` -- `ret 8` (13)
- 0040fc50 / 0040ff80 / 00b8eed0 (28-byte), 00f37cc0 (44-byte) PODs: placement new under the null test with a
  memberwise USER copy constructor (direct dword moves through esi; the implicit copy would be a block move).
- 005e16f0 pair<unsigned long, CAnimationEntry> (the landed 0x005E0E20 layout): implicit pair copy + the intrusive
  CIVCountedPointer copy (`p = o.p ; if (p) ++p->count`).
- 006f10f0 / 006f14a0 CDefendingCombatantInfo::CProng (PDB: bool +0, C3DVector +1, long +0x10, CIntelligentPointer
  <CThingCreatureBase> +0x14): the C3DVector record is declared under `#pragma pack(push, 1)` (alignment 1) so the
  vector lands at +1 while the timer stays aligned at +0x10; implicit CProng copy constructor.
- 00806c00 / 00807980 CTargetPoint: forceinline copy constructors (CBaseIntelligentPointer() + derived vtable store +
  SetPItem(GetPItem())), C3DVector user copy constructor (direct stores).
- 0077e280 {CCharString, 12-byte record}, 0088abc0 / 00a458b0 {CCharString, long, 12-byte record}, 00a6ed80
  {CCharString, 12-byte record, long}: the out-of-line CCharString copy then a 12-byte record with a memberwise user
  copy constructor. The strength-reduced pointer pair addresses the record's LAST word, so its offset gives the member
  order: +0x10 when the word precedes the record (0088abc0), +0xC when it follows (00a6ed80).

### Insertion sorts (4)
- 00a46680 / 00a717a0 `__insertion_sort(first, last, comp)` (`ret 4`): `if (first == last) return; for (i = first + 1;
  i != last; ++i) __linear_insert(first, i, *i, comp);` -- the by-value element is built in the argument area by its
  implicit copy constructor (same element layouts as 00a458b0 / 00a6ed80 in the same TUs). 00a46680's callee calls the
  comparator through a register (`call ebx`, a `__fastcall` function pointer); 00a717a0's callee inlines
  CCharString::operator< (an empty functor) -- both re-push the 4-byte slot identically.
- 004d03c0 `__unguarded_insertion_sort_aux(first, last, T*, comp)` (`ret 8`), {float key, word}: matched on attempt 2
  once the pair had STLport's memberwise user copy constructor (the key then lives only in the 8-byte frame and the
  final store reloads it through eax; the implicit copy spends ebx+ebp, 84 bytes).
- 00471239 the same algorithm for {unsigned long key, word} in a size-optimised frame TU (`y` off + `s` on): matched
  on attempt 2 once the inlined step was `__forceinline` (under "s" it stayed a call).

### Others (3)
- 00471be9 `list<24-byte POD>::erase(first, last)` (size TU, `ret 0Ch`, `this` unused because STLport's list keeps no
  size): `while (first != last) erase(first++); return last;` with the one-node erase __forceinline; leaf pinned to the
  mangled range overload (both erase overloads are emitted).
- 005810ae STLport `__copy(list_iterator, list_iterator, back_insert_iterator<vector<CHeroMarriageInfo>>,
  const input_iterator_tag&, ptrdiff_t*)` (size TU, `ret 10h`): hidden result pointer in ecx, the tag reference in edx
  (the first register-eligible argument), iterators on the stack; `*result = *first` is the inlined push_back call.
  Uses engine/CHeroMarriageInfo.h.
- 00672210 `__copy_backward_ptrs(first, last, result, const __false_type&)` over longs (`ret 8`) and 007108a0
  `copy_backward(first, last, result)` over pointers (`ret 4`): `for (n = last - first; n > 0; --n) *--result = *--last;`
  with the inner helper inlined (same body bytes, different entry).

## Parked for Fable 5.1 (1)
- 00c88cd0 _global::DSP_AddVector_00c88cd0 DIFFER(49v62): retail keeps the block count in esi (`mov esi,eax` ; frame `and esp,-16 ; sub esp,0Ch ; push esi`), loads dst/src before the `shr ; test ; jbe`, and copies dst per iteration (`mov eax,ecx`) to store through after the early `add ecx,10h`; ours has no counter register, loads after the test and stores through the stepped pointer | tried: __m128* + `*d = _mm_add_ps(*d, *s)` with `n = count >> 2` down-count (57, src-dst merged induction `[edx+eax]`), float* + `_mm_load_ps`/`_mm_store_ps` + `i < blocks` up-count (49, best: jbe and separate steps), an inlined `CVector4SSE::operator+=` (57), the 49-byte form with /G7 (49) | next: a named per-iteration pointer local (`__m128* out = d; *out = _mm_add_ps(*out, *s); ++d; ++s;`) or post-increment stores (`*d++ = ...`) with locals for dst/src declared before `blocks`; check whether this DSP TU is one of the 11 QFE-4035 objects or used other SSE codegen flags, else lane gap: SSE scheduling/regalloc.

## Skipped
- none (no row holds a second function within its length; the chunk lengths agreed with the bodies' `ret`).

## Label corrections
Rows (catalog name is the propagated Dinkumware `_Copy_impl<...>` everywhere):
- 0040fc50, 0040ff80, 00b8eed0, 00f37cc0: STLport __uninitialized_copy of 28-/44-byte PODs (pair<float, CCameraTag> fits 28 bytes for the first two but is unverified; 00f37cc0 is 44 bytes).
- 00471239, 004d03c0: SGI/STLport __unguarded_insertion_sort_aux (unsigned-long and float keys), not copies.
- 00471be9: STLport list<24-byte POD>::erase(first, last) (called from the list's operator=), not a copy.
- 004e9bda, 004ed102: STLport __copy_ptrs<CShopItemDef*> (catalog says pair<unsigned long, C3DDummyObject>).
- 00517200, 0051cae0, 006314c0 (CConfigOptionsDefaultsDef), 006b65c0 (CThemeChannel), 00719f00 (CSubtitledMessage), 0075f130 (CKeyCamera), 00770230, 00889830 (CBlendedEnvironmentTheme), 00a2bbb0 (C3DMeshInfo), 00a40040, 00a82650, 00adc0d0, 00adc120: STLport __copy, none over pair<CCharString, CCharString>.
- 005e16f0: __uninitialized_copy<pair<unsigned long, CAnimationEntry>*>.
- 006f10f0, 006f14a0: __uninitialized_copy<CDefendingCombatantInfo::CProng*>.
- 00748af0: STLport __copy over 16-byte {CIntelligentPointer, 8-byte record} (not CTargetPoint, which is 28 bytes).
- 0077e280, 0088abc0, 00a458b0, 00a6ed80: __uninitialized_copy over {CCharString, ...} records.
- 00806c00, 00807980 (__uninitialized_copy) and 00806e10, 00806ea0 (__copy): the element CTargetPoint is right, the algorithm name is not.
- 00a46680, 00a717a0: __insertion_sort(first, last, comp).
- 005810ae: the instantiation is right (list<CHeroMarriageInfo> -> back_inserter(vector<CHeroMarriageInfo>)); the body is STLport __copy (input iterators).
- 00672210, 007108a0: copy_backward (longs / pointers), not pair<float, EConversationAttitude> (8 bytes).
- 00c88cd0: an SSE DSP kernel (dst += src) in the DSP function table @ 0x012BF1E0, not a copy.
Callees and callers met on the way:
- 0x00454886 and 0x0045BC09 ("CActiveFile::operator="): vector<4-byte>::operator= bodies.
- 0x006B6180 ("CBlendedEnvironmentTheme::operator="): CThemeChannel::operator= (as its landed source says); 0x00719E80 (a "CSubtitledMessage" copy constructor in the catalog): CSubtitledMessage::operator=; 0x00A2B3C0 ("CAnimationEntry::operator="): C3DMeshInfo::operator=; 0x0075EF00 ("CEnginePrimitive2DText::operator="): CKeyCamera::operator=; 0x00431F10 ("CThingBuildingDef::operator="): CDefClassBase::operator= -- all already landed under the right names.
- 0x00A46530 ("Vector_InsertConditionalShift_Decal") and 0x00A70F40 ("Vector_ConstructElement_Pair"): STLport __linear_insert(first, last, value, comp).
- 0x006721C0 ("std::InsertSorted_Pair"): __linear_insert over longs; 0x00710D00 ("StableSortMerge_Pair_EConversationAttitude"): a backward merge over pointers ordered by the long at +0x54 of each pointee.
- 0x00748D60: STLport __copy_ptrs (calls 00748af0); 0x004D0A30: __unguarded_insertion_sort (calls 004d03c0); 0x004711F5 ("make_heap<CThing**,...>"): a __final_insertion_sort over 8-byte pairs (calls 00471239); 0x004ED0C6: vector<CShopItemDef>::erase(first, last), 0x004ED161 its _M_fill_insert.
- The landed vector<CCombatWheel::SSlot>::operator= @ 0x006F3C20 names its element "SSlot": it is CDefendingCombatantInfo::CProng (PDB layout; the landed CProng::SetPOccupyingAttacker @ 0x006F17A0 uses the same class).
