# Chunk 275 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `push edi` at a switch head with `pop edi` duplicated into the default arm (0x005467F0 CZoomingComponent::ProcessEvent): write the whole shared post-switch tail out in EVERY case arm, each ending in `return;`. VC7.1 tail-merges the copies and keeps the callee-saved push at the switch head. One tail after the switch sinks the push to the last block. A forceinline helper holding the tail is NOT equivalent. Found by the Fable advisor.
- oldZoom copy loads scheduled after a byte store (0x005467F0): give the 8-byte struct a user memberwise `operator=`.
- A local list whose frame word is reused after a gather call (0x0063E950 GetHeightZBelowAt): the gather template instance is in the same TU, and an auto_inline(off) stand-in that only push_backs lets VC keep the list sentinel in esi and give its slot to a later float.
- An inlined functor (From*/To*/Height) collapses to plain locals: write the loop body by hand with `from`, `to` and `height` locals. 0x0063E950 inlines CThingFindHeightZBelowAt::operator() 0x00668AB0.
- C3DVector user copy ctor (again): `C3DVector from = pos; C3DVector to(from.X, from.Y, from.Z - k)` copies X/Y through the FPU (`fld ; fst from ; ... fstp to`) and Z through an integer register only with the user copy ctor (0x0063E950).
- C2DCoordI temporaries built inside loops reproduce an `and esp,-8` frame under 3077 (0x0063DDF0 Validate). Compiled-out ASSERT comparisons leave their out-of-line ConvertWorldMapCoordsToLocalCoords calls with unused results; write the calls into named locals.
- Real signatures from `ghidra_out/_fablewin_api.tsv` (FableWin.pdb mangled names) fixed several mislabelled pointer-vs-reference callees (CreateLineSegment, FindMaxGroundZInCell, FindGroundCollisionPointWithinCell). The bytes did not change, but grep there before guessing a callee type.
- Lane gap candidate (0x0063D960 / 0x0063DB30): VC7.1 frame slot order did not follow declaration order, scope blocks or init form. Ours ranks the loop-body `next` (shared with the `to - from` temporary) lowest; retail puts it above `direction`/`pos`. Six forms tried.

- A forceinline GetTC lookup key that shares a dead argument slot with other locals (0x0062ABA0): when the key will not pack because the out-of-line LowerBound takes `const long*`, add a same-TU `#pragma auto_inline(off)` `__declspec(noinline)` body for that LowerBound instance (lever of 0x008967F0). The key then stops escaping and packs into the dead `event` slot. Keep the second retail copy 0x004365B0 extern, so the two stay distinct symbols. Found by the Fable advisor.
- A `movsx` scratch register (ecx v eax) on a repeated `GetPlayer(event->Player)`: stage the signed char through a named `long number` (0x0062ABA0).
- An address-taken CMemoryBuffer read that the compiler keeps in a register afterwards (`mov esi,[slot]` once): the value comes from a by-value forceinline ReadLong(). A plain `long slot; Read(4,&slot)` reloads after every call.
- Two reads into the same dead stream-argument slot (0x00757990): the source reads the trailing field back into the same `count` variable. A second named local takes its own frame slot.
- A pair default value whose float is a `mov [],0` immediate while the longs share a zero register: give the entry struct a user ctor that sets the float to `0.0f` (0x00757990).
- A `cmp size,[remaining]; jg` order in an inlined memory-stream Read with a variable size: write the test as `size <= m_remaining`. The constant-size reads are unaffected (0x00757990).
- A CRGBColour pair stored in reverse declaration order and a C2DBoxI built before a vector ctor (0x004FF570): declare the colour whose bytes come first, then use the four-long box ctor (two C2DCoordI temporaries schedule the zero stores differently).
- IS_THING_ALIVE(CBaseObject* const&) with a CThing-derived argument: model the conversion with a real empty-offset base class so the converted temporary is built in the dead argument slot (0x0062BEF0). An exact-type reference makes the compiler re-read the variable from that slot.
- In 0x0062BEF0 (four HasTC uses) the plain inline HasTC was emitted out of line (`push 46h; call`); `__forceinline` fixed it. The cause was not isolated.
- Look for landed sources of the callees first (grep the callee address in rebuild/src/compiled). 0x00500230 CreateNavMap matched first try by assembling the CFileFormatLevel, CMemoryFile and CNavigatorManager models from four landed files.

Review at landing:
- 005467f0 needed the advisor (more than 4 grades); its source duplicates the tail by design.
- 0063ddf0 / 0063d960 use local retail CWorldMap models (Maps +0x20, Area +0x5C) next to rebuild/include/engine/CWorldMap.h. The header's 16-byte STL containers put Maps at +0x24 and Area at +0x6C; the evidence is the landed IsPosPassable 0x004FCBD0.
- 0063e950 models CWorldMap with World at +0x08 only (as the header); it reuses the landed 0x00668AB0 CPhysicsMeshObject model.
- 0062aba0 uses the advisor's same-TU LowerBound 0x0040F020 stand-in (a binary search body). The verifier treats it as an ordinary same-TU helper. The 0x004365B0 copy stays extern.
- 004ff570 / 00500230 / 0062aba0 / 00757990 use local retail-offset models (CWorldMap Maps +0x20, CThing bitset +0x20 / map +0x44, CMemoryFile, CFileFormatLevel packed) next to engine/ headers that carry the PDB's 16-byte containers.
- 0x00789AB0 name (IsToUseFirstPersonTargetingMode) in the 0062bef0 differ is inferred.
