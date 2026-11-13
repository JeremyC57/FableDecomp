
## bwf1 author session (2026-11-12) -- interim
- Best: with a non-capturing body of SetupRenderModeShadersAndConstants 0x00BA2350 visible in the TU (PROBE stand-in, v/r1.cpp), DIFFER(4056v4056), one residue (cull SetRenderState bookmark edx/esi, 0x00BA5EB3). Moved by: callee-body visibility (lazy reads of &shader_flags / &render_degenerate), primitive declared before mesh_primitive, inline RenderMeshPrimitiveBlocks kept.
- Did not move the cull tie: SetRenderState EARLY_RETURN / DESIRED / bookmark test+or operand order, SetPolyCullMode state ref, extra functions before/after the row, SetupStaticBlocks body visible.

## bwf1 author session (2026-11-12) -- hand-back: DIFFER, BLOCKED on a callee body (57 grades + 5 toy probes)
**Root cause found (new level: callee body visibility, Q32 lesson).** The prior's "mipmap piece coloured into ebx" residue was not a
register tie. Retail reads the two out-params it passed by address to SetupRenderModeShadersAndConstants LAZILY and caches them:
`mov ebx,[ebp-1Ch]` (shader_flags) only at 0x00BA5FEB, after SetVSConstants / UpdatePixelShader*, then kept in ebx across further
calls; `mov al,[ebp-4]` (render_degenerate_triangles) at 0x00BA60F2, after the IndexBuffer call. VC7.1 does that only when the body
of the callee that received the address is in the TU and does not capture the pointers (toy probe/t5.cpp v t1..t4.cpp: declaration
only => eager copy at the inline entry; visible non-capturing body => exactly retail's lazy-load-then-cache). 0x00BA2350
SetupRenderModeShadersAndConstants is defined earlier in the same retail TU (CEnginePrimitiveManagerStaticMeshesBase.cpp).
- With a PROBE stand-in body for it (probe/u1_standin_PROBE.cpp: writes *shader_flags / *render_degenerate_triangles, calls an
  unknown function; NOT landable) the row grades DIFFER(4056v4056), 22 hunks of which 21 are grader mask noise (`mov [x],0`,
  `movzx edx,[ecx+0x143b990]`): the ONLY real residue is the cull SetRenderState at 0x00BA5EB3/0x00BA5EB9 (retail Cur mask -> edx,
  state BookmarkMask -> esi, `or edx,esi` = Cur-merge; ours Cur -> esi, BM -> edx = BM-merge). The frame (0x50), every slot, the
  mipmap reloads at both calls, ebx = mesh primitive 0x00BA5D35..0x00BA5F2A, the cull spill [ebp-34h] all match.
- A stand-in without unknown calls is much worse (149 hunks: c2 then also knows globals survive the call) -> the real body must be the
  real one. SetupStaticBlocks 0x00BA50A0 visibility changes nothing. Stand-in variants (args passed or not) do not move the cull tie.
- Source facts settled on the probe (all in differ/00ba5260.cpp): `primitive` declared before `mesh_primitive` (fixes the
  0x00BA5393 store order); the FableWin inline RenderMeshPrimitiveBlocks kept (fixes the x87 fld/fmul order 0x00BA5434); block A
  ({ texture_stages ... }) needed, block B not (removed; same bytes); occlusion_handle at function scope (late = 66 hunks).
- Did not move the cull tie (all with the probe): SetRenderState EARLY_RETURN / SETRENDERSTATE_DESIRED / bookmark test order /
  `BM = Cur | BM` text (c1 canonicalises); SetPolyCullMode with a named CStateInfo& (neutral) or a named value (breaks the frame);
  named `POLY_CULL_MODE` local; `(*GPRenderManager).`; cull wrapped in a forceinline helper (two shapes); declaration order of
  used_blank_textures / occlusion_handle; GetAppropriateMipmap texts (named radius / texture_size / two-statement distance: all
  neutral); extra functions before / after the row. A named `CRenderManager* render_manager = GPRenderManager;` (or reference)
  right before the call DOES flip the bookmark orientation to retail's, but moves the manager to eax / state to ecx (29 hunks).
- Also tried: GFLimitReturn<long> for the clamp (FableWin has no GFLimit<long,int,int>; frame temps -> worse), /Oa (152 hunks),
  `long` mipmap_level, mipmap declared at the top (no change).
**Blocker / next step:** the row waits for 0x00BA2350's REAL body (bwf2/00ba2350, 9632 bytes, no draft yet). Once it is landed, put
it in this TU before the row (`#pragma auto_inline(off)` around it, real body) and regrade; then only the cull bookmark orientation
should remain. Best next idea for that: c2prio on probe/u1_standin_PROBE.cpp v h1 (v/h1.cpp) at the cull block (source line of
SetPolyCullMode) to see what decides which load merges with the OR result; try a named manager local typed differently / scoped so
that the manager stays in ecx.
**Headers:** C3DPrimitive2.h and CIVCountedPointer.h re-based onto the current repo (additions only); CRenderStateManager.h overlay
removed (repo text suffices). No `// Flags:` needed. Names: unchanged (prior NAME_FIXES kept; added the 0x00BA77B0 callee
declaration signature flag from the DOSSIER).
