# acctB hand-back, chunk 2035 (1 reserved row worked: matched; the other row not started)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006FDD60 CGameCameraBase::Update (`?Update@CGameCameraBase@@UAEXXZ`) | RELOCATION_MATCH 1659v1659, test PASS (g++ and VC7.1) | **true length 1659, not the reserved 2018** (SPLITS.tsv): `ret` at 0x006FE3DA; 0x006FE3E0..0x006FE53F holds 22 unreferenced, uncatalogued STLport list<C3DVector> inline copies of this TU. No flag, no asm helper; `#pragma intrinsic(sqrt, sin, cos, acos)` as in landed rows. 7 grades |
| 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject | not started | re-queued as its own acctB chunk |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME on 0x006FDA20, in NAME_FIXES.tsv). Purity and
padded-struct greps are clean.

Review items for landing:

- **Same-TU body of GetRotationAngle 0x006FD4F0** (`__declspec(noinline)`, landed separately): retail compiles it in this
  TU and Update is its only caller; with the body visible the frame sharing is retail's. Grade only the Update leaf; whether
  the copy here is byte-exact with the landed 0x006FD4F0 was not checked -- swap in the landed body if they differ.
- **NAME_FIXES.tsv:** 0x006FD4F0 is the PRIVATE `?GetRotationAngle@CGameCameraBase@@AAEMABVC3DVector@@0@Z` (the landed
  definer declares it public in a local class and emits QAE: respell it when landing this caller); 0x006FDA20 is
  `?IsThingCollision@CGameCameraBase@@AAE_NXZ` (catalog `CGameCameraBase_TracePhysics`); 0x0049D9D0 is bound once as
  `GetCurrentPlayer`, should be `PeekMyPlayer`. Open: 0x00415FD3 DivideByGameSpeed has two conflicting landed spellings
  (no PDB record for either).
- C3DVector::Normalise is used with its FableWin body through the GFSqr helper (evidence: FableWin's function order
  Normalise, GFSqr, GFFastOneOverSqrt): check it is the same body other landed rows expand.
- Descriptive names: the row's locals only.

To land: `hdr/` -- opt-in `FABLE_RETAIL_CGAMECAMERABASE_UPDATE` (declares Update and the private GetRotationAngle,
IsLandscapeCollision, IsThingCollision) in CGameCameraBase.h, and `FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND`
(const member 0x0063D960) in CWorldMap.h; default text unchanged. Rebuild the oracle from Fable.exe (no retail bytes are in
this folder).
