# Chunk 385 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **PeekTC with two lookups:** write `Find(long type)` as `LowerBound((ETCInterfaceType)type)`. The conversion
  temporary gives both inlined lookups one shared `push ecx` key word, and the compare keeps the immediate
  (`cmp [eax],46h`).
  - A named key local per lookup gives `sub esp,8`.
  - A same-type `ETCInterfaceType(type)` binds as an lvalue and does not help.
  - Examples: 006a15a0, 006a1620, 0069f3e0.
- **Int-wide bool:** `return PeekTC(&x) && !x->User.GetPItem();` (and `a == b || a == "NULL" || a == ""`) gives
  `mov eax,1` / `xor eax,eax` (006a8af0, 006a7c60).
- **iter_swap leaf:** the substring leaf `iter_swap` also matches `__iter_swap`. Use `?iter_swap@std@@` and write the
  swap body in iter_swap itself; a separate inline __iter_swap template stays out of line (006a2d40).
- **STLport list iterator:** it needs the declared copy ctor `iterator(const iterator& x) : _M_node(x._M_node) {}`.
  With the implicit one VC7.1 shrink-wraps the edi push into the node-copy block (006a4a70 / 006a4ab0).
- **Nested pair value in an _Rb_tree:** a 20-byte value `pair<const K, pair<C3DVector,long> >` must copy implicitly.
  A declared memberwise copy ctor on both levels stops _M_create_node inlining, and forcing it walks the inner pair
  through advanced pointers (006ae900).
- **STLport `less<T>()` zero fill:** the `push ecx ; push 0 ; mov byte ptr [esp+4],0` comparator needs `less<>`
  WITHOUT the binary_function base. Then `less<_Tp>()` is POD and VC7.1 zero-fills the byte; with the empty base it
  passes a dead word (006b5a50).
- **Empty tags:** a POD empty tag (`struct __false_type {};`) passed by const& gets a `mov byte ptr [esp+N],0` zero
  fill. Give the tags a user ctor (006b5ed0).
- **vector::insert shift:** `__copy_backward_ptrs` must be `__forceinline`; plain inline stays a call (006b5ed0).
- **Two locals in one dead argument slot:** two disjoint-lifetime locals (a CDefPointer out slot and a pushed value)
  share one dead argument slot only when every same-TU stand-in keeps their addresses from escaping.
  - The GetDef stand-in only writes `*out`.
  - The `_M_insert_overflow` stand-in passes `__x` by value to its extern, never `&__x`.
  - Escaping `&__x` gives the value its own `push ecx` word (006a22d0, 164v155 before the fix).
- **Float compare against a call result:** the call result must be a NAMED float local
  (`float d = F(...); if (d < r * r)`) to get `fld r ; fmul r ; fxch st(1) ; fcompp ; test ah,5 ; jp`.
  - Written inline, VC flips it to `r*r > d` (`test ah,41h ; jne`, no fxch).
  - `r * r > F()` evaluates r*r before the call and spills it (006a7050).
- **CEnvironment retail offsets:** the retail CArray is 12 bytes; PriorityStackChannels is at +0x48 with 12-byte
  stacks of 0x30-byte entries; GameTickState is at +0x54 and PrevGameTickState at +0xc74 (0xc20 bytes each), with
  DiffuseVector at +0xbf4. The donor header (16-byte CArray, 0xcb0-byte states) does not fit
  (006b3880 / 006b3b30 / 006b3460).
- **Inlined member in a static:** inlining a member whose first test reads a flag the caller just cleared folds the
  test away. ConsoleRestoreDefaultTheme is `clear ; TransitionToDefaultTheme(args...) ; set` (006b3b30).
- **Template-function leaves:** a function-template instantiation mangles as `??$name@...`, so a
  `?name@std@@` leaf matches no symbol and q.py silently grades the largest function. Use `??$lower_bound@` (it
  does not match `??$__lower_bound@` / `??$__less@`). This was caught by objdump on 006b5a50; the grade was the same
  only because the TU defines one real function.
- **GFSmoothTowards<float>:** `??$GFSmoothTowards@M@@YIXABM0AAM@Z` is __fastcall. A plain if / else-if clamp plus a
  sign-selected overshoot test matches as written (006adc20).

Review at landing:
- 006a6290: engine/CThingPhysical.h puts HealthOverTimeEffects at +0xa8; retail reads the list at +0x98. The source
  uses a local model. Regenerate or reconcile the header.
- 006b3880 / 006b3b30 / 006b3460: engine/CEnvironment.h (donor sizes) does not fit retail: the CArray is 12 bytes,
  not 16, and the game-tick state is 0xc20 bytes, not 0xcb0. All three use local models with a `// Layout:` line.
- 006a7050 / 006a7c60: retail CThing has PhysicsTC at +0x60 and SerialisationSectionName at +0x58 (donor +0x70 /
  +0x68). Both use local CThing models.
- Already landed: 0x006A8B40 is labelled CThingFilter_HasFreeActionPoint but tests type 0x2d and
  IsActionPointFree, and 0x006A8A80 carries the same label. One of the two is mislabelled.
- Already in rebuild/src/compiled: CTCCarrying_IsSlotPrimarySlot_006a1330.cpp has a "Tried ... (62v60) / Next ...
  else lane gap" note and no grade line, and 006a1330 is not in ARTIFACT_INDEX.tsv / COVERAGE.json. Check whether it
  really matches. Its dtor-with-count-local release form matched here inside 006a22d0, which suggests it does.
- Over the 4-grade budget:
  - 006a1270 took 5 grades; the fifth was a register-swap retry before parking.
  - 006a15a0 / 006a1620 took more than 4 grades (the PeekTC key-form search that found the conversion temporary).
  - 006ae900 and 006a4a70 needed 3-4 grades.
- engine_api.tsv still names 0x00A14440 "GetScaled" (corrected to C3DVector::Normalise by several chunks) and
  0x00A442E0 "Vector3_ClosestPointOnLineSegment" (it is the squared distance to an infinite line).
