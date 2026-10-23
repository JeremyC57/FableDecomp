# Chunk 317 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Bool flag from an `&&` chain** (00eea7c0): `bool found = a() && b(); if (found) {...}` gives retail's layout, with `found = 1` stored before the body and the `found = 0` block placed after the later arm. `if (a() && b()) { found = true; ... } else found = false;` puts the false store inline.
- **Virtual IsAlive inline vs call** (00eeb7b0): the same handle method can appear both expanded (`PImp_Data && PImp_Data->vslot75()`) and as a direct call in one function. Model the expanded site with a `__forceinline` helper over PImp_Data and the other as the declared call.
- **Quest-script mains are cheap** (00eea7c0, 00eeb7b0 both matched in 1-2 grades): reuse the CExpressionWaitScript_Main VPAD interface model. Generate the VPAD gaps with a small script from a slot->declaration map. Resolve slots via rtti-vtables.tsv (CGameScriptInterface 0x01260F0C) plus the landed virtual declarations (`grep -rhoE "virtual [^;]*Name\(" rebuild/src`).
- **Movie handle variants**: in WaitScript the movie ctor is inline and its dtor is expanded through the PImp. In Picklock the ctor is inline and the dtor is an out-of-line 0x6E7B80. In Dig both ctor 0x6E7B60 and dtor 0x6E7B80 are out of line. Declare to match the call sites.
- **Inlined SetState variants** (render core 0x1436E18):
  - "MaskLocal": `mask = core->m_captureMask; if ((mask & s.m) == 0) { s.m |= mask; ... }`.
  - "StateMaskFirst": `sm = s.m; if ((core->m & sm) == 0) { s.m = sm | core->m; ... }`.
  - They flip which register holds which mask. StateMaskFirst fixed 00b76bc0's +0x28b8 block. The pure test/or operand commutation (00b5e0f0, 00b6fa40, 00b6e890) is still open: park it early.
- **Float literal vs extern** (00b75ca0): 0x0122F59C (0.5f) is a literal. Declaring it extern swaps the fld/fmul order.
- **CRGBFloatColour product** (00b76bc0): `CRGBFloatColour c = lowEnd * *ambient;` before the constant upload, then `C4DVector(c)`, restores retail's frame size. Use function-local statics; the guard bits are 1, then 2.
- **Named global local for x87 order** (00b54310): `const T* g = g_pGlobal;`, then `x * g->Scale`, gives retail's fld order where the inline global read does not.
- **SetIndices / SetStreamSource**: inline as if/else whole device calls (`if (ib) dev->SetIndices(ib->P) else dev->SetIndices(0)`). A ternary argument mis-schedules.
- **Retail CEngineWaterRenderer** has 12-byte STLport CArrays (PDB 16), so offsets after +0x12c are 0xc less per array. Use a local view with a `// Layout:` comment until the engine header is fixed.
- **Test gotcha**: under VC7.1, `T t = T();` on a POD-ish struct is NOT zero-initialised. Use memset in tests. A test that passes on g++ can fail tall.py otherwise.
- **Tooling**: q.py runs conflict when run in parallel; grade sequentially. A normalised diff (relocations to X, `[0x..]` small displacements to [X]) makes 1-instruction residues obvious.

Review at landing:
- These rows needed more than 4 grades:
  - 00b54310 RenderMist (11)
  - 00b76bc0 RenderBackground (16)
  - 00b75ca0 (3)
  - parked 00b6e890 and 00b6fa40 (30+ each, sweeps)
- 00b6e3e0, 00b75ca0, 00b76bc0 and 00b77e20 use a local CEngineWaterRenderer view next to rebuild/include/engine/CEngineWaterRenderer.h. The header uses PDB 16-byte CArrays, and retail is 12 bytes per array, so fix the header.
- 00eea7c0 and 00eeb7b0 use local CScriptThing views with virtual slots next to the flattened engine/CScriptThing.h (as the landed 00eea460 does).
- Landed Render_00b33010 misnames globals 0x1436E3C and 0x1436E38.
- Propagated labels found wrong: 0x6E7B60, 0x4AA840, 0x7E74D0 and 0x485F3A (see NOTES).
