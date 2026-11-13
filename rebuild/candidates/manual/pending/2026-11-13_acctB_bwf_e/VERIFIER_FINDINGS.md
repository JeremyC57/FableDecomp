# Verifier findings of the bwf2 author workflow (two independent lenses per matched row; `#2` = re-verification after the repair)
Recorded verbatim from the workflow run record, so the landing coordinator can judge each open item. A row whose `#2` lines are both ok was repaired and re-verified; rows without `#2` lines that show ISSUES had their repair recorded but the re-verification is still running or pending.

## verify-evidence:0045a159: ok

## verify-mechanical:0045a159: ok

## verify-evidence:004830a1: ISSUES
- NAME_FIXES.tsv row 0x00485AEF does not give a usable correct symbol: it says `?clear@?$_Rb_tree@VCWideString@@...(map<CWideString,bool> Signatures clear)`, an elided placeholder, and cites one source (the call with ecx=this+0xE0). Neither Ego_r nor FableWin has a matching _Rb_tree<CWideString,...>::clear symbol in the extracts, so the row needs the full STLport instantiation spelling, or it must be marked descriptive with two pieces of evidence (rule 6).
- Source line 195 `menu->FindChild("UI_MENU_ENTRY_SAVE");` throws its result away and has no comment. Retail does make this call with the result unused (FindChild at 0x0048351E, push 0x12373EC, no store before the next call at 0x00483534), but the source must say which retail bytes require it, or use a plausible unused local with that citation.
- Source lines 414-415 `bool valid = VerifySignatureCached(...) && (!hasSignature || VerifySignatureCached(signaturePath));` set a local that is never used, with no comment. It is needed for the 8 calls to 0x0047ED0A in retail, but the row must cite that retail fact (the extra GetPathNameFromFileName/VerifySignatureCached pair between AddActionOnSelected and AddChild(fileDescription)), or it reads as arbitrary.
- Callee 0x00407030 is declared `CUserProfileManager::IsDebugProfile() const` in the hdr overlay. That address is an ICF-folded `xor al,al; ret` body already landed as CThingFilter_IsUnconscious_operatorcall (rebuild/src/compiled/00/40/CThingFilterIsUnconscious_operator_00407030.cpp; label _global::ReturnFalse_ret0_00407030). DOSSIER.md calls this binding 'ONE source -- descriptive', but NOTES.md and NAME_FIXES.tsv do not record it as an inferred, one-source binding of a second name to an already-named address.

## verify-mechanical:004830a1: ok

## verify-evidence:005a04f7: ISSUES
- NOTES.md says the landed row 0x0059B79A (rebuild/src/compiled/00/59/CTCInventoryTrade_GetMenuBranchFromDefIndex_0059b79a.cpp) spells 0x005D96F0 as `GetInventoryTabState`, but no NAME_FIXES.tsv was handed back and the note gives neither the correct symbol `?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z` nor two sources. The same misspelling is also in the landed rows 0x0059C0C6 and 0x0059C447. That landed 0x0059B79A row also uses a non-PDB signature (`CMenuBranch* (long, CMenuTree*, long*)` instead of `CComponent* (long, CComponent*, EItem_Menu_Type&)`). This row now carries a second, PDB-spelled body for the same retail address, so the fix has to be handed back explicitly.
- Line 95 defines a local stub `class CThingPlayerCreature : public CThing { };` even though the shared header rebuild/include/engine/CThingPlayerCreature.h exists and derives the class from CThingCreatureBase. The stub has the wrong base class, and no comment explains why the shared header is not used for the PeekPControlledCreature()->PeekTC call.
- The same-TU template bodies CDefinitionManager::GetDef<T> (lines 107-119) and CBasicString<T>::Compare / operator== (lines 120-138) have no comment in the row source. No comment says which retail fact shows the bodies were visible (the out-of-line COMDAT calls 0x0042B312 and 0x004115A0) or names the landed rows they copy (0x005A29F4 and 0x005A5B31); those appear only in NOTES.md.
- `CCountedPointer<CComponent> unusedComponent;` (line 215) is never used, and the source does not cite the retail bytes that require it: a second inline-zeroed CCountedPointer at [ebp+30h]/[ebp+34h] (`mov [ebp-50h],edi ; mov [ebp-4Ch],edi`) destroyed through 0x004291DE at the epilogue. A reviewer would see it as arbitrary.
- The callee comment (line 13) calls 0x0042925C the `CCountedPointer<CText>` dtor, but the compiled relocation is `?ReleaseData@?$CCountedPointer@VCText@NUISystem@@@@QAEXXZ`. The coordinator's vland log flags it RELOC_NAME against the landed label `CCountedPointer<NUISystem::CText>::~CCountedPointer`, so one retail address currently has two names and the conflict is not explained in NOTES.

## verify-mechanical:005a04f7: ISSUES
- Check 4: src.cpp line 95 defines a local `class CThingPlayerCreature : public CThing { };` while rebuild/include/engine/CThingPlayerCreature.h exists. That is a local model of a header class. Two attempts to swap in the shared header both failed to compile (verify_mechanical/src_hdr.cpp: PeekTC is not a member; verify_mechanical/src_hdr2.cpp with FABLE_RETAIL_CTHINGCREATUREBASE plus FABLE_RETAIL_CTHINGPLAYERCREATURE: CHitParameters errors and negative subscripts in CThingCreatureBase.h). The same stub already sits in 325 landed rows, so this is the lane's existing convention, but under the check-4 rule it is still an issue.
- Check 5: the same-TU body of CTCInventoryTrade::GetMenuBranchFromDefIndex (0x0059B79A) has the same logic as the landed rebuild/src/compiled/00/59/CTCInventoryTrade_GetMenuBranchFromDefIndex_0059b79a.cpp, but the code is spelled differently. Row: CComponent*/FindChild, EItem_Menu_Type& with ITEM_MENU_TYPE_* constants, CTCInventoryItem::GetUIInventoryCategory. Landed: CMenuBranch*/CMenuTree*/GetBranch, long* with literal 0/1/2, GetInventoryTabState. This breaks the one-address-one-name rule. Either the row or the landed spelling must be reconciled, and no NAME_FIXES.tsv was handed back.
- Check 5 (minor): the template same-TU bodies differ in spelling from their landed rows. Row CDefinitionManager::GetDef<T> (0x0042B312) uses out->SetDynamicCast(def) where the landed GetDef_0042b312 uses `out = def`. Row CBasicString<T>::Compare/operator== (0x004115A0) is a template where the landed row is a non-template CBasicStringChar::operator== built on base-offset pointer arithmetic. Same logic, different code.

## verify-evidence:005aaf28: ISSUES
- Source compares `weaponDef->Class == 2` although CWeaponDef::Class is EWeaponClass and rebuild/include/engine/EWeaponClass.h defines WC_HEAVY=2 (retail 0x005ABD10 `cmp dword ptr [eax+0x2c],2`); a Lionhead programmer would have written the enumerator, so spell it `WC_HEAVY` (same bytes).
- The test's second case is commented 'selling for less than bought: loss line tinted B,G' but it actually checks 'Profit: 30' (price 80, bought 50), and the loss branch (profit < 0, B/G tint, -profit text) is never exercised; a mutant changing `profit >= 0` to `profit > 0` still prints PASS, so fix the comment and add a loss case (e.g. bought > sell price).
- NAME_FIXES.tsv does not carry the landed row rebuild/src/compiled/00/5b/CTCInventoryBase_GetBoughtPrice_005be8d3.cpp, whose leaf `GetBoughtPrice@CTCInventoryBase` collides with this row's binding of NInventory::CTCInventoryBase::GetBoughtPrice to 0x005BEAD5 (one name, two addresses). Evidence: 0x005BEAD5 tail-jumps to CCategory::GetBoughtPrice 0x005BE839; 0x005BE8D3 ends in 0x005BE8B1 (CCategory::GetPerItemData); labels record 0x005BE8D3 as NInventory::CTCInventoryBase::GetPerItemDataForItem (wave-1 decision); Ego_r has both ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z and ?GetPerItemDataForItem@CCategory@NInventory@@. The collision is older than this row, so landing need not wait for it, but the respelling should be queued.

## verify-evidence:005aaf28#2: ok

## verify-mechanical:005aaf28: ok

## verify-mechanical:005aaf28#2: ok

## verify-evidence:005ad571: ISSUES
- NAME_FIXES.tsv is incomplete: the coordinator's vland.log (RELOC_NAME warnings) shows three more landed callees that this row's evidence proves misnamed, and none of them was handed back. 0x005BEAD5 is landed as CTCInventoryBase::GetItemDescription (CTCInventoryBase_GetItemDescription_005bead5.cpp) but is ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z: Ego_r 0x00621385, FableWin 017c1915/02367dce, one stack argument, and its result is subtracted from price at 0x005ADDE7.
- NAME_FIXES omission: 0x005D99D0 is landed as GetInventoryCategory (global_GetInventoryCategory_005d99d0.cpp) but is ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z (Ego_r 0x0055a9a0, FableWin 017beab7/01e03620).
- NAME_FIXES omission: 0x005BC4C4 is landed as DrawGetEnvironment (global_DrawGetEnvironment_005bc4c4.cpp; manifest label GetMember_dword0xb8_005bc4c4) but is ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ (Ego_r 0x0061f34a, FableWin 01800d36/02368b47).
- Minor: the landed 0x0042B312 is spelled with an address suffix (CDefinitionManager_GetDef0042b312_0042b312.cpp), and this row identifies it as GetDef<CUIMiscThingsDef>. That correction could also go into NAME_FIXES.
- Minor comment inaccuracy: the source puts the `movzx ax,al` that justifies `text = NGameText::ConstructString(fmt, args)` at 0x005AE4F7, but in retail it is at 0x005AE4EA (the call to operator=(wchar_t) is at 0x005AE4F2, and 0x005AE4F7 is the next instruction after it).
- Minor: the source's caller list (RefreshDescriptions / RefreshBuyDescriptionsPC / RefreshSellDescriptionsPC) leaves out NUISystem::CManager::Action 0x0041E6D3, which calls this function three times (0x00425095, 0x004256FF, 0x00425CFF).

## verify-evidence:005ad571#2: ok

## verify-mechanical:005ad571: ok

## verify-mechanical:005ad571#2: ok

## verify-evidence:00607330: ISSUES
- hdr/engine/CHeroLogBook.h (FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS) declares `const std::map<...>& GetLogEntries() const`, but the only PDB spelling (FableWin.pdb, _fablewin_api.tsv / FableWin.exe.signatures.tsv) is `?GetLogEntries@CHeroLogBook@@QBE?BV?$map@...@std@@XZ`, which llvm-undname reads as returning `const std::map<...>` BY VALUE. The return type disagrees with the PDB, and the header comment cites that symbol as if it matched; it also has a single source, since Ego_r.pdb does not list it.
- The compiled object references `??1?$_List_base@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ` at 0x0042ABCA (coordverify/vland.log WARN RELOC_NAME). FableWin.pdb and the landed row stdlistNUISystemCComponent_list_0042abca.cpp both name that address `??1?$list@PAVCComponent@NUISystem@@V?$allocator@...@std@@@std@@QAE@XZ`, so the callee symbol the row emits does not match the retail name that the source comment claims (~list 0x0042ABCA).
- The row defines the CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) template body locally (lines 86-97). That duplicates the shared-header body in rebuild/include/engine/CDefinitionManager.h (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, which is noinline) instead of adding an inline-variant opt-in to the shared header, and the row text uses `!def.IsNull()` where the header has `def.Get()`.
- At site 1 the guard `if (entry && arrow)` sits inside `if (entry)`, so the `entry` test is redundant. NOTES.md says this redundant test is what makes VC7.1 call ~list 0x0042ABCA out of line at site 1 (and the RELOCATION_MATCH depends on it), but the source comment never says which retail bytes require it. Unexplained in the source, it reads as an arbitrary construct put there to steer the compiler.
- Two callee bodies are made visible, and the row source does not justify them or name the landed rows they copy. For GetSubDef<CAppearanceDef> (included via engine/CDefClassBase_GetSubDef.h with GETSUBDEF_NOINLINE), the source says only 'body is visible but kept out of line' and cites no retail fact; NOTES gives the reason as appearance kept in edi. The landed row CDefClassBase_GetSubDefCAppearanceDef_0042b0a2.cpp is not named. For list::_M_insert_dispatch (FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE), the row source does not mention the visible body at all, and the landed row stdlistNUISystemCComponent_Minsertdispatch_0042b4c3.cpp is not named.

## verify-mechanical:00607330: ok

## verify-evidence:0073b130: ISSUES
- NOTES.md claims the landed name of 0x007441D0 (`_global::Sort_SElement8_007441d0`) is really `std::sort<pair<unsigned long,CRegisteredModeInfo>*,CKeyPairCompareLess<..> >` but gives no mangled symbol and no two sources, and xref shows 0x007441D0 is also called from 0x007442DE and 0x00744305 (a possibly folded 8-byte-element sort body), so the rename is unproven and must either carry evidence (address, exact PDB symbol, two sources, proof the body is not shared by another instantiation) or be dropped.
- NOTES.md claims 0x00742460 `CModeBasic::Clone` is really `CModeBasic::Alloc` without stating the correct mangled symbol (`?Alloc@CModeBasic@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z`) or its two sources in the note itself (the evidence, vtable 0x012675BC stored after the CModeBase ctor 0x0073EAC0 call and the only reference from this table at 0x0073CA54, sits only in DOSSIER.md); no NAME_FIXES.tsv is handed back for it.
- This row introduces the PDB spellings `CTCCreatureModeManager::RegisteredModes` (0x013BAD44) and `CRegisteredModeInfo::AllocFunc`, while the landed CTCCreatureModeManager_AllocateMode_0073a8e0.cpp names the same retail object `g_CreatureModeRegistry` (local `struct CModeRegistry`) and the member `Allocate` (typed `CModeBase* (__fastcall*)(CThing*)`, not `CThing&`); one address now has two names and the author handed back no NAME_FIXES.tsv entry for the landed made-up spellings.

## verify-mechanical:0073b130: ok

## verify-evidence:007cacb0: ISSUES
- $W/NOTES.md does not mark the two made-up names the source uses as descriptive: `g_ItemUp` (file-static C3DVector at 0x01383A5C, no PDB/FableWin symbol found; DOSSIER.md line 55 calls it descriptive) and `CASoundManager::Get2DEmitter` (vtable slot 9, CASoundManager.h:141 says '(descriptive)'). Both are spelled the same way as the landed PrepareToShowItems 0x007CA670 and the shared header, so the names stay as they are, but NOTES.md has to list them as made-up.

## verify-evidence:007cacb0#2: ok

## verify-mechanical:007cacb0: ok

## verify-mechanical:007cacb0#2: ok

## verify-evidence:00a04630: ok

## verify-mechanical:00a04630: ok

## verify-evidence:00a079b0: ok

## verify-mechanical:00a079b0: ok

## verify-evidence:00b72180: ok
- Minor, does not block landing: the carried CGraphicDataBank::PeekGraphic / PeekGraphicInfoFrame bodies say they come from the 'matched sky rows 0x00B64FA0 / 0x00B65A20'. Those rows are matched only in the lane (agent2048) and are not landed in rebuild/src. The landed copy is rebuild/src/compiled/00/ba/CEngineInternalPrimitive2DClockSprite_Update_00badb40.cpp, and the comment should name that landed row. Also, PeekGraphic is plain `inline` here but FABLE_FORCEINLINE there.
- Minor, does not block landing: in the test, case 3's comment says 'only the sea pass asked', but the call is CreateDynamicTextures(false, true, true) and the expected log includes the ice levels 20 and 21, so the comment should say sea and ice.
- Landing to-do already flagged by the author: 8 of the hdr/ overlay headers are copies derived from 2048 that are older than HEAD. The coordinator must re-base them before landing.

## verify-mechanical:00b72180: ISSUES
- Overlay headers are older than the repo copies and remove existing repo lines, which is not allowed. `diff rebuild/include/<h> $W/hdr/<h> | grep -c '^<'` gives C2DVector.h 23, CEngineResourceManager.h 37, CGraphicFrame.h 9, CGraphicInfo.h 2, CGraphicInfoFrame.h 4, CIVCountedPointeeBase.h 1, CIVCountedPointer.h 7 (for example, the repo's FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE block and its fable_compiler.h include are gone), CRenderStateManager.h 15 and CShaderRenderManager.h 24. hdr/README.md lists 8 stale headers and leaves out CIVCountedPointeeBase.h. The overlay has to be re-based on HEAD before landing.
- hdrcheck2.py reports no default=DIFF, but it reports DIFF for opt-ins that landed rows already use: FABLE_RETAIL_CENGINERESOURCEMANAGER (9 landed users), FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES (6), FABLE_RETAIL_CENGINESKYRENDERER (1) and FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW (1). Landing the overlay as it is would change headers that those landed rows depend on.
- The source defines CEngineWaterRenderer::SetRenderTargetToSurface, and its code differs from the landed row rebuild/src/compiled/00/b7/CEngineWaterRenderer_SetRenderTargetToSurface_00b7ad20.cpp. The row calls `NEngineInternal::GPRenderManager->SetVWindow(...)`, while the landed row calls `g_pRenderManager->SetViewport(...)`, for the same addresses 0x01436E18 and 0x00A0B560. The two rows now give one address two names. NAME_FIXES.tsv does not carry the respelling (SetViewport to SetVWindow, g_pRenderManager to NEngineInternal::GPRenderManager), so the landed row has to be respelled or the row aligned with it. The row's version is also FABLE_FORCEINLINE where the landed one is a plain definition.
- The row only builds with its overlay. With FABLE_HDR_OVERLAY pointed at an empty directory, fg.py gives TIMEOUT ('cl hung: usually an #error in a header'). The row needs at least the new engine/CRenderTarget.h plus the overlay opt-ins in CEngineWaterRenderer.h, CGPUScratchBufferTextureManager.h, CTexture.h, CSurface.h, CScopedShadersEnable.h and CRenderStateCaptureBlock.h, and it inherits five other NEW headers from the 2048 overlay (CEngineWeatherSettings.h, CIDrawEnvironment.h, CIDrawWorld.h, CSkyDef.h, CWeatherMask.h). Those all have to land with the row.

## verify-evidence:00b9ece0: ISSUES
- NOTES.md lists only the 0x00BA1A70 signature fix, yet DOSSIER.md line 37 proves another landed spelling wrong and the author never handed it back. The landed callee 0x00A5D5F0 (rebuild/src/compiled/00/a5/CShaderResource_GetPixelShader_00a5d5f0.cpp, `// Leaf: GetPixelShader`) is really ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z. The evidence is the FableWin PDB at 0x032EA750, the manifest label, the 65 VSHADER_ literal call sites in this row, and the CVertexShader targets. It needs a NAME_FIXES entry (or NOTES line) with that evidence so the coordinator respells it.
- The 0x00BA1A70 fix in NOTES.md says 'from DOSSIER, not re-proved here' and gives no mangled symbol or sources in NOTES itself. The evidence is in DOSSIER.md section 2 (Ego_r.pdb 0x00A55E60 and FableWin.pdb 0x02E98FE0, both ??0CEnginePrimitiveManagerStaticMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z). Carry it into NOTES, and also note that the 0x00BA1A70 row's own ctor (long type) and its local stand-in struct are wrong by the same evidence.
- The behaviour test has two weak spots. Its check `lo.Static[1].D == 0` passes no matter what, because the model never assigns Static[1]. The model also fetches the pixel shader from vertex bank 1 instead of a separate pixel-shader resource, as the real function does through GetPShaderResource(). These do not block landing but should be tightened.

## verify-mechanical:00b9ece0: ok

## verify-evidence:00bb5040: ok

## verify-mechanical:00bb5040: ok

## verify-evidence:00bbf4a0: ISSUES
- Faithfulness/body justification: the row defines the bodies of CEnginePrimitiveRenderUtil::SetupBlendOpState, SetupAlphaBlendingStatesFromBlendMode, ApplyVertexShader, ApplyPixelShader, GetCurrentCamera and the template GFDebugPDynamicCast with no comment that names the row they copy (they are verbatim from the not-yet-landed sibling /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp) or the retail fact that shows the body was visible. The sibling row carries that justification in its 'Inline-budget' comment (lines 33-36); this row does not.
- Unexplained compiler steering: SetupBlendOpState and SetupAlphaBlendingStatesFromBlendMode are marked FABLE_FORCEINLINE, and so is the in-row GFFastSin copy. The source never says which retail bytes need this. The header comment only says the pass-6/7 and point-light pickers are 'expanded' and that GFFastCos/GFFastSin are 'written as the landed out-of-line bodies'. The sibling 2059 states the inline-budget numbers (cost c1/f/score) that force it; this row should carry an equivalent line.
- GFDebugPDynamicCast is redefined locally (`template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To)`) even though the identical template already lives in the shared header rebuild/include/engine/CThing.h:47. This is a duplicate definition outside a shared header (the FableWin symbol ??$GFDebugPDynamicCast@PAVCEnginePrimitiveManager@@PAVCEnginePrimitiveManagerParticleGroup@@... confirms the cast exists). It should come from a shared header or carry a comment explaining why it can't.
- Names: the global at 0x01436E38 is spelled g_pDisplacementRenderer_01436e38. That is an address-suffixed made-up name, and NOTES.md does not list it as descriptive (only g_particleSpriteRenderEnabled_0139b6e0 is listed). The same address is also landed elsewhere as g_pEngineSettings_01436e38 and g_pShadowRenderer (CEngineShadowRenderer*), which breaks one-address-one-name. The row's own evidence (FableWin inline ?GetEnable3DDisplacement@CEngineScreenEffectDisplacementRenderer@@QBE_NXZ read at +0xEF) contradicts the g_pShadowRenderer / g_pEngineSettings spellings, yet NOTES.md says 'No landed name proved wrong' and returns no NAME_FIXES entry for them.

## verify-evidence:00bbf4a0#2: ok

## verify-mechanical:00bbf4a0: ok

## verify-mechanical:00bbf4a0#2: ok

## verify-evidence:00bcff10: ISSUES
- $W/NOTES.md does not list the made-up global name g_pResourceManager (0x01436E98, no PDB name) as descriptive. Only the source comment and DOSSIER call it descriptive, and the rule requires made-up names to be marked descriptive in NOTES.
- The DOSSIER proves two wrong landed spellings in 0x00B99D90 (rebuild/src/compiled/00/b9/CEnginePrimitiveManagerRepeatedStaticMeshes_..._00b99d90.cpp). It declares 0x009BEE00 as CalculateTextureMemoryFromQuality, which should be CDisplayManager::GetPixelShaderVersion, ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ, found in both Ego_r and FableWin. It also declares 0x00BEBC30 as CEnginePrimitiveGroupManager(CEngineSceneArray*), which should be ??0CEnginePrimitiveManagerMeshBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z. Neither appears in $W/NAME_FIXES.tsv, so 0x009BEE00 and 0x00BEBC30 would each keep two names once this row lands. NOTES.md also gives the 0x00BEBC30 parameter-type fix as 'not re-proven here', with no evidence line.

## verify-mechanical:00bcff10: ok

## verify-evidence:00d30030: ISSUES
- Line 67 `inline CScriptThing::~CScriptThing() { }` carries a real callee body (retail out-of-line ~CScriptThing 0x004AA840, landed rebuild/src/compiled/00/4a/CScriptThing_CScriptThing_004aa840.cpp) with no comment saying which retail fact shows the body was visible (the inline-expanded ~CScriptThing at the early exits: ~CCountedPointer + ~CBaseClass 0x0099A2E0 calls) or naming the landed row it copies; check 5 needs that justification line.
- The morality epilogue's `CGameScriptInterfaceBase* game = Game;` plus explicit `game->PauseAllNonScriptedEntities(true/false)` at every exit hand-expands the PDB class CWideScreenMagicPauseEntities (??0CWideScreenMagicPauseEntities@@QAE@AAVCGameScriptInterfaceBase@@@Z / ??1 in egor_pdb_names.tsv), which the original very likely used, and the source has no comment for it. NOTES.md says the RAII form was tried and is worse, but the source should say so, citing retail's cached Game pointer in edi (0x00D3160B `mov edi,[esi+40h]`, reused for Pause(false) at 0x00D31714 and in the early exits).

## verify-mechanical:00d30030: ISSUES
- The hdr/engine/CScriptGameResourceObjectBase.h overlay and its FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR opt-in have no effect. With an overlay holding only stlport/fable_containers.h, fg.py still prints RELOCATION_MATCH, while hdr/README.md says the opt-in is needed to call 0x006E7A60. Drop the header change and the #define, or show a build that needs them, before landing.

## verify-evidence:00d6d670: ISSUES
- The source carries real callee bodies for CScriptThing::GetPos (FABLE_FORCEINLINE, retail out-of-line 0x004AA980, CScriptThing_GetPos_004aa980.cpp) and CScriptThing::IsAlive (inline, retail 0x004AB130, CScriptThing_IsAlive_004ab130.cpp), but the header comment never says which retail fact shows these bodies were visible (both expanded inline, no call to 0x004AA980 or 0x004AB130 anywhere in the 0x16AA bytes) or which landed row each copies. It only lists 'CScriptThing slots 6, ... 75'. A one-line justification is needed, like the one already given for ~CScriptThing and CTimer::GetTime.

## verify-mechanical:00d6d670: ok
