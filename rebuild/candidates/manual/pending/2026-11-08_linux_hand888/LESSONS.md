# Chunk 887 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- (chunk 887) A local VC7.1 packs into a dead incoming-argument home ([esp+0x80] after the pushes) costs disp32 on every access; in
  0x00BB0970 that is the whole +67-byte length gap. Retail keeps the same local at local 0 with an 8-byte-larger frame. `const`,
  declaration order, an if/assign spelling, a pointer instead of the reference, and same-TU hidden-result callee bodies all left it in
  the argument home.
- (chunk 887) `bank->PeekGraphic(i) ? &bank->PeekGraphic(i)->Frames[f] : 0` written straight in the caller compiles the second call as
  branches. Put it in the PDB's inline `CGraphicDataBank::PeekGraphicInfoFrame(index, frame)` and it gives retail's `cmp ; sbb ; and`
  select on the first call's pointer.
- (chunk 887) The UV-offset lerp `(End - Start) * factor + Start` is C2DVector operator chains: the Y difference is spilled before X is
  multiplied. Explicit X/Y float statements do not reproduce that order. Four-element `+=` loops stay rolled, so write the eight adds out.
- (chunk 887) Inline GetFrame 0x0041C260 with mipmap 0: the `out = Data->Frame` assignment needs no null re-test in the CGraphicFrame
  destructor. VC7.1 drops the destructor's `if (FrameData)` after the deref, and the failure return carries no destructor.
- (chunk 887/888) RenderClockPrim 0x00BAFA00 opens `lea ebp,[esp-6Ch] ; sub esp,8Ch` because of the inline-asm GFFloatToLongCeil.
  VC7.1 chooses the BIASED ebp frame from the weight (reference count) of the frame symbols beyond disp8, not from the frame size
  (probe fc20: one far reference -> `mov ebp,esp`; fc22: three -> biased; bias = the largest that keeps the arguments within +7Fh).

## Session 2 (chunk 888, 2026-11-08)
- `const CTextureQuadUVs& quad = frame->GetTextureQuadUVs(); uvs = quad;` (the landed Draw2DGraphic 0x009DCA80 spelling) makes the
  returned temporary a reference-bound `$S` symbol of packer weight 2 (a plain `uvs = frame->GetTextureQuadUVs();` temporary has
  weight 1): in 0x00BAFA00 that alone moves the frame to retail's biased 0x8C layout; but the reference keeps the temporary alive to
  the end of its block, so it can no longer share a slot with later locals (0x00BB0970: frame 0x80 v 0x74).
- Two textures kept as `const CTexture* textures[2]` are ONE 8-byte frame symbol; VC7.1 right-aligns it in a larger slot (retail
  [ebp+18h]/[ebp+1Ch] = the debug box slot +8/+0xC, 0x00BAFA00). Smaller members of a packed slot sit at its top end.
- The clock fraction `if (angle <= 0.0 || angle >= 1.0f) clamp; else if (angle < 0.125f || angle > 0.875f) fmod(angle, .125)*8;
  else fmod(angle - .125f, .25)*4` reproduces retail's jnp/je/jnp/je compare encodings (0x00BAFE4A..).
- An index fill loop written through a local `unsigned short* indices = sub->Indices;` keeps retail's index/i/following registers;
  `sub->Indices[index++]` lets c2 strength-reduce to a pointer (0x00BB0560..). An identical `indices[index++] = ...` head in both
  arms of an `if` is always hoisted out (exp/loop probes).
- Jump threading: a `bool premultiply = false;` set to true in two switch cases and tested RIGHT after the switch is threaded by c2,
  so each case jumps straight into its arm; the cases' identical setter tails then cross-jump at machine level, leaving
  `mov esi,[table+K] ; jmp` in one case (0x00BBB2C0). Any code between the switch and the test (a colour constructed before the
  `if`) or a `mode == 3 || mode == 4` re-test is not threaded (0x00BBAC60: 682 -> 97 diff lines).
- The landed SetUpPointLights 0x00BBC0B0 spelling `CEnginePrimitiveThemeChannelDesc channels; channels =
  CEnginePrimitiveThemeChannelDesc();` is what gives retail's 3-byte zero temporary + dword copy before GetLightsInSphereFast
  (0x00BBB4B4..0x00BBB4CA); passing the temporary directly loses the copy and a 4-byte frame slot.
- `g_pResourceManager->PeekGraphicBank(bank)->EntryExists(i)` (inline accessor) loads the global before the argument
  (`mov eax,[1436E98h] ; push edi ; mov edi,[ebp+8] ; mov edx,[edi+4Ch]`, 0x00BBAC66); `GraphicBanks[0]->` loads it after.
- A `long light_count = 0;` scheduled inside the SetSelfIlluminationAmbient(100) expansion means the store is written BEFORE the
  setter call in source (0x00BBB50B).
- Probe tools (agent887): `fas.sh src tag` (the /FAs `_name$ = off` slot table), `pk.sh src tag Func` (c2 packer list: size, weight,
  flags), `slots2.sh` (sorted slot groups), `fpomap.py` (FPO esp offsets -> frame offsets), `rfb.py` (register+frame-blind diff).
  The register+frame-blind count separates structure from colouring: 0x00BAFA00 is 1091 lines but only 118 blind.

Review at landing:
- All three rows are parked above 4 grades (0x00BAFA00 10 this session, 0x00BB0970 31, 0x00BBAC60 22); 0x00BA6240 92.
- Local models next to existing headers: the decal row keeps local CGraphicFrame / CGraphicFrameData / CGraphicInfo / bank /
  resource-manager / CEngineInternalPrimitiveDecal / CEnginePrimitiveManagerDecal / CTVertexNormTex1 models (engine/CGraphicFrame.h has
  a retail branch without the release body; no CEngineInternalPrimitiveDecal.h / CEnginePrimitiveManagerDecal.h exist).
- The landed 0x00BB8090 RenderMeshDecalStencilClear and 0x00BAD2E0 RenderSubPrimitive still carry local CRenderManagerCoreView /
  CTrackedRenderState models; the shared CRenderStateManager / CRenderManagerCore retail branch now covers them.
- hdr/ adds two opt-ins: FABLE_RETAIL_CRENDERSTATEMANAGER_BORDERCOLOUR and FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHEREFAST.
