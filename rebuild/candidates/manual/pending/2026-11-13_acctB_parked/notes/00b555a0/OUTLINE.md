# OUTLINE 0x00B555A0 CEngineWeatherRenderer::Render(const CEngineComponentRenderParams&)

## Frame
- `sub esp,0Ch`, no ebp frame, no EH, no /GS, `ret 4`. Pushes ebx (0xB555B7), ebp (0xB555BE), esi (0xB555BF), edi (0xB555EB); pops 0xB5643C..3F.
- ebx = 1 from 0xB555E4 to the end (the `true` / 1 values and every `DirtyListFlag = true` store `mov [edx+10h],bl`).
- ebp = &params from 0xB55E34 (`mov ebp,[esp+20h]`); before that ebp is a scratch for desired values.
- Frame map (orig = esp after `sub esp,0Ch`):
  - orig+0 (1 byte)  CScopedShadersEnable shaders.OldVertexEnable  (ctor 0xB555EC store; dtor read 0xB56432)
  - orig+1 (1 byte)  shaders.OldPixelEnable (ctor 0xB55603; dtor 0xB56457)
  - orig+2..3        unused (the capture block's bool Open, if modelled, is dead)
  - orig+4           `this` spill (0xB555A8; reloaded 0xB55E28 and 0xB563DC)
  - orig+8           CScopedVertexShaderConstantLayout layout.OldIndex (0xB5561E store from [0x013BC478]; dtor push 0xB56423)
  - orig+0x10        params (argument)

## Source sketch (retail order)
```cpp
void CEngineWeatherRenderer::Render(const CEngineComponentRenderParams& params)
{
    if (!RenderEnabled)                       // 0xB555A3 test [ecx+8]; je 0xB56492
        return;
    CRenderStateScopedCaptureBlock capture;   // 0xB555B2 inline StartCaptureBlock on [0x013CB508]:
                                              //   StateStack[StateStackSize].State = &BookmarkDummyState (+0x3A24);
                                              //   ++StateStackSize (+0x2808); CurrentBookmarkMask (+0x2814) <<= 1
    CScopedShadersEnable shaders(true, true); // 0xB555DF old = [0x013BC800]; if (old != 1) EnableVertexShaders 0x987FE0 (0xB555F7)
                                              // 0xB555FC old = [0x013BC801]; if (old != 1) EnablePixelShaders 0x988110 (0xB5560E)
    CScopedVertexShaderConstantLayout layout(1); // 0xB55613 old = [0x013BC478]; SetVertexShaderConstantLayout(1) 0x98B5E0 (0xB55622)

    // section 1: each line = one inline SetRenderState on [0x01436E18], global re-read after every changed state
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);                    // 0xB55627 (+0x2854)  join 0xB55696
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);                        // 0xB55696 (+0x28CC)  join 0xB556FC
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);            // 0xB556FC (+0x28A4, [0x01396DE8]) join 0xB5576A
    NEngineInternal::GPRenderManager->SetAlphaRef(1);                                  // 0xB5576A (+0x28E0)  join 0xB557D0
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);                       // 0xB557D0 (+0x28B8)  join 0xB55836
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);  // 0xB55836 (+0x291C, [0x01396F78]) join 0xB558A4
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);// 0xB558A4 (+0x2930, [0x01396F7C]) join 0xB55912
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);           // 0xB55912 (+0x2EE4, [0x01396DC8])
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);           // 0xB5597E (+0x2F84)
    NEngineInternal::GPRenderManager->SetTextureUVModeW(0, TEXTURE_UV_CLAMP);          // 0xB559EA (+0x3024, [0x01396DD0])
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);           // 0xB55A56 (+0x2EF8)
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);           // 0xB55AC2 (+0x2F98)
    NEngineInternal::GPRenderManager->SetTextureUVModeW(1, TEXTURE_UV_CLAMP);          // 0xB55B2E (+0x3038)
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_WRAP);           // 0xB55B9A (+0x2F0C)
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_WRAP);           // 0xB55C06 (+0x2FAC)
    NEngineInternal::GPRenderManager->SetTextureUVModeW(2, TEXTURE_UV_CLAMP);          // 0xB55C72 (+0x304C)
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_WRAP);           // 0xB55CDE (+0x2F20)
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_WRAP);           // 0xB55D4A (+0x2FC0)
    NEngineInternal::GPRenderManager->SetTextureUVModeW(3, TEXTURE_UV_CLAMP);          // 0xB55DB6 (+0x3060) join 0xB55E1D
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();                     // 0xB55E1D ecx=[0x01436E14]; call 0x988290

    if (Settings.Raining)                     // 0xB55E2C [esi+5D0h]
        RenderRain(params);                   // 0xB55E3D call 0xB54890
    if (Settings.Snowing)                     // 0xB55E42 [esi+5D1h]
        RenderSnow(params);                   // 0xB55E4F call 0xB54E30
    if (Settings.Mist) {                      // 0xB55E54 [esi+5D2h]; je 0xB563E6
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);                     // 0xB55E62 (+0x2868 = ZBufferEnable, NOT write) join 0xB55ED1
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_MIRROR);     // 0xB55ED1 ([0x01396DCC])
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_MIRROR);     // 0xB55F43
        NEngineInternal::GPRenderManager->SetTextureUVModeW(0, TEXTURE_UV_MIRROR);     // 0xB55FAF
        NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_MIRROR);     // 0xB5601B
        NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_MIRROR);     // 0xB56087
        NEngineInternal::GPRenderManager->SetTextureUVModeW(1, TEXTURE_UV_MIRROR);     // 0xB560F3
        NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_MIRROR);     // 0xB5615F
        NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_MIRROR);     // 0xB561CB
        NEngineInternal::GPRenderManager->SetTextureUVModeW(2, TEXTURE_UV_MIRROR);     // 0xB56237
        NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_MIRROR);     // 0xB562A3
        NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_MIRROR);     // 0xB5630F
        NEngineInternal::GPRenderManager->SetTextureUVModeW(3, TEXTURE_UV_MIRROR);     // 0xB5637B join 0xB563DC
        RenderMist(params);                   // 0xB563DC ecx=[esp+14h]; call 0xB54310
    }
    NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);   // 0xB563E6 [+0x3CF4]=0; PD3DDevice(+0x3CF0)->SetIndices(0) vt+0x1A0 (0xB56400)
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);  // 0xB56406 reload global; SetStreamSource(0,0,0,0) vt+0x190 (0xB5641D)
}   // 0xB56423 ~layout: SetVertexShaderConstantLayout(old) (0xB5642D)
    // 0xB56432 ~shaders vertex: if (old != [0x013BC800]) old ? Enable 0xB5644B (jmp 0xB56457) : Disable 0x9880E0 (0xB56452)
    // 0xB56457 ~shaders pixel: if (old == [0x013BC801]) -> 0xB56487; old ? EnablePixelShaders 0xB5646C then tail-dup
    //          RestoreCaptureBlock 0xB56477 + `add esp,0Ch ; ret 4` 0xB5647C : DisablePixelShaders 0xB56482
    // 0xB56487 ~capture: [0x013CB508]->RestoreCaptureBlock() 0xB5648D; 0xB56492 `add esp,0Ch ; ret 4` (also the early-return target)
```
- Inline expansion of each setter = CRenderStateManager::SetRenderState (repo engine/CRenderStateManager.h, retail branch; variant
  FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED in agent2048's overlay) + AddStateToUpdateList: `if (desired != value) { if (!(mask & Current))
  { mask |= Current; StateStack[n].State=&s; StateStack[n].Value=desired; ++n; } desired = value; if (!Dirty) { Dirty = true; UpdateList[m++] = &s; } }`.
  Retail stores `DesiredState = value` BEFORE testing DirtyListFlag (`mov al,[edx+10h] ; test al,al ; mov [edx+4],..`) -- same as matched rows.
- The three scope objects: repo headers engine/CRenderStateCaptureBlock.h (default branch: ctor = StartCaptureBlock(this), dtor = RestoreCaptureBlock),
  engine/CScopedShadersEnable.h, engine/CScopedVertexShaderConstantLayout.h -- declaration order capture, shaders, layout (retail ctor/dtor order proves it;
  ApplyRadialBlur 0x00B5E0F0 uses capture, layout, shaders -- do NOT copy its order).
- No jump tables, no cold blocks beyond the duplicated destructor tail, no loops (the 12 UV calls are written out; VC7.1 does not unroll a stage loop).
- Needed includes / opt-ins (start): `#define FABLE_RETAIL_CRENDERMANAGERCORE`, FABLE_RETAIL_CENGINEWEATHERRENDERER, _BUFFERS, _RENDERRAIN, _RENDERSNOW,
  FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL, + new _RENDER opt-in (Render override, RenderMist decl); engine/CRenderManager.h, CShaderRenderManager.h,
  CRenderStateCaptureBlock.h, CScopedShadersEnable.h, CScopedVertexShaderConstantLayout.h, CEngineWeatherRenderer.h; overlay from agent2048/hdr.

## Register / join facts per section (second scout pass; check your diff against these)
- Section 1 blocks (0xB55627..0xB55E1D): ecx = manager, edx = &state, eax = manager / scratch, esi = scratch / desired, ebx = 1. Each block's join label is the
  FIRST instruction of the next block, which (for table values) RE-LOADS the table: e.g. 0xB5597E `mov edi,[0x1396DC8]` is the V0 block start and the U0
  skip-branch target. The changed path ends with `mov eax,[0x01436E18]` (e.g. 0xB55979) just before the label.
- Constant-value blocks: block 1 (ZWrite 0) `mov esi,[eax+2858h] ; test esi,esi`; blocks 2,4,5 (value 1) `cmp esi,ebx`; desired store `mov [edx+4],0` (imm) or
  `mov [edx+4],ebx`. Table blocks: `mov edi,[tbl]` / `mov esi,[eax+X+4] ; cmp esi,edi` (UV) or `mov esi,[tbl] ; mov ebp,[eax+X+4] ; cmp ebp,esi ; mov edi,esi` (3,6,7).
- Mist section (0xB55E62..0xB563DC): ZBufferEnable block like section 1 (edx = &state, ecx = manager). UV blocks: esi = &state, edx = manager, ecx = table value,
  edi = copy; the compare is `cmp [eax+X+4],ecx` (memory operand) and the join label is AT that cmp (e.g. 0xB55F43): the table is reloaded only on the changed
  path (`mov eax,[0x01436E18] ; mov ecx,[0x1396DCC]` at 0xB55F38/3D) -- the unchanged path reuses ecx. The LAST block (W3, 0xB5637B) swaps roles: esi = manager,
  edx = &state. That is why `this` must be reloaded from the stack spill at 0xB563DC (`mov ecx,[esp+14h]`) for RenderMist.
- Block start addresses for the mist UV calls (labels = the cmp): U0 0xB55ED1 (starts with the table load, no previous UV), V0 0xB55F43, W0 0xB55FAF,
  U1 0xB5601B, V1 0xB56087, W1 0xB560F3, U2 0xB5615F, V2 0xB561CB, W2 0xB56237, U3 0xB562A3, V3 0xB5630F, W3 0xB5637B; join after W3 = 0xB563DC.
- Section 1 values (verified per table load): ZWrite 0 / ATestEnable 1 / ATestFunc tbl[0x1396DE8]=GREATER / ARef 1 / ABlendEnable 1 / Src tbl[0x1396F78]=SRC_ALPHA /
  Dest tbl[0x1396F7C]=INV_SRC_ALPHA / per stage 0..3: U WRAP [0x1396DC8], V WRAP [0x1396DC8], W CLAMP [0x1396DD0]. Mist: ZBufferEnable(false) then all 12 MIRROR [0x1396DCC].
