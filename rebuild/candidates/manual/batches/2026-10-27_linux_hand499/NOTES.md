# Hand chunk 499 (strmbase + script entities/levels): 36 of 40 byte-exact

10 MATCH, 26 RELOCATION_MATCH, 0 retail_asm, 0 DIFFER, 4 SKIP. Every authored row has a passing behaviour test; vland is clean (no RELOC_DUPLICATE / RELOC_IDENTITY).

## What matched
- 00cb0d80 RELOCATION_MATCH -- CAMThread::InitialThreadProc(void* pv)
- 00cb15d0 RELOCATION_MATCH -- CBaseList::AddAfterI(POSITION pos, void* pObj)
- 00cb1dc0 MATCH -- CMediaType::operator==(const CMediaType& rt) const
- 00cb6650 RELOCATION_MATCH -- CVectorMap<CCharString,CScriptInfo,CKeyPairCompareLess<CCharString,CScriptInfo> >::LowerBound(const CCharString& key) const
- 00cd23b9 MATCH -- CScriptGameResourceObjectScriptedThingBase::IsValid() const
- 00cddab0 MATCH -- NScript::CIsThingConscious::operator()() const
- 00cddae0 RELOCATION_MATCH -- NScript::CIsThingConscious::Clone() const
- 00cef8e0 RELOCATION_MATCH -- NScript::CGameflowScript::OnPersist(CPersistContext& context)
- 00cefa00 MATCH -- NScript::CGameflowAssistanceScript::Init()
- 00cf8850 RELOCATION_MATCH -- CScriptGameResourceObjectMovieBase::Release()
- 00cfc950 MATCH -- NScript::CQ_AwakeningTheOracleScript::CChunkCollectionIndicator::OnPredicateFail()
- 00d02b20 RELOCATION_MATCH -- NScript::CQ_BanditCampScript::CGate1GuardInner::Init()
- 00d072c0 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Init()
- 00d07dd0 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CCampHostage::Init()
- 00d08720 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CCampHostage2::Init()
- 00d08fb0 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::OnPredicateFail()
- 00d0a7b0 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CBanditKing::OnPredicateFail()
- 00d0c390 RELOCATION_MATCH -- NScript::CQ_BanditCampBossBattleScript::CBanditForger::Init()
- 00d154a0 RELOCATION_MATCH -- NScript::CQ_BountyHuntScript::CBH_GangGruntSpawn::Init()
- 00d1dea0 RELOCATION_MATCH -- NScript::CQ_BreakSiegeScript::CKG_ChiefBreakSiege::OnPredicateFail()
- 00d1e140 RELOCATION_MATCH -- NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::OnPredicateFail()
- 00d1e1b0 RELOCATION_MATCH -- NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Init()
- 00d1f5c0 MATCH -- NScript::CQ_BreakSiegeScript::CBanditAttacker::Init()
- 00d1fce0 RELOCATION_MATCH -- NScript::CQ_BreakSiegeScript::CDisplacedTrader::Init()
- 00d242c0 MATCH -- NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuard::Init()
- 00d273a0 MATCH -- NScript::CQ_EndGameScript::Init()
- 00d29020 RELOCATION_MATCH -- NScript::CQ_EndGameScript::CHookCoastTeleporter::Init()
- 00d29620 RELOCATION_MATCH -- NScript::CQ_EndGameScript::CMazeJournal::Init()
- 00d2aa10 RELOCATION_MATCH -- NScript::CQ_EndGameFocalSitesScript::OnPersist(CPersistContext& context)
- 00d34fb0 RELOCATION_MATCH -- CFireHeartPatternDef::Copy(const CDefClassBase* other)
- 00d35070 RELOCATION_MATCH -- CFireHeartPatternDef::Transfer(CPersistContext& context)
- 00d36620 RELOCATION_MATCH -- NScript::CQ_FireHeartScript::CFH_Prisoner::Init()
- 00d36730 RELOCATION_MATCH -- NScript::CQ_FireHeartScript::CProphetCage::Init()
- 00d40cc0 MATCH -- NScript::CQ_GuildTrainingScript::CMeleeApprentice::Init()
- 00d43120 RELOCATION_MATCH -- NScript::CQ_GuildTrainingScript::CKillBird::OnPredicateFail()
- 00d55da0 MATCH -- NScript::CQ_GuildTrainingMeleeScript::Init()

## Parked for Fable 5.1
None. No row was left DIFFER.

## Skipped
Each of these oracle rows packs several retail functions separated by int3 padding, so one source cannot cover the row:
- 00caf430 -- two+ functions: three COM forwarding thunks 0x00CAF430/0x00CAF440/0x00CAF450 and a scalar deleting dtor 0x00CAF460 (vtable 0x012BF8B8)
- 00cafc40 -- many functions: three forwarding thunks, E_FAIL stub 0x00CAFC70, three `sub [esp+4],4` adjustor thunks, `sub ecx,8` thunk 0x00CAFCB0
- 00cdf120 -- two functions: NScript::CIsThingAlive::operator() 0x00CDF120 (jmp [eax+0x12c]) and CIsThingAlive::Clone 0x00CDF130
- 00ce1090 -- two functions: forwarders 0x00CE1090 ([this+0x34] slot 5) and 0x00CE10A0 (slot 6)

## Names

### Proven
Entity rows: the vtable slot plus the landed Alloc store site plus the Ego_r.pdb method set. Level rows: the vtable plus the landed ctor/Alloc. strmbase rows: DirectShow SDK source. CIsThingConscious: the landed vtable 0x00CDDB10. CFireHeartPatternDef: vtable 0x012CCC50, stored by TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize. LowerBound const: PDB overloads plus caller 0x00CB5B30.
00cb0d80
00cb15d0
00cb1dc0
00cb6650
00cddab0
00cddae0
00cef8e0
00cefa00
00cfc950
00d02b20
00d072c0
00d07dd0
00d08720
00d08fb0
00d0a7b0
00d0c390
00d154a0
00d1dea0
00d1e140
00d1e1b0
00d1f5c0
00d1fce0
00d242c0
00d273a0
00d29020
00d29620
00d2aa10
00d34fb0
00d35070
00d36620
00d36730
00d40cc0
00d43120
00d55da0

### Probable
00cd23b9
00cf8850

00cd23b9 IsValid is the only non-virtual const bool method in the PDB class, and its callers are scripted-thing handles. 00cf8850 Release resets the PImp at +8 through 0x008AB970 (SetImplementation).

### Descriptive
None. The only unnamed items are enum values 34 and 0.

## Label corrections
- 00cd23b9: the propagated label C3DMeshInfo::HasPhysicsMesh is wrong. The function is CScriptGameResourceObjectScriptedThingBase::IsValid. Landed callers call it IsScripting, which is an invented name.
- 00cf8850: the label _Dest_val<...CFlash> is wrong. It is CScriptGameResourceObjectMovieBase::Release.
- 0x008AB2B0: landed CCountedPointer_008aa7f0::Reset should be CCountedPointer<CScriptGameResourceObjectMovieBase>::Reset.
- 0x008AB970: MovieResourceHandle::Reset should be SetImplementation.
- 0x00CDCA70: landed Transfer<EPostSavePosition> should be Transfer<EGameflowPosition>.
- 0x00CDCF80: manifest Transfer<signed char> should be TransferVector<bool>.
- 0x00462E78: label CSimpleMemoryPool_AssignFrom should be vector<bool>::operator=.
- 0x00704580: stand-in CCountedPointer_Set should be CCountedPointer<CScriptThing>::operator=.
- 0x0049B8D0: label Transfer<CCharString> should be TransferVector<CCharString>.
- 0x004106F0: the binding is not a template; it should be ??$Transfer@K@ (Transfer<unsigned long>).
- landed 0x00CB0D20: CWorkerThread_00cb0d20 dtor should be CAMThread::~CAMThread.
- 0x00D02B10 and siblings: the landed generic getter structs are each entity's GetParentScript (vtable slot 3).

## Header debt
- CScriptThing.h, CGameScriptInterface.h and CDefClassBase.h flatten the vptr. Rows that need virtual calls or by-value copies with a vtable therefore use local polymorphic models.
- 00d154a0 and 00d1fce0 keep a local CGameScriptInterface model because the binding needs that class name plus its virtuals. vland reports SHARED_HEADER_AVAILABLE for both.
- 00d40cc0 now uses engine/CScriptThing.h.
- C3DMeshInfo.h does not apply to 00cd23b9, which was mislabelled.

## Callee names
Every callee is bound to its real mangled name. No `_gapscan::sub_` placeholders remain. Interface slots come from ghidra_out/cgsi_slots_aligned.tsv (slot = offset / 4). g_pGameScriptInterface is 0x0143E8F8 and pGlobalScriptDef is 0x0143E90C (EndGame_BarDecrementAmount is at +0xbe8).
