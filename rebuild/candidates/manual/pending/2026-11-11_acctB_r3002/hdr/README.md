# Chunk 3002 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3002/hdr`)

Chunk 3002 made NO header change of its own. hdr/engine is the union of the two prior overlays it continued from (no file name
overlaps, so the union is exact): agent2022's (row 0x007F26B0; section 1 below) and agent2006's (row 0x00550DC0; section 2 below).
Both prior differ sources were re-graded with this merged overlay before any change and reproduced their recorded grades
(0x007F26B0 DIFFER(1793v1802), 0x00550DC0 DIFFER(1399v1399)). The matched row 0x007F26B0 additionally turns on the EXISTING repo
opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (rebuild/include/engine/CSpecialEffectsDef.h, chunk 782: same-TU noinline body of
CSpecialEffectsDef::GetEffectFromString 0x00669D30), no header edit.

## 1. From chunk 2022 (agent2022/hdr/README.md, verbatim)

## Chunk 2022 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2022/hdr`)

Every change to an existing header is an additive opt-in (`#ifdef FABLE_RETAIL_...`): with none of the new macros defined each copied file
preprocesses to its source text. Copied unchanged from the acctB c2007 hand-back (`pending/2026-11-11_acctB_c2007/hdr/engine`, which itself
carries the c2002 CTCTavernGame.h / CRightHandedSet.h / CInputProcessTavernGame.h): every other file under hdr/engine not listed below
(CTCTavernGame.h, CTCTavernGameCoinBase.h, Coin.h, CTCTavernGameShoveHaPenny.h, CInputProcessGameBase.h, CInputProcessTavernGame.h,
CRightHandedSet.h, ...). Row 0x007D3820 needs the c2007 CoinBase / Coin / CTCTavernGame / input-process files; land them first.

### New headers
| header | content | evidence |
|---|---|---|
| engine/CTCTavernGameCoinGolf.h (row 0x007D3820) | `CTCTavernGameCoinGolf : CTCTavernGameCoinBase`, vtable 0x0126F49C overrides, the Ego_r/FableWin method set (EnableCourse, GetCamera public; SetupForNewRound / FadeOutTableOccupants private); members ZAngle +0x148 .. CurrentCourse +0x17C, sizeof 0x180 | RTTI bases; rtti-vtables.tsv slots; Ego_r layout - 0x18; Alloc 0x004D59DF `push 180h`; ProcessInput / landed GetCamera 0x007D3560 offsets |
| engine/CLightningOrbDef.h (row 0x007F26B0) | `CLightningOrbDef : CSubDefClassBase`, StatGetTypeName inline, Transfer / Copy / GetSizeofClass; LightningDefIndex +0x28 .. ExplosionDefIndex +0x38 | RTTI bases; Ego_r layout (no container); Initialise reads +0x28 / +0x30; landed FrameUpdate +0x34 / +0x38 (the generated copy stays in _quarantine) |
| engine/CTCLightningOrb.h (row 0x007F26B0) | `CTCLightningOrb : CTCBase`, slots 6/8/9/13/21/22, Initialise, SetLifeTime, inline SetPParent / SetPTarget; members PParent +0x0C .. PSpline +0x2C, sizeof 0x34 | Ego_r / FableWin name sets; vtable 0x01240A2C; Ego_r layout; Initialise expands both setters |
| engine/CTCLightningNode.h (row 0x007F26B0) | `CTCLightningNode : CTCBase`, inline SetPParent / SetPSourceOfFaction / SetPSourceOfHit / SetPDef; members PParent +0x0C .. LightningBranches +0x2C, sizeof 0x38 | Ego_r / FableWin names; out-of-line copies 0x007F39A0 / 0x007F39F0 / 0x007F3A40 / 0x007F3A90 write +0x0C / +0x14 / +0x1C / +0x24 |
| engine/NBBBMaths_CLinearSpline.h (row 0x007F26B0) | `NBBBMaths::CLinearSpline` with the two STLport vectors (sizeof 0x18), GetLength / GetPointWithParam / AddControlPoint / ProcessPoints(long) declared | Ego_r layout + donor order; `push 18h` new and the six zero stores in Initialise; landed 0x00A580F0 |

### Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CTavernGameCoinGolfDef.h | FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF | the polymorphic retail class `: CSubDefClassBase` (members at the Ego_r offsets, inline StatGetTypeName); the generated flat layout stays the default. Evidence: RTTI bases, the landed GetSubDef 0x007D4BD0 literal, ProcessInput reads +0x34 / +0x38 / +0x3C / +0x44 / +0x48 |
| engine/C2DVector.h | FABLE_RETAIL_C2DVECTOR_ROTATE (inside the NORMALISE block) | Rotate(float) 0x00A141D0, AccurateNormalise() 0x00A14510 (declarations), inline operator-= (Ego_r / FableWin ??Z). Evidence: bound names + ProcessInput call sites 0x007D3B07 / 0x007D3C0C, in-place subtract 0x007D3B7B |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES | Rotate(float, float) 0x00A14260 declaration (Ego_r / FableWin ?Rotate@C3DVector@@QAEXMM@Z; GetCamera 0x007D3560 calls it) |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND (needs _MOVEMENT) | inline SetForceResendEveryRenderFrame(bool) (Ego_r / FableWin name; bit 1 of +0x59 = ForceResendEveryRenderFrame, `or byte ptr [eax+59h],2` 0x007F2C98) |
| engine/CTCDParticleEmitter.h | FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE (default branch) | static __fastcall Create(long, const C3DVector&, bool) 0x006E0880 declaration (Ego_r / FableWin; the bound spelling) + `class C3DVector;` forward |

Landing note: re-copy HEAD first and re-apply the blocks (other chunks may have changed the same headers since 2026-11-11). The lightning
headers serve only the parked row 0x007F26B0.

## 2. From chunk 2006 (agent2006/hdr/README.md, verbatim)

## Chunk 2006 header overlay (hdr/engine)

Every change is an addition behind an opt-in macro; with the macros undefined each header preprocesses exactly as the repo copy
(CV_BodyGuardScript.h and CQ_HobbeCaveScript.h are the pending hand-back 2026-11-11_linux_handH40g's versions, whose blocks are
also opt-in). Grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2006/hdr`.

| header | opt-in | what | evidence | used by |
|---|---|---|---|---|
| engine/CEngineInternalAttachedPrimitive.h | `FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS` | declarations of GetParentTransform 0x00B398E0, GetParentTransformAnimatedMesh 0x00B39080, GetChildInverseTransform 0x00B39900, GetChildInverseTransformStaticMesh 0x00B393D0 (reference parameter); GetOutputPrimitive / UpdateOutputPrimitive when the UPDATE opt-in is off. No layout change | Ego_r.pdb + FableWin.pdb ?<Name>@CEngineInternalAttachedPrimitive@@ spellings; retail TU order 0x00B38DB0..0x00B399A0 | 0x00B399A0 (match) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` | `SetMultiply(const CMatrix3x4&, const CMatrix3x4&)` (this = m1 * m2) and `PreMultiply(const CMatrix3x4&)` written as `CMatrix3x4 copy(*this); SetMultiply(m, copy);`, both FABLE_FORCEINLINE | Ego_r.pdb / FableWin.pdb ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z, ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z; retail 0x00B399F9..0x00B39EDB (three expansions, `rep movsd` copy); the only nesting that gives retail's 36 x87 sums | 0x00B399A0 (match). Mutually exclusive with the chunk 764 `FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY` |
| engine/CRGBFloatColour.h | `FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR` | `CRGBFloatColour(const CRGBColour&)` written through `Initialise(r, g, b, a)` (channels converted A, B, G, R) | Ego_r.pdb ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z and ?Initialise@CRGBFloatColour@@QAEXMMMM@Z; retail 0x00550EA8..0x00550F53 conversion order | 0x00550DC0 (parked) |
| engine/NUISystem_CTable.h | `FABLE_RETAIL_CTABLE_DRAW` | `virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*)` override | Ego_r.pdb / FableWin.pdb ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z; CTable vtable 0x0124A224 | 0x00550DC0 (parked) |
| engine/CV_BodyGuardScript.h | `FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN` (H40g, unchanged) | the nested CBodyGuard entity class (Ego_r 0x3c bytes) incl. `SetHenchmanAsHired(bool)` and `GetGoldCost()` | hand815 / H40g README; both PDBs | 0x00E621F0 (match) |
| engine/CQ_HobbeCaveScript.h | `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBESPAWN_MAIN` (new; the file also carries H40g's `..._CBANDITSACRIFICE_MAIN` block unchanged) | the nested CHobbeSpawn entity class: ParentClass +0x14, PMasterData +0x18, NumberToGenerate +0x1c, size assert 0x20 | struct_layouts_egor.tsv CHobbeSpawn (32 bytes, member names); both PDB name sets (Alloc SI, Main / Init EAE, GetParentScript UBE); landed Alloc 0x00D8F310 | 0x00D91E00 (match) |

Notes:
- The shared CRGBFloatColour.h declares a free `operator*`; both PDBs have the member ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z. Both
  give the same bytes in 0x00550DC0 (not changed here: header debt).
- Re-base onto HEAD before merging (other agents may have touched these headers since the copies were taken); every block is a
  pure insertion.
