# Chunk 639 -- 5 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 639` (vland --land 5/5 RELOCATION_MATCH + PASS, multi-check 0):
- CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc 0x00BF9FA0 (private, returns NEnginePrimitiveManagerRepeatedZSprites::
  CAddPrimitiveDesc) -- PDB Peek* type accessors plus the PDB ctor parameter order (fade distance by value, __forceinline).
- NScript::CQ_FireHeartScript::DisableFollowers 0x00D3A050 -- index through a named begin() pointer; handle class modelled in-file
  (the shared header expands its implicit ctor/dtor inline).
- CInputManagerDX ctor 0x00A60050 (const CInputManagerInit&, bool) -- inline CSystemManager::GetWinInstanceHandle() accessor.
- NScript::CQ_TraderEscortScript::CDarkwoodRockTroll::Main 0x00E03D30 -- the real CGameScriptInterface slot signatures from the shared header.
- CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden 0x00894EC0 -- chunk 638's lever: in-file stand-in bodies
  for the HasTC/HasDefName list searches take the filter's address; the None search's filter is a named local without a constructor.
Respell pass at landing (one address, one name): 0x00CD3CCB (STLport vector<CScriptGameResourceObjectScriptedThingBase>::resize) was
bound as CScriptedThingVector::resize by 0x00EB6A00 and CThingRefVector::Resize by 0x00CD3DA4; those callers were respelled to the
STLport spelling and rechecked (see the respell/ directory and the FINDINGS_LOG line for the outcome).
Five rows stay DIFFER in `pending/2026-10-27_linux_hand639/differ/` (0073acc0 CTCCreatureModeManager::AddMode early-return placement
-- vtable 0x012673CC slot names modernised: 5 OnActivate, 6 OnDeactivate, 9 IsAllowingPlayerSpecialMovementMoves, 11
IsRemoveOnDeactivate --, 00641ae0 CTCLight::DrawFlare scratch rotation, 00ba0fc0 AddStaticMesh flags-store schedule, 00b92300
CreateOcclusionGroup edi/ebx tie, 0065be6c CTCVillage::UpdateHornTasks ebx/edi tie -- the shared CDefPointer.h release gives
`dec ; cmp ; jne` where retail has `dec ; jne`; inherits a #pragma pack block, strip before landing). Lessons: PDB inline accessors
(Peek* getters, GetWinInstanceHandle, PeekGameTimeManager, real script-interface slot signatures) remove same-length register swaps;
a named begin() pointer gives retail's mov/lea; `if (--count == 0)` inside the pointee is `dec ; jne`, `return --RefCount;` compared in
the caller is `dec ; cmp ; jne`; a same-file noinline body for an out-of-line callee stops the re-read of a by-value counted pointer's
slot; in-class loop helpers need __forceinline; name _purecall slots from a derived class that overrides exactly one virtual of that
signature. 12 NAME_FIXES + 4 review rows queued (prefix c639): SetupPrimitiveDesc private (caller 0x00BFACC0 spells QAE), vector
resize 0x00CD3CCB, InvalidateAll private, AddToList PAPAV, CScriptBase::IsActiveThreadTerminating 0x00CB7940, g_pPrimitiveOcclusionManager,
RemoveMode, CWorld::GetFrame static __fastcall, IsTimeBetween, CTCBase::PeekWorld, CIsHornTaskWithThisHorn::operator() const,
~CCountedPointer<CVillageTaskHorn>; CModeBase slot names OnResume/OnLeave -> OnActivate/OnDeactivate in landed RemoveMode/ClearModes.
Labels: ghidra_out/labels_chunk639_2026-10-27.tsv.
