# Hand chunk 2038 (CEnginePrimitiveManagerParticleDecalGroup): 1 of 1 byte-exact

## What matched (1)
- 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup -- RELOCATION_MATCH 1951v1951, test PASS (g++ and VC7.1),
  read-only vland RELOCATION_MATCH + behaviour PASS. 4 grades in pass 1 (v1 1935v1951, v2 1951v1951 frame-only, v3 RELOCATION_MATCH,
  final src via gall.sh RELOCATION_MATCH). Source `src/00bb8750.cpp`, test `test/00bb8750.cpp`, overlay `hdr/` (README there).
  - What it does: landscape-decal vertex shader picked by the point-light count (inline ApplyLandscapeVertexShader) and pixel shader; five
    render states through CRenderStateManager's inline setters (AlphaBlendEnable 1, ZBufferEnable 1, ZBufferWriteEnable 0, AlphaTestEnable 1,
    AlphaRef 1); world transform = SetIdentity + SetTranslateOnly(camera position); per decal of group.Primitives (STLport list of
    CEnginePrimitiveHandle, GetData() cast to CEngineInternalPrimitiveParticleDecal): floor / ceil cell extent through GFFloatToLongFloor /
    Ceil, colour constant 0 (premultiplied by alpha when the bool is set), a CArray<CTVertexNormTex1> grid ((w+1)*(h+1) vertices: x, y,
    WorldPos.Z, RHSet.GetUp(), uv = pos * (1/size) + (0.5 - centre/size)), a CArray<unsigned short> index list (two triangles per cell,
    diagonal chosen by the parity of (long)(x + y)), PrepareForRender(0x15), DrawAsIndexedTris(vertices, 32, n, indices, tris).
  - Shape lessons: (1) `row = width + 1` named: retail keeps row in ebx (`inc ebx` 0x00BB8BC7) and spills width; the index writes read row /
    width exactly as retail's leas (`row + flip + vi`, `vi - flip + 1`, `row + vi`, `width + vi + 2`), `flip = 1 - (... & 1)` named,
    `vi += 1 + ((vi + 2) % row == 0)` (neg / sbb / inc; a `? 2 : 1` gives `add edx,2`, an `if` adds a branch -- both checked on own builds).
    (2) Frame: with `v_scale` computed before `num_verts`, v_scale's frame node conflicted with `height`, so the vertex counter took height's
    word and the five weight-4 nodes sorted differently (it / num_tris / min x / min y / y): every instruction equal, 28 frame offsets off.
    Computing num_verts first (height's last use) lets v_scale share height's word ([ebp+54h], 0x00BB8BDF) and the packer then reproduces
    retail's whole frame. Measured with the frame-packer trace (tools/pk/run.py = agent3016 pk55 copy; tools/sim2.py replay) before grading.

## Names and evidence
- Row: Ego_r.pdb + FableWin.pdb `?RenderLandscapeDecalGroup@CEnginePrimitiveManagerParticleDecalGroup@@AAEXABVCEngineInternalPrimitiveParticleDecalGroup@@_NJ@Z`
  (private, ret 0Ch); FableWin donor order; sole caller RenderSubPrimitive 0x00BB9533 (under `group.LandscapeDecal`, +0xC0). Leaf confirmed
  with objdump -t.
- Callees / globals: all PDB spellings through the shared headers (LABELS.tsv, NAME_FIXES.tsv for the wrong landed spellings).
  NEngineInternal::GPShaderRenderManager 0x01436E14, GPRenderManager 0x01436E18, GPCamera 0x01436EA0: established spellings (no PDB data
  symbol on this machine; vland WARN RELOC_NAME: 0x01436EA0 is labelled g_pEngineCamera, the c2028 / c2037 data-symbol review item).
- Inline helper bodies proven against retail's unreferenced out-of-line copies (free qx.py probes): ApplyLandscapeVertexShader 0x00BB97F0
  RELOCATION_MATCH; CArray<CTVertexNormTex1>::operator[] 0x00BB9850 MATCH; allocator<CTVertexNormTex1>::allocate 0x00BB98A0 same instructions
  (my first QLEN was one byte short). The others (SetIdentity, SetTranslateOnly, PeekCameraPos, GetUp, CRGBFloatColour(const CRGBColour&),
  GetAsCArray, the render-state setters, PrepareForRender) are the existing shared bodies, confirmed by this row's match.
- Descriptive (no PDB locals / parameter names on this machine): parameter names `group`, `premultiplied_alpha` (the bool multiplies R, G, B
  by A), `point_light_count` (indexes PointLightShaderIndex[]); locals min_x .. vi. No made-up function, class or global name.
- Class keys: V for every class (Fable.exe RTTI `.?AV...`, pdb_class_keys.tsv); bases from ghidra_out/rtti_bases.tsv.

## CEngineMap question (brief)
- This row needs NO CEngineMap / CEngineWorldMap inline (it never reads landscape heights: z = decal.WorldPos.Z), so it gives no evidence for
  the c2037 CEngineMap.h opt-in choice.

## Asm helper sites (ASM_RULES)
- GFFloatToLongFloor 0x00BB8A75..0x00BB8A8F, 0x00BB8AA1..0x00BB8ABB; GFFloatToLongCeil 0x00BB8ACD..0x00BB8AE7, 0x00BB8AF9..0x00BB8B13 (bare
  `fistp dword`, compare fix-up on memory; the eight 0.5 / 1.0 frame constants hoisted out of the decal loop). Per-site attempts:
  probe/asm/spell.cpp (c2037's harness, every spelling rewritten to take this row's `pos -/+ size * 0.5f` input), probe/asm/run.py,
  result probe/asm/result.tsv: Floor 30 C++ spellings, Ceil 31: all `call __ftol2` (/O2) / `fistp qword` (/QIfist); 12 intrinsics: SSE
  converts only. Summarised in the row's `// Retail-asm:` block. Expected purity grade: retail_asm.

## Header debt / review at landing
- hdr/: three repo copies with added opt-ins + four new headers (hdr/README.md). The CMatrix3x4 SetIdentity block is the c2005 text.
- Landed local models that the new typed headers could replace later: 0x00BB8090 RenderMeshDecalStencilClear (local CRenderManagerCoreView,
  wrong DrawAsIndexedTris spelling in NAME_FIXES.tsv), 0x00BBC460 / 0x00BBC130 particle-group rows (local list node / group models),
  0x00BB9FD0 vector<CTVertexNormTex1> (local STLport + CTVertexNormTex1 model).
- Catalog observation for the reserved sibling (NOT my row, not authored): 0x00BB8EF0 RenderSubPrimitive ends at 0x00BB9685 (`ret 8` at
  0x00BB9682, then int3), 1941 bytes, not 2659: 0x00BB9690..0x00BB9952 is a run of unreferenced inline COMDATs of this TU (list begin / end,
  vector / CArray accessors, allocators, ApplyLandscapeVertexShader 0x00BB97F0, ApplyMeshVertexShader 0x00BB9810, ...).

## Commands (resume)
- Grade: `bash tools/gr.sh <src> <tag>` (exports the overlay; q.py + md.py summaries) or `bash gall.sh` with FABLE_HDR_OVERLAY exported.
- Free probes: `python3 tools/cc.py <src> <tag>` (compile only, own disassembly to tmp/cc_<tag>.txt), `bash tools/pk.sh <src> <tag>`
  (frame-packer trace + sim2 slot list), `QLEN=0x.. python3 tools/qx.py <addr> <src> <leaf>` (out-of-line inline body v a retail COMDAT).
- Measurements: grade 1 v1 DIFFER(1935v1951) (index exprs / width in a register / frame); grade 2 v2 DIFFER(1951v1951), 575/575 frame-masked,
  547 exact (28 frame offsets); grade 3 v3 RELOCATION_MATCH; grade 4 src RELOCATION_MATCH. Probes on top: 8 compile-only builds, 2 packer
  traces, 3 out-of-line COMDAT checks, 1 attempts TU (2 compiles), tall.sh / tall.py, one read-only vland.

## Parked for Fable 5.1 (0)
## Skipped
- none
## Label corrections
- See NAME_FIXES.tsv (callee spellings bound wrong in landed rows) and the RenderSubPrimitive extent note above.
