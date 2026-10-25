# Chunk 400 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport 4.6 tree lookups** (0x0082E500 CScriptedMapBrush::RemoveThing): `find` and `lower_bound` are inline wrappers
  over out-of-line `_M_find` and `_M_lower_bound` (0x0082EEB0, 0x0082EF20, 0x0082F0D0). Those return the NODE in eax,
  not an iterator. Modelling the callee as returning a real iterator, which has a user ctor, routes it through a hidden
  pointer and gave 216v183.
- **`_STLP_DEFAULT_CONSTRUCTED(_Tp)` in map::operator[]** (0x0082FB90): this is `__default_constructed((_Tp*)0)`, an
  out-of-line `__fastcall` with `ecx = 0` and a hidden return pointer (0x0082F5C0). It is NOT an inline `_Tp()`.
- **`__forceinline` the vector copy loop** (0x0082C980): for a __false_type (non-POD) `vector::operator=`, the
  `__copy` / `__copy_ptrs` loop must be `__forceinline`. With plain `inline`, VC7.1 leaves it as a cdecl call (206v222).
- **`__forceinline __pop_heap`** (0x0081BC90 `__pop_heap_aux`): with plain `inline`, `__pop_heap` stays out of line.
  Retail inlines it for the 40-byte pair but calls it from 0x0081D450 (24-byte pair), so the form is per instantiation.
- **Memberwise user copy ctors on small value types** (C2DExtentsI, C2DBoxI, C3DVector, STLport `pair`) change
  addressing and register choice:
  - 0x0082CB70 / 0x0082CC40: C2DExtentsI's memberwise copy gives the strength-reduced cursor `[reg+N]` stores.
  - 0x008260C0 / 0x00825A80: C3DVector's gives direct `[ecx+N]` stores.
  - The 0x0081B730 .. 0x0081D4D0 heap/partition family: `pair`'s builds every by-value element in the argument area
    (key dword, then `rep movsd` or a through-address copy).
- **An explicit-specialisation declaration keeps a callee out of line** (0x0082CB70):
  `template<> vector<MinimapTheme>::vector(const vector&);` keeps the MinimapTheme vector copy ctor (0x0082C620) as a
  call while the rest of CMinimapData's implicit copy constructor inlines.
- **A user default ctor on a CReplaceSerialise record flips ConvertPreviousUnknownVersionUp's registers**
  (0x00825590 vs 0x00825700). The brush, whose C2DBoxI has an empty inline `C2DBoxI() {}`, keeps `held` in ebx/ebp.
  The ctor-less height-map cell keeps `result.object` in ebx.
- **Create factories** (0x0082D920 CThingBuilding::Create, 0x00833800 CThingAICreature::Create) need four things:
  - Declare `T* result = 0;` AFTER the allocation (Alloc() / `new (false)`), so the zero lives in ebp and serves every
    zero push.
  - A same-TU `__declspec(noinline)` CTCParamsPhysicsBase ctor body.
  - CRightHandedSet with an assignment-body ctor.
  - C3DVector as a POD here.
- **Same-TU noinline callee bodies pin the caller's registers**:
  - 0x008286A0 CRegionConnectionGraph::SetRegionsAsConnected: the LowerBound 0x00827B00 body keeps the two region ids
    and the flag in ebp/edi/ebx. An extern declaration spills them.
  - 0x0081C270: the uninitialized helpers.
  - 0x0082F630: `__uninitialized_fill_n`.
  - 0x0082F7B0: `_M_create_node`.
- **`_M_insert_overflow` with `fill_len == 1`** (0x0081C270): the `_Construct` is INLINE, a null-tested placement
  copy, unlike the landed CQuake twin 0x0069D300. The `__false_type` temporaries are homed in the `atend` slot.
- **Squared length summed in Z, Y, X order** (0x00833230, 0x008332E0 IsMessageEventWithin{Sight,Hearing}Range):
  `float s = Z*Z; s += Y*Y; s += X*X;` gives the retail `fmul`/`fadd` order. X, Y, Z order is the same length, but the
  operands are swapped.
- **VC7.1 inlines a tree `_M_clone_node` unevenly** (0x008260C0 `_Rb_tree::_M_copy`): it keeps the first
  `_M_clone_node(x)` as a call and inlines the one in the loop by itself. No source lever is needed; verbatim STLport
  reproduces it.
- **A leaf that matches no object symbol silently grades the LARGEST function in the TU** (0x0082CB70): the leaf
  `?__uninitialized_copy@std@@YIPAV...` matched nothing, and the grade still read RELOCATION_MATCH for a different
  function. Use the `??$__uninitialized_copy@PBVCMinimapData@@` template form and check it against `objdump -t`.

Review at landing:
- 0x0082D920 CThingBuilding::Create used the full 4-grade budget. It matched on the 4th.
- Local models sit next to donor-layout engine headers for CScriptedMapBrush (brush_model), CMiniMapDisplay /
  CMinimapData, CRegionConnectionGraph and CThingAICreature's perception call path. They were kept local because the
  STLport 4.6 tree, vector and list layouts must be exact (4-byte list, 12-byte vector). Check them against
  `engine/<Class>.h` before landing.
- 0x0081C270 models the vector in `namespace std` rather than `std`. Check the mangled leaf when landing.
- 0x0081E550 FtoL is a retail_asm candidate, re-authored with the minimal `fistp dword` helper (as
  CSprite_00b1b5e0::GetHeight 0x00B1B5E0).
- 0x0082FB90 `map::operator[]` is parked as scheduling only, 192v192.
- In the already-landed code, 0x00824FB0 is labelled "BuyNode". It is the map's STLport `_M_create_node`.
- No catalog row in this chunk holds several functions.
