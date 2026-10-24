# Hand chunk 359 (_Move_backward<pair<ulong,CDefClassInfo>> .. _Push_heap<const CAnimationEntry**,CTurningAnimCompare>): 36 of 40 byte-exact

All 36 sources grade MATCH (3) or RELOCATION_MATCH (33) in `grades.txt`. Every test prints `<addr>_TEST PASS` under g++
(`tall.sh`) and VC7.1 (`tall.py`). Every row is an STLport/SGI helper or a small constructor. Almost every row label is
propagated across byte-identical bodies, and most of them are wrong (see Label corrections).

## What matched (36)

- **std::pair constructors (9):** 0x00592DFB / 0x005F8578, 0x00592962, 0x005921A4, 0x005927F4, 0x00695250, 0x0065470C.
  These are explicit instantiations of a local `std::pair` with STLport's memberwise copy constructor and two-argument
  constructor.
  - `CDefPointer` add-refs through a `mutable` count at +4 of the def. It stores the pointer before its null test.
  - The /Os modules (`push [esp+N]` pass-throughs, unaligned entry points) need `#pragma optimize("s", on)` to keep the
    key load/store pair together.
  - 0x0065470C also needs C3DVector's memberwise user copy constructor. With an implicit copy constructor the vector is
    copied with `movsd`.
- **std::make_pair (2):** 0x00695B70 `<long,CCharString>` and 0x0056CB40 `<unsigned long,CAnimationEntry>`.
  - These are cdecl, with the hidden result as the first stack argument: an inline template plus an explicit
    instantiation.
- **Constructors (2):**
  - 0x0064A4E9 `NPlayerGui::CQuestInfoElementTimer(const CTimer*, long, float)` needs /Os. `Graphic` must be in the
    initialiser list and `TimerIndex = timer->TimerIndex` in the body, which puts the Graphic store before the vtable
    store.
  - 0x00DC2570 `NScript::CQ_OpeningGraveyardSecretPassageScript(void*, CGameScriptInterface*)` uses the landed
    CScriptBase / `__declspec(novtable)` CQuestScriptObject model, plus four default-constructed `hero_object`
    CCharStrings at +0x58.
- **Out-of-line `__pop_heap` (9):** 0x004BABF0, 0x004D0B50, 0x005F8D4D, 0x005F8D82, 0x00808BD0, 0x0081B910, 0x009E7060,
  0x00A32B50, 0x00CB6B00. The body is `*result = *first; __adjust_heap(first, 0, last - first, value, comp);` and it
  destroys the by-value `value`.
  - The record's copy-constructor style decides the argument-area copy. Memberwise gives scalar moves.
    `{ *this = other; }` gives `add eax,4` followed by a block copy (0x0081B910) or `rep movsd` (0x004D0B50).
- **`__pop_heap_aux` (7):** 0x00471150, 0x00471576, 0x004D0D60, 0x0076AC70, 0x00808DB0, 0x009B6D80 / 0x00A41260.
  The body is `__pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, (long*)0)` with `__pop_heap` inlined.
  - In the /Os TUs `__pop_heap` needs `__forceinline` (0x00471576, 0x00471150).
  - 0x00471150 is a /Os TU with a frame pointer (`push ebp ; mov ebp,esp ... leave`). It needs
    `#pragma optimize("y", off)` + `("s", on)`.
  - 0x0076AC70's element holds a polymorphic CParticleUpdateParamDesc (CBaseClass base). Its temporary becomes a frame
    local and the implicit destructor reduces to a `~CBaseClass` call. Implicit assignment skips the vptr.
- **`__copy_backward` (2):** 0x009B7870 `pair<ulong,CDefClassInfo>` and 0x006FB830 `pair<ulong,CSmallVector<CEvent,32>>`.
  The body is `for (n = last - first; n > 0; --n) *--result = *--last; return result;`. The trip count is homed in the
  dead `result` slot.
  - In 0x006FB830 the small vector's inline operator= must be written `if (&other != this) Copy(other);`: the operand
    order shows in `cmp edi,ebx`.
- **`__insertion_sort` (1):** 0x0077ECB0 over `pair<CCharString,C3DVector>`. The loop is
  `for (i = first + 1; i != last; ++i) __linear_insert(first, i, T(*i), comp);` with the element built in the argument
  area.
- **`__destroy_aux` (2):** 0x00519EF0 / 0x00A83420 over `pair<unsigned __int64, std::list<C2DLineF>>`. The STLport list
  destructor is inlined: `clear()` frees every node unconditionally and relinks the header, then `if (node) free(node)`.
- **`std::swap` (3):** 0x00A331B0 / 0x00A70320 `pair<long,C3DMeshVolume>` and 0x00A70490 `pair<CCharString,C3DMeshVolume>`.
  The body is `T tmp = a; a = b; b = tmp;` with the landed C3DMeshVolume / CCountedPointer model. The volume's
  operator= is out of line at 0x00A32960.
- **`__push_heap` (1):** 0x0087DCA0 `<const CAnimationEntry**, int, const CAnimationEntry*, CTurningAnimCompare>`.
  - The functor rounds each `fabs` into a float local, which produces the `fxch st(1) ; fxch st(1)` pair.
  - Retail quirk: both `GetRotationInAnimation` calls use the left operand, so the comparison is always false and the
    sift never moves. The source reproduces this.

## Parked for Fable 5.1 (4)

- 004c3bf0 _global::__pop_heap_aux DIFFER(103v103): retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier (prologue scheduling only) | tried: pair<ulong,CTextureMorph> memberwise (86), CTextureMorph `{*this=o}`, memberwise CTextureMorph, 12-byte flagged record `{*this=o}` + inline __pop_heap (103, push ebp only) | next: lane gap: prologue scheduling (same bytes otherwise; byte twins 0x004C3C60 / 0x00ADB4D0 differ identically, so one shared source form or compiler build)
- 004c3c60 _global::__pop_heap_aux DIFFER(103v103): retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier | tried: STLport memberwise pair copy (86, no frame), pair copy constructor `{ *this = other; }` (103, push ebp only) | next: lane gap: prologue scheduling (same as 0x004C3BF0)
- 00adb4d0 _global::__pop_heap_aux DIFFER(103v103): retail `push ebx ; lea eax,[edx-0Ch] ; mov ebx,eax ; push ebp`, ours pushes ebp one slot earlier | tried: 12-byte record `{*this=o}` + `__forceinline` __pop_heap (103), a named `back` local in place of the temporary (104) | next: lane gap: prologue scheduling (same as 0x004C3BF0)
- 0076a590 _global::__pop_heap_aux DIFFER(85v97): retail keeps a 16-byte frame (`sub esp,10h`) and spills the temporary's third member early (`mov [esp+0Ch],eax`, read back as `mov esi,[esp+28h]` for the argument copy) without touching ebx; ours keeps it in edi, saves ebx and copies the root through ebx | tried: plain `inline` __pop_heap (55: call left out of line), `__forceinline` + implicit record copy (103: int moves), `__forceinline` + memberwise CCacheGroup copy (85: x87 float copies match), CNavigationPosition-shaped {C2DVector memberwise, long} (85, same) | next: make the third member a 4-byte struct type (a struct copy is not a register candidate) or find the real record (the bytes fix only {float, float, dword}; CCacheGroup is the one Ego_r.pdb fit)

## Skipped

None. Every row holds exactly one function: the int3 padding after `ret` ends each body.

## Label corrections

These are propagated manifest labels that the bytes contradict.

| Address | Label | Actually |
|---|---|---|
| 0x0077ECB0 | `_Move_backward<pair<ulong,CDefClassInfo>>` | `__insertion_sort<pair<CCharString,C3DVector>*,CKeyPairCompareLess<CCharString,C3DVector>>`. Its callee 0x0077E960 is `__linear_insert`. |
| 0x00519EF0, 0x00A83420 | `_Move_backward<pair<ulong,CSoundPair>>` | `__destroy_aux<pair<unsigned __int64,std::list<C2DLineF>>*>` (16-byte element, STLport list at +8) |
| 0x009B7870, 0x006FB830 | `_Move_backward` | The element types are right, but the functions are STLport `__copy_backward`. |
| 0x0064A4E9 | `_Pair_base<CCharString>` | `NPlayerGui::CQuestInfoElementTimer::CQuestInfoElementTimer` (vtable 0x0125A458, base constructor 0x00656795) |
| 0x00DC2570 | `_Pair_base<CCharString>` | `NScript::CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript` (vtable 0x012DA11C, factory "S_QOGSP" 0x00DCBFD0) |
| 0x004D0B50 | `_Pair_base<CDateAndTime>` | `__pop_heap` of a 28-byte keyed record |
| 0x0056CB40 | `_Pair_base<CComponent*>` | `std::make_pair<unsigned long,CAnimationEntry>` |
| 0x00695250 | `_Pair_base<CComponent*>` | `pair<long,CCharString>(const long&, const CCharString&)` |
| 0x00695B70 | `_Pair_base<CComponent*>` | `std::make_pair<long,CCharString>` |
| 0x005F8D4D, 0x005F8D82 | `_Pop_heap<pair<C3DVector,long>>` | `__pop_heap` of an 8-byte `pair<long, 4-byte>` (/Os) |
| 0x0081B910 | `_Pop_heap<pair<ulong,CSoundPair>>` | `__pop_heap` of a 24-byte keyed record |
| 0x00A32B50 | `_Pop_heap<pair<ulong,CSoundPair>>` | `__pop_heap<pair<unsigned long,C3DHelperPoint>>` |
| 0x009B6D80, 0x00A41260 | `_Pop_heap_0<pair<CCharString,C3DHelperPoint>>` | `__pop_heap_aux<pair<CCharString,unsigned long>>` |
| 0x00A70490 | `_Pop_heap_0<pair<CCharString,C3DMeshVolume>>` | `std::swap<pair<CCharString,C3DMeshVolume>>` |
| 0x00A331B0, 0x00A70320 | `_Pop_heap_0<pair<long,C3DMeshVolume>>` | `std::swap<pair<long,C3DMeshVolume>>` |
| 0x00471150 | `_Pop_heap_0<pair<float,CCameraTag>>` | `__pop_heap_aux<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>>` |
| 0x004C3C60 | `_Pop_heap_0<pair<float,CCameraTag>>` | `__pop_heap_aux<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>>` |
| 0x004C3BF0 | `_Pop_heap_0<pair<float,CCameraTag>>` | `__pop_heap_aux` of a 12-byte record ordered by bit 0 of +8 |
| 0x00ADB4D0 | `_Pop_heap_0<pair<float,CCameraTag>>` | `__pop_heap_aux` of a 12-byte record keyed by the signed word at +4 |
| 0x004D0D60 | `_Pop_heap_0<pair<long,CRegisteredMusicEntry>>` | `__pop_heap_aux<pair<float,CCharString>>` |
| 0x0076A590 | `_Pop_heap_0<pair<long,EHeroAbility>>` | `__pop_heap_aux` of `pair<unsigned long, {float,float,dword}>` |
| 0x0076AC70 | `_Pop_heap_0<pair<long,EHeroAbility>>` | `__pop_heap_aux<pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<ul,ul>>>` |
| 0x00808DB0 | `_Pop_heap_0<pair<ulong,CSoundPair>>` | `__pop_heap_aux<pair<C3DVector,long>,CCompareTargetedPositionsForFirstPersonTargeting>`. It calls the out-of-line `__pop_heap` 0x00808BD0. |

The `_Adjust_heap` callee labels are wrong too:

| Address | Label | Actually |
|---|---|---|
| 0x004C2C70 | `_Adjust_heap<pair<ulong,CTextureMorph>>` | Tests `byte [+8] & 1`, which a key-pair compare cannot do. |
| 0x00ADB000 | `_Adjust_heap<pair<EExpressionType,CExpressionSet::CEntry>>` | Compares the signed word at +4. |
