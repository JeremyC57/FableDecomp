# Chunk 2038 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00BB8750 (frame): "every instruction equal, frame words swapped" was one live-range conflict, not a sort tie: computing a float
  (`v_scale = 1.0f / size.Y`) before the last use of an int (`height` in `num_verts = row * (height + 1)`) makes their frame nodes conflict,
  so the int's word went to another node and five equal-density nodes re-sorted. Moving the statement past height's last use (the scheduler
  still emits the same instruction order) gave retail's frame exactly. Read the packer trace (agent3016 pk55/run.py + sim2.py) for WHICH
  nodes share a word before trying reference-count devices.
- 0x00BB8750 (CSE / registers): a `lea r,[r+r+2]` built from a spilled `width` while `width + 1` lives in a callee-saved register means
  the source named `row = width + 1` and wrote both `row + ...` and `width + ... + 2` in the expressions; writing `vi + width + 1 + ...`
  everywhere folds the constants into other leas and keeps width in the register.
- 0x00BB8750: `vi += 1 + (expr % row == 0);` gives retail's `neg ; sbb ; inc ; lea edi,[edi+edx+1]`; `? 2 : 1` gives `add edx,2 ; add`,
  an `if (...) ++vi;` a branch.
- 0x00BB8750: a CMatrix3x4 identity with its translation overwritten (twelve immediate stores, zeros E43..E12 then ones E33, E22, E11, then
  the three copies) is SetIdentity() (the c2005 chained-assignment body) + SetTranslateOnly(v).
- 0x00BB8750: `CArray<T> local(n)` with FABLE_RETAIL_CARRAY calls the out-of-line vector<T>(n) for a non-POD T (CTVertexNormTex1, 0x00BB9FD0)
  and expands it for unsigned short (VC7.1 turns the zero fill_n loop into `rep stosd ; adc ecx,ecx ; rep stosw`).
- The catalog extent of 0x00BB8EF0 RenderSubPrimitive (2659 bytes) also covers 0x00BB9690..0x00BB9952, a run of unreferenced inline COMDATs
  of the decal TU (list begin / end, CArray<CTVertexNormTex1>::operator[], allocators, ApplyLandscapeVertexShader 0x00BB97F0,
  ApplyMeshVertexShader 0x00BB9810): free answer keys for the TU's inline bodies; RenderSubPrimitive itself ends at 0x00BB9685 (1941 bytes).

Review at landing:
- hdr/ overlay: 3 repo copies with added opt-ins (CMatrix3x4 SetIdentity = the c2005 block, CEngineInternalPrimitiveParticleGroup and
  CEnginePrimitiveManagerParticleDecalGroup typed branches) + 4 new headers; re-base on HEAD.
- 0x01436EA0 spelled NEngineInternal::GPCamera (vland WARN RELOC_NAME v label g_pEngineCamera; the open data-symbol review).
- NAME_FIXES.tsv: 7 callee spellings bound wrong by landed rows (0x00B54310, 0x00B5E700, 0x00B63010, 0x00B67480, 0x00B99670, 0x009E1280,
  0x00BB8090).
- Row grade should be retail_asm (GFFloatToLongFloor / Ceil helpers, `// Retail-asm:` block with the per-site attempts summary).
- Catalog: 0x00BB8EF0's length (see above) when that row is cut.
