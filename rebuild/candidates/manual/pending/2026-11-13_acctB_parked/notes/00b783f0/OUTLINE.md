# OUTLINE 0x00B783F0 CEngineWaterRenderer::Render(const CEngineComponentRenderParams& params)

## Frame
- Prologue `sub esp,28h; push ebx; push ebp; mov ebp,[esp+34h] (params); ... push esi; push edi; mov edi,ecx (this)`. No ebp frame,
  no SEH, no /GS. Saved order ebx, ebp, esi, edi. `ret 4`. One epilogue 0x00B7A865 (early returns jump there).
- Frame map (esp after the four pushes; locals 0x10..0x37, arg at 0x3c):
  | slot | holds |
  |---|---|
  | bl (ebx, until 0x00B7913B) | fg = !ForegroundRenderList.empty() |
  | 0x12 | bg = !BackgroundRenderList.empty() \|\| !AlphaedBackgroundRenderList.empty() |
  | 0x13 | iceBg = !IceBackgroundRenderList.empty() \|\| !IceAlphaedBackgroundRenderList.empty() |
  | 0x14 | seaNonRefl = !SeaNonReflectiveForegroundRenderList.empty() |
  | 0x15 | seaBg = HaveBackgroundSea && Settings.EnableSea |
  | 0x16 | iceFg = !IceForegroundRenderList.empty() |
  | 0x17 | seaRefl = !SeaReflectiveForegroundRenderList.empty() |
  | 0x18 | spill of `this` |
  | 0x1c | time (float, copied as dword) = params.Params->BulletTimeCurrent.WFInterpolate (CEngineComponentParams +0x3c) |
  | 0x20,0x21 | CScopedShadersEnable shaders: OldVertexEnable, OldPixelEnable |
  | 0x24 | CRenderStateCaptureBlock block: Open |
  | 0x28,0x2c,0x30,0x34 | the four float args of each inlined SetTextureBumpEnvMatrix (bit copies) |
  | 0x3c (the params arg slot, reused) | seaMid = !SeaMidgroundRenderList.empty() \|\| !SeaAlphaedMidgroundRenderList.empty() |
  | ebp | &params until 0x00B79257; later reused as constant 1 (0x00B79AC6) |
  | (no slot) | CRenderStateScopedCaptureBlock capture (empty class) |
- Destruction order at the end: block (0x24) -> shaders (0x20) -> capture. So declaration order: capture, shaders, block.

## Body (retail order)
```
00B783F0  float time = params.Params->BulletTimeCurrent.WFInterpolate;           // read first (0x00B78400), stored 0x00B78417
          bool fg        = !ForegroundRenderList.empty();                         // 0x1fc/0x200 setne bl
          bool bg        = !BackgroundRenderList.empty() || !AlphaedBackgroundRenderList.empty();      // 0x208.. / 0x214.. -> [0x12]
          bool iceFg     = !IceForegroundRenderList.empty();                      // 0x220 setne dl -> [0x16]
          bool iceBg     = !IceBackgroundRenderList.empty() || !IceAlphaedBackgroundRenderList.empty(); // -> [0x13]
          bool seaRefl   = !SeaReflectiveForegroundRenderList.empty();            // 0x244 setne cl -> [0x17]
          bool seaNonRefl= !SeaNonReflectiveForegroundRenderList.empty();         // 0x250 setne [0x14]
          bool seaMid    = !SeaMidgroundRenderList.empty() || !SeaAlphaedMidgroundRenderList.empty();   // -> [0x3c]
          bool seaBg     = HaveBackgroundSea && Settings.EnableSea;                // 0x276 && 0x285 -> al/[0x15]
          (the eight are computed in exactly this order, 0x00B78403..0x00B784ED)
00B784EB  if (!iceFg && !iceBg && !fg && !bg && !seaRefl && !seaNonRefl && !seaMid && !seaBg) return;   // test order: dl,[13],bl,[12],cl,[14],[3c],al -> 0x00B7A865
          (i.e. `if (!(ice || water || sea)) return;` with ice = iceFg||iceBg, water = fg||bg, sea = seaRefl||seaNonRefl||seaMid||seaBg)
00B78523  Settings.UpdateConstants(params);                                       // 0x00BEA110, ecx = this+0x280
00B7852F  if (!CreateDynamicTextures(fg || bg,                                     // eax, pushed last (1st arg)
                                     seaRefl || seaNonRefl || seaMid || seaBg,    // ecx (2nd)
                                     iceFg || iceBg))                             // edx, computed FIRST (3rd arg; right-to-left)
              return;                                                             // 0x00B78584 call 0x00B72180; je 0x00B7A865
00B78591  CRenderStateScopedCaptureBlock capture;     // inline StartCaptureBlock on [0x013CB508]: StateStack[n++].State=&BookmarkDummyState; mask<<=1
00B785BF  CScopedShadersEnable shaders(true, true);   // save [0x013BC800] -> [0x20]; if !=1 EnableVertexShaders 0x00987FE0; same pixel [0x21] / 0x00988110
00B785ED  CRenderStateCaptureBlock block; block.Capture();   // StartCaptureBlock again; Open=1 stored at 0x00B7862E (scheduled into the next block)
-- set-up A
00B7861B  rsm.SetFogEnable(false);                    // rsm = *g_pRenderManagerCore (0x01436E18, reloaded before each setter)
00B7868E  SetCommonRenderStates();   // inline, expansion 1:
            SetSpecularEnable(false) 0x00B7868E; SetZBufferEnable(true) 0x00B786F7; SetZBufferWriteEnable(true) 0x00B78761;
00B787C6    if (Settings.EnableBumpMap) {             // 0x28d; je 0x00B789D2
00B787E4      SetTextureBumpEnvMatrix(1, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);  // Mat00 0x37b8, Mat10 0x38f8, Mat01 0x3858, Mat11 0x3998
00B78988      SetTextureBumpEnvMatrix(2, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);  // Mat00[2] 0x37cc ... jmp 0x00B78BBD (tail-merged into else's stage-2 body)
            } else {
00B789D2      SetTextureBumpEnvMatrix(1, 0.0f, 0.0f, 0.0f, 0.0f);
00B78B79      SetTextureBumpEnvMatrix(2, 0.0f, 0.0f, 0.0f, 0.0f);  // shared tail 0x00B78C04..0x00B78D30 (Mat10[2], Mat01[2], Mat11[2])
            }
00B78D30    SetAlphaTestEnable(true); 00B78D9C SetAlphaTestFunc(ALPHA_TEST_GREATER) [0x01396DE8];
00B78E0B    SetAlphaRef(1); 00B78E77 SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA) [0x01396F78];
00B78EE6    SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA) [0x01396F7C]; 00B78F55 SetAlphaBlendEnable(true);
00B78FC9  rsm.SetFogEnable(true);
00B7902E  g_pShaderRenderManager->ResetWorldTransform();                         // [0x01436E14] -> 0x00988290
-- passes 1
00B79039  if (Settings.EnableForeground && fg && HighendWaterEnabled) RenderForeground(time);        // 0x289, bl, 0x274 -> 0x00B761D0
00B79061  if (Settings.EnableBackground && bg) RenderBackground(time);                             // 0x28a, [0x12] -> 0x00B76BC0
00B7907B  if (Settings.EnableIceForeground && iceFg && HighendWaterEnabled) RenderIceForeground(time);   // 0x28b,[0x16],0x274 -> 0x00B751F0
00B7909F  if (Settings.EnableIceBackground && iceBg && HighendWaterEnabled) RenderIceBackground(time);   // 0x28c,[0x13],0x274 -> 0x00B75CA0
00B790C3  if (Settings.EnableSea) {                                                                  // je 0x00B7A7BF (threaded past the 2nd test)
00B790D1    if (Settings.EnableSeaForeground && seaRefl && HighendWaterEnabled) RenderSeaReflectiveForeground(time);  // 0x286,[0x17],0x274 -> 0x00B77370
          }
00B790F5  if (Settings.EnableSea && (seaNonRefl || seaMid || seaBg)) {                              // 0x285 re-read; [0x14],[0x3c],[0x15]; else -> 0x00B7A7BF
00B7911F    block.Restore();                         // Open=0 (0x00B79125) then RestoreCaptureBlock 0x00A05840 on [0x013CB508]
00B7912F    rsm.AttachTextureToStage(0, NULL); (1, NULL); (2, NULL); (3, NULL);   // CurrentlySelectedTextures[s] (0x3d00+4s) != 0 -> =0, SetTexture(s,0) (+0x104);
                                                                                  // if (TextureStagesInUse 0x3cb8 < s+1) = s+1. ebx = 0 here.
00B79241    if (HighendWaterEnabled) {
00B7924B      g_pEngineSkyRenderer->GetEnvironmentMap(EnvironmentMap, params);   // [0x01436E50], push ebp(params), push [this+0x1e0]; 0x00B665E0
00B7925E      EnvironmentMap->GenerateMipmapsWithGPU();                         // 0x009FAF70 (reloads this+0x1e0)
            }
00B7926F    rsm.SetFogEnable(false);
00B792D7    SetCommonRenderStates();   // expansion 2: Spec 0x00B792D7, Z 0x00B7933F, ZW 0x00B793AC, bump test 0x00B79414
                                       //   (enabled 0x00B79432 / stage 2 0x00B795D9 .. jmp 0x00B79A64; disabled 0x00B7975C / stage 2 0x00B798FF),
                                       //   AT 0x00B79ABA (ebp=1), ATF 0x00B79B27, AR 0x00B79B95, Src 0x00B79BFD, Dst 0x00B79C6B, AB 0x00B79CD9
00B79D41    rsm.SetFogEnable(true);
00B79DA9    SetCommonRenderStates();   // expansion 3 (NO fog around it): Spec 0x00B79DA9, Z 0x00B79E15, ZW 0x00B79E7D, bump test 0x00B79EDF
                                       //   (enabled 0x00B79F01 -> jmp 0x00B7A270; disabled 0x00B7A0BB; stage 2 fully shared 0x00B7A270..0x00B7A415),
                                       //   AT 0x00B7A415, ATF 0x00B7A47A, AR 0x00B7A4E5, Src 0x00B7A54A, Dst 0x00B7A5B5, AB 0x00B7A620
00B7A685    if (Settings.EnableSeaForeground && seaNonRefl && HighendWaterEnabled) RenderSeaNonReflectiveForeground(time);  // 0x286,[0x14],0x274 -> 0x00B77E20 (ebx = this now)
00B7A6AD    if (Settings.EnableSeaMidground && seaMid) RenderSeaMidground(time);                // 0x287,[0x3c] -> 0x00B6FA40
00B7A6CB    if (Settings.EnableSeaBackground && seaBg) {                                        // 0x288,[0x15]
00B7A6E5      rsm.SetAlphaTestEnable(false); 00B7A74C rsm.SetAlphaBlendEnable(false);           // edi = 0
00B7A7B1      RenderSeaBackground(time);                                                        // 0x00B70230 (ecx reloaded from [0x18])
            }
          }
-- tail
00B7A7BF  rsm.SelectVertexBuffer(0, NULL);            // PD3DDevice->SetStreamSource(0,0,0,0) (+0x190)
00B7A7DC  rsm.SelectIndexBuffer(NULL, 0);             // PSelectedIndexBuffer(0x3cf4)=0; SetIndices(0) (+0x1a0)
00B7A7FC  FinishUsingDynamicTextures();               // 0x00B6D630
00B7A805  ~block:   if (Open) RestoreCaptureBlock();
00B7A818  ~shaders: if (Old != VertexShadersEnabled) Old ? EnableVertexShaders : DisableVertexShaders; same for pixel (0x00B7A839)
00B7A85A  ~capture: RestoreCaptureBlock();
00B7A865  epilogue
```
Notes for the author
- "rsm" = `*g_pRenderManagerCore` (CRenderManagerCore* at 0x01436E18); each setter re-loads the global (59 loads), so call through
  the global every time, don't cache a reference.
- SetCommonRenderStates must be a FORCEINLINE private member (overlay) -- three expansions in one function will exceed /Ob2 budget otherwise
  (same lesson as the FORCEINLINE setters, CRenderStateManager.h:14-15).
- SetTextureBumpEnvMatrix stage-2 code is tail-merged differently in each of the three expansions -- that is optimiser behaviour, not
  source; write the same if/else each time.
- Expected residue class: bookmark `test`/`or` operand order per SetRenderState block (header comment, CRenderStateManager.h:21-22;
  agent2048 NOTES/LESSONS: the named `desired_state` local spelling).
