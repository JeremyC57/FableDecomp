# NF39 (round 4) -- name-fix slice land1011fj .. land1113fz (290 entries), continuing NF37

## Result (counted from APPLIED.tsv / STATUS.tsv / RENAMES.tsv)
- Entries: done 64, done-already 2, rejected 0, partial 6, blocked 15, deferred 0, carry 203 (total 290).
- Rows changed: 66, all rechecked in one run (recheck_final.log): MATCH PASS 13, RELOCATION_MATCH PASS 53.
- RENAMES.tsv: section 1 (self rows) 66 lines, section 2 (binders) 10 lines.
- No landed funclet row (rebuild/src/compiled/01/21/*Unwind*) includes a changed row (grep of every changed row's file name).
- Rows deferred to NF40: none (no lock of a row I needed was held by NF40). Rejected: none (REJECTED.tsv has its header only).
- Grades on changed rows: 94 in all, max 5 per row (STATUS.tsv grades_used); the row I gave up on, 0x009B9BA0 (e40), used 7 and stays unchanged.

## Entries by status
### done
- e9 land1011fq 00bbbf40: self row 0x00BBBF40 renamed onto its PDB spelling ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e16 land1011fr 00916640: self row 0x00916640 renamed onto its PDB spelling ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e20 land1011fu 00adb570: self row 0x00ADB570 renamed onto its PDB spelling ?size@?$list@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QBEIXZ (section 1), MATCH; landed callers already bind it (probe)
- e21 land1011fu 00a96da0: self row 0x00A96DA0 renamed onto ??1CVertexGroupBlock@@QAE@XZ (section 1, RM); the last wrong binder 0x00ADBE50 ported onto CVertexGroupBlock.h as ??_GCVertexGroupBlock@@QAEPAXI@Z (FableWin.pdb name set), RM; 5 of 5 callers bind it
- e27 land1011fx 00706860: self row 0x00706860 renamed onto its PDB spelling ?GetHiddenMaterialNames@CTCHeroAttachableAppearanceModifiers@@QBE_NAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e34 land1113fy 007d7c40: self row 0x007D7C40 renamed onto its PDB spelling ?FindTarget@CTCTavernGameSpotTheAddition@@AAEPAVCThingObject@@M@Z (section 1), MATCH; landed callers already bind it (probe)
- e45 land1113fy 007448f0: self row 0x007448F0 renamed onto its PDB spelling ?GetSightRadius@CTCPerceiveThing@@QBEMXZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e53 land1113fy 00a52650: self row 0x00A52650 renamed onto its PDB spelling ?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z (section 1), MATCH; landed callers already bind it (probe)
- e60 land1113fy 00668a80: self row 0x00668A80 renamed onto its PDB spelling ?IsFinished@CAnimSet@@QBE_NXZ (section 1), MATCH; landed callers already bind it (probe)
- e64 land1113fy 006df860: self row 0x006DF860 renamed onto its PDB spelling ??1CCreatureAction_UnsheatheToShowToSheatheTrophy@@UAE@XZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e72 land1113fy 009ffb20: self row 0x009FFB20 renamed onto its PDB spelling ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e79 land1113fy 009531a0: self row 0x009531A0 renamed onto its PDB spelling ??1CCreatureAction_DragonRearUpFull@@UAE@XZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e80 land1113fy 00751a70: self row 0x00751A70 renamed onto its PDB spelling ?IsOverriddenByParent@CTCLook@@QBE_NXZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e81 land1113fy 00842400: self row 0x00842400 renamed onto ??0CCreatureAction_PlayConversationAnimation@@QAE@AAVCThingCreatureBase@@W4EConversationAttitude@@ABVCSoundPair@@JJ@Z (section 1, RM); its callee 0x004CD9B0 respelled to the landed CConversationAttitude::GetAnimName (section 2); caller already bound
- e82 land1113fy 0048da89: self row 0x0048DA89 renamed onto its PDB spelling ?GetStringWidth@CFontBank@@QBEMPBG@Z (section 1), MATCH; landed callers already bind it (probe)
- e83 land1113fy 0048daa2: self row 0x0048DAA2 renamed onto its PDB spelling ?GetStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@_N@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e84 land1113fy 0049417d: self row 0x0049417D renamed onto its PDB spelling ?push_back@?$vector@VCEmbeddedGraphic@CSubtitleRenderer@@V?$allocator@VCEmbeddedGraphic@CSubtitleRenderer@@@std@@@std@@QAEXABVCEmbeddedGraphic@CSubtitleRenderer@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e86 land1113fy 00437260: self row 0x00437260 renamed onto its PDB spelling ??0CWStringParser@@QAE@PBG@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e89 land1113fy 00a2e4b0: self row 0x00A2E4B0 renamed onto its PDB spelling ??0C3DDummyObject@@QAE@XZ (section 1), MATCH; landed callers already bind it (probe)
- e104 land1113fy 007a4210: self row 0x007A4210 renamed onto its PDB spelling ??0?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@QAE@PAVCLightningBeam@CTCWillLightning@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e114 land1113fy 00b2b050: self row 0x00B2B050 renamed onto its PDB spelling ?IsNearlyIdentical@CEngineSurfaceCompositionManager@@AAE_NABVCEngineTextureMorph@@0M@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e115 land1113fy 00b2aef0: self row 0x00B2AEF0 renamed onto its PDB spelling ?GetDimensionsAndAdjustMipmapLevel@CEngineSurfaceCompositionManager@@AAEXAAVC2DExtentsI@@PBVCGraphicInfoFrame@@AAK@Z (section 1), MATCH; landed callers already bind it (probe)
- e116 land1113fy 00f38990: self row 0x00F38990 renamed onto its PDB spelling ??0CSurfaceCompositor@@QAE@AAVCSurface@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e121 land1113fy 007f39a0: self row 0x007F39A0 renamed onto its PDB spelling ?SetPParent@CTCLightningNode@@QAEXAAVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e122 land1113fy 007f39f0: self row 0x007F39F0 renamed onto its PDB spelling ?SetPSourceOfFaction@CTCLightningNode@@QAEXPBVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e123 land1113fy 007f3a40: self row 0x007F3A40 renamed onto its PDB spelling ?SetPSourceOfHit@CTCLightningNode@@QAEXPAVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e124 land1113fy 007f3bb0: self row 0x007F3BB0 renamed onto its PDB spelling ??$GetDef@VCLightningDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCLightningDef@@@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e125 land1113fy 007f4780: self row 0x007F4780 renamed onto its PDB spelling ?Reset@?$CCountedPointer@VCLinearSpline@NBBBMaths@@@@QAEXPAVCLinearSpline@NBBBMaths@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e130 land1113fy 00b89080: self row 0x00B89080 renamed onto its PDB spelling ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e133 land1113fy 008b8590: self row 0x008B8590 renamed onto ??0CCreatureAction_UnsheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z (section 1, RM); 3 landed callers bind it; the 4th, 0x00904F60, carries no relocation to it in its object (NOREL) but declares the constructor with the same (CThingCreatureBase&) spelling, so no wrong binder exists
- e134 land1113fy 0092c520: self row 0x0092C520 renamed onto ?IsInSleepTime@CAIStateGroup_MinionSleep@@QBE_NXZ (section 1, RM); no landed row calls it out of line (the landed Update 0x0092DA20 defines and expands the same body); its callee 0x006BB390 respelled to ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z (section 2)
- e145 land1113fy 00916640: self row 0x00916640 renamed onto its PDB spelling ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e146 land1113fy 00915810: self row 0x00915810 renamed onto its PDB spelling ??0CCreatureAction_PutGoodsDown@@QAE@AAVCThingCreatureBase@@ABVCCharString@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e152 land1113fy 0046ce28: self row 0x0046CE28 renamed onto ?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z (section 1, MATCH); a vtable slot with no direct landed caller, nothing else binds a wrong spelling (probe: 0 callers)
- e153 land1113fy 0046ce2d: its only caller, the thunk 0x0046CE28, now binds ??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z (ported in the same change as e152, MATCH)
- e155 land1113fy 005e8f50: self row 0x005E8F50 renamed onto its PDB spelling ?erase@?$map@JKU?$less@J@std@@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBJK@std@@U?$_Nonconst_traits@U?$pair@$$CBJK@std@@@2@@2@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e156 land1113fy 006b5d80: self row already bound; the one wrong landed binder 0x006B6340 (T::VecDel stand-in) ported onto CBlendedEnvironmentTheme.h as its ??_G (RM); the atexit stub 0x01229390 carries no relocation to it (NOREL) but declares NEnvironmentInternal::CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme, the same spelling
- e163 land1113fy 0041b9c0: self row 0x0041B9C0 renamed onto ?TouchResource@CResource@@QAEXXZ (section 1, RM); retail has no call to it (every use expands it inline), so no caller follows; its callees 0x009FBFB0 / 0x009FBF90 respelled to ListRemove / ListInsert (section 2)
- e167 land1113fy 009c3130: self row 0x009C3130 renamed onto its PDB spelling ??0CFileInfo@CFileInstaller@@QAE@ABVCWideString@@0J_N1@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e168 land1113fy 009c6a70: self row 0x009C6A70 renamed onto its PDB spelling ??0?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@QAE@PBVCFileInfo@CFileInstaller@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e169 land1113fy 009c6640: both DIR32 users bind DeleteData<const CFileInfo>: 0x009C47A0 already did, 0x009C6A70 respelled by its own port (e168); the self row 0x009C6640 renamed too (section 1, RELOCATION_MATCH)
- e177 land1113fy 0097c900: self row 0x0097C900 renamed onto its PDB spelling ??1CCreatureAction_ScorpionChargeInto@@UAE@XZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e178 land1113fy 008c4e20: self row 0x008C4E20 renamed onto its PDB spelling ??0CCreatureAction_ScorpionChargeInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e180 land1113fy 0057fc46: self row 0x0057FC46 renamed onto its PDB spelling ?GetTypeBalancedAttractiveness@CTattooDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@V?$allocator@VCAppearanceModifierScalingDef@@@std@@@std@@@Z (section 1), MATCH; landed callers already bind it (probe)
- e191 land1113fy 00437980: self row 0x00437980 renamed onto its PDB spelling ?AddAutoPickup@CPlayerGui@@QAEXJ@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e197 land1113fy 009ad390: self row 0x009AD390 renamed onto its PDB spelling ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e202 land1113fy 007f3e90: self row 0x007F3E90 renamed onto its PDB spelling ?erase@?$vector@V?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@V?$allocator@V?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@@std@@@std@@QAEPAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV3@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e203 land1113fy 007f3f60: self row 0x007F3F60 renamed onto its PDB spelling ??0?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@QAE@PAVCLightningBranch@CTCLightningNode@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e204 land1113fy 007f3ad0: self row 0x007F3AD0 renamed onto its PDB spelling ??0CLightningBranch@CTCLightningNode@@QAE@AAVCThing@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e234 land1113fy 00a5b210: self row 0x00A5B210 renamed onto its PDB spelling ?OpenFromFile@CCachedFile@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e237 land1113fy 009ad3b0: self row 0x009AD3B0 renamed onto ?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z (section 1, RM); 11 landed callers bind it; the 12th, 0x00B687D0 BuildThemes, has no relocation to it and no declaration of it (its model does not reach the call), so no wrong spelling exists
- e238 land1113fy 0055e7c0: self row 0x0055E7C0 renamed onto its PDB spelling ?ActivityEnumToCharString@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@ABW4EOnlineScoreboardActivity@1@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e242 land1113fy 00564b20: self row 0x00564B20 renamed onto its PDB spelling ??0?$pair@MVCWideString@@@std@@QAE@ABU01@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e245 land1113fy 00cb5ac0: self row 0x00CB5AC0 renamed onto its PDB spelling ?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e246 land1113fy 007e37c0: self row 0x007E37C0 renamed onto its PDB spelling ??$GetDefaultVal@VCDefString@@@CPersistContext@@KI?AVCDefString@@XZ (section 1), MATCH; landed callers already bind it (probe)
- e247 land1113fy 00f333b0: self row 0x00F333B0 renamed onto its PDB spelling ??$TransferVector@VCDefString@@@CPersistContext@@QAEXPBDAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e248 land1113fy 004107e0: self row 0x004107E0 renamed onto its PDB spelling ??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e249 land1113fy 007e3aa0: self row 0x007E3AA0 renamed onto its PDB spelling ??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@@Z (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e250 land1113fy 0040e160: self row 0x0040E160 renamed onto its PDB spelling ??$GetDefaultVal@J@CPersistContext@@KIJXZ (section 1), MATCH; landed callers already bind it (probe)
- e251 land1113fy 0040e250: self row 0x0040E250 renamed onto its PDB spelling ??$GetDefaultVal@M@CPersistContext@@KIMXZ (section 1), RELOCATION_MATCH; landed callers already bind it (probe)
- e257 land1113fy 0057aa1b: self row 0x0057AA1B renamed onto its PDB spelling ?GetNumRegionsExplored@CTCHeroStats@@QBEJXZ (section 1), MATCH; landed callers already bind it (probe)
- e263 land1113fy 0042925c: self row 0x0042925C renamed onto ReleaseData<CText> (section 1, RELOCATION_MATCH); 20 of 21 callers already bound it; the 21st, ShareData<CText> 0x00429460, ported onto engine/CCountedPointer.h and renamed (section 1, outside the queue), now binds it
- e265 land1113fy 00742460: self row 0x00742460 renamed onto ?Alloc@CModeBasic@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z (section 1, RM); its only reference is the DIR32 data store 0x0073CA54 in the landed RegisterModes 0x0073B130, whose declaration MODE_REGISTRATION_INTERFACE(CModeBasic) already spells it
- e278 land1113fy 00ac4f20: self row 0x00AC4F20 renamed onto its PDB spelling ?GetCount@CMirroredTextureHighlightsChunk@@QBEKXZ (section 1), MATCH; landed callers already bind it (probe)
### done-already
- e56 land1113fy 008ef910: self row and both landed callers already bind ?UpdateCoinSounds@CTCTavernGameCoinBase@@QAEX_N@Z (probe ok=2 bad=0)
- e186 land1113fy 00989700: self row binds the claim; 24 landed callers bind it; 0x00BEACA0 declares the private UpdateCameraPos with the claimed spelling but its object has no relocation to it (NOREL), and 0x00BE6F70 / 0x00BF4570 model their render paths through pending candidate headers that do not reference 0x00989700 at all; no wrong spelling found
### partial
- e73 land1113fy 00437900: self row 0x00437900 renamed onto ?RemovePerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z (section 1, RM, rechecked PASS); 4 landed callers bind it; 3 binders left unported (port, 0 grades): 0x00744730 (a GObj::NotifyB stand-in), 0x00744DB0 and 0x00745730 (local CPlayerGui models with a CThing* argument) -- each needs its row moved onto engine/CPlayerGui.h
- e74 land1113fy 004378f0: self row 0x004378F0 renamed onto ?AddPerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z (section 1, RM, rechecked PASS); 4 landed callers bind it; binder 0x00744730 (a GObj::NotifyA stand-in) left unported (port, 0 grades)
- e111 land1113fy 00f36f00: 1 of 9 wrong binders fixed: 0x00B89080 now binds ?EndTextureUse@CGPUScratchBufferTextureManager@@QAEXJ@Z (ported with e130, RM); the other 8 binders not reached
- e138 land1113fy 006bb390: 1 of 9 wrong binders fixed: 0x0092C520 now binds ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z (ported with e134, RM); the other 8 binders not reached
- e164 land1113fy 009fbfb0: 1 of 2 wrong binders fixed: 0x0041B9C0 now binds ?ListRemove@CResource@@AAEXXZ (ported with e163, RM); 0x0041B550 (an NUISystem::CResource stand-in) not ported (0 grades)
- e165 land1113fy 009fbf90: 1 of 2 wrong binders fixed: 0x0041B9C0 now binds ?ListInsert@CResource@@AAEXPAV1@0@Z (ported with e163, RM); 0x0041B550 (an NUISystem::CResource stand-in) not ported (0 grades)
### blocked
- e14 land1011fr 00a0aaa0: port (0 grades): the 6 wrong binders are CRenderManager2D rows that model CRenderManagerCore / CRenderManager2D and their callees locally; each needs a move onto engine/CRenderManager2D.h (not reached in this pass beyond reading)
- e19 land1011fs 0083b170: port (0 grades): the self row is one row of a std::_Rb_tree instantiation whose key/value are the shared per-instantiation placeholders CUnprovenKey/Value_0083b170; renaming it onto map<EObjectAugmentationType,CWeaponTrailGraphicSet> (stlport/fable_map.h + CWeaponDef) means respelling every row of that instantiation together
- e24 land1011fv 00410890: port (0 grades): the wrong binder 0x004083E0 models CDefinitionManager locally (GetDef non-const QAE vs the header's const GetDef); needs that row moved onto engine/CDefinitionManager.h with the GETDEF_GLOBAL_BODY opt-in
- e29 land1011fx 00a36100: port (0 grades): HasHelperPoint needs C3DMeshObject's statistics member and C3DMeshStats::HasDummyObject (0x00A2EA20) declared in a shared header; neither header declares them (authoring)
- e40 land1113fy 009b9ba0: port (7 grades): SkipPastString on the shared CStringParser.h needs CalcInQuotes 0x009B9570 out of line (no such opt-in: the header force-inlines it) and a MoveStringPos spelling the header lacks; with both added in an overlay the best try is DIFFER(81v81): only the register of the final StringPos add differs (retail add ebx,edi = pos+=n kept in ebx; VC7.1 gave add edi,ebx) and lea operand order; 27 landed callers already bind ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z; row left unchanged (attempt kept in work/blocked/)
- e108 land1113fy 0074d220: port (0 grades): binders 0x00749F00 / 0x0074A100 model CDefinitionManager locally (non-const GetDef with struct defs); need a move onto engine/CDefinitionManager.h
- e117 land1113fy 00f39a50: port (0 grades): CompositeQuad on engine/CSurfaceCompositor.h also needs the private SetBlendMode 0x00F38A10, the CSystemManager -> CDisplayManager -> CRenderManagerCore chain with DrawAsIndexedTris and the quad index table 0x013AC928 named in shared headers (none declare them: authoring)
- e126 land1113fy 007f23d0: port (0 grades): the lightning-orb rows model CTCLightningNode / CLightningDef / the thing classes locally; the move onto engine/CTCLightningNode.h needs more opt-ins than this pass reached
- e127 land1113fy 007f24e0: port (0 grades): as e126 (same lightning-orb family, 453 bytes)
- e136 land1113fy 0094dbc0: port (0 grades): ForceRiseUp models CBaseIntelligentPointer / CAIBrain / CTCCreatureModeManager / CCreatureAction_RiseUp locally around the CAIStateGroup_TrollBase header; needs those moved onto their shared headers
- e170 land1113fy 009c46b0: port (0 grades): the row reaches CCharStringCore and the CFileInstaller map through renaming macros; the move onto fable_string.h / engine/CFileInstaller.h was not reached
- e236 land1113fy 0063f3a0: port (0 grades): 9 wrong binders, each a row with a local CDefinitionManager model (GetDef family); not reached
- e258 land1113fy 0061a260: port (0 grades): the self row is a static __stdcall stand-in; the claim is a const thiscall member of CTCInventoryQuests that ignores this, so the landed callers (TakeQuest 0x006222AA and the quest-list rows that declare IsQuestCardRegionFree) must be respelled in the same pass
- e282 land1113fy 00ba77b0: port (0 grades): the binder is the callee declaration inside 0x00BA77B0, which models CEnginePrimitiveManagerStaticMeshesBase / the sub-primitive / cloth-simulation classes locally; respelling RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh&, ERenderPassType) needs the row moved onto engine/CEnginePrimitiveManagerStaticMeshesBase.h (no header for CEnginePrimitiveManager2DStaticMeshes)
- e287 land1113fy 0061a260: port (0 grades): duplicate of e258 (same address and claim, the game-agent report); see e258
### carry
- 203 entries never reached in this pass (APPLIED note "not reached"); their NF37 round3 notes still apply.

## Rows changed (STATUS.tsv)
- 0x00BBBF40 RELOCATION_MATCH PASS (1 grades; e9)
- 0x00916640 RELOCATION_MATCH PASS (1 grades; e16 e145)
- 0x00A96DA0 RELOCATION_MATCH PASS (1 grades; e21)
- 0x00706860 RELOCATION_MATCH PASS (1 grades; e27)
- 0x0046CE28 MATCH PASS (1 grades; e152 e153)
- 0x0040E160 MATCH PASS (1 grades; e250)
- 0x0040E250 RELOCATION_MATCH PASS (1 grades; e251)
- 0x007E37C0 MATCH PASS (1 grades; e246)
- 0x0042925C RELOCATION_MATCH PASS (1 grades; e263)
- 0x00429460 RELOCATION_MATCH PASS (1 grades; e263)
- 0x009AD390 RELOCATION_MATCH PASS (4 grades; e197)
- 0x009AD3B0 RELOCATION_MATCH PASS (2 grades; e237)
- 0x004107E0 RELOCATION_MATCH PASS (2 grades; e248)
- 0x007E3AA0 RELOCATION_MATCH PASS (1 grades; e249)
- 0x00F333B0 RELOCATION_MATCH PASS (2 grades; e247)
- 0x00668A80 MATCH PASS (1 grades; e60)
- 0x00751A70 RELOCATION_MATCH PASS (2 grades; e80)
- 0x00437260 RELOCATION_MATCH PASS (1 grades; e86)
- 0x00A2E4B0 MATCH PASS (1 grades; e89)
- 0x00A52650 MATCH PASS (2 grades; e53)
- 0x007A4210 RELOCATION_MATCH PASS (1 grades; e104)
- 0x007F4780 RELOCATION_MATCH PASS (1 grades; e125)
- 0x007F3F60 RELOCATION_MATCH PASS (2 grades; e203)
- 0x007F3E90 RELOCATION_MATCH PASS (1 grades; e202)
- 0x007F3AD0 RELOCATION_MATCH PASS (1 grades; e204)
- 0x007F39A0 RELOCATION_MATCH PASS (1 grades; e121)
- 0x007F39F0 RELOCATION_MATCH PASS (1 grades; e122)
- 0x007F3A40 RELOCATION_MATCH PASS (1 grades; e123)
- 0x007F3BB0 RELOCATION_MATCH PASS (2 grades; e124)
- 0x009C6A70 RELOCATION_MATCH PASS (1 grades; e168 e169)
- 0x009C6640 RELOCATION_MATCH PASS (2 grades; e169)
- 0x009C3130 RELOCATION_MATCH PASS (1 grades; e167)
- 0x00437980 RELOCATION_MATCH PASS (1 grades; e191)
- 0x004378F0 RELOCATION_MATCH PASS (1 grades; e74)
- 0x00437900 RELOCATION_MATCH PASS (1 grades; e73)
- 0x009FFB20 RELOCATION_MATCH PASS (4 grades; e72)
- 0x00AC4F20 MATCH PASS (1 grades; e278)
- 0x0048DA89 MATCH PASS (1 grades; e82)
- 0x0048DAA2 RELOCATION_MATCH PASS (1 grades; e83)
- 0x006B6340 RELOCATION_MATCH PASS (1 grades; e156)
- 0x00ADBE50 RELOCATION_MATCH PASS (1 grades; e21)
- 0x00ADB570 MATCH PASS (3 grades; e20)
- 0x007D7C40 MATCH PASS (1 grades; e34)
- 0x00CB5AC0 RELOCATION_MATCH PASS (1 grades; e245)
- 0x0097C900 RELOCATION_MATCH PASS (2 grades; e177)
- 0x009531A0 RELOCATION_MATCH PASS (2 grades; e79)
- 0x00564B20 RELOCATION_MATCH PASS (1 grades; e242)
- 0x0055E7C0 RELOCATION_MATCH PASS (1 grades; e238)
- 0x006DF860 RELOCATION_MATCH PASS (2 grades; e64)
- 0x0057FC46 MATCH PASS (1 grades; e180)
- 0x00742460 RELOCATION_MATCH PASS (1 grades; e265)
- 0x005E8F50 RELOCATION_MATCH PASS (1 grades; e155)
- 0x0049417D RELOCATION_MATCH PASS (2 grades; e84)
- 0x008B8590 RELOCATION_MATCH PASS (2 grades; e133)
- 0x00A5B210 RELOCATION_MATCH PASS (1 grades; e234)
- 0x0057AA1B MATCH PASS (2 grades; e257)
- 0x00915810 RELOCATION_MATCH PASS (1 grades; e146)
- 0x00842400 RELOCATION_MATCH PASS (1 grades; e81)
- 0x007448F0 RELOCATION_MATCH PASS (2 grades; e45)
- 0x00B2AEF0 MATCH PASS (1 grades; e115)
- 0x008C4E20 RELOCATION_MATCH PASS (2 grades; e178)
- 0x00F38990 RELOCATION_MATCH PASS (1 grades; e116)
- 0x00B89080 RELOCATION_MATCH PASS (1 grades; e111 e130)
- 0x0092C520 RELOCATION_MATCH PASS (1 grades; e134 e138)
- 0x0041B9C0 RELOCATION_MATCH PASS (1 grades; e163 e164 e165)
- 0x00B2B050 RELOCATION_MATCH PASS (5 grades; e114)

## Header opt-ins
See hdr/README.md (10 overlay headers; each a copy of the current repo header plus `#ifdef` opt-ins only, verified by diff: no repo line removed or changed; none shared with NF40's overlay).

## Also found (outside my entries, not acted on)
- 0x00753C70 binds ?IsLookTurningOverridenByParent@CThing@@QAE_NXZ at 0x0085F200; the callee's landed spelling (Ego_r/FableWin) is ?IsLookTurningOverridenByParent@NCreatureMode@@YI_NABVCThing@@@Z (not in my queue)
- 0x0064639C self row is CPlayerGuiThingList_0064639c::RemoveThing (stand-in); the forwarder 0x00437900 (CPlayerGui::RemovePerceivingThing) jumps there with ecx = PDrawPerceivers, so it is Ego_r ?RemovePerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z
- 0x0073EAC0: ~27 sibling generated 'Clone' rows (same 44-byte factory shape, e.g. the CModeBasic batch) bind it as a CModeBase copy constructor; Ego_r.pdb/FableWin.pdb have only ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z, and those bodies are NCreatureMode::CModeX::Alloc factories (data refs from RegisterModes 0x0073B130)
- 0x0057FC71 / 0x0057FC9C (CTattooDef::GetTypeBalancedScariness / Goodstrength) likely carry the same `CTattooDef_Methods` stand-in as 0x0057FC46; the TYPEBALANCED opt-in names them.
- 0x00744950 GetExtendedSightRadius and 0x00744580 GetTimeBasedVisibilityMultiplier still use the renaming-macro CTCPerceiveThingLayout view; the new TIMEVISIBILITY_INLINE opt-in serves 0x00744950 as is.
- 0x00B2AF50 CalcColourDifference is still a `CEngineSurfaceCompositionManager_Methods` stand-in; the new CALCCOLOURDIFFERENCE opt-in declares its PDB spelling.

## Method
- Started from NF37's notes and each entry's round3_note; screened all 290 entries against the seeded NF37 caller probe, reloc_bindings.tsv
  and a source grep (work/tab2.tsv, work/wrong.tsv), then worked the cheapest closers first: SELF_WRONG entries whose landed callers
  already bind the claimed spelling (only the self row moves), then BINDERS_WRONG entries with one or two wrong binders.
- Every changed row was copied to rows/<addr>.cpp, claimed in $S/w22/nflocks/<addr> (owner NF39; no row was held by NF40), moved onto
  the shared headers (opt-ins in hdr/ only), graded privately with q.py (bin/g.sh; one compile at a time, `timeout 240`), and its
  relocations checked against the claimed spelling. Recheck batches recheck1/2/3.log, then one final run over all changed rows:
  recheck_final.log.
- Implicit special members retail emitted out of line (deleting destructors, implicit destructors, a force-inlined TouchResource) are
  emitted with non-graded helpers (`Kill_<addr>` / `Use_<addr>`), the pattern the landed G042 and map-destructor rows use.
- No made-up names: every new spelling is a PDB name (Ego_r.pdb and/or FableWin.pdb) or the compiler's spelling of a PDB-typed STL
  instantiation (list::size, map::erase, vector::push_back, pair copy ctor; library-descriptive, template arguments proven by the
  callers' bindings and the PDB member types).
- Tests: the landed tests of the changed rows are self-contained copies of the old models (they define their own functions), so they
  were kept; 0x0046CE28 and 0x009AD390 tests were rewritten earlier in this pass (LINK_FAIL against the old symbol).

## Rule slips (self-reported)
- 0x00A52650: my rewrite first dropped the row's `// Retail-asm:` line (my viewing filter hid it); recheck2 reported NOT_GENUINE; the
  line was restored from the landed row and the row rechecked PASS.
- RENAMES.tsv section 1: 28 lines written this session with module `Fable.exe` and name `Class::Method`; corrected to module = class,
  name = method before hand-back (backup work/RENAMES.bak).
- e73 / e74 were first auto-marked `done` by my helper although binders were left; re-marked `partial` with the binders named.
- One overlay comment lost a backtick span to shell substitution while editing (CStringParser.h, now withdrawn to work/blocked/).
- Two Bash calls broke the "every call exits 0" rule: a payload-inspection script raised (AttributeError, exit 1), and a foreground
  `sleep 45` was refused by the harness. Neither changed a file.
- 0x00B2B050: one grade (of 5) ran with a half-applied edit (`#pragma auto_inline(off)` left on for the whole TU); fixed and regraded RM.
- 0x009B9BA0 (e40): 7 grades spent and the row left unchanged (register-allocation DIFFER), which is within the 10-grade cap.
- The NF39 helper `fin.py` wrote a generic "landed callers already bind it (probe)" note; every done / done-already note was audited
  against work/wrong.tsv before hand-back and rewritten where the entry had no direct caller or a NOREL caller (e133, e134, e152, e156,
  e163, e186, e237, e265).

## Checks run at hand-back
- One address, one name: every section-1 leaf was searched in reloc_bindings.tsv (bound to another target?) and in the `// Leaf:` lines
  of all landed rows at other addresses: 0 conflicts.
- Type-independent STL bodies (list<T>::size 0x00ADB570, pair<float,CWideString> copy ctor 0x00564B20, map<long,unsigned long>::erase
  0x005E8F50, vector<CEmbeddedGraphic>::push_back 0x0049417D): xref.py shows one calling function each (0x00AD9F10 with three call
  sites; 0x00563920; 0x005E5F30; 0x00492381), the one that already binds the spelling, so none is an ICF fold shared by other types.
- 0x009AD390 (e197, 74 callers, 72 not probed): reloc_bindings.tsv binds only ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z
  to 0x009AD390, and the landed declarations grep with that same `(const CCharString&, long) const` signature.

## RELOC_NAME warnings in recheck_final.log (stale catalog labels of the targets; none is a wrong binding)
- 00429460   WARN RELOC_NAME: ?ReleaseData@?$CCountedPointer@VCText@NUISystem@@@@QAEXXZ -> 0042925c labelled 'CCountedPointer<NUISystem::CText>::~CCountedPointer'
- 00437900   WARN RELOC_NAME: ?RemovePerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z -> 0064639c labelled 'CPlayerGuiThingList_0064639c::RemoveThing'
- 007a4210   WARN RELOC_NAME: ?DeleteData@?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@SIXPAX@Z -> 007a4120 labelled '_global::DeleteObject_007a4120'
- 007f3e90   WARN RELOC_NAME: ??$__copy@PAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV1@00ABUrandom_access_iterat
- 007f3f60   WARN RELOC_NAME: ?DeleteData@?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@SIXPAX@Z -> 007f3f00 labelled '_global::DeleteObject_CLightningBranch_007f3f00'
- 0092c520   WARN RELOC_NAME: ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z -> 006bb390 labelled '`anonymousnamespace'::Game_IsTimeOfDayInRange'
- 009ad390   WARN RELOC_NAME: ?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z -> 009ad2e0 labelled 'CFactionDatabase::FindFactionBySymbolName'
- 009ad3b0   WARN RELOC_NAME: ?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z -> 009ad2e0 labelled 'CFactionDatabase::FindFactionBySymbolName'
- 009c6a70   WARN RELOC_NAME: ?DeleteData@?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@SIXPAX@Z -> 009c6640 labelled 'SCountedWideStringPair_009c6640::DeleteObject'
- 00a96da0   WARN RELOC_NAME: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKJ@s
- Targets 0x0042925C and 0x009C6640 are renamed in my RENAMES section 1; 0x006BB390 is e138 (partial), 0x0064639C is in work/alsofound.txt; 0x007A4120 / 0x007F3F00 / 0x009AD2E0 / 0x004507A1 and the copy helper carry propagated labels outside my entries.

