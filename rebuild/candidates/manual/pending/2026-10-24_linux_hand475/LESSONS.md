# Chunk 475 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU noinline body of an out-of-line member that takes a local's `this` (here vector::operator= 0x00669BB0, in the
  same TU) makes VC7.1 treat local vectors as uncaptured. Inline push_backs then cache `_M_finish` in edi/ebp with
  write-back, as retail does (0x006668A0: 272 to 185 diff lines). Rule 24 extends from address-taken scalars to `this`
  arguments.
- A STLport `pair` returned by value (set::insert) needs a user constructor to return through the hidden pointer;
  without one VC7.1 returns it in edx:eax (0x0065CE9B).
- A member the PDB mangles `UAE` must be declared `virtual` in the model, even when the call site is direct
  (0x0065CE9B CTCVillage::OnSerialise).
- In the /Os frame-pointer CTCVillage TU, register ties follow use weight. A `const` pointer local
  (`const CGameTimeManager* tm = PeekWorld().PGameTimeManager;`) flipped GetTimes' ebx/edi tie. A float declared after
  the zero stores (`best`) is homed in the dead reference-argument slot (0x00658716).
- A stack CMemoryBuffer (inline `char Data[0x20]`) puts the /GS cookie on CWorld event actions; grade them with
  `// Flags: /GS` (0x00629DE0, 0x0062CE60). A probe script that drops the flags reports a false DIFFER.
- A local `vector<CWideString>` destructor inlines (`_Destroy` loop + free) only as `__forceinline ~vector()`; plain
  inline leaves it a call (0x0062CE60).
- A named class local passed by const reference pushes `lea reg,[slot]` after its constructor; a temporary pushes the
  constructor's returned eax, and VC evaluates the other arguments first (0x0062CE60 `CWideString name(names[i])`,
  0x006668A0 `CCharString defaultName("DEFAULT")` in a block).
- The one-ternary out-param GetInterface (`*out = (T*)((it==last || t<it->type) ? last : it)->component;`) loads the
  component straight into ecx. The if/assign and returned-pointer forms route it through eax (0x0062CE60).
- STLport map iteration that retail inlines (`_M_increment` expanded) needs `__forceinline` on the iterator's
  `operator++`. An early `return` inside the scope of a counted pointer gets its own release + epilogue copy. A plain
  fill loop over two pointer locals becomes `rep stosd/stosb` (0x00681E60).
- VC7.1 reassociates float products without /Op: `-(3.0f / D * t)` compiled as `t / D * 3`. Write `-(t * (3.0f / D))`
  for retail's `fld 3; fdiv D; fmul t; fchs`. Named float locals (`wave = sin(..)`, `decay = exp(..)`) pin the
  sin-then-exp order (0x00698540).
- `sin(2.0 * PI * x)` with a float PI is retail's `fmul qword [(double)(float)2pi = 6.2831854820251465]`. The qword
  constant shows the double literal times a float constant (0x00698540).
- STLport `vector::erase(it)` over a non-POD 48-byte element is the `__copy` loop of `mov ecx,0Ch; rep movsd` with the
  signed /48 magic `0x2AAAAAAB; sar edx,3` (0x00698540).
- A global int divided inside a loop that contains a call stays `fidiv [g]` per iteration. Retail's
  `mov eax,[g]; mov [tmp],eax` before the loop + preheader `fild [tmp]` means a local copy made before the loop
  (0x00698540; the copy's slot packing did not reproduce).
- `ReleaseTemporary()` on a ref-counted pointee (DecRef(), then `GetRefCount() == 0`) produces a dead-looking reload.
  It looks like a volatile member and is not one; try it before parking a "volatile candidate" (0x00699BF0).

Review at landing:
- Rows over 4 grades: none. 00619a10 and 006726b0 used exactly 4. 006668a0 and 00698540 used 1 grade each after many
  free cmp.py probes (probe/gp*.cpp, probe/uq*.cpp).
- 005fddd0 and 0062fe60 are STATUS `SKIP` = not attempted (single functions, not two-function rows); they need a fresh
  attempt.
- Local models sit next to existing engine headers: CGameCameraManager (rebuild/include/engine/CGameCameraManager.h
  carries Ego_r offsets; retail EarthQuakes is +0xEC, Camera +0x04) and the CThingCreatureBase / CTCVillage slices are
  local retail-layout models.
- Already-landed source: EASkipCutScene names its +0x5EC field "TextBoxButtonASoundCriteria"; retail +0x5E8 is A and
  +0x5EC is B.
- calls.py shows propagated coverage names for callees (e.g. 0x49D870 "GFGetPreMainMemoryUsed" = GFGetCurrentFrame,
  0x686A80 "GetDisplayEngine" = CTCBase::PeekWorld, 0x4107C0 "GetCellBoundingBox" = CPersistContext::Transfer(bool&)).
  Fix them at label time.
