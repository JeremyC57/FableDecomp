# Chunk 394 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU stand-ins tell VC7.1 that a reference does not escape.** Write a same-TU `#pragma auto_inline(off)`
  `__declspec(noinline)` stand-in for an out-of-line callee that only reads or writes through its pointer/reference
  arguments. VC7.1 then:
  - keeps the pointee in a register across later calls (no reload);
  - homes empty tags and `const T&` literal temporaries in dead argument slots;
  - picks retail's frame-slot order.

  Examples: 0x0076D4F0 (CalculateEnemyTC out-pointers), 0x0077A350 (`vector<long>::_M_insert_overflow`: the `1L` and
  tag temporaries in the dead `total` slot), 0x0076D1B0 (`__find`). The stand-in must not leak the address: a body that
  passes `&value` on to an extern still causes the reload; a body that only dereferences it does not.
- **Two textually identical sums become a CSE pair**, not one local (0x00778610). VC7.1 computes the sum once, spills
  it, copies it to a second slot through eax and pops the dead shared subterm (`fadd st(1)` ... `fstp st(0)`). Writing a
  single local plus a copy coalesces into one slot.
- **Repeated subexpressions show up as `fld st(0)` and scattered spills** (0x0076DEB0). An x87 `fld st(0)` duplicate with
  spills to non-adjacent slots means the source wrote the same subexpression twice (`offset * 0.5f` in both pair
  arguments). A named `half` local gives a different frame (0x50 against retail's 0x54).
- **Constant multiplies inside one expression are folded** (0x00771C70). VC7.1 turns `x / 8.0f * 0.5f` into one
  `fmul 0.0625`. Retail's `fmul 0.125 ; fmul 0.5` needs the division in its own statement (`float fraction = x / 8.0f;`).
- **x87 operand order follows evaluation order** (0x00771C70). For `a + b` of two inline C3DVector products, the right
  operand is evaluated first: retail computed sin before cos, so the source is `E_Y * cos + E_X * sin`.
- **An inlined accessor with two tail-calling arms must be written with `?:`** (0x0076EA60). Written as
  `return f(i >= 0 ? A[i] : A[0]);` it compiles standalone to the same bytes as two arms (0x0076E5F0 still
  RELOCATION_MATCHes). Inlined into a loop, only the `?:` form gives retail's `jl ; lea` select feeding ONE call; the
  two-arm form duplicates the call.
- **A null returned from ebx across calls is a result local** (0x00779E90). `CThing* result = 0; ... if (ok) result =
  thing; return result;` gives `xor ebx,ebx` ... `mov eax,ebx`. `return thing` / `return 0` arms give `xor eax,eax`, and
  early returns give `neg al ; sbb ; and`.
- **`return a && b;` from two named bool results** gives `mov eax,1` / `xor eax,eax` (0x0076D4F0). The early
  `return false` exits in the same function stay `xor al,al`.
- **A user C3DVector copy constructor is needed throughout the weapon-trail TU** (0x0076FD40, 0x0076BE30, 0x0077E0C0,
  0x007700D0, 0x0076DEB0). Without it the compiler emits `rep movsd` or pointer-block copies instead of in-order dword
  moves, or different x87 spills.
- **A call that retail treats as never returning is `__declspec(noreturn)`** (0x00777DB0). For an out-of-line
  `_M_throw_length_error` after which retail assumes ecx is still live and shrink-wraps the pushes, declare it
  `__declspec(noreturn)`.
- **`__forceinline` is needed where VC7.1 declines to inline.** `__adjust_heap` inside `__make_heap` (0x007772F0) and
  SGI `__copy` inside `vector::erase` (0x00770790) must be `__forceinline`; plain `inline` leaves a call.
- **The default list constructor in `resize(n, _Tp())` must not zero the proxy** (0x00770D60). Use
  `_M_node(allocator<_Node>())`; STLport's `_M_node(a, 0)` leaves a dead `mov [esp+0Ch],0`.
- **For an empty-bodied `for` over a map, write the walk exactly** (0x007773A0 CShopDef::Test). Two
  `for (it = m.begin(); it != m.end(); ++it) {}` loops with the inline `_Rb_global<bool>::_M_increment` match on the
  first grade. Compiled-out per-entry checks leave exactly this shape.

Review at landing:
- 0x0076E5F0 (landed CTCWeaponTrail::GetLastWeaponTrailPoints) uses the two-arm form. The `?:` form also grades
  RELOCATION_MATCH there, and it is the only form that inlines correctly into 0x0076EA60. Consider unifying the model.
- 0x00770D60's list model (proxy-only default constructor) differs from 0x007702B0's. Both are needed as written; check
  this if they share a header.
- The local class models below stand in for missing engine headers. Only CTCWeaponTrail / CTCEnemy have landed local
  models; the rest have no header in rebuild/include/engine.
  - CTCEnemy (0x0076D4F0, parked 0x0076D1B0)
  - CTCSolidToThing (0x00770DE0)
  - CShopDef (0x007773A0)
  - CTCHeroPodium (0x00771C70)
  - CTCDExperienceOrb / CExperienceDef (0x00779E10, 0x00779E90, 0x0077A350)
  - CCamera / CGameCameraManager (0x0076DEB0)
- Descriptive names with no PDB symbol found: 0x007720D0 `MoveTowards` and 0x0077C360 `CLoopedRange::GetFraction` (the
  caller is the CTCGraphicAppearanceSprite slot 0 at 0x0077C4D0).
- 0x00771C70 has no direct call site (its users inline it). It sits between CTCHeroPodium functions, and FableWin.pdb
  names `?GetPositionForMember@CTCHeroPodium@@ABE?AVC3DVector@@J@Z`.
- 0x0076DEB0 is a file-local (anonymous-namespace) helper. The source keeps it emitted with an exported caller,
  `CallGetProjectileTrailElementFromPoint`, which is not part of the graded body.
- No row needed more than 4 grades. 0x00779E90 and 0x0076DEB0 matched on their fourth.
