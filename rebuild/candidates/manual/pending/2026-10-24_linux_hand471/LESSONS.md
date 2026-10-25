# Chunk 471 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Two divisions by the same converted count. Writing `float r = (float)n;` and then `a / r` twice makes VC7.1 emit one reciprocal and two `fmul`s. Writing `(float)n` at each use keeps retail's two `fdiv`s, which share one conversion slot (0x00C69C60 CReverbProcessor::ProcessInput).
- The biased `/Os` frame (`lea ebp,[esp-N]` ... `leave`) comes from `#pragma optimize("s", on)` alone. Adding `optimize("y", off)` removes it. It also needs a big enough frame: 0x007BCC40 only got it once the acceleration was a named local (0x00650E98, 0x007BCC40).
- A struct copy from an out-of-line ctor's named temporary compiles to `movs` by default. Give the struct an inline member-wise `operator=` to get retail's dword moves; it also lets an inlined `(pos, extents)` ctor be scalar-replaced into the target (C2DBoxF, 0x00650E98).
- Naming the `PeekGuiDef()` result in a local (`long tick = PeekGuiDef()->TickGraphic;`) moves the tick value into edx as retail has it (0x00650E98).
- Pass the result of `IS_THING_ALIVE(CThing* const&)` through a converted temporary (the parameter declared as `CBaseObject* const&`). The CThing* then stays in esi instead of being homed in a frame slot (0x007BCC40).
- A tolerance squared at run time (`fld [eps] ; fmul [eps]`) is a shared `extern const float` global (here 0x0129BA3C), not a literal: a literal `K*K` is folded (0x007BCC40).
- `GetRandomInRange(float low, float high)` as a `__forceinline` member with two float parameters gives retail's copy order; the by-value CFloatRange form copies the struct first (0x007BCC40).
- Converting a float member to `long` for a `__fastcall` argument: put the conversion in a named local before the call (`long accuracy = (long)AccuracyPercentage;`); that fixes the argument order against retail (0x007BCC40).
- A random sign written `GFRandom(1, seed) == 0 ? -1.0f : 1.0f` gives retail's block layout; the `? 1.0f : -1.0f` spelling swaps the blocks (0x007BCC40).
- A zero-velocity fallback written per field (`v.X = 0; v.Y = 0.1f; v.Z = 0;`) avoids a C3DVector temporary plus `movs` (0x007BCC40).
- x87 commutative operand order is NOT steered by source order in these TUs. VC7.1 canonicalises:
  - the caption X sum (0x00650E98);
  - the squared length Y,Z,X (0x007BCC40);
  - the forward length Z,Y,X (0x007BEE40).

  Park these after 1-2 grades.
- A float ternary assigned through a float local gives the x87 `fld`/`fstp` form; assigned straight to a member it becomes integer moves (0x00714900).
- Read a call result before an unrelated member store, or VC7.1 schedules the store first (0x00714900).
- Squared length written `X*X + Y*Y + Z*Z` over an output member is emitted Z, Y, X (0x00714900).
- An inline reference accessor for the physics position fixes register rotation that a direct member chain produces (0x00714900).
- An STLport iterator tag or `__true_type` passed by value needs a user default constructor; otherwise VC7.1 zero-stores the empty temporary (`mov byte ptr [esp+14h],0`) (0x00547380, 0x00B131E0).
- A same-TU `__declspec(noinline)` stand-in for a by-value comparator's `operator()` that does not capture `this` brings back retail's software-pipelined comparator copy in `__adjust_heap` (0x0080AD90).
- In a `copy_if` predicate, `GetPItem` must be its own statement before `GetSubDef(&def)`; the one-line chain pushes `&def` first. The same-TU noinline GetSubDef / `_M_insert_overflow` stand-ins keep the def holder in the dead `dest` slot (0x007B7140).
- libpk assertions are a macro: a block-scoped log line handed to the sink, then the trace temporary, with /EHsc. Reuse the 0x00C4A1A0 model for PKCBufferedReader / PKCTRArray rows.
- A ring walk that ends by setting the index to the count (`mov edi,ecx`) is written that way in the source, not with `break` (0x00C4AC70).
- Lane gap, frame-slot packing: VC7.1 overlays hidden-return temporaries in one slot where retail gives each its own slot. RTM and QFE behave the same, and no scoping or local form changes it (0x0055C6F0, 0x00B17410).
- Lane gap, store-to-load forwarding: retail keeps a destination bit-field word in a register across same-base stores; VC7.1 reloads it for every field (0x00B131E0).
- Lane gap, loop-invariant hoisting: retail builds a loop's zero C3DVector argument temporary once in the preheader; VC7.1 rebuilds it in the loop (0x00B094B0).
- WordWrap_String TokenizeW / TokenizeA (0x00A10A60, 0x00A10B80) call LTCG helpers that take the character in ax with no push. Park them after 1 grade as an LTCG lane gap.
- The CPSC float-to-long helpers are retail inline asm (bare `fistp dword`). They are retail_asm candidates, as with the landed 0x00B102C0 (0x00B17BD0, 0x00B1BC80).
- Tooling: a compile-only scorer that maps every `[ebp+-N]` to one token (SCOREN=1) separates frame-slot choice from real instruction differences. Permutation searches over statement orders in it found most of the 0x00650E98 / 0x007BCC40 shape fixes without spending grades.

Review at landing:
- 007b7140 (STLport copy_if) needed 6 compiles, including 1 SRC_FAIL; that is over the 4-grade budget. Its matching source carries two same-TU stand-in bodies (GetSubDef, `_M_insert_overflow`), marked in the source.
- More rows carry same-TU `__declspec(noinline)` stand-in bodies for out-of-TU callees: 0080ad90 (comparator operator()) and 007153b0 (GetSubDef / LowerBound). Check that the lander accepts them; they are not part of the row's own bytes.
- Local models next to existing engine headers: 00714900, 00714e00, 007153b0 and 007b7140 declare slices of
  CThingCreatureBase / CTCPhysicsBase / CTCBase locally (rebuild/include/engine/ has CThingCreatureBase.h, CTCPhysicsBase.h,
  CTCBase.h); the other classes these rows touch (scripted actions, NUISystem components, CTCCrateStack) have no header.
  Retype the four with `retype_landed.py` where the headers fit the bytes.
- Already-landed errors:
  - 0x007BEDD0 is landed as CTCPhysicsRigidBody::GetFacingVector. It is the shot definition's target-position accessor: it reads this+0x51 and is called from InitMovement 0x007BCC40.
  - 0x00C6A5D0 is landed as CEditControlCentre::GetCurrentMask. It is libpk CReverbProcessor::Process, which calls ProcessInput 0x00C69C60, then ProcessCombs 0x00C69FB0 and the later stages, and stores `active` into +0x28c.
  - 0x00BEDD70 (landed `__adjust_heap`): its model is pair<long,CCompositeSurfaceInfo>, but its catalog label and file name say pair<long,CCountedPointer<CNavConnectorNode>>.
- Catalog rows with the wrong class:
  - 00b84470 is the CEngineSubPrimitiveRenderer ctor, not NHeroInformationScreens::CBase::CBase.
  - 00c69c60 is libpk CReverbProcessor::ProcessInput, not CQ_GuildTrainingSkillScript::UpdateAnimation.
  - 0054e6f0 is the inherited CViewport::Draw in CScrollingViewport's slot 2.
- No catalog row in this chunk holds two functions.
