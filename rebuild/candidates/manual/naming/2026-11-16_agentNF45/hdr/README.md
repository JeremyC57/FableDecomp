# NF45 header overlay (round 7)

There are 5 files and 17 new opt-ins. Each file is a copy of the repo header as it stands in the working tree at hand-back, with `#ifdef` blocks added.

- **No repo line was removed or edited.** `diff repo overlay` shows only `>` lines.
- **Without the opt-ins, every file is token-identical to the repo header.** This was checked with `cpp -P`, with whitespace squeezed:
  - each of the 5 headers on its own, both on the default path and with `FABLE_RETAIL_CRENDERMANAGERCORE` defined;
  - `engine/CRenderManager.h` and `engine/CRenderStateCaptureBlock.h`, which pull the render-manager chain together.

  All runs matched byte for byte.
- **Macro names are member-specific (`FABLE_RETAIL_<CLASS>_<MEMBER>`).** NF46's overlay can merge alongside.
- The rows that use each opt-in are listed in brackets.

## engine/CRenderStateManager.h (8, retail branch: inline setters)
Each block wraps the existing out-of-line declaration:

```
#ifdef X
    FABLE_FORCEINLINE <setter> { SetRenderState(..., DeviceState.<member>); }
#else
    <the existing declaration, unchanged>
#endif
```

The Ego_r.pdb / FableWin.pdb setter is inline in retail; no out-of-line copy exists.

| Opt-in | Setter and state | Rows |
|---|---|---|
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETFOGMODE_INLINE` | SetFogMode(FOG_MODE), through D3DFogTableMode 0x01396F58 | 0x00B47630 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETFOGVERTEXMODE_INLINE` | SetFogVertexMode(FOG_MODE) | 0x00B47630 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETTEXTUREFACTOR_INLINE` | SetTextureFactor(unsigned long) | 0x00B90700, 0x00B90CD0 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETNORMALIZENORMALS_INLINE` | SetNormalizeNormals(bool) | 0x00B32230, 0x00B32300 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETAMBIENTMATERIALCOLOURSOURCE_INLINE` | through D3DMaterialColourSourceTable 0x01396FF4 | 0x00B32300 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETDIFFUSEMATERIALCOLOURSOURCE_INLINE` | as above | 0x00B32300 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETDEPTHOFFSET_INLINE` | SetDepthOffset(float): the float's bits into DeviceState.DepthOffset | 0x00B4F510 |
| `FABLE_RETAIL_CRENDERSTATEMANAGER_SETSLOPESCALEDDEPTHOFFSET_INLINE` | DeviceState.DepthOffsetSlope | 0x00B4F510 |

## engine/CRenderManagerCore.h (4, retail branch; declarations only)
| Opt-in | Declaration | Rows |
|---|---|---|
| `FABLE_RETAIL_CRENDERMANAGERCORE_SETMATERIAL` | `?SetMaterial@CRenderManagerCore@@QAEXABVCRGBColour@@0@Z` = 0x00A0AA10 | 0x00B32300 |
| `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWCURRENTBUFFERSASPOINTLIST` | `(long, long)` = 0x00A0AE70 | 0x00B54210 |
| `FABLE_RETAIL_CRENDERMANAGERCORE_SETVIEWTRANSFORM` | 0x00A0B0A0 | 0x00B2FC50 |
| `FABLE_RETAIL_CRENDERMANAGERCORE_SETPERSPECTIVETRANSFORM` | the four-float overload, 0x00A0B320 | 0x00B2FC50 |

The landed self row 0x00A0AA10 still spells the placeholder `?M@C00a0aa10@@QAEXHH@Z` (catalog `Nop_ret8`); see NOTES.md, Open.

## engine/CEngineScreenEffectDisplacementRenderer.h (2, default branch)
- **`FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_STENCILRENDERINGCAPTUREBLOCK`** (0x00B871B0):
  - includes engine/CRenderStateCaptureBlock.h;
  - types +0x144 as `CRenderStateCaptureBlock StencilRenderingCaptureBlock` plus 1 byte of padding;
  - the opaque 2-byte array stays in the `#else` branch.
- **`FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_DISPLACEMENTPASSES`** (0x00B33010): declares the Ego_r.pdb pass brackets.

  | Method | Address |
  |---|---|
  | BeginDisplacementRendering | 0x00B87680 |
  | FinishDisplacementRendering | 0x00B869A0 |
  | BeginDisplacementStencilRendering | 0x00B871B0 |
  | FinishDisplacementStencilRendering | 0x00B86BB0 |

## engine/CEngineOcclusionManager.h (2, default branch; declarations only)
The Ego_r → retail delta for this class is 0x1588F0, the same delta that places GetResult at 0x00B8D0B0.

- **`FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ADDFLARETEST`** (0x00B662F0): `AddFlareTest(unsigned long, const C3DVector&, float, long)` = 0x00B8D040.
- **`FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_RENDEROCCLUSIONBOX`** (0x00BF7100): `RenderOcclusionBox(const CMatrix3x4&)` = 0x00B8D420.

## engine/CEngineShadowRenderer.h (1, default branch)
- **`FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWBUFFERRENDERSTATECAPTUREBLOCK`** (0x00B4F510, 0x00B4D2C0):
  - includes engine/CRenderStateCaptureBlock.h;
  - types +0x24C as `CRenderStateCaptureBlock` plus 3 bytes of padding, as the retail branch of the same header does;
  - the opaque 4-byte array stays in the `#else` branch.
