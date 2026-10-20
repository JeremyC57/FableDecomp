# Hand chunk 213 (CTargetingDef .. CThingBuilding): 20 of 40 byte-exact

Lane: RTM cl 13.10.3077 (re-graded every row after the lane change; nothing moved). No row needed a non-default
pragma beyond the three def operator= rows (`#pragma optimize("s", on)`), none needed /GS except InitiateDivorce,
none /EHsc except CBaseRenderer::Stop. No SKIP rows.

## What matched (20)
- 0045d729 CTargetingDef::operator= (size TU; map operator= 0x45D7FE named CRetailTree_0045d871::operator=)
- 004e2e0d CTavernGameSpotTheAdditionDef::operator= (size TU)
- 004ec174 CObjectAugmentationsDef::operator= (size TU)
- 004345a0 CConfigOptionsDefaultsDef::operator= (inlined CDefClassBase/CParentDefClassBase chain)
- 006cfda0 CTCWife::OnHeroViolence
- 006cef20 CTCWife::InitiateDivorce (/GS; by-value `AddBool(bool)` folds the constant flag)
- 00a98860 CTemporaryBuffer::CTemporaryBuffer (declares `operator new[]` 0xBFEB22)
- 00ee3ee0 CTH_Trigger_Guild_Rose::Main (SetPredicate 0xF35B10 / IsActiveThreadTerminating 0xF35B30 -- the PDB names,
  chosen over the landed RegisterFilter/IsDone aliases; CGSI slots 70/108/293/477 named `<Verb>_slotN`)
- 009fa280 CTexture::Initialise (inline `CPixelFormat::GetD3DFormat()` table lookup)
- 00a6a360 CTextureManager::CTextureManager (the pools' CArray as STLport vector(0): that is what writes end-of-storage twice)
- 00ca66d0 CBaseRenderer::Stop (/EHsc; catalog CTextureRenderer::vfunc_4; CBaseFilter is 0x50 because of the __int64 member)
- 0054ecb0 NUISystem::CText::CanScroll (a C2DVector local whose X is never set gives the 0x10 frame)
- 0054fb10 NUISystem::CText::ForceWidth (inline `CWideString::operator[]` makes the character a temporary before the fastcall)
- 004c72d0 CThing::SaveSerialiseCreatableTCs
- 004c8cf0 CThing::SetInLimbo
- 004c8da0 CThing::OnCollision (the colliding thing is the FOURTH argument; 0xA9 = CTCAssassinRush, 0xA8 unnamed)
- 004c9030 CThing::CThing (bitset ctor = memset of nine words; STLport list header node; bitfield init list)
- 004ca930 CThing::CreateFromDef (packed def: TCs +0x39, NoTCs +0x3D, bits +0x41)
- 00830dd0 CThingAICreature::InitialiseMovementSpeeds (scoped CCharString + named CAnimFilterAnyGroup local)
- 0082d1d0 CThingBuilding slot 34 (catalog vfunc_34; landed twin 0x006A9D10 model, scale 1.0 instead of 0.7)

## Parked for Fable 5.1 (20)
- 006d00a0 CTCWife::Murdered DIFFER(217v217): final SetMarriageState argument load before the component load | tried: out-param GetInterface, HasTC+GetTC wrapper, Find-form GetTC, named stats local | next: lane gap: argument scheduled before the component load
- 006cf3b0 CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn DIFFER(206v206): ParentThing argument loaded before the component in all four calls | tried: 4 forms incl. `&&` | next: lane gap: same as Murdered
- 006cf480 CTCWife::InitWifeAtEndOfMarriageFrescoEndFadeIn DIFFER(175v163): `mov ebx,ecx ; lea edi,[ecx+28h]` def-address CSE + 8-byte frame vs `mov edi,[esp+8]` 4-byte frame | tried: plain local, CDefHolder, `long dowry` local, Object->Dowry | next: lane gap: QFE-style address CSE
- 007e2f00 CTCWobble::FrameUpdate DIFFER(199v199): `lea ecx,[esp+8]` placed across a neighbouring push | tried: pointer-local this, 3 other forms | next: lane gap: esp-relative lea scheduling
- 00757680 CTCWound::~CTCWound DIFFER(216v217): ebp/ebx colouring of this and the zero | tried: `const long none=0` local etc. | next: lane gap: callee-saved colouring
- 006238a0 CTestQuestNameCompare::operator() DIFFER(218v225): lea/push order + esi threading of nameB's data pointer | tried: GetPString forms | next: lane gap: RTM value threading
- 00a69b70 CTextureManager::AllocateTexture DIFFER(170v176): this/index/tex colouring (retail esi/edi/ebx) and the add-ref folded to `inc [esi+4]` where retail keeps `mov esi,ebx ; mov eax,[esi+4] ; inc eax ; mov [esi+4],eax` | tried: vptr+Object layout, `old` release, `RefCount = RefCount + 1`, explicit base pointer | next: lane gap: unfolded add-ref on a forwarded store
- 00a69c20 CTextureManager::AllocateVolumeTexture DIFFER(180v186): same as 00a69b70 | tried: same | next: same
- 0081b0a0 CTextureMorphs::GetAllMorphsOfType DIFFER(178v177): zero kept in ebx across resize (`cmp esi,ebx`), out in ebp; retail zero edx / out ebx / loop temp ebp pushed in the loop | tried: early-return vs shared false exit, compiler-generated copy, resize(n) wrapper | next: lane gap: constant-zero CSE across a call
- 004c9f60 CThing::AddAndCopyTC DIFFER(168v166): this/type colouring; component loaded into eax then copied to ecx where retail loads straight into ecx (retail reads the unset pointer when the type is absent -- a retail bug kept in the model) | tried: template out-param, plain out-param, Find-form, Find-form with map pointer | next: lane gap: `this` register for a virtual call on a looked-up pointer
- 004c7e90 CThing::IsUnderScriptSupervision DIFFER(165v169): virtual call on the CreateScriptThing temporary through eax where retail re-addresses the frame temporary; Info reload for delete in eax vs ecx | tried: `&&` temporary, const-ref helper, `&temp` extension, inline member on the temporary | next: lane gap: hidden-result temporary addressed by frame
- 004c7f40 CThing::InitFromThingInfo DIFFER(175v167): inlined operator==("NULL") materialises a bool (`sete al ; jmp ; test al,al`) | tried: if/return, ternary in operator==, landed strcmp(text,"") form, ternary in the if | next: lane gap: jump threading of an inlined two-source bool
- 00830cb0 CThingAICreature slot 34 DIFFER(201v201): `fcom 0.0` before `fst [ecx+8]` in retail, after in ours (byte-wise form); the landed-twin form is 206v201 | tried: g_zero externs, record-field compares, twin model with literals | next: lane gap: FPU compare/store order
- 00830f20 CThingAICreature slot 73 (HasAnimation) DIFFER(231v183): `lea edi,[esi+34h]` before the filter call and the def pointer kept in esi for every release; 8-byte frame | tried: holder reloads, `old` release, mode-first ordering | next: table reference local before the filter call + release on the cached pointer
- 008328d0 CThingAICreature::OnHeroPush DIFFER(220v218): CCharString temporary gets a frame slot where retail reuses the dead pusher slot shared with the lookup key (`lea ecx,[esp+8]` hoisted above the branch) | tried: Find-form GetTC, braces, out-param GetInterface | next: lane gap: dead-parameter slot sharing between arms
- 00833010 CThingAICreature::SetNewBrain DIFFER(176v178): retail loads g_pThingManager into ebx before the GetPlayerManager call and keeps the brain in ebp | tried: empty CAIBrain, sized CAIBrain, noalias (not in VC7.1), const global | next: lane gap: global load hoisted above a call (whole-program optimisation)
- 0082d2d0 CThingBuilding::PeekPSimDefinition (catalog PeekPMaterial) DIFFER(254v221): holder release compiled unaligned-style (`add eax,4 ; mov [eax],edx`) because the building def is a pack(1) class | tried: derived holder, `old` release, base-typed `old` | next: unpack the def (every used field is 4-aligned), pack only CEngineGraphic
- 0082d5c0 CThingBuilding::PeekPMaterial DIFFER(260v227): same as 0082d2d0 | tried: same | next: same
- 0082d6f0 CThingBuilding::OnCreate (catalog vfunc_8) DIFFER(238v218): same pack(1) release shape + ebx/edi colouring of the appearance pointer and the holder | tried: same | next: same unpack
- 0082def0 CThingBuilding::IsUnlocked DIFFER(201v193): two key slots (8-byte frame) where retail shares one; entry `test byte ptr [ecx+24h],2` splits into `mov al / test al` | tried: early-return, positive structure, GetPItem cached, out-param GetInterface | next: lane gap: frame-slot sharing of inlined lookup keys

## Skipped
- none

## Label corrections
- 0x006D3DC0 is CDefClassBase::GetSubDef<CWifeDef> (not the manifest's instantiation)
- 0x006AEBA0 CTCRegionFollower::RemoveRegionFollower (manifest label propagated)
- 0x009F4980 CScratchBuffer::Lock(void**) (manifest "GetRegion" propagated)
- 0x00758B00 list<CWound>::~list (manifest label propagated)
- 0x00EE2DB0 CV_TravellingHeroesScript member taking one CCharString by value (name unknown)
- 0x00CA66D0 is strmbase CBaseRenderer::Stop (IBaseFilter slot 4), catalog "CTextureRenderer::vfunc_4"
- 0x00682140 CTCCreatureOpinionOfHero::ApplyOpinionTransientOffset
- 0x0081DAA0 vector<CTextureMorphEntry>::resize (manifest "vector<CBrainBehaviour>::resize" propagated)
- 0x009A4EC0 GFGetSystemManager (__fastcall) -- confirmed by use in CTexture::Initialise
- 0x0041E5F2 CFrontEndManager::GetInstance / 0x0041CF47 NUISystem::CManager::GetUIScale (C2DVector by hidden pointer)
- 0x0054F4B0 GetCharWidthW(unsigned short) __fastcall (`mov si,cx`), CText slot 144 = SetValue(const CWideString&, bool, bool)
- 0x004C79F0 and 0x004C7A00 are both CThing::GetPlayerManager() (`g_pThingManager->PlayerManager`, +0x30); manifest "CViewBase::GetDisplayEngine" is propagated onto them
- 0x004C7990 CThing::GetDefinitionManager, 0x004C7A70 GetDisplayEngine, 0x004C7AA0 PeekWorld (landed names)
- 0x004CBFB0 CTCAssassinRush::SetThingCollisionPtr confirmed (TC 0xA9 bit tested in OnCollision); 0x004CC000 is the TC-0xA8 handler, not CVillageTaskCarryCrate::AssignCreatureToTask
- 0x0042AF3C CDefinitionManager::GetDef(long, CDefPointer<const CThingBaseDef>*) (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>" propagated)
- 0x0042B0A2 CDefClassBase::GetSubDef (manifest "GetSubDef<CAbilityDef>" propagated)
- 0x0099A330 on CPlayer: a bool query (landed twin calls it CInitBaseClass::IsInitialised; manifest "GetCanItemBeQuickAssigned" propagated)
- 0x006A4E40 a CThing normalised-time getter (manifest "CParticleSystem::GetNormalisedTime" propagated)
- 0x004AB840 / 0x0082E1E0 / 0x0066C340 CDefinitionManager::GetDef instantiations (building def, sim-building def, material def)
- 0x0082D2D0 reads CThingBuildingDef::SimBuildingDef (+0x74) -> PeekPSimDefinition; 0x0082D5C0 reads WallMaterial (+0x88) -> PeekPMaterial
- 0x008018B0 is the CTCIdleScheduler brain-reset hook (manifest "LTextBinTree_RebuildWalkerArray" propagated)
- 0x0073A6E0 the creature mode manager's GetCurrentMode (manifest "CPixelShader::CountReferences" propagated); 0x005DC2E0 animation-table find
- CThing vtable slot 34 (0x004CD220 base) is a highlight-info filler `bool (SThingHighlightInfo*)` shared by CThingAICreature 0x00830CB0, CThingBuilding 0x0082D1D0, CThingPlayerCreature 0x006A9D10
- CThingInfo retail layout is {UniqueID 8, DefName CCharString, Player char} (PDB lists TypeName/SubTypeName/ScriptName/Player)
