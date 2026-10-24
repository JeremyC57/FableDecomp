# Hand chunk 361 (_Push_heap .. _Sort_heap): 39 of 40 byte-exact

All 40 rows are STLport algorithm instantiations (`__push_heap`, `sort_heap`, `remove_copy_if` / `remove_copy` /
`__unique_copy`). The catalog names (`_Push_heap`, `_Sort_heap`, `_Adjust_heap`, `_Remove_if`) are the Ego_r.pdb
(Dinkumware) spellings, propagated onto the retail bodies. The proof is in 0x0080B9F0: the out-of-line `__pop_heap_aux`
takes (first, last, `_Tp*`, comp), which is STLport's parameter order, and the code also shows `hole > top`,
`secondChild - 1` and a sort_heap loop with no inner count test. Every row is `/Gr` (`__fastcall`: first in ecx, second
argument in edx, the callee pops the rest). gall.sh: 15 MATCH + 24 RELOCATION_MATCH. tall.sh and tall.py: 39/39 PASS.

## What matched (39)
- **_Push_heap (25 of 26), STLport `__push_heap`.** It percolates `val` up from `hole` while `hole > top &&
  comp(first[parent], val)`, then stores `val`; the callee destroys its by-value element.
  - The compare must be written `hole > top`. The Dinkumware `top < hole` swaps the cmp operands.
  - Element stride, signed vs unsigned jcc, and the ret count give the true element type.
  - Plain rotated loop: 00769470, 0086a7f0, 00ae5af0, 00a6b1e0 (less<pair> lexicographic), 00a31380, 00819c30,
    0086a770, 009b1ca0 and 0086f100.
  - `#pragma optimize("s", on)` gives the unrotated `jmp`-to-test loop: 005f7d85, 005f7dd4, 00593965.
  - Unsigned 64-bit keys compare the high word first (`ja done ; jb move`, then `jae`): 00a7d6f0, 00a7d7a0, 00a820e0
    and 00cb93e0. The 8-aligned pair's padding word is copied only when the element is trivial.
  - Polymorphic second member, where the implicit operator= skips the vptr and the generated dtor only calls the
    base dtor: 00769e20 (CParticleUpdateParamDesc) and 006a2e20 (CIntelligentPointer, `SetPItem(GetPItem())`).
  - 005e1310: the intrusive CIVCountedPointer release has to go through a local pointer to get `dec [reg+4]`.
  - Register-pressure rows: 006fb9c0, 00bee420, 009b77b0, 00992940, 006be9d0, 009d3040. Read the key into a local
    once, compare through an (entry, key) overload of the comparator, and do the final store memberwise or through a
    named destination.
- **_Remove_if (7 of 7).** Every caller inlines `find_if` and then calls the copy half, so these rows are:
  - **STLport remove_copy_if** (`if (!pred(*first)) *result++ = *first`):
    - 0080df90: the comparator holds a CIntelligentPointer, passed by value and destroyed in the callee.
    - 00748d80, CDeadDangerThing: drops a null thing, a thing with flag byte +0x91 bit 0 set, or a zero DangerStatus.
    - 00807b70, CTargetPointIsNotEnemyOfThing: calls `CTCEnemy::AreThingsEnemies` 0x0076D2F0.
    - 00809d60, `CFilter_Bind2<IsInFOV, IsStealableStockItem>`. Both filter `operator()`s must be `__forceinline`.
      HasTC needs `if (bits[t>>5] & (1UL << (t&31))) return true; return false;` to get
      `test dword [eax+28h],100000h`, and the GetTC ternary must compare against the by-value type (immediate
      `cmp [eax],54h`).
    - For CTargetPoint, the C3DVector has a user copy ctor and no operator=. That gives the pointer-register copy
      (`mov ecx,edi ; mov eax,[ecx] ; mov edx,ebp`) inside CTargetPoint's implicit assignment.
  - **STLport remove_copy** (value by const reference): 00737ad0 and 0074ce40, over 8-byte intelligent pointers.
  - **STLport `__unique_copy`** (forward-iterator tag, `ret 0Ch`): 00671be0.
- **_Sort_heap (7 of 7), STLport `sort_heap`.** The loop is `while (last - first > 1) pop_heap(first, last--, comp);`.
  - 00777ee0 (int) and 00a5a7d0 (float): the whole `__pop_heap_aux -> __pop_heap -> __adjust_heap` chain is inline
    (needs `__forceinline` in the lane) and only `__push_heap` is called. Two details decide the bytes:
    - pop_heap must not test the count itself; Dinkumware's extra `1 < last - first` gives `sar ; cmp 1` instead of
      `and -4 ; cmp 4`.
    - The lone-child step must use `secondChild - 1`; `bottom - 1` swaps eax/ecx.
  - 004d15a0, 004d1600, 0067f040, 008182e0: `__adjust_heap` stays out of line.
    - The element's copy semantics show in the bytes. A user copy ctor makes the float key go through `fld`/`fstp`
      (004d15a0). std::pair<float,CCharString> is implicit and copies the float as a dword (004d1600); its 8-byte local
      also gives the `and esp,-8` frame. A POD gives `rep movsd` (0067f040, CDegradableInfo). C3DVector members give
      the mixed x87/dword copy (008182e0).
  - 0080b9f0: `__pop_heap_aux` itself stays out of line (0x0080B430). The `push 0` value-type pointer comes before the
    52-byte comparator, which is copied memberwise each pass; its bool is cached in bl.

## Parked for Fable 5.1 (1)
- 00cbaba0 std::_Push_heap<pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>> DIFFER(218v207): in the by-value pointer release, retail keeps the count block in esi (`mov esi,[esp+24h] ; test esi,esi ; dec dword ptr [esi] ; jne` then `[esi+8]`/`[esi+4]`/`push esi`) where ours re-reads val.m_pCount after the dec (`mov eax,[esp+18h] ; cmp dword ptr [eax],0`); the loop and the out-of-line final assignment (0x00CBA520) already match | tried: implicit pair operator= with an inline CCountedPointer operator= (267v207), nested Release/AddRef helpers, out-of-line pair operator= plus a memberwise loop copy, `__forceinline` Assign | next: write ~CCountedPointer with a block local (`SCountBlock* c = m_pCount; if (c && --c->m_references == 0) { c->m_pfnDestroy(c->m_pObject); operator delete(c); }`)

## Skipped
None. No row holds two functions. The rows whose length is larger than the code (006a2e20, 00bee420, 00a31380,
009b77b0, 00769e20, 009b1ca0, 009d3040, 00671be0, 00809d60, 0080df90, 0067f040, 0080b9f0, 006be9d0, 00cbaba0) end in
int3/nop padding only: each matched body ends at its `ret N` and nothing else follows. BOUNDS_FIX.tsv has no entry for
any address in this chunk, and no row looked cut wrong.

## Label corrections
- **Family-wide:** retail is STLport. `_Push_heap` is `__push_heap`, `_Sort_heap` is `sort_heap`, `_Adjust_heap` is
  `__adjust_heap`, and the `_Remove_if` rows are the remove_copy_if / remove_copy / __unique_copy halves.
- **Push_heap rows:**
  - 005f7d85, 005f7dd4: an 8-byte pair with a signed key, not pair<unsigned long,CSoundPair>.
  - 00769470: pair<unsigned long,C3DVector>. The labelled pair<long,CSkeletalMorph> is the signed twin 0x0086A7F0.
  - 00769e20: pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<ul,ul>> with
    CParticleUpdateCCompareParamDesc, not CTextureMorph.
  - 0086f100: pair<EExpressionType,CExpressionSet::CEntry>, not CTextureMorph.
  - 00a7d6f0, 00a7d7a0: pair<unsigned __int64,CANavQuadTreeNode*>. pair<pair<long,long>,CEngineTextureDecal> is 0x00BEE420.
  - 00992940: pair<long,CSoundBankRecord>. pair<unsigned long,CDefClassInfo> is 0x009B77B0.
  - 00a820e0: pair<unsigned __int64, an 8-byte list (0x00517720 TU)>. CPersistInfo is 0x00CB93E0.
- **Remove_if rows:**
  - 00671be0: `__unique_copy<ENavigatorType*, equal_to>`, not `_Remove_if<C3DVector*, unary_negate<...>>`.
  - 00737ad0, 0074ce40: remove_copy over CIntelligentPointer<const CThing> and CIntelligentPointer<CThingCreatureBase>
    (8-byte stride), not the pair remove_if.
- **Sort_heap rows:**
  - 00777ee0: `sort_heap<long*, less<long>>`. It compares signed integers, so it is not float*.
  - 004d15a0: `sort_heap<pair<float,EConversationAttitude>*, CKeyPairCompareLess<float,EConversationAttitude>>`, not
    pair<long,unsigned long>.
- **Callees:**
  - 0x00776750 is `__push_heap<long*>`, not float*.
  - 0x00A586D0 is `__push_heap<float*>`, not pair<float,CCharString>.
  - 0x0067E8F0 is `__adjust_heap<CDegradableInfo*>` and 0x008175D0 is `__adjust_heap<THitLocationIntersectionElement*>`;
    both are labelled CDebugBox.
  - 0x0080B430 is `__pop_heap_aux<CTargetPoint*,CTargetPoint,CThingCompare_BestForTargeting2>` (labelled FUN_).
  - 0x006A7380 is `CThingFilter_IsInFOV::operator()(const CTargetPoint&)`, a 16-byte forwarder of
    `&tp.TargetingPosition` to 0x006A7270 (labelled `_Cons_val<...EHeroMorphType...>`).
