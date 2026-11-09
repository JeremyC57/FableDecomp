# Chunk 883 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- `frame->AddPositionChild(list->FindChild("X"))` loads frame's vptr before the FindChild call; retail's order (call first, then the vptr load, `list` kept in edi) needs a named local: `CComponent* child = list->FindChild("X"); frame->AddPositionChild(child);` (0x006096E0 position phase, 25 pairs).
- Same for a by-value argument from a call: `mesh->SetOrientation(GetStartingRightHandedSet(i))` -> `CRightHandedSet orientation = ...; mesh->SetOrientation(orientation);` (0x0060B3CE).
- A struct returned by value from a `static __fastcall` gets its hidden result pointer in ecx only when the struct is non-aggregate: declaring the PDB constructors of CEngineGraphic (declaration only) gave retail's `lea ecx,[esp+10Ch]` + in-place read (0x0060B565/0x0060B571).
- When retail calls GetDef<T> / GetSubDef<T> out of line but the caller's handle release has no null test / dead `Object = 0` store and the appearance release is `dec [ecx+4]`, the template BODY must be visible (c2 sees it is not inlined but learns the out-param is written) -- header opt-ins with the landed 0x0042B040 / 0x0042B0A2 logic.
- The auto-inline budget is spent in source order; STLport 4.6's extra layers (`_Construct` in `_M_create_node`, `__stl_alloc_rebind` in `_STLP_alloc_proxy`) are what made the 4th action group fall out of budget at retail's point (0x0060A97F calls 0x0042BE50).
- A loop index passed to a call: pass the loop variable (register), not an address-taken copy made for `find(key)` -- the address-taken copy forces a reload and changes size (0x0060C7BF).
- /d2inlT bisect: T70 gives 591 calls, T71 = T72 = retail's 590; T73/T74/no flag inline `++it` (0x0047B3E0). /d2inlS<N> had no effect here.

Review at landing:
- 0x006096E0 needed ~60 grades; parked on allocator residue only (see NOTES.md).
- The differ source declares an explicit specialization of the CategoriesList tree's `clear()` to keep it out of line; consider a fable_map.h opt-in instead.
- Already-landed sources with wrong names: 0x006166E0 (`CTCCarryable_OnKill`), 0x00615820 (`CUnproven*` types) -- NAME_FIXES.tsv.
- hdr/ overlay: new CEngineGraphic.h, CAnimFilterBase.h, CAnimationSet.h, CAppearanceDef.h; opt-ins in 8 existing headers plus whole-file overlays of stlport/fable_containers.h and fable_map.h (hdr/README.md).
- (2026-11-09) A callee-saved piece that loses to a higher-priority ebx-only value is cut by the split pass into one piece per
  free region; a region with two references (the then-arm's two CreateComponent calls, w 2, K 7 and 8) scores 2*2*(7+8) - 6 = 54
  and beats a counted pointer's Info (34) that is live across the second call -- one lone reference per region would be dropped.
- (2026-11-09) Forcing by c2 id is ambiguous when ids are recycled (the piece u794 reuses id 195); force by creation ordinal
  (tools/c2pf C2_FORCEUID) and check both sides of the pair inequality: raising the winner's rival or lowering the piece gave the
  same 99.03% / 8 segments, which proves a pairwise lever, not a global one.
- (2026-11-09) `if (it != end()) do { ...; ++it; } while (it != end());` is not a code-identical twin of `for` in VC7.1: the for
  keeps the `jmp cond` + aligned body rotation, and the do-while expanded the tree ++it (0x0047B3E0) at the default threshold.
- (2026-11-09) Moving a counted pointer's default declaration above a call moves its `Data = Info = 0` stores above the call
  (c2 does not sink them): declaration position is code, not just scope.
- (2026-11-09 s2) A split piece leaves the K count of a re-sort only when it is dropped (1 reference, spill <= 0); a 2-reference
  piece is checked for copy forwarding but stays listed (prio -6 / 0) and still counts. The "forward" event is the check, not
  removal.
- (2026-11-09 s2) c2's split-region dataflow (0x1078B51C): pass 1 backward liveness (OUT +0x3C = OR succ IN +0x38;
  IN = GEN +0x30 | OUT & ~KILL +0x34), IN/OUT saved to +0x24/+0x28, pass 2 forward reach (IN = OR pred OUT & live-in;
  OUT = (IN & ~KILL | GEN2 +0x2C) & live-out); one bit per piece, 32 per round.
- (2026-11-09 s2) A large function can be insensitive to every local code-identical spelling (125 edits, all byte-identical
  objects): there the remaining lever is in an allocator pass's structure, not in the IL surface.
