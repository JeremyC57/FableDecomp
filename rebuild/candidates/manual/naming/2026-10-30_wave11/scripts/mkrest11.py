# LABELS.tsv / RENAMES.tsv for wave 11 (hand tables; every name below is emitted / defined by a rechecked ov_final row)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows()
CAT = {}
for l in open(ROOT + '/rebuild/manifest/functions.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) > 2: CAT.setdefault(q[0], (q[1], q[2]))
AUTO = {}
for l in open(ROOT + '/rebuild/oracles/auto-re-candidates.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) > 1: AUTO.setdefault(q[0].lower(), q[1])
EF = 'Ego_r.pdb + FableWin.pdb (exact, both lists)'
L = [  # address, qualified name, mangled, evidence
 ('0043368d', 'CStartupDefinitionManager::Get', '?Get@CStartupDefinitionManager@@SIAAV1@XZ', EF + '; static __fastcall returning a reference; the asmre callers 0x004083E0 / 0x0042E98F call it so (RELOCATION_MATCH PASS)'),
 ('004300fb', 'CDefinitionManager::GetDef<CFrontEndDef>', '??$GetDef@VCFrontEndDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCFrontEndDef@@@@@Z', 'FableWin.pdb + the defining row\'s CFrontEndDef def-pointer use; the catalog\'s GetDef<CEnvironmentDef> is propagated'),
 ('00430096', 'CDefinitionManager::GetDef<CEngineDef>', '??$GetDef@VCEngineDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEngineDef@@@@@Z', 'FableWin.pdb + the defining row\'s CEngineDef def-pointer use'),
 ('00b26340', 'GFAllocEngine', '?GFAllocEngine@@YIPAVCIEngine@@XZ', EF + ' (Ego_r 0x009B0B40); `new CEngine` (0x178) + ??0CEngine 0x00B260B0; catalog CEnginePrimitive2DViewportManager::CreateInternalPrimitive is propagated'),
 ('0041a100', 'NResourceDirectoryNames::GetGraphicBankDir', '?GetGraphicBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ', EF + '; the row already defines it; catalog CUserProfileManager::GetAutoSaveFileName is propagated'),
 ('0041a1e0', 'NResourceDirectoryNames::GetShaderBankDir', '?GetShaderBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ', EF + '; the row already defines it; catalog label propagated'),
 ('0042f7c1', 'CEngineInit::~CEngineInit', '??1CEngineInit@@QAE@XZ', 'Ego_r.pdb 0x0043CB3F + FableWin; the out-of-line dtor of the five CWideString paths (asmre row RELOCATION_MATCH PASS)'),
 ('0099b940', 'CWideString::operator+=', '??YCWideString@@QAEABV0@PBG@Z', EF + '; the row already emits it (wide text is unsigned short); auto-re-candidates says CCharString::SetFromUnicodeString'),
 ('005bd993', 'NInventory::CCategory::GetHighlightedItemMaxInCategory', '?GetHighlightedItemMaxInCategory@CCategory@NInventory@@QBEJXZ', EF + '; asm_bake row (name only): it tail-jumps to 0x005BD931 GetMaximumNumberOfItems on the highlighted item; the invented GetMaxItemsPerSlot / catalog CRenderTarget::DetachTarget are wrong'),
 ('005bd931', 'NInventory::CItem::GetMaximumNumberOfItems', '?GetMaximumNumberOfItems@CItem@NInventory@@QBEJXZ', EF + '; NInventory::CItem (the row now defines it)'),
 ('00416258', 'CMainGameComponent::GetPredictedTimeSinceLastRenderFrame', '?GetPredictedTimeSinceLastRenderFrame@CMainGameComponent@@IBENXZ', 'Ego_r.pdb 0x004154B7 + FableWin; protected const double; the catalog repeats GetCurrentGameTime (0x00416231)'),
 ('00414c90', 'CTBaseSingleton<CConsole>::Get', '?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ', 'FableWin.pdb; the callers 0x0042F75E / 0x00687FD0 use the console it returns (Initialise / ProcessInput); auto-re-candidates says CCodeSectionManager'),
 ('00404a80', 'NUISystem::CIME::Get', '?Get@CIME@NUISystem@@SIAAV12@XZ', 'Ego_r.pdb 0x00404100 + FableWin 0x018BD220; returns the CIME object 0x013B7CD8; catalog CEngineInternalPrimitiveBase::AddChildPrimitive is propagated'),
 ('00404b80', 'NUISystem::CIME::SetEditBoxCaretPos', '?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z', EF + '; calls ImeUi_SetCaretPosition 0x00CB2860; the catalog repeats FinaliseString (0x00404B50)'),
 ('00404ba0', 'NUISystem::CIME::ShouldEditBoxShowCaret', '?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ', EF + '; returns ImeUi_GetCaretStatus 0x00CB2EC0 when initialised; catalog CNetworkClient::GetGameEventPackageSet is propagated'),
 ('00cb28e0', 'ImeUi_IsEnabled', '?ImeUi_IsEnabled@@YI_NXZ', 'Ego_r.pdb 0x00C92E80 + FableWin 0x0357EE10; only caller NUISystem::CIME::IsIMEActive 0x00404C50; CAIStateGroup_Freeze::IsRunnable is the label of 0x009224E0'),
 ('00cb2860', 'ImeUi_SetCaretPosition', '?ImeUi_SetCaretPosition@@YIXII@Z', 'Ego_r.pdb 0x00C92CE0 + FableWin 0x0357E930; only caller NUISystem::CIME::SetEditBoxCaretPos 0x00404B80; CDrawMouseCursor::Draw is propagated'),
 ('00cb2ec0', 'ImeUi_GetCaretStatus', '?ImeUi_GetCaretStatus@@YI_NXZ', 'Ego_r.pdb 0x00C94CD0 + FableWin 0x035814B0; NUISystem::CIME::ShouldEditBoxShowCaret 0x00404BA0 returns it; IsMeleeWeaponClass is the label of 0x005DA510'),
 ('00cb38e0', 'ImeUi_RenderUI', '?ImeUi_RenderUI@@YIX_N0@Z', 'Ego_r.pdb 0x00C90800 + FableWin 0x0357BB30; NUISystem::CIME::Render 0x00404C00 calls it (pair in the final recheck); not landed; catalog ImeUI_RenderCompositionString'),
 ('00b93f40', 'CEngineScene::RenderScene', '?RenderScene@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@@Z', EF + '; queue chunk 588; the row now defines it (catalog RenderLayerCells is invented)'),
 ('00b94060', 'CEngineScene::RenderSceneShadowBuffer', '?RenderSceneShadowBuffer@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCShadowClipVolume@@@Z', EF + '; queue chunk 588; the row now defines it (catalog RenderLayerCellsShadow is invented)'),
 ('00b917e0', 'CEngineScene::RenderPrimitiveListToShadowBuffer', '?RenderPrimitiveListToShadowBuffer@CEngineScene@@AAEXPAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@ABVCShadowClipVolume@@AAVCEngineSceneRenderStats@@@Z', EF + '; private; RenderSceneShadowBuffer 0x00B94060 calls it twice (lists A / B); the catalog repeats RenderPrimitiveList (0x00B91760)'),
 ('00ce4b70', 'NScript::CHeroBoastsScript::BoastHobbeCavePerpertratorKilled', '?BoastHobbeCavePerpertratorKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', 'Ego_r.pdb member order (fifth of the seven between BoastUseNoHealthPotions 0x00CE4600 and BoastBreakSiegeNoAssistance 0x00CE5050) + the master flag its body tests; PDB spelling Perpertrator'),
 ('00ce5620', 'NScript::CHeroBoastsScript::BoastHobbeContestKillMoreHobbes', '?BoastHobbeContestKillMoreHobbes@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', EF + '; the row already named it (leaf); catalog BoastBanditCampKillManyBandits is propagated'),
 ('00ce56e0', 'NScript::CHeroBoastsScript::BoastGraveyardKillAllInnocents', '?BoastGraveyardKillAllInnocents@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', EF + '; the row already named it (leaf); catalog label propagated'),
 ('00ce5e10', 'NScript::CHeroBoastsScript::BoastBountyHuntDecapitation', '?BoastBountyHuntDecapitation@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', EF + '; the row already named it (leaf); catalog label propagated'),
 ('00ce6a40', 'NScript::CHeroBoastsScript::BoastFireHeartFreeAllPrisoners', '?BoastFireHeartFreeAllPrisoners@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', EF + '; the row already named it (leaf); catalog label propagated'),
 ('00ce6b10', 'NScript::CHeroBoastsScript::BoastFireHeartKillAllPrisoners', '?BoastFireHeartKillAllPrisoners@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z', EF + '; the row already named it (leaf); catalog label propagated'),
 ('00af2250', 'NParticleEngine::CParticleSystem::GetSystemTransform', '?GetSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z', 'Ego_r 0x00AB5800 + FableWin; GetCurrentPosition 0x00AF2640 calls it (local to world); GetInverseSystemTransform is 0x00AF22C0 (same order as Ego_r 0x00AB5880)'),
 ('00af2780', 'NParticleEngine::CPSCBase::GetCurrentPositionLocal', '?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z', 'Ego_r 0x00AB5A70 + FableWin; calls GetInverseSystemTransform 0x00AF22C0 (its sibling GetCurrentPosition 0x00AF2640 calls GetSystemTransform 0x00AF2250)'),
 ('00af2640', 'NParticleEngine::CPSCBase::GetCurrentPosition', '?GetCurrentPosition@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z', 'Ego_r 0x00AB5930 + FableWin; CPSCLight::AddToBoundingSphere 0x00B18AF0 calls it (pair in the final recheck); catalog Particle_AddToBoundingSphere'),
 ('00ae83d0', 'CArray<C3DGroup2>::push_back', '?push_back@?$CArray@VC3DGroup2@@@@QAEXABVC3DGroup2@@@Z', EF + '; 0x00AE8490 pushes a default C3DGroup2 through it; catalog Vector_PushBackExpand / std::vector<C3DGroup2>::push_back'),
 ('00eb15c0', 'NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoGivingRewardToHero', '?DoGivingRewardToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ', 'Ego_r.pdb / FableWin.pdb member order = retail order 0x00EAF810 .. 0x00EB1EC0; text TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED'),
 ('00eb19c0', 'NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoSayingWellDoneToHero', '?DoSayingWellDoneToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ', 'member order as above; text TEXT_QST_B07_SHERIFF_PLEASED'),
 ('00eb1ec0', 'NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::AttackHero', '?AttackHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ', 'member order (last private member before Init); the body gives Me the hero as best enemy target; the catalog Init label is propagated'),
 ('0099f830', 'GFIntToCharString', '?GFIntToCharString@@YI?AVCCharString@@J@Z', 'Ego_r 0x00AFB260 + FableWin; bound by 0x006B9560 and now by 0x00D3ABE0'),
 ('0099f690', 'operator+(const char*, const CCharString&)', '??H@YI?AVCCharString@@PBDABV0@@Z', 'Ego_r 0x00AFAEE0 + FableWin 0x02F63030; bound by 0x006B9560 and now by 0x00D3ABE0'),
]
out = [('address', 'name', 'note')]
for a, n, m, ev in L:
    c = CAT.get(a); au = AUTO.get(a)
    cur = 'catalog %s%s' % ('%s::%s' % (c[1], c[0]) if c else '-', (' / auto-re %s' % au) if au else '')
    out.append((a, n, '%s; %s (%s)' % (cur, ev, m)))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
RN = [  # address, new src basename (no extension), note
 ('00404a80', 'NUISystemCIME_Get', 'the row defines ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r / FableWin); the old name is a propagated CEngineInternalPrimitiveBase label'),
 ('00404b80', 'NUISystemCIME_SetEditBoxCaretPos', 'defines ?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z; the old name repeats 0x00404B50'),
 ('00404ba0', 'NUISystemCIME_ShouldEditBoxShowCaret', 'defines ?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ; the old name is a propagated CNetworkClient label'),
 ('00cb28e0', 'global_ImeUi_IsEnabled', 'defines ?ImeUi_IsEnabled@@YI_NXZ (was CAIStateGroup_Freeze::IsRunnable, the label of 0x009224E0)'),
 ('00cb2860', 'global_ImeUi_SetCaretPosition', 'defines ?ImeUi_SetCaretPosition@@YIXII@Z (was CDrawMouseCursor::Draw)'),
 ('00cb2ec0', 'global_ImeUi_GetCaretStatus', 'defines ?ImeUi_GetCaretStatus@@YI_NXZ (was IsMeleeWeaponClass, the label of 0x005DA510)'),
 ('0043368d', 'CStartupDefinitionManager_Get', 'defines ?Get@CStartupDefinitionManager@@SIAAV1@XZ'),
 ('00b26340', 'global_GFAllocEngine', 'defines ?GFAllocEngine@@YIPAVCIEngine@@XZ'),
 ('0042f7c1', 'CEngineInit_CEngineInit', 'defines ??1CEngineInit@@QAE@XZ (destructor; file naming as the landed PKCDefaultFileManager_PKCDefaultFileManager_00c1a230)'),
 ('00416258', 'CMainGameComponent_GetPredictedTimeSinceLastRenderFrame', 'defines ?GetPredictedTimeSinceLastRenderFrame@CMainGameComponent@@IBENXZ; the old name repeats 0x00416231'),
 ('005bd993', 'NInventoryCCategory_GetHighlightedItemMaxInCategory', 'asm_bake row (body untouched): the name GetMaxItemsPerSlot was invented; Ego_r / FableWin ?GetHighlightedItemMaxInCategory@CCategory@NInventory@@QBEJXZ'),
 ('00b93f40', 'CEngineScene_RenderScene', 'defines ?RenderScene@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@@Z'),
 ('00b94060', 'CEngineScene_RenderSceneShadowBuffer', 'defines ?RenderSceneShadowBuffer@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCShadowClipVolume@@@Z'),
 ('00b917e0', 'CEngineScene_RenderPrimitiveListToShadowBuffer', 'defines the private ?RenderPrimitiveListToShadowBuffer@CEngineScene@@AAE...; the old name repeats 0x00B91760'),
 ('00ce4b70', 'NScriptCHeroBoastsScript_BoastHobbeCavePerpertratorKilled', 'defines ?BoastHobbeCavePerpertratorKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00ce5620', 'NScriptCHeroBoastsScript_BoastHobbeContestKillMoreHobbes', 'defines ?BoastHobbeContestKillMoreHobbes@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00ce56e0', 'NScriptCHeroBoastsScript_BoastGraveyardKillAllInnocents', 'defines ?BoastGraveyardKillAllInnocents@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00ce5e10', 'NScriptCHeroBoastsScript_BoastBountyHuntDecapitation', 'defines ?BoastBountyHuntDecapitation@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00ce6a40', 'NScriptCHeroBoastsScript_BoastFireHeartFreeAllPrisoners', 'defines ?BoastFireHeartFreeAllPrisoners@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00ce6b10', 'NScriptCHeroBoastsScript_BoastFireHeartKillAllPrisoners', 'defines ?BoastFireHeartKillAllPrisoners@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z'),
 ('00c596e0', 'CVtable012b5c10_Initialise', 'defines ?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z (slot 1 of the libpk voice base 0x012B5C10; as the landed CVtable012b5c60_Initialise_00c5a1d0)'),
 ('00af2780', 'NParticleEngineCPSCBase_GetCurrentPositionLocal', 'defines ?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z'),
 ('00c1a070', 'PKCDefaultFileManager_GetFileSize', 'defines ?GetFileSize@PKCDefaultFileManager@@QAE_NKPAK@Z (trace .\\PKCDefaultFileManager.cpp; dtor 0x00C1A230)'),
 ('00c1ae50', 'PKCTRRedblackMap_Find', 'defines ?Find@PKCTRRedblackMap@@QAEJPBKPAPAX@Z (the class of the landed ctor 0x00C1AE20)'),
 ('00c54bd0', 'CVoice00c54bd0_Reset', 'the owner is the libpk voice (ring +0xB4 cleared by CAudioRingBuffer::Clear 0x00C5CE40), not CAssistanceOwner'),
 ('00eb15c0', 'CMurderTwistSheriff_DoGivingRewardToHero', 'defines ?DoGivingRewardToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ (invented DoRemindingHeroToKillSheriff)'),
 ('00eb19c0', 'CMurderTwistSheriff_DoSayingWellDoneToHero', 'defines ?DoSayingWellDoneToHero@...@@AAEXXZ (invented DoSuspicious)'),
 ('00eb1ec0', 'CMurderTwistSheriff_AttackHero', 'defines ?AttackHero@...@@AAEXXZ (the catalog Init label is propagated)'),
 ('00dcf4c0', 'CFarmRearEntrance_Main', 'defines ?Main@CFarmRearEntrance@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ (vtable 0x012DB4B8 stored by the factory 0x00DCF3D0 registered as "FarmRearEntrance")'),
]
out = [('old_path', 'new_path', 'address', 'note')]
for a, nb, note in RN:
    s = R[a]['Source']; t = R[a]['TestSource']; d = os.path.dirname(s)
    ns = '%s/%s_%s.cpp' % (d, nb, a); nt = '%s/%s_%s_test.cpp' % (os.path.dirname(t), nb, a)
    assert not os.path.exists(ROOT + '/rebuild/src/compiled/' + ns), ns
    if s == ns: continue
    out.append(('rebuild/src/compiled/' + s, 'rebuild/src/compiled/' + ns, a, 'git mv src + test; build_candidates.ps1 Source/TestSource follow; PassPattern unchanged; ' + note))
    if os.path.exists(ROOT + '/rebuild/tests/' + t): out.append(('rebuild/tests/' + t, 'rebuild/tests/' + nt, a, ''))
open(os.path.join(W, 'RENAMES.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print('labels', len(L), 'renames', len(RN), 'rename lines', len(out) - 1)
