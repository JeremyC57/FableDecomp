# Chunk 3004 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B4FC40) The x87 term order of an inlined helper's sum over MEMORY operands (the expanded CMatrix4x4::PostMultiply loop, column 0 =
  the column whose operands sit at offset 0 of the matrices) is keyed on the CALLER's own symbol count, period 8: dead or forwarded named
  locals of the outer function anywhere in its body (before or after the loop) rotate only that one sum; locals of inline bodies expanded
  earlier, other functions in the TU, temporaries bound to arguments and dead locals inside the inlined body do not; an extra inline wrapper
  level around the call, or a USED local inside the inlined body, does. Here 5..7 extra outer names gave retail's order (0, 8 gave ours).
- (0x00B4FC40) Measure such a residue for free with a c2 trace run that keeps the object (tools/c2h/host.py + tools/tord.sh: the loop's
  per-column k-order and the frame size), sweep dead locals as a DIAGNOSTIC to learn the needed count, then add that many NATURAL forwarded
  names (accessor results bound to a reference / enum / pointer local): they emit no instruction and no frame slot. Named CLASS locals
  (C4DVector constants) are not neutral -- each takes a frame slot (0x90 -> 0xA0).
- (0x00B4FC40) The same lever had no effect on 0x00A4E300 (sums over local-matrix field candidates): there the order follows the field
  candidates' ids, not the outer symbol count -- check which kind of operand the residue sum reads before reaching for this lever.
- (tooling) tools/c2h/: a generic c2dbg gdb host (common.py, host.py keeps the object), dumpblk.py (dumps a block's machine tuples at the
  allocator's block pass: tuple +0 next, +4 opcode (names at c2.dll 0x10874398), +0x10 prev, +0x28.. operand nodes; memory node 0x252 has
  the displacement at +0x30); tools/ordobj.py (loop k-order + frame of an object), tools/oolcmp.py (compile a fragment and compare one
  out-of-line function with a retail address, free), tools/dbgsize.py (/Od sizes, to compare with FableWin.pdb slot sizes -- calibration
  did not fit: FableWin's flags differ).

Review at landing:
- 0x00B4FC40: its five named locals are the measured lever for a c2 symbol-count-keyed x87 order (not byte evidence of the original names);
  the names are descriptive and the `// Named accessor results` line 2 says so. vland WARN RELOC_NAME: 0x01396DC8 is labelled
  g_textureUVModeToD3D in the repo labels while the shared CRenderStateManager.h spells it CRenderStateManager::D3DTextureUVModeTable (the
  same table the landed setter rows use) -- one name per address should be settled.
- Carried from chunk 2024 (still open): NAME_FIXES.tsv (0x00B4FC40 / 0x00B50510 spellings), 0x01436E98 / 0x01436E60 / 0x01436E9C global
  spellings (NEngineInternal::GP* family vs g_p*), the landed 0x00B50510 local model.
- 0x00A4E300 is parked unchanged from chunk 2014 (its differ source needs that chunk's overlay opt-ins, all in hdr/; FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED is unproven).
