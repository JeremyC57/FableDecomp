# NF142 overlay

No overlay headers. Every changed row grades on the repo's shared headers as they stand (working tree with nf139 / nf136 applied), using
only existing opt-ins (FABLE_RETAIL_CDISPLAYMANAGER, _CSYSTEMMANAGER, _SCOPED_PTR, _CUISCREENUNITCONVERTER_RESETDIMENSIONS /
_ORIGINAL_DIMENSIONS, _CPIXELFORMAT, _CSURFACE (+ _LOADGRAPHICFRAME, _BITMAP, _GETD3DSURFACE, _COPYFROMMEMORY), _CTEXTURE).
No pin_asserts run (nothing overlaid); orig/ is empty. No shared-header member renamed; no header claim needed. Landed includers unchanged.
