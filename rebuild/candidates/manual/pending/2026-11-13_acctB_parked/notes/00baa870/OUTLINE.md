# OUTLINE 0x00BAA870 CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite)

## Frame facts
- `sub esp,124h; push ebp; push edi` (ebp = this, edi = &sprite) ... `push ebx; push esi` after the IsLoaded test. No frame pointer, no EH
  (TU without /EHsc), no /GS. `ret 4`. Offsets below are esp-relative AFTER all four pushes ("base"): locals 0x10..0x133, sprite arg 0x138.
- Frame map (base offsets):
  0x10,0x14,0x18,0x1C float scratch (math); late reuse: 0x10 = count*2, 0x14 = count*4, 0x18 = GPShaderRenderManager, 0x1C = loop base
  0x20..0x28 C3DVector scratch; 0x20/0x24 = CEngineFadeDistance fade (GetTransitionFade result); late: const CTexture* textures[2] {0x20,0x24}
  0x2C..0x58 CMatrix3x4 tex = camera->NoScaleViewTransform (rep movsd 12) then rescaled in place
  0x5C..0x64 C3DVector; 0x68 this (stored 0x00BAA884); 0x6C..0x74 C3DVector; 0x78..0x80 C3DVector centre (copied from Mesh->BoundingSphere)
  0x84 float radius; 0x88..0x90 C3DVector; 0x94/0x95 CScopedShadersEnable shaders(true,true)
  0x98/0x9C CCountedPointer<const C3DMesh2> mesh3d (dies at 0x00BAA9B9, out-of-line dtor 0x00A36EC0); 0x98 then reused by
  CScopedVertexShaderConstantLayout layout(2) (old index); 0xA0..0xA8, 0xB0..0xB8, 0xBC..0xC4 C3DVector; 0xAC unsigned long query;
  0xC8, 0xCC float; 0xD4..0x100 CMatrix3x4 inv = camera->InverseNoScaleViewTransform; 0x104..0x133 CPreTransposedBoneMatrix bone.

## Blocks (retail order)
```
0x00BAA870  C3DMeshInfo* mesh = sprite.Mesh;
            if (!mesh->IsLoaded()) {                                    // call 0x00A23E40
                mesh->BeginLoading(0, false, false); return;           // 0x00BAA89A, epilogue 0x00BAA89F
            }
0x00BAA8AA  if (sprite.Mesh->GetBestCurrentDetailLevel() != 0)          // inline (agent2028 overlay C3DMeshInfo.h): MeshData null -> -1;
                sprite.Mesh->BeginLoading(0, false, false);            //   loop 0x00BAA8C0 HasLOD(i)=i<4&&LODDataSizes[i]; LODMeshes[i] -> i
                                                                       //   call 0x00BAA8DD; join 0x00BAA8E2
0x00BAA8E2  if (CurrentRenderedMesh != sprite.Mesh) {                   // this+0xC0
                CCountedPointer<const C3DMesh2> mesh3d;                 // zero-init 0x98/0x9C
                if (!sprite.Mesh->GetMesh(mesh3d, 0, g_pEnginePrimitiveRenderer->BlockOnLoad ? 0x11 : 0)) {   // neg/sbb/and 11h; call 0x00A25E70
                    /* ~mesh3d 0x00A36EC0 */ return;                    // 0x00BAA926, epilogue 0x00BAA932
                }
0x00BAA93F      if ([0x01436E88]->byte_0x24) {
                    GPLightingManager->SetEnableFadedShadowFactor(GPLightingManager->ShadowMode == 1);   // inline 0x00BAA94B..0x00BAA971
                    GPLightingManager->UpdateGlobalLightingStates();   // 0x00B46C80
                    g_pBillboardGenerator(0x01436E4C)->RenderBillboard(mesh3d.Data, &BillboardTexture,
                                     sprite.MinDistance, sprite.MaxScale, sprite.AlphaRef /*byte +0x28 -> long*/);   // 0x00B8B780
                }
0x00BAA9A9      CurrentRenderedMesh = sprite.Mesh;
            }   // ~mesh3d -> call 0x00A36EC0 (0x00BAA9B9)
0x00BAA9BE  CScopedShadersEnable shaders(true, true);                   // cmp [0x013BC800],1 -> EnableVertexShaders 0x00987FE0;
                                                                       // cmp [0x013BC801],1 -> EnablePixelShaders 0x00988110
0x00BAA9F2  CScopedVertexShaderConstantLayout layout(2);                // save [0x013BC478]; SetVertexShaderConstantLayout(2) 0x0098B5E0
0x00BAAA0B  CRenderStateScopedCaptureBlock capture;                     // inline StartCaptureBlock on CRenderStateCaptureBlock::StateManager
0x00BAAA39  GPLightingManager->SetEnableFadedShadowFactor(GPLightingManager->ShadowMode == 1); ->UpdateGlobalLightingStates();  // 2nd copy
0x00BAAA6A  GPShaderRenderManager->ApplyVertexShader(&VertexShader, false);   // 0x00988020
            GPShaderRenderManager->ApplyPixelShader(&PixelShader);           // 0x00988140
0x00BAAA8E  ---- maths (pure x87, no calls until 0x00BAAEC4) ----
            mesh = sprite.Mesh (ebp); camera = g_pEngineCamera (ebx)
            C3DVector centre = mesh->BoundingSphere.Center (0x78..0x80, copied as dwords); float radius = BoundingSphere.Radius (0x84)
            CMatrix3x4 tex = camera->NoScaleViewTransform (0x2C)
            tex.pos.x(0x50) = dot(col0(tex), -centre); tex.pos.y(0x54) = dot(col1,-centre); tz = dot(col2,-centre)   [m0*-x+m3*-y+m6*-z]
            float inv = 1.0f / radius (0x1C);  s = 0.5f*inv, t = -0.5f*inv
            m0,m3,m6,pos.x *= s ; m1,m4,m7 *= t ; (0x00BAAB87 copies inv = camera->InverseNoScaleViewTransform to 0xD4)
            pos.y = t*pos.y ; m2,m5,m8 *= 0.0f ; pos.z(0x58) = tz*0.0f ; pos.x += 0.5f ; pos.y += 0.5f   (=> scale(0.5/r,-0.5/r,0)+0.5 texture proj)
0x00BAAC06  ux = (inv.m0 + inv.m6)*0.0f + inv.m3 ; uy = (inv.m1 + inv.m7)*0.0f + inv.m4    (camera up projected; 0.0 = 0x0122DEDC)
            float k = GFFastOneOverSqrt(ux*ux + uy*uy)  [0x00BAAC46..0x00BAACA0; len2 at 0x14, estimate 0x10]
            ... normalised (ux,uy)*k -> 0x6C/0x70; vertical extent from mesh->BoundingBox (min.z +0x18, max.z +0x24):
            0x64 = (max.z+min.z)*0.5*k*0  + (max.z-min.z)*0.5 ... ; scaled-by-radius vectors 0x20..0x28 and 0x88..0x90 ;
            transformed points 0xA0..0xA8, 0xB0..0xB8 (tex * point), 0xCC   [follow listing 0x00BAACDC..0x00BAAEA5 literally]
0x00BAAEA7  float p1 = pow(fabsf(v70 - vB4) * VerticalControlPointWeightingFactor, ControlPointWeightingPower);   // _CIpow 0x00BFEB40
0x00BAAECD  float p2 = pow(fabsf(vA4 - vCC), ControlPointWeightingPower);   (fabs results also kept in 0x18)
            float w = 1.0f/(p1+p2); a = w*p2 (0x14); b = w*p1 (0x10)  -> blended vectors 0x2C.., 0x5C..0x64, 0x88..0x90, 0xBC..0xC4, 0xB4
0x00BAB045  mesh->BoundingBox / sphere: d = sqrtf(dx*dx+dy*dy+dz*dz) with (x - y)*0.5 halves (0x00BAB078..0x00BAB0D7 fsqrt)
            f = d * inv * 0.5 ; g = v6C - f
0x00BAB0EB  v1C = (0.0f < g) ? g : 0.0f   ( fcomp; test ah,41h; jne -> store; else 0 )          // clamp low
0x00BAB10A  v18 = (1.0f > g + v6C?) ... : 1.0f   ( test ah,5; jp -> store; else 1.0f )         // clamp high
0x00BAB12D  CEngineFadeDistance fade = sprite.GetTransitionFade();      // call 0x00BAA460, hidden ptr &0x20 (ebp = sprite from here)
0x00BAB140  shader constants (all on GPShaderRenderManager):
            SetUserVertexShaderConstant(0, camera->(+0x54 C3DVector), cosf(camera->(+0x60) * (1/(4*PI)) * TWO_PI) * (fade.FadeBeginDistance - radius*0.01f)); // 0x00989A60; TWO_PI is double (fmul qword)
            SetUserVertexShaderConstant(1, camera->(+0xD4), +0xD8, +0xDC, +0xE0);                       // 0x00989B00
            SetUserVertexShaderConstant(2, v14, v14, 1.0f/256 (imm 3B800000h), NonStippleAlphaFactor*0.5f); // 0x00BAB1D2
            float rr = 1.0f / radius;  (0x00BAB1D7, kept 0x10 via fst)
            SetUserVertexShaderConstant(3, C3DVector(v5C..)*rr? [0x6C.. scaled], 0.0f);   SetUserVertexShaderConstant(4, (0x88..)*rr, 0);
            SetUserVertexShaderConstant(5, (0x6C..)*rr..., 0);   SetUserVertexShaderConstant(6, (0xBC..0xC4), 0);   // 0x00BAB216/53/91/A8
            SetUserVertexShaderConstant(7, v1C, v18 - ?, 1.0f/(vB4 - vC0), 0.0f);     // 0x00BAB2E2 (args: 0, f/(..), (0x24-0x28), eax=v1C)
0x00BAB2E7  CPreTransposedBoneMatrix bone; bone.Set(camera->NoScaleViewTransform);   // 0x00987F70 (no ctor call)
            SetUserVertexShaderConstants(8, 3, (float*)&bone);                   // 0x009896D0
0x00BAB317  SetUserVertexShaderConstant(0xB, (cam+0x20 + cam+0x18)*(1/64) + 1/64(double), (cam+0x1C + cam+0x14)*(1/64)+1/64(double),
                                         (cam+0x18 - cam+0x20)*(-1/64), (cam+0x1C - cam+0x14)*(1/64));   // check arg order from pushes
0x00BAB38E  float zf = g_pEnginePrimitiveRenderer->ZSpriteDrawDistanceFactor;
            a = 1.0f/(zf*sprite.FadeDistance.MaxDrawDistance(+0x18) - zf*sprite.FadeDistance.FadeBeginDistance(+0x14));
            b = 1.0f/(fade.MaxDrawDistance - fade.FadeBeginDistance);
            SetUserVertexShaderConstant(0xC, b, -(b*fade.FadeBeginDistance), a*-1.0f, a*zf_begin + 1.0f);  // verify order vs pushes
0x00BAB404  SetUserVertexShaderConstants(0x10, sprite.ObjectCount, (float*)sprite.ObjectsEntries);   // 0x009896D0
0x00BAB419  const CTexture* textures[2] = { BillboardTexture.Texture, &([0x01436E98]+0x2AAC) };
0x00BAB42B  rm = GPRenderManager:
            rm->SetAlphaTestEnable(EnableZSpriteAlpha);        // 0x28CC (setne)
            rm->SetAlphaRef(sprite.AlphaRef);                  // 0x28E0 (movzx +0x28)
            rm->SetAlphaTestFunc((ALPHA_TEST_FUNC)4);          // 0x28A4 table 0x01396DE8 = D3DAlphaFuncModeTable[4]
            rm->SetSpecularEnable(true);                        // 0x29E4 (value 1; edi=1 reused)
            rm->SetFogEnable(true);                             // 0x2944 (1)
            rm->SetTextureUVModeU(1, (TEXTURE_UV_MODE)0);       // 0x2EF8 table 0x01396DC8[0]
            rm->SetTextureUVModeV(1, (TEXTURE_UV_MODE)0);       // 0x2F98
            rm->SetTextureFilterModeMag(1, (TEXTURE_FILTER_MODE)0);  // 0x3178 table 0x01396E58[0]
            rm->SetTextureFilterModeMin(1, (TEXTURE_FILTER_MODE)0);  // 0x30D8 (last one: no reload of rm)
0x00BAB80C  GPShaderRenderManager->PrepareForRender(0x200);   // 4:SetVSConstants(GetAsCArray),1,2,0x2000 WorldSpaceFog,4 Ambient,
                                                               // 0x20000 FogColour,0x400 LightGlobals,!WorldMatrixIsIdentity->ResetWorldTransform,0x80 CombinedProj
0x00BAB8C9  unsigned long query; PrepareOcclusionTest(sprite, (ERenderPassType)2, query);   // 0x00B905A0, ecx = this
            if (query != 0xFFFFFFFF) g_pOcclusionManager->StartOcclusionTest();          // 0x00B8D090
0x00BAB8F0  long count = sprite.ObjectCount;   (count*2 -> 0x10, count*4 -> 0x14)
            GPShaderRenderManager->DrawBuffersIndexed(textures, 2, VertexBuffer, 0, count*4, IndexBuffer, 0, count*2, PT_NULL);
              // SetStreamSource(0,VB.PVertexBuffer,0,VB.Stride) 0x00BAB923; loop 0x00BAB940..0x00BAB98E AttachTextureToStage(i,textures[i])
              // (strength-reduced on CurrentlySelectedTextures +0x3D00..+0x3D08); SetIndices 0x00BAB99E; RealiseRenderState 0x00BAB9B1;
              // DrawIndexedPrimitive(IndexBuffer.PrimitiveType,0,0,count*4,0,count*2) 0x00BAB9CD
0x00BAB9D3  if (query != 0xFFFFFFFF) g_pOcclusionManager->EndOcclusionTest(query);    // 0x00B8D0A0
0x00BAB9EB  ~capture -> StateManager->RestoreCaptureBlock()   0x00A05840
0x00BAB9F6  ~layout  -> SetVertexShaderConstantLayout(old)    0x0098B5E0
0x00BABA08  ~shaders -> VS: if (old != cur) old ? Enable(0x00987FE0) : Disable(0x009880E0);
            PS: if (old != cur) old ? Enable(0x00988110)+epilogue 0x00BABA49 : Disable(0x00988190) 0x00BABA56; epilogue 0x00BABA5B
```
No jump tables. Only cold blocks: the two early returns (inline in order) and the tail-duplicated PS-enable epilogue.
