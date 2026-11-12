# Q20 header overlay (grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ20/hdr`)

Every existing header is a full copy of the current repo header (re-diffed 2026-11-12: additions only, except C3DVector.h where the
existing `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS)` line becomes `#elif` behind the new branch) plus `#ifdef` opt-ins; the
default path of every existing header is unchanged. Three headers are new.

| header | opt-in | change | rows |
|---|---|---|---|
| engine/C3DVector.h | `FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ` (checked before NORMALISE_LOCALS) | Normalise body `float x=X;float y=Y;float z=Z;float one_on_magnitude=GFFastOneOverSqrt(x*x+y*y+z*z);X*=..;Y*=..;Z*=..;return *this;` (retail loads X,Y,Z and pops the three copies after the store, 0x008B7047..0x008B7088) | 0x008B7040 (match) |
| engine/CCreatureAction_JumpOutFromCover.h | NEW header | `: CCombatActionBase`, `virtual void Initialise(); // 0x008B7040`, private `C3DVector JumpDirection; // +0x114`, sizeof 0x120 (Ego_r.pdb) | 0x008B7040 (match) |
| engine/CRenderManager2D.h | `FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ` | declares `bool Draw2DBoxWithZ(const C2DBoxF&, float, const CRGBColour&, unsigned long)` 0x009DDC60 and private `Add2DTri(...)` 0x009DC030 (Ego_r.pdb) | 0x009DDC60 |
| engine/CTVertexRHWColSpecTex1.h | `FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ` | includes fable_compiler.h; `extern float g_ScreenRHW; // 0x0139652C`; FABLE_FORCEINLINE `CGenericVertex<T>::SetRHWFromZ()` (FableWin.pdb name): Z in [0,0.01) -> g_ScreenRHW, [-0.01,0) -> -g_ScreenRHW, else 1/Z. Plain out-of-class `inline` was NOT inlined (473-byte body) | 0x009DDC60 |
| engine/CTCPhysicsBase.h | `FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE` (inactive when SETASMOVING + CTCSHOTDEFINITIONBASE_COLLISION supply the inline body) | declares `void SetVelocity(const C3DVector&); // 0x0066F9B0` | 0x007BCF7F, 0x007BD845 |
| engine/NThingShot_CTCShotDefinitionBase.h | `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS` (needs MOVEMENT) | protected inline `long GetShotAccuracyPercentage() const` and `const CThingShotDef& GetShotDef() const` (Ego_r + FableWin PDB names; no out-of-line copy) | 0x007BCF7F, 0x007BD845 |
| engine/NThingShot_CTCShotDefinitionMultiArrow.h | NEW header | Acceleration +0x9C, MaxCosAngleToTurn +0xA8, MaxAngleToTurn +0xAC, StraightArrow +0xB0, sizeof 0xB4 | 0x007BCF7F |
| engine/NThingShot_CTCShotDefinitionDrainLife.h | NEW header | MaxCosAngleToTurn +0x9C, MaxAngleToTurn +0xA0, AmountToHeal +0xA4, Speed +0xA8, PSoulEffect +0xAC, sizeof 0xB4 | 0x007BD845 |
| engine/CThingDrainLifeShotDef.h | `FABLE_RETAIL_CTHINGDRAINLIFESHOTDEF_CLASS` (flat model kept in `#else`) | real `: CSubDefClassBase` with inline StatGetTypeName and MaxDistance/MaxAngle/MaxAngleToTurnDegs/InitialSpeed/SpeedRange +0x28..+0x38, sizeof 0x3C | 0x007BD845 |
| engine/CPixelFormat.h | `FABLE_RETAIL_CPIXELFORMAT_INIT` | inline ctor `CPixelFormat(const CPixelFormatInit&)` (FableWin.pdb ??0CPixelFormat@@QAE@ABVCPixelFormatInit@@@Z, no -1 store), `Initialise(const CPixelFormatInit&)` 0x009E3640, `IsCompressed() const` 0x009E3940 | 0x009FB750 |
| engine/CTexture.h | `FABLE_RETAIL_CTEXTURE_LOADFROMDATASTREAM` | declares `LoadFromDataStreamToPreallocatedSurface(CDataInputStream&)` 0x009FB750 | 0x009FB750 |

Only the first two rows back a MATCH (0x008B7040); 0x007B0360 matched on repo headers alone. The rest support the four DIFFER ports and can
be landed with them later or dropped. CPixelFormatInit / CTextureFileInfo (Ego_r.pdb, 6 / 19 bytes packed) have no shared header; they are
modelled locally in differ/009fb750.cpp.
