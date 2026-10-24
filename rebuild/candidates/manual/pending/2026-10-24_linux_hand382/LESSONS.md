# Chunk 382 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CPlayerGuiDef retail offsets are PDB - 0x30 from about +0x48C to +0x4E0: twelve STLport vectors before them are 12
  bytes in retail, not 16. SmackEffect is at +0x48C, MoneyTextOffset at +0x4AC, StatBarsFontName at +0x4E0
  (0x643D8C, 0x646A46, 0x65431D). The CUIStateDef def base is also 0x3C, not the PDB's 0x40 (0x625630).
- A fixed 4-entry table loop: with `unsigned int i`, retail walks an index and uses `jb`. With `int i`, VC
  strength-reduces to a pointer and uses `jl` (0x63B750 / 0x63B7F0).
- A by-value C2DBoxI copy in retail's load order needs a memberwise USER copy ctor. A named reference to a call result
  (`CMap& map = GetMap(*it);`) orders that call before the next call's argument pushes (0x63E2E0).
- A by-value class temporary (here a 0x40-byte CPhysicsMeshObject) inside an `&&` chain gets a conditional-destruction
  flag byte. Fix: move it into a `__forceinline` helper with a NAMED local; the named local also orders the argument
  pushes after its construction (0x63F150).
- Filter functors called from another filter's operator() stay out-of-line calls unless they are `__forceinline`
  (0x63F1E0, 0x63F2C0). VC7.1 does not merge two identical virtual calls in the two arms of a ternary (0x63F150).
- In the NPlayerGui size TU (`#pragma optimize("s", on)`), a `__fastcall` function returning C2DVector/C2DBoxF takes the
  hidden result in ecx and the argument in edx. The (x, y) ctor arguments evaluate right to left, so Y is computed and
  parked first (0x643CA1, 0x643BFC).
- An inline `operator+` computes X first and shares its temporary's frame slot. When retail computes Y first, build a
  NAMED local with the (x, y) ctor and per-coordinate sums (0x646A46).
- `if (a || b || c || d) return true; return false;` gives retail's 8-bit `xor al,al` / `mov al,1` exits; returning
  the `||` expression widens both to eax (0x637F40).
- STLport `equal_range` with both walks inlined runs the upper_bound walk first, because the pair ctor arguments
  evaluate right to left (0x639250).
- `/Oa` (`// Flags: /Oa`) keeps a member load cached across a call (`info->PThingMap` in edi). Without it VC reloads
  after the call (0x6384B0, still DIFFER on an ebx/ebp swap).
- Re-confirmed: a same-TU `__declspec(noinline)` callee body under `#pragma auto_inline(off)` lets VC keep a caller-saved
  register live across the call. Here `def` stays in edx across the CStatBase ctor call (0x65431D).
- Re-confirmed: a dword `fld ; fistp` rounding in an ebp frame is retail inline asm. C++ always gives `__ftol2`
  (0x643B9F: a `retail_asm` candidate).

Review at landing:
- 0x6384B0 needs `// Flags: /Oa` for its near-match (the differ/ source carries it).
- 0x643B9F GFGetGraphicFrame is a candidate for the `retail_asm` grade (bare `fistp dword`) under the 2026-10-24 owner
  exception; this chunk was barred from `__asm`.
- 0x63F150 / 0x63F1E0 class names are descriptive (no PDB thing filter tests IsPointContainedInBoundingBox).
  0x63F1E0's layout (IsNot by pointer, the inner test by value) contradicts the PDB CFilter_Bind2 {F1*, F2*}, so it is
  not a Bind2.
- Landed 0x62F650: its element type should be CCountedPointer<CStrikeEvent> (it is the sibling of the 0x62F610 list).
- Landed 0x431020 (CPersistContext::Transfer, catalog "UpdateShadowScene") model is garbage; same for the landed
  0x655B69 (list<CChangeText> ctor) and 0x438010 (CGameUI::Draw2DText) models.
- The CRetailTree operator= rows 0x631220 / 0x6312E0 are STLport, not VC6.
- Landed 0x64CC11's note calls 0x65431D "the sibling copy". It is wrong: 0x65431D is CStatBar's ctor, which calls
  0x64CC11 (CStatBase's ctor).
- Landed FindLastUnblockedThemePosition (0x63DB30) declares a non-PDB `FindMaxThemeMinCameraZInCell`. The real callee
  is FindMaxThemeZInCell 0x63B7F0. The landed 0x63A7E0 is likely GetThemeSizeZAtBlock.
- 0x9E1DA0 is named GFAXToVX in one landed file and GFVXToAX in another; settle one.
- Callee label corrections (not chunk rows): 0x437990 `_Cons_val` is the CGameUI AddHitByMelee forwarder;
  0xA12890 `BeginInputLoading` is CPhysicsMeshObject::IsPointContainedInBoundingBox; 0x4294E2 is
  `_Rb_global::_M_decrement`.
