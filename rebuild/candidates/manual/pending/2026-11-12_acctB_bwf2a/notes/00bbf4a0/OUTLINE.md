# OUTLINE 0x00BBF4A0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)

## Frame
E = esp at entry ([E]=ret, E+4 = &sub, E+8 = pass). `sub esp,38h` -> F = E-0x38. Saved: edi (0xBBF4A8), ebx+esi (0xBBF4C4/C5, after the
switch test), ebp (0xBBF5E0, after the GetFrame early-out). No EH, no cookie, `ret 8`.
| slot | holds |
|---|---|
| F+0x00 | this (spilled at 0xBBF4AD) |
| F+0x04 | CGraphicFrame frame (FrameData; =0 store at 0xBBF510); dtor expanded at 0xBBF51D and 0xBC0490 |
| F+0x08 | group (CEngineInternalPrimitiveParticleSpriteGroup*) |
| F+0x0C | info (const CGraphicInfo*) |
| F+0x10 | fild int temp (colour bytes) / copy of `pos` (cos) |
| F+0x14 | fistp result i (cos) ; F+0x18 pos copy (sin) ; F+0x1C fistp i (sin) ; F+0x20 pos-0.5 (cos) ; F+0x24 pos / pos-0.5 (sin) / fild temp |
| F+0x28..0x34 | CRGBFloatColour colour {R,G,B,A} |
| E+4 (sub slot) | bool additive (byte); later CShaderRenderManager* in the tail flush; then 3-byte CEnginePrimitiveThemeChannelDesc temp (RestoreDefaultPrimitiveParameters) |
| E+8 (pass slot) | float zOffset (memory var, =0 at 0xBBFECE); then list node iterator; fild temp |
Registers: edi=0 early; ebx = group_manager (0xBBF5D6) then &ConstantBlock[0] (0xBBFF3F); ebp = 0x10 (pass 7 AlphaRef) / group / this; esi = count in loop.

## Pseudo-C++ in retail order
```
// 0xBBF4A0
if (!g_particleSpriteRenderEnabled_0139b6e0) return;                    // je 0xBC04E2 (bare epilogue, frame not constructed)
CEngineInternalPrimitiveParticleSpriteGroup* group = (..*)sub.GetParentBase();     // [sub+0xC]
const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(group->SpriteBankIndex);
        // inline: list +0x1E0..0x1E4 (0x2C elems), index < size ? &list[index] : 0   (0xBBF4C6..0xBBF504)
CGraphicFrame frame;                                                    // FrameData = 0
if (!info->GetFrame(frame, 0, 0, 0)) return;                            // call 0x009FF8D0 @0xBBF514; fail path 0xBBF51D: ~CGraphicFrame inline, ret 8 @0xBBF577
// 0xBBF57A
NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.FrameData->GetTexture());
        // inline: tex = PTexture.Data ? Data->PD3DTexture : 0; if (CurrentlySelectedTextures[0] != tex) {store; PD3DDevice->SetTexture(0, tex) [+0x104]}
        //         if (TextureStagesInUse < 1) TextureStagesInUse = 1;       (0xBBF5B6..0xBBF5CB)
CEnginePrimitiveManagerParticleGroup* group_manager = GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24), (CEnginePrimitiveManagerParticleGroup*)0);  // ebx @0xBBF5D6
bool additive;                                                          // lives in the `sub` arg slot
if (pass == 6 /*RENDER_PASS_DISPLACEMENT_BUFFER*/ && g_pDisplacementRenderer_01436e38->GetEnable3DDisplacement()) {   // 0xBBF5DD / 0xBBF5F2 (+0xEF); false -> else-branch 0xBBFA1D
    additive = false;                                                   // 0xBBF614
    SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);                   // BlendOpMode +0x2C3C <- table 0x1396ED0[0]   0xBBF600..0xBBF66F
    SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA); // Src +0x291C <- [4] 0x1396F78 (0xBBF674), Dest +0x2930 <- [5] 0x1396F7C (0xBBF6E3); return value unused
    // 0xBBF752: shader choice on group->Flags (word +0xB0)
    if (group->Flags & 1) Apply3DFaceMeDisplacementVertexShader();    // this+0x40 -> 0xBBF7F0 shared call
    else if (group->Flags & 0x40) { RM->SetPolyCullMode(POLY_CULL_NONE /*[2] 0x1396FB8*/); ApplyCrossedSpriteDisplacementVertexShader(); }  // +0x2890; this+0x38; 0xBBF773..0xBBF7E7
    else ApplyBillboardDisplacementVertexShader();                       // this+0x30 (0xBBF7E9)
    // all three tail-merge into ApplyVertexShader(.., false) call @0xBBF7F9
    ApplyPixelShader(PixelShaderDisplacement);                           // this+0x58, call 0x00988140 @0xBBF808 ; jmp 0xBBFE4E
} else if (pass == 7 /*RENDER_PASS_DISPLACEMENT_STENCIL*/) {           // 0xBBF812
    additive = false;                                                   // 0xBBF829
    RM->SetAlphaTestEnable(true);   // +0x28CC <- 1 (edi)   0xBBF81B
    RM->SetAlphaRef(16);            // +0x28E0 <- 0x10 (ebp) 0xBBF887
    RM->SetAlphaTestFunc(ALPHA_TEST_GREATER);  // +0x28A4 <- [4] 0x1396DE8  0xBBF8EE
    // 0xBBF95D: same 3-way displacement shader choice as pass 6 (blocks 0xBBF968..0xBBF9FD), call 0x00988020 @0xBBFA04
    ApplyPixelShader(PixelShaderDisplacement);                           // @0xBBFA13 ; jmp 0xBBFE52 (skips the `this` reload)
} else {                                                                // 0xBBFA1D
    SetupBlendOpState(group->BlendOp);                                   // +0xB8: 1 -> [1] 0x1396ED4, 2 -> [2] 0x1396ED8, else [0]; one SetRenderState expansion @0xBBFA51 (NO enable switch, unlike 2059)
    additive = SetupAlphaBlendingStatesFromBlendMode(group->BlendMode);  // +0xB4 @0xBBFAAC:
        //   default (0xBBFACE): Src [4] / Dest [5] (0xBBFB30), additive=0 @0xBBFB98, jmp 0xBBFCDD
        //   ==4 ADDSMOOTH (0xBBFBA2): Src [1] 0x1396F6C, then Dest [3] 0x1396F74 via shared block 0xBBFC76
        //   ==3 ADDITIVE  (0xBBFC0C): Src [1], Dest [1] via 0xBBFC76; both: additive=1 @0xBBFCD8
    // 0xBBFCDD
    unsigned long lights = 0;
    if (group->Flags & 0x80) lights = group_manager->SetUpPointLights();     // call 0x00BBC0B0 @0xBBFCEB
    else { g_lighting->SetSelfIlluminationAmbient(100); g_lighting->UpdateGlobalLightingStates(); }  // +0xE8 / |=2 at +0xE4; call 0x00B46C80 @0xBBFD1E
    // 0xBBFD23
    if (group->Flags & 1) Apply3DFaceMePointLightVertexShader(lights);       // arr +0x24; idx = lights > size-1 ? size-1 : lights; jmp to shared call 0xBBFE28/0xBBFE34
    else if (group->Flags & 0x40) { RM->SetPolyCullMode(POLY_CULL_NONE); ApplyCrossedSpritePointLightVertexShader(lights); }  // 0xBBFD5F..; arr +0x18; own call 0xBBFDFD; jmp 0xBBFE39
    else ApplyBillboardPointLightVertexShader(lights);                       // 0xBBFE04, arr +0x0C, call @0xBBFE34
    group_manager->SetupPixelShader(0, &PixelShaderStipple, &PixelShader);   // 0xBBFE39: push this+0x48, this+0x50, 0; call 0x00BBCC70 @0xBBFE49
}
// 0xBBFE4E (common)
NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &VertexBuffer);   // null-tested this+0x60: SetStreamSource(0, vb->PVertexBuffer, 0, vb->Stride) else (0,0,0,0) [+0x190]
NEngineInternal::GPRenderManager->SelectIndexBuffer(&IndexBuffer, 0);      // PSelectedIndexBuffer +0x3CF4 = this+0x94; SetIndices(ib ? ib->PD3DIndexBuffer : 0) [+0x1A0]
// 0xBBFEBB  two floats, both 0.0f: zOffset (memory, pass slot) and wOffset (x87)
float zOffset = 0.0f, wOffset = 0.0f;           // names descriptive
if (group->Flags & 2) zOffset = 1.0f;          // 0xBBFECC
else if (group->Flags & 4) zOffset = -1.0f;    // 0xBBFEE0
else if (group->Flags & 8) wOffset = -1.0f;    // 0xBBFEEC
else if (group->Flags & 0x10) wOffset = 1.0f;  // 0xBBFEFA
const CMatrix3x4& m = GetCurrentCamera().<InverseNoScaleViewTransform accessor>;   // cam+0x144 (0xBBFF0A)
ConstantBlock[0] = { m[+0x00], m[+0x0C], zOffset, wOffset };                    // +0xA8..+0xB4
ConstantBlock[1] = { m[+0x04], m[+0x10], (float)hdr.w6 / (float)hdr.w0, (float)hdr.w8 / (float)hdr.w2 };   // info words +6/+0, +8/+2 (fild/fidiv)
ConstantBlock[2] = { m[+0x08], m[+0x14], (group->Flags & 0x20) ? 1.0f : 0.0f, <W untouched> };              // 0xBBFFA9
NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(1, 3, &ConstantBlock[0].X);   // push ebx,3,1 ; call 0x009896D0 @0xBBFFD1
// 0xBBFFD6  loop over group->Primitives (std::list<CEnginePrimitiveHandle>, head +0x48)
unsigned long count = 0;                        // esi
for (it = group->Primitives.begin(); it != end(); ++it) {           // body 0xBBFFF0, iterator in the pass slot; latch 0xBC02E5..0xBC02F6
    const CEngineInternalPrimitiveParticleSprite& sprite = (..&)it->GetData();   // node+0xC
    if (sprite.Size.X > 0 && sprite.Size.Y > 0                          // fcomp 0.0 ; test ah,41h ; jne (skip when <= 0)
        && info->GetFrame(frame, sprite.Frame, 0, 0)) {                // call 0x009FF8D0 @0xBC0030 (same frame local)
        CRGBFloatColour colour(sprite.Colour);                         // R=byte+0x62, G=+0x61, B=+0x60, A=+0x63, *1/255
        colour.A *= 2.0f;  /* emitted as fadd st,st before the store @0xBC0097 */
        if (additive) { colour.R *= colour.A; colour.G *= colour.A; colour.B *= colour.A; }   // 0xBC006B..0xBC00BF
        ConstantBlock[count].X/Y/Z = sprite.WorldPos.X/Y/Z;  ++count;            // 0xBC00C3 (W not written)
        ConstantBlock[count] = { GFFastCos(sprite.Angle), GFFastSin(sprite.Angle), sprite.Size.X, sprite.Size.Y };  ++count;   // 0xBC00E0..0xBC019B (pos=Angle*1024 CSE'd)
        ConstantBlock[count] = { colour.R, colour.G, colour.B, colour.A };  ++count;                                     // 0xBC019E..0xBC01C6
        ConstantBlock[count].X = frame.FrameData->TLX * (1/32767.f); .Y = TLY * (1/32767.f);  ++count;   // 0xBC01C2..0xBC0200 (Z,W untouched)
        if (count == 40) {                                               // cmp esi,28h @0xBC01F3
            SRM->SetUserVertexShaderConstants(4, count, &ConstantBlock[0].X);   // push ebx, esi(40), 4 @0xBC0213
            SRM->PrepareForRender(0x15);                                    // 0xBC0218..0xBC02CB
            RM->DrawCurrentBuffersAsIndexedTris(0, 40, 0, 20);              // @0xBC02DE
            count = 0;
        }
    }
}
// 0xBC02FC
if (count) {
    unsigned long sprites = count >> 2;          // shr esi,2 ; ebp = sprites*4
    SRM->SetUserVertexShaderConstants(4, sprites * 4, &ConstantBlock[0].X);    // @0xBC0318
    SRM->PrepareForRender(0x15);                                              // 0xBC031D..0xBC03D4 (SRM kept in the `sub` slot)
    RM->DrawCurrentBuffersAsIndexedTris(0, sprites * 4, 0, sprites * 2);      // @0xBC03E8
}
// 0xBC03ED
g_lighting->RestoreDefaultPrimitiveParameters();   // +0xCE fade flag (|1 at +0xE4), theme channel {0,0,0} temp at E+4 (+0xE0..E2, |0xF), self-illum 0 (|2)
// 0xBC0490  ~CGraphicFrame (inline) ; pop esi/ebx/edi ; add esp,38h ; ret 8 @0xBC04E6
```
Notes: no cold blocks after the epilogue; no jump tables (the BlendOp / BlendMode switches compile to dec/sub chains). The 3DFaceMe and
billboard point-light paths share one ApplyVertexShader call (0xBBFE28/0xBBFE34); the crossed path has its own (0xBBFDFD).
