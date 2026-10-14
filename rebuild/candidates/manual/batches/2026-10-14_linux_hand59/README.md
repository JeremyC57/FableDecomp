# Hand chunk 59 (CMouseCursor .. CNavQuadTreeNode): 27 of 40 byte-exact

Authored by the coordinator (Linux lane, DDK 13.10.4035 cl.exe under Wine), graded with `q.py`, behaviour-tested with g++ and the lane's cl.exe.

## What landed (27)
- NUISystem::CMouseCursor ctor and `??_G` (0055c650 / 0055c8d0: three vtables = CSprite over a primary base, a CObserver secondary base at +4 passed to the front-end manager by implicit upcast, and a third base at +0x18; the factory needs the ctor INLINE in the TU), CMouseDX::SetHasInputFocus (00ab47d0: the agent52 CJoystickDX shape with the HRESULT checks `== 0 || == 1`), CMovableResourceMemoryPool::MoveResource (memmove then the resource's slot 1), CMovie::GetSize twins (00547be0 / 0054e5a0: a C2DVector copied from the member, scaled in place, returned through the memberwise copy constructor), the SSE vector product 00c889b0 (a STRUCT wrapping an `__m128`, passed by reference and returned by hidden pointer -- a bare `__m128` returns in xmm0), CMultiStatBar::GetCurrentChange (/Os: `push 6 ; pop eax`; a `volatile` next pointer keeps the retail double read; the `6` return placed last by the `if (state != 1) { .. }` form), the DirectShow pause 00ca9a90 (propagated label), CMultiStaticMeshEntryDef copy ctor (004eb864: `/Os` makes every member go through eax), the two CMurderTwistBrotherOlder::Init states, the relative-target setter 00a7d9c0 (the X/Y delta through an inline 2D subtraction returning a named local by copy constructor: both differences on the FPU stack before the stores, the Z through an integer move), the two music-entry offset resets, the NOVI quest scripts (Barrel OnPredicateFail with a `pack(1)` quest vector at +0x76, BarrelThug / TeddyGirl / Villager Init, Bully / TeddyGirl GivenTeddy with the named CCharString local in its own scope, the ftol script 00e99760), the unoptimised getter 009f76c0 (`optimize("", off)`), CNavBlockedQuadTreeNode(bool, bool) (the CANavQuadTreeNode base inline with x = y = -1.0f through one integer register), CNavQuadTree::SetUpMapStructure (`pack(1)`, the origin at +7) and IsActionPoint (`? true : false` for `setne al`), NUISystem::CNavButton::IsAcceptButton (the root test as one `&&`).

## Parked (12, evidence in `parked/`) and skipped (1)
- 0073a840: the 96-byte row covers TWO functions (0x0073A840 and the 18-byte 0x0073A880 after the padding) -- not one symbol; skipped.
- 006bd780 / 006bd840 IsDangerMusicEnabled: the last of five `mov eax ; test eax` loads becomes `cmp [ecx],0` in the lane (if, `&&`, `!..&&..` and pointer forms).
- 006eab00 collect-entries: the push_back value temporary lives in the dead argument slot in retail; the lane allocates a frame slot (pointer / reference parameter, cast rvalue).
- 008f6f90 flag scan: register choice (retail counts in eax and keeps the flag in bl).
- 00a76900 CNavBlockedQuadTreeNode(const node*): the +8 store sinks to the derived vtable store in the lane (fields and a C2DVector copy tried).
- 00a76950 SaveToFile: retail keeps the flag byte in a frame slot; the lane homes it in the dead `a` argument slot.
- 00a76a30 CNavNavigableLeafNode ctor: the +0x14 / +0x18 zero stores hoisted above the derived vtable store (plain members and an embedded member object).
- 00a76c10 RemoveNeighbours: `add esp,4` placed before the stores in the lane, between them in retail.
- 00a76c30 IsNeighbourOf: the `find` tag temporary over the live key slot (the known gap).
- 00a779b0 ConvertIndexesToPointers: the inlined map `find` -- the null-root entry test jumps to the final compare in retail, to the folded `found = head` store in the lane (both branch senses).
- 00a787d0 AddToIndexTable: the pair's `second` store sinks below the hidden-result push in the lane (constructor and POD forms).
- 00eb4710 IsFeelingThreatened: the `__fastcall` bool result tested as `test eax,eax` (lane `test al,al`), the FPU compare branch inverted, and the `sbb eax,eax` widening.
