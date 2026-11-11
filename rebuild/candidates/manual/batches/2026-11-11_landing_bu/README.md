# 2026-11-11 landing bu -- Codex pass 20 return: 34 of 40 (5 large unworked rows + 35 parked near misses)

Source: `pending/2026-11-10_codex_pass20/return/` (START_HERE_CLAUDE, STATUS, ROW_DETAILS, LESSONS, LABELS, NAME_FIXES, BOUNDS_FIX, the six
parks with their sources / probes). SHA256SUMS verified. vland --land: 34 RELOCATION_MATCH, tests PASS.
- Extents corrected (BOUNDS_FIX): 0x006D18A0 CTCWife::Marry 1330, 0x00785620 CTCVolumeContainmentTracker::Update 1301, 0x007677E0
  CTCObjectAugmentations::AttachAugmentationParticles 1467, 0x0076EFA0 CTCWeaponTrail::UpdateWeaponTrails 1312.
- Identities from each row's emitted leaf (ten assigned labels were wrong: 0x00C3DB80 / 0x00C3D470 are CCodaContext::Create3DSource / Process,
  0x00AA39A0 is C3DKeyframe::ToMatrix, 0x00924B90 is the anonymous-namespace CCreatureAction_KrakenRoarLoop::FinishAction ...).
- 0x00C3DB80's test: the constructor-throws case needs /EHsc in the test build; it is now under `#if !defined(_MSC_VER) || defined(_CPPUNWIND)`
  (the harness builds tests /Od /W3; g++ and an /EHsc build still run it, both checked).
Headers: 168-file overlay 3-way merged onto HEAD (base 8764d6ebd): 94 clean, 64 new, 6 conflicts resolved (CThingGameObject, CThing,
CMainGameComponent: both sides kept; NGraphicAppearance_CTCGraphicAppearance, CTCStealth, CGameDefinitionManager: one `#if defined(A) ||
defined(B)` for two macros guarding the same body). Headers both sides had created: CTargetingDef / CInterpolationInfo keep the repo model (the
Codex CTargetingDef serves only the parked 0x007F6200, kept beside it); CTCDParticleEmitter / CGameTimeManager keep the repo model as default and
take the Codex PDB model under new FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL / FABLE_RETAIL_CGAMETIMEMANAGER_FULL (defined by 0x008DAA80 /
0x0065BE6C). hdrcheck2: every default path equal (C2DBoxI / CSystemManager / CTCStealth differ only in whitespace / __LINE__-numbered asserts;
CCodaSource gains one forward declaration).
Parked 6 (return/parked/): 0x004B2890, 0x0067E290, 0x007F6200, 0x00919240, 0x00A55F90, 0x00BCF4F0. Queued land1011bu: 176 name fixes.
