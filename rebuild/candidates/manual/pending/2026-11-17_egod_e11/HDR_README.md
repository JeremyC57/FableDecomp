# audit e11 fixed overlay (the 7 headers rows 0x0065CB6B / 0x0095B9E0 / 0x00B45720 include; copied from w23/e11m/hdr)
Only change vs w23/e11m/hdr: engine/CAIStateGroup_HoverHornetQueen.h, opt-in FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_DIVEBOMB, COMMENTS ONLY
(same line count, no code change): the four circle-point helpers are NOT absent from Fable.exe -- retail keeps out-of-line copies at
0x0095A3D0 GetNextCirclePoint, 0x0095A3F0 GetPreviousCirclePoint, 0x0095A9A0 GotoNextCirclePoint, 0x0095A9D0 GotoPreviousCirclePoint
(uncatalogued; the row source's helper bodies grade MATCH / MATCH / RELOCATION_MATCH / RELOCATION_MATCH against them).
All other opt-ins and their evidence: see w23/e11m/hdr/README.md.
