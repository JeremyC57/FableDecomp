# agentNF32 header overlay

Full copies of the CURRENT repo headers with `#ifdef` additions only. Grade with FABLE_HDR_OVERLAY=<this dir>.

- engine/CSurface.h: new opt-in `FABLE_RETAIL_CSURFACE_GETBYTELENGTH` (retail class branch, after the `_LOCK` block): declares
  `unsigned long GetByteLength() const;` (0x009F2FC0, Ego_r.pdb 0x00B46CA0 / FableWin.pdb 0x030C5F90 ?GetByteLength@CSurface@@QBEKXZ).
  Used by row 0x009F2FC0 (with FABLE_RETAIL_CSURFACE_PIXELFORMAT), which defines it beside GetPixelFormat. Do not combine with
  FABLE_RETAIL_CSURFACE_LOCK / _CLEAR (they declare or define the same member). No layout change.
