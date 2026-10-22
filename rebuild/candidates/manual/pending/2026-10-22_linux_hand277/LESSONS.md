# Chunk 277 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Implicit literal conversions are built first.** A string literal passed to a `const CCharString&` parameter as an
  implicit conversion builds its temp before VC evaluates any other argument. An explicit `CCharString("...")` temp is
  built in argument order, with the vtable/ctor result pushed. Choose the form from where the ctor calls sit. (0x00E369B0
  uses implicit conversions; 0x007AD300 uses explicit temps.)
- **Name the target before an intelligent-pointer assignment.** `CThing* player = GetCreature(); CTCDiceGame* game =
  DiceGame; game->CurrentPlayer = CIntelligentPointerThing(player);` loads the target pointer before the
  intelligent-pointer temp is built. It also fixed a zero/result register swap (0x009285D0).
- **DIFFER with 0 diff lines: compare the raw bytes.** A float literal can differ only in the constant pool.
  0.0027777f is 0x3B360A13, while 1/360 is 0x3B360B61 (0x0095E900; the same constant appears at 0x0093B3E4).
- **A structured `for(;;)` can rebuild a goto layout.** A `for(;;)` with explicit `first`/`moving` bool flags reproduces
  the retail block order (0x0095E900).
- **Use the mask form of HasTC to get the bit test.** `UsingComponents[t>>5] & (1UL<<(t&31))` gives
  `test ch,1` / `test dword`; the shift form gives `shr/and` (0x00920A00, 0x0095E900).
- **Some small operators must be forceinline.** In some TUs VC calls the C3DVector operators and the
  `CIntelligentPointer(CThing*)` ctor out of line until they are marked `__forceinline` (0x00920A00).
- **Write a "repeat until good or out of tries" loop as a `while`.** `while ((A || B || C || D) && tries > 0)` gives the
  retail fall-through loop entry and `test ah,41h; je` (0x00912840). A `for(;;){ if(ok) break; if(tries<=0) break; }`
  rotates the loop (extra jmp) and gives `jp`.
- **A scoped seed local puts the seed call first.** `CRandomSeed* seed = GetPWorldSeed();` in its own `case {}` scope,
  before `GFFloatRandom(seed, range)`, calls GetPWorldSeed before the range is pushed. A forceinline `Random(const
  float&)` wrapper did not (0x00912840).
- **An inline GetCreature() loads PBrain early.** `GetCreature() { return PBrain->Creature; }` makes VC load PBrain
  before the probe-vector pushes and Creature right before the call. An inline cast of `PBrain->Creature` loads both
  late (0x00912840).
- **Operand order of a facing + position sum.** `creature->GetPosition() + physics->GetFacing()` calls GetFacing first
  (its temp lands in esi) and emits `fld facing; fadd pos`. Naming the facing makes a frame copy (0x009130D0).
- **A spilled operand must be named first.** A member loaded before a call and spilled (Radius before the slot-83
  call) needs a named local declared before the expression (0x009130D0).
- **`path.size()` belongs inside the loop.** Written in the inner loop's condition expression, VC hoists it past the
  loop guard, as retail does. Named before the loop, it is computed before the guard and interleaved (0x009130D0).
- **STLport deque pop_front frees with a null test.** `if (first) free(first)` before `_M_set_node(node+1)`. Write the
  call as `free()`, which is the 0xBFEA14 thunk (0x009130D0).
- **Byte-vector staging buffer: zero `first` last.** In the ConvertToVBFormat family, declare the vector before
  `convertedFormat` and zero last/end before first. Use a named `SNonscalarPtrIteratorTag` local in the inlined
  CStreamInfo array dtor. Together these put the resize zero byte in the retail slot (0x00A91180, sibling of the landed
  0x00A92740).

Review at landing:
- 0x00920A00, 0x0094CB40, 0x00A8F9E0 and 0x009130D0 each needed more than 4 grades and are parked.
- 0x0088BF30 uses the advisor's out-only `_M_insert_overflow` stand-in.
- The 0x00912840 and 0x009130D0 drafts model the STLport `deque<CBezierCurve>` locally: a 0x60 buffer holding 2
  curves; `_M_initialize_map` 0x9137F0, `_M_push_back_aux_v` 0x913D60, dtor 0x913CC0. No engine header exists for it.
- **Brain field offsets.** engine/CAIBrain.h gives the debug-build `PCurrentUpdateZone` +0xa8 and
  `InitialisedCreaturePosition` +0x93. Retail uses +0xa0 and +0x8f (0x0088BCB0, 0x009130D0), so the header cannot model
  the retail brain fields.
- 0x0097EC00 and 0x0097E890 are advisor-landed (batch a3). They stay in differ/, not src/.
- No catalog row in this chunk holds two functions.
