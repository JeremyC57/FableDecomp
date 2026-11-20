# NF195 overlay (2026-11-20)

## stlport/fable_containers.h
Copy of the repo working-tree header (base in orig/stlport/fable_containers.h). One add-only opt-in:
- `FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY` (new, member-specific): wraps the existing `resize(size_type, const _Tp&)` block
  (`#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE ... #endif`) in `#ifdef <new> <declaration> #else ... #endif`. With the macro
  undefined the header is token-identical to the repo copy (the added lines are only the new `#ifdef` arm and its `#endif`).
  Used by the six q4567 rows (0x00B75CA0 0x00B77E20 0x00B7EEC0 0x00B7F0F0 0x00B7F3B0 0x00B7F600): vector<CPreTransposedBoneMatrix>::resize
  0x00B7EDF0 is called out of line there; the visible noinline form (RESIZE_OUTOFLINE) matched the row but emitted a 94-byte COMDAT
  copy that differs from the owner's 102 bytes, so the pick-any rule needs the declaration-only form.
- pin_asserts.py: `stlport/fable_containers.h pinned 0` (the header has no FABLE_STATIC_ASSERT lines that moved).
- Landed includers: no landed row defines the new macro, so the default path (token-identical) leaves every includer unchanged.
- Other agents' overlays read first: NF193 overlays only stlport/fable_string.h, NF194 only engine/ headers; neither touches this file.

## engine/CSubtitleRenderer.h
Copy of the repo working-tree header (base in orig/engine/CSubtitleRenderer.h). One add-only opt-in:
- `FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK` (new, nested in the existing `FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED`
  block): `#pragma pack(push, 8)` / `#pragma pack(pop)` around the nested CEmbeddedHighlight record, so it is 0x14 bytes as in Ego_r.pdb
  (the header's file-wide `#pragma pack(push, 1)` made it 0x11). The outer class is unchanged (its FABLE_STATIC_ASSERTs compile).
  Without the macro the header is token-identical to the repo copy. Used by row 0x00442870 (q6240).
- pin_asserts.py: `engine/CSubtitleRenderer.h pinned 0`.
- Landed includers: 2 rows define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED (0x00492381, 0x0049417D); neither defines the new macro, so
  none changes. Coordinator: the pack(1) layout of CEmbeddedHighlight is a header defect (Ego_r 0x14); a coordinated fix would make the
  opt-in the default.
