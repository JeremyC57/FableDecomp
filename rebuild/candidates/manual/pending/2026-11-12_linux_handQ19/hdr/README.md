# Q19 header overlay (grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ19/hdr`)

Every file is a full copy of the current repo header (re-merged 2026-11-12 after CTCVillage.h gained ISWORKTIME) plus `#ifdef`
opt-ins; the default path of every existing header is unchanged. Two headers are new.

| header | opt-in | change | rows |
|---|---|---|---|
| engine/C3DVector.h | `FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN` | GFFastOneOverSqrt body `return (3.0f-((r*r)*value))*r*0.5f;` (same instructions; two no-code PAREN tuples, between NEWTON_PAREN and NEWTON_GROUPED) | 0x0079B6F0 (match) |
| engine/C3DVector.h | `FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_STEP_PAREN` | body `((3.0f-(r*r*value))*r)*0.5f` (the other two-tuple spelling; used by no matched row, may be dropped) | probe only (0x00837220) |
| engine/CVectorMap.h | `FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE` (nested in the INSERT opt-in, `_MSC_VER` only) | `__forceinline` on `Insert(const value_type&)` | 0x008E16A0 |
| engine/CTCVillage.h | `FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES` | `#include "stlport/fable_map.h"`; declares `void UpdateMirroredCrimeData(); // 0x0065C5BB`; types `PlayersCrimes` (+0x64) as the Ego_r.pdb `std::set<long>` instead of `unsigned char[0xc]` (same size; offsetof assert holds) | 0x0065C5BB |
| engine/CTCSummonSpell.h | `FABLE_RETAIL_CTCSUMMONSPELL_RESETMINIONPOS` | declares `void ResetMinionPos(); // 0x0079B6F0` | 0x0079B6F0 |
| engine/CCreatureAction_ScorpionSummon.h | `FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CREATEBABIES` | private `void CreateBabies(); // 0x008C4FD0` | 0x008C4FD0 |
| engine/CCreatureAction_RiseUp.h | NEW header | `CCreatureAction_RiseUp : CCombatActionBase`, ctors (CThingCreatureBase&) 0x008BD880 and (CThingCreatureBase&, CThing&), private `bool FaceTarget` +0x114, sizeof 0x118 (Ego_r.pdb) | 0x008C4FD0 |
| engine/CCreatureAction_ReleaseTimeSpell.h | `FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL` | the real class : CCombatActionBase with ctor (void*, CThingCreatureBase&, const CCharString&), virtual FinishAction, private Clone, `void* HasteKey` +0x114, sizeof 0x118; the generated model stays in `#else` | 0x008CDD40 |
| engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h | `FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_CTOR` | declares the ctor (void* haste_key, CThingCreatureBase&, const CCharString&, CThing&, long, long, CCreatureActionBase*) 0x0069F1F0 (the `public:virtual void FinishAction();` line split in two, no change) | 0x008CDD40 |
| engine/CGameCameraManager.h | `FABLE_RETAIL_CGAMECAMERAMANAGER_RELEASESCRIPTEDCONTROL` | declares `void ReleaseScriptedControl(); // 0x00698950` | 0x008CDD40 |
| engine/NCreatureMode_CModeScreamerSuperAttack.h | NEW header | `NCreatureMode::CModeScreamerSuperAttack : CModeStandBase`: Filter +0x68, `CIntelligentPointer<const CThing> PTarget` +0x74, `CIntelligentPointer<CThing> PLightningBeamParticleEmitter` +0x7c, sizeof 0x84; vtable 0x01278338 (slot 0 0x008AC4B0, 1 GetType 0x008AC490, 3 FrameUpdate 0x008E16A0, 5 OnActivate 0x008D8330, 6 OnDeactivate 0x008D79A0) | 0x008E16A0 |
