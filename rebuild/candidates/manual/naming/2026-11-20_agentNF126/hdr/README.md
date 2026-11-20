# NF126 header overlay (round 10). Every change is an `#ifdef` opt-in; with no opt-in defined each header is token-identical to its base.
Bases: orig/<path> = the repo working-tree copy taken 2026-11-20 (after NF124's landing ac2a0db547 and NF122's 9d3d50c56e; byte-equal to the
repo header at hand-back). NF124's opt-ins (CTCEnemy.h, CTCCreatureModeManager.h GETNOFRAMES..., NCreatureMode_CModeSitFloor.h) are in the repo
headers now; this overlay copies none of those headers, so there is nothing to rebase.

## engine/CWorld.h (base: rebuild/include/engine/CWorld.h)
- FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK: in the flattened default class (the `#else` layout), right after `PMainMeshBank_Info` (+0x40):
  `CMeshDataBank* GetMeshBank(){return PMainMeshBank_Data;}` -- FableWin.pdb ?GetMeshBank@CWorld@@QAEPAVCMeshDataBank@@XZ (public, inline in
  retail), the same body as the retail section's GetMeshBank. Inline member function only: no layout change.
- Used by rows/008e9850.cpp (NCreatureMode::CModeStrafe::SetChangeOfWeapon): retail loads [world+3Ch] into eax and moves it to ecx for
  PeekAnimation; the direct member read gives `mov ecx,[eax+3Ch]` (DIFFER 249v251), the inline getter matches.
- pin_asserts.py pinned 0 (every assert in the base is already FABLE_STATIC_ASSERT_AT).
- Header users: 759 landed files in rebuild/src/compiled include engine/CWorld.h; none defines FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK, so the
  opt-in changes none of them.
- NF123 (CAIBrain, CScriptedMapBrush, LHHttp2, LHHttpHeaderStatus, LHNetworkTypes, LSocket) and NF125 (CTCHeroExperience) do not overlay this header.

Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/ (checked at hand-back).
