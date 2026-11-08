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
