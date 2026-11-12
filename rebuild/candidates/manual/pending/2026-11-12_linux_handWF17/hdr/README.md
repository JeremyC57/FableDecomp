## 0x007E2A60

- engine/CTCHeroStats.h: FABLE_RETAIL_CTCHEROSTATS_FINEPAID -- branch with AddMoney, KickHeroToMostRecentSafeRegion and inline Get/SetFinePaid, Get/SetTimesThrownOut over FinePaid +0x100 / TimesThrownOut +0x104 (row 0x007E2A60).
- engine/CTCVillage.h: FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE -- IsHeroMurderer, ClearCrimes, public EndSecuritySweep 0x006590BF (the jmp thunk; Ego_r/FableWin PDB name) and private InvalidateAndClearAllSecuritySweepTasks 0x006590A0; no friend (row 0x007E2A60).
- engine/CDisplayEngine.h: FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT -- StartScreenFadeOut(float,float,CRGBColour,float) 0x00434C00 declaration outside the CTCWIFE_MARRY gate (row 0x007E2A60).
- engine/CMessageEventManager.h: FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL -- FindMostRecentMessage<TFilter> declaration only (retail calls the COMDAT 0x004AB8B0 out of line) (row 0x007E2A60).
- engine/CThing.h: FABLE_RETAIL_CTHING_HASTC_MATERIALISED -- own-name opt-in for the existing `const bool&` HasTC body (same branch as CTCWEAPONTRAIL_UPDATE, which also changes 4 unrelated headers); comment kept on the #if line: extra header lines moved two frame slots in this row (row 0x007E2A60).

## 0x0075B000

- engine/CTCPhysicsRigidBody.h: FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY (nested in FABLE_RETAIL_CTCPHYSICSRIGIDBODY) -- Body typed as CRigidBody (new engine/CRigidBody.h) + private InitialiseBody 0x0075B000 / CalculateVertices(CRigidBody::CRigidBodyConfig&) 0x00759EC0 declarations; no layout change.
- engine/CRigidBody.h (NEW header): CRigidBody + nested CRigidBodyConfig at retail offsets (Ego_r names; 0x1b8 / 0xb4), inline Reset() (FableWin.pdb ?Reset@CRigidBodyConfig@CRigidBody@@QAEXXZ), opt-in FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE declares the called implicit operator= 0x0075BA30.
- engine/CPhysicsMeshObject.h: FABLE_RETAIL_CPHYSICSMESHOBJECT_RIGIDBODY -- declares GetBoundingBoxZEdge 0x00A11BA0, GetCentreOfGravity 0x00A11EC0, GetRigidBodyBoundingPoints(std::vector<C3DVector>&) 0x00A12840 (Ego_r/FableWin names).
- engine/C3DMeshObject.h: FABLE_RETAIL_C3DMESHOBJECT_POSITIONMATRIXFROMRHSET -- static CMatrix3x4 __fastcall GetPositionMatrixFromRHSet(const CRightHandedSet&) 0x00A358F0 (FableWin ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z).

## 0x008548B0

- engine/CVectorMap.h: FABLE_RETAIL_CVECTORMAP_INSERT_VECTOR_PUSHBACK -- Insert expands std::vector::push_back directly (not CArray::push_back), so a TU that also has FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE expands Insert's push_back while a bare map.push_back calls CArray::push_back out of line (row 0x008548B0).
- engine/NParticleEngine_CParticleUpdateParameters.h: FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE (ctor declared only, retail COMDAT 0x006E24D0) and FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_NOINLINE (same body noinline; unused by the final row) (row 0x008548B0).
- engine/CCreatureAction_SpiritAttack.h: FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_TYPED -- the typed class (CCreatureActionBase base, private CreateAttackEffect, CIntelligentPointer<CThing> PTarget +0xA8 / PParticleEmitter +0xB0, sizeof 0xB8) in place of the flat generated layout (row 0x008548B0; FrameUpdate 0x00854B90 could move onto it too).

## 0x00958840

- engine/CAIStateGroupBase.h: FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD_YELL -- protected `EAIActionReturn AIAction_FindAGuardAndYellForHelp(C3DVector, unsigned long, bool)` 0x0097FEA0 (Ego_r ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z); declaration only.
- engine/CThingBuilding.h: FABLE_RETAIL_CTHINGBUILDING_INCREMENTFORCEOPENCOUNT -- inline `IncrementForceOpenCount() { ++ForceOpenCount; }` (FableWin ?IncrementForceOpenCount@CThingBuilding@@QAEXXZ; retail `inc [eax+0D0h]`), nested in FORCEOPENCOUNT.
- engine/CTCCreatureOpinionOfHero.h: FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDAGUARD -- inline `SetInFindAGuardAndYellForHelp(bool)` on the existing InFindAGuardAndYellForHelp +0x343 (FableWin ?SetInFindAGuardAndYellForHelp@CTCCreatureOpinionOfHero@@QAEX_N@Z).
- engine/CMessageEventManager.h: FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDFROMDEF -- `AddMessage(const CMessageEventDef&, const C3DVector&, CThing*, CThing*, long)` 0x00687730 (Ego_r ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z); declaration only.
- (no new overlay) uses the repo's existing FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY opt-in (engine/CDefinitionManager.h) for the same-TU GetDef<CMessageEventDef> body.

## 0x006CF530

- engine/CTCShop.h: FABLE_RETAIL_CTCSHOP_GETSTOCKLIST declares `const std::map<long,long> GetStockList() const;` (0x00774610).

## 0x0079D7E0

engine/CTCBulletTime.h: + FABLE_RETAIL_CTCBULLETTIME_UNPAUSE -- declares public UnPause() 0x0079D7E0 and private PlaySoundW(const CCharString&, bool) 0x0079CE80 (Ego_r ?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z); needs _MEMBERS + _PLAYEFFECT (fable_string.h).
engine/CTCCarrying.h: + FABLE_RETAIL_CTCCARRYING_SETBYPASSFILTER -- declares SetAllCarriedThingsAsBypassFilter(void*) 0x006A2520 (Ego_r ?SetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXPAX@Z).
engine/NScreenEffect_CScreenEffectPriorityStack.h: ARGPASSER branch's ScreenEffectPriority gains SCREEN_EFFECT_PRIORITY_SPELL = 2 (the FULL branch's enumerator, same value) for UnPause 0x0079D7E0.
- (coordinator, at landing) NScreenEffect_CScreenEffectPriorityStack.h: the ARGPASSER branch's enum line gains SCREEN_EFFECT_PRIORITY_SPELL = 2 in place (one rewritten enum line). Approved: additive, same enumerator and value as the FULL branch, and no landed row or test defines FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER.

## 0x00977E00

engine/CAIStateGroup_Guildmaster.h: FABLE_RETAIL_CAISTATEGROUP_GUILDMASTER -- retail polymorphic class over CAIStateGroupBase (Update slot 12, protected GetBetterTeleportPos(C3DVector&)/IsStillRunnable/FindAndStoreBodyguards, real member types); row 0x00977E00.
engine/CCombatAction_WillShieldInto.h: FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO -- class over CCombatActionBase with the Ego_r ctor (CThingCreatureBase&, float, float) 0x00956760, ShieldUpTime +0x114 / ShieldHealth +0x118; row 0x00977E00.

## 0x00854DD0 (not landed)

- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR declares the 9-arg ctor 0x00789238 (Ego_r ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z) and AddDamageWithAttribute(EDamageAttribute,float) 0x00868CA0 (row 0x00854DD0; USED by the best source).
- engine/C3DVector.h: experimental operator- bodies FABLE_RETAIL_C3DVECTOR_SUBTRACT_MEMBERWISE / _SUBTRACT_COPY_ASSIGN / _SUBTRACT_FRIEND / _SUBTRACT_LOCALS, ctor spellings _CTOR_ASSIGN / _CTOR_FORCEINLINE / _CTOR_ASSIGN_FORCEINLINE, copy ctor _COPY_ASSIGN_BODY (row 0x00854DD0 probes; NOT used by the best source -- drop at landing unless a later attempt needs one).
- engine/CTCPhysicsBase.h: FABLE_RETAIL_CTCPHYSICSBASE_GETRADIUS_DEFERRED (GetRadius defined after the class; probe only, not used by the best source).
- engine/C3DVector.h (attempt 2 probes, NOT used by the best source -- drop at landing): FABLE_RETAIL_C3DVECTOR_IMPLICIT_COPY (no user copy ctor), _SUBTRACT_DEFERRED / _SUBTRACT_DEFERRED_MEMBERWISE (operator- defined after the class), _PLAIN_INLINE (FABLE_VECTOR_INLINE = inline), _SUBTRACT_PLAIN / _SUBTRACT_PLAIN_MEMBERWISE, _SUBTRACT_LOCALS_ZYX / _SUBTRACT_LOCAL_X / _SUBTRACT_LOCALS_XY, _CTOR_DEFERRED / _CTOR_DEFERRED_ASSIGN, _CTOR_CONST / _CTOR_CONSTREF.
- engine/CTCPhysicsBase.h (attempt 3 PROBE, NOT used by the best source -- drop at landing unless a second source proves the access): FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_PUBLIC_PROBE makes Radius (+0x34) public so the row can read physics->Radius as an lvalue (fixes the 0x00854ED2 fadd order).

## 0x0075A7A0 (not landed)

engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP -- operator^ with the X and Z components parenthesised (Y bare); row 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions (keeps the first CMatrix3x4::operator* argument set-up at 0x0075A8CF..0x0075A8D5). Full copy of the current repo header; the new arm wraps the existing CROSS_* #ifdef chain in `#ifdef ..._XZ_GROUP / #else / #endif` (additions only: `diff` shows no removed line; default path unchanged).
engine/CMatrix3x4.h: FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR -- declares C3DVector CMatrix3x4::operator*(const C3DVector&) const (Ego_r/FableWin ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z, landed 0x00A56530); row 0x0075A7A0. Full copy of the current repo header plus this opt-in only (declaration, no layout change).

