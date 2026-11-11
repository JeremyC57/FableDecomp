# Chunk 2022 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2022/hdr`)

Every change to an existing header is an additive opt-in (`#ifdef FABLE_RETAIL_...`): with none of the new macros defined each copied file
preprocesses to its source text. Copied unchanged from the acctB c2007 hand-back (`pending/2026-11-11_acctB_c2007/hdr/engine`, which itself
carries the c2002 CTCTavernGame.h / CRightHandedSet.h / CInputProcessTavernGame.h): every other file under hdr/engine not listed below
(CTCTavernGame.h, CTCTavernGameCoinBase.h, Coin.h, CTCTavernGameShoveHaPenny.h, CInputProcessGameBase.h, CInputProcessTavernGame.h,
CRightHandedSet.h, ...). Row 0x007D3820 needs the c2007 CoinBase / Coin / CTCTavernGame / input-process files; land them first.

## New headers
| header | content | evidence |
|---|---|---|
| engine/CTCTavernGameCoinGolf.h (row 0x007D3820) | `CTCTavernGameCoinGolf : CTCTavernGameCoinBase`, vtable 0x0126F49C overrides, the Ego_r/FableWin method set (EnableCourse, GetCamera public; SetupForNewRound / FadeOutTableOccupants private); members ZAngle +0x148 .. CurrentCourse +0x17C, sizeof 0x180 | RTTI bases; rtti-vtables.tsv slots; Ego_r layout - 0x18; Alloc 0x004D59DF `push 180h`; ProcessInput / landed GetCamera 0x007D3560 offsets |
| engine/CLightningOrbDef.h (row 0x007F26B0) | `CLightningOrbDef : CSubDefClassBase`, StatGetTypeName inline, Transfer / Copy / GetSizeofClass; LightningDefIndex +0x28 .. ExplosionDefIndex +0x38 | RTTI bases; Ego_r layout (no container); Initialise reads +0x28 / +0x30; landed FrameUpdate +0x34 / +0x38 (the generated copy stays in _quarantine) |
| engine/CTCLightningOrb.h (row 0x007F26B0) | `CTCLightningOrb : CTCBase`, slots 6/8/9/13/21/22, Initialise, SetLifeTime, inline SetPParent / SetPTarget; members PParent +0x0C .. PSpline +0x2C, sizeof 0x34 | Ego_r / FableWin name sets; vtable 0x01240A2C; Ego_r layout; Initialise expands both setters |
| engine/CTCLightningNode.h (row 0x007F26B0) | `CTCLightningNode : CTCBase`, inline SetPParent / SetPSourceOfFaction / SetPSourceOfHit / SetPDef; members PParent +0x0C .. LightningBranches +0x2C, sizeof 0x38 | Ego_r / FableWin names; out-of-line copies 0x007F39A0 / 0x007F39F0 / 0x007F3A40 / 0x007F3A90 write +0x0C / +0x14 / +0x1C / +0x24 |
| engine/NBBBMaths_CLinearSpline.h (row 0x007F26B0) | `NBBBMaths::CLinearSpline` with the two STLport vectors (sizeof 0x18), GetLength / GetPointWithParam / AddControlPoint / ProcessPoints(long) declared | Ego_r layout + donor order; `push 18h` new and the six zero stores in Initialise; landed 0x00A580F0 |

## Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CTavernGameCoinGolfDef.h | FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF | the polymorphic retail class `: CSubDefClassBase` (members at the Ego_r offsets, inline StatGetTypeName); the generated flat layout stays the default. Evidence: RTTI bases, the landed GetSubDef 0x007D4BD0 literal, ProcessInput reads +0x34 / +0x38 / +0x3C / +0x44 / +0x48 |
| engine/C2DVector.h | FABLE_RETAIL_C2DVECTOR_ROTATE (inside the NORMALISE block) | Rotate(float) 0x00A141D0, AccurateNormalise() 0x00A14510 (declarations), inline operator-= (Ego_r / FableWin ??Z). Evidence: bound names + ProcessInput call sites 0x007D3B07 / 0x007D3C0C, in-place subtract 0x007D3B7B |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES | Rotate(float, float) 0x00A14260 declaration (Ego_r / FableWin ?Rotate@C3DVector@@QAEXMM@Z; GetCamera 0x007D3560 calls it) |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND (needs _MOVEMENT) | inline SetForceResendEveryRenderFrame(bool) (Ego_r / FableWin name; bit 1 of +0x59 = ForceResendEveryRenderFrame, `or byte ptr [eax+59h],2` 0x007F2C98) |
| engine/CTCDParticleEmitter.h | FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE (default branch) | static __fastcall Create(long, const C3DVector&, bool) 0x006E0880 declaration (Ego_r / FableWin; the bound spelling) + `class C3DVector;` forward |

Landing note: re-copy HEAD first and re-apply the blocks (other chunks may have changed the same headers since 2026-11-11). The lightning
headers serve only the parked row 0x007F26B0.
