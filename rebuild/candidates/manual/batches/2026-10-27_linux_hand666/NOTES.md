# Hand chunk 666 (CConversation .. CTCKraken): 10 of 10 byte-exact

## Proven
- 0x00E54D80 ??1CConversation@@UAE@XZ -- caller 0x00E54D60 is slot 0 (??_G) of CConversation's vtable 0x012E5FF0 (the vtable the landed copy ctor 0x00E54CA0 stores); Ego_r + FableWin ??1CConversation@@UAE@XZ; body frees Ego_r's four vector<CCharString> members in reverse at retail STLport offsets; landed 0x00F33B60 already binds this spelling.
- 0x00534EB0 ??$__find@PAV?$CCountedPointer@VCComponent@NUISystem@@@@V1@@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@0ABV1@ABUrandom_access_iterator_tag@0@@Z -- callers NUISystem::CComponent::RemoveChild 0x00533BC0 (landed, binds this spelling) / 0x00532C55 / 0x0053530A pass first/last of vector<CCountedPointer<CComponent> > + a reference to an element; 8-byte stride, Data-only compare = FableWin free ??$?8 operator==; STLport 4.6 _algobase.c text verbatim (MATCH).
- 0x006A6880 ?WorldUpdate@CThingPhysical@@UAE_NXZ -- CThingPhysical vtable slot 19 (rtti-vtables) + Ego_r.pdb; tail jmp to CThing::WorldUpdate 0x004C8050; list at +0x98 = Ego_r HealthOverTimeEffects with CHealthOverTimeHelper {FramesLeftForAddingHealth, HealthLeftToAdd}.
- 0x00B2B910 ??1CCompositeSurfaceCache@@UAE@XZ -- first store vtable 0x012A116C (RTTI .?AVCCompositeSurfaceCache@@), slot 0 ??_G 0x00B2EE00 calls it; Ego_r ??1CCompositeSurfaceCache@@UAE@XZ; members = Ego_r layout at STLport sizes.
- 0x009FE6C0 ??1CFrameData@CGraphicInfoFrame@@UAE@XZ -- first store vtable 0x0129C89C (RTTI .?AVCFrameData@CGraphicInfoFrame@@), sole caller is its slot-0 ??_G 0x00A010A0; Ego_r ??1CFrameData@CGraphicInfoFrame@@UAE@XZ; members = Ego_r layout (no STL).
- 0x009E3870 ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ -- Ego_r.pdb; body fills exactly the 6-byte Ego_r CPixelFormatInit from the Ego_r CD3DFormatInfo table entry of FormatIndex and returns it through the hidden pointer.
- 0x004615FB ?Transfer@CArmourDef@@UAEXAAVCPersistContext@@@Z -- CArmourDef vtable 0x01234AEC slot 18 + Ego_r/FableWin; members = Ego_r CArmourDef at retail offsets (same as the landed operator= 0x00461344).
- 0x004DF562 ?Transfer@CWillResponseDef@@UAEXAAVCPersistContext@@@Z -- CWillResponseDef vtable 0x0123E324 slot 18 + Ego_r/FableWin; the eight Ego_r bools +0x25..+0x2c.
- 0x0071C260 ?KillParticles@CTCHeroMorph@@QAEXXZ -- FableWin.pdb; FableWin donor order (SetTan, KillIdleParticles, KillParticles, HasAPermanentTattooCoveringArea, InitialiseCustomTattoos, OnInitialActivate = vtable slot 6, AddParticles) is the retail order; list +0x14 = Ego_r ParticleEmitters; tail target 0x0071C1F0 = KillIdleParticles (same loop over +0x18 IdleParticleEmitters; the TU reproduces it too).
- 0x007FC730 ?SetMode@CTCKraken@@AAEXW4EKrakenMode@1@@Z -- Ego_r.pdb; donor order CreateTentacles / InitTentaclesOnly / InitTentaclesAndHead / SetMode / StartLowAttackBeam / StartHighAttackBeam / EndAttackBeam / SetPhase / OnInitialActivate / FrameUpdate = retail 0x007FC210..0x007FC9D0 (OnInitialActivate / FrameUpdate are vtable 0x01241B94 slots 6 / 9); store at +0x64 = Ego_r Mode (+0x74 less the two 4-byte STLport lists); the beam starters store 4 / 5 and EndAttackBeam 3 into the same word.
- Callees (two sources each, see NAME_FIXES.tsv): CTCKraken::CreateTentacles / KillBeamEffect / KillTentacles / InitTentaclesOnly / InitTentaclesAndHead (private AAE), EndHeadEffects / RaiseSunkenTentacles (QAE); CThingPhysical::InternalModifyHealth (private AAE); CManagedTexture::Release (QAE_N) and ~CManagedTexture (UAE); CGraphicFrame::UnInitialise; CGraphicFrameData::Uninitialise; CBaseIntelligentPointer::GetPItem (returns CBaseObject*); CThing::Kill(bool); CThing::WorldUpdate; ~CResource; CPersistContext::Transfer<bool>/<long> (member template, landed spelling).

## Descriptive / unproven
- CPersistContext::TransferMap<V,K>(std::map<K,V>&) for 0x00461660 / 0x00461894 (called by 0x004615FB): the PDBs only hold the named TransferMap<V,K>(const char*, map&[, const V&]) forms; the one-argument binary-build spelling follows the landed one-argument Transfer<T>(T&) convention. Mangles ??$TransferMap@MW4EObjectAugmentationType@@@CPersistContext@@QAEXAAV?$map@...@std@@@Z. No prior binding for either address.
- The CD3DFormatInfo table 0x0129BA40 has no PDB data symbol; 0x009E3870 uses the bound spelling ?g_D3DFormatInfo@@3QBUCD3DFormatInfo@@B (one of eight competing bound spellings: g_FablePixelFormatInfoTable_0129BA40, g_FablePixelFormatTable, g_PixelFormatDescriptors, g_PixelFormatTable x2, g_pixelFormatInfo, g_texFormatTable). The owner should pick one canonical spelling; the struct name CD3DFormatInfo is the Ego_r type.
- Enumerator names of CTCKraken::EKrakenMode, CPixelFormatInit::Type and EPixelFormatType are not in any PDB export: the sources use the numeric values. Which of InitTentaclesOnly / InitTentaclesAndHead serves mode 1 vs 2 is not decidable from bytes (identical bodies, merged arm); the source pairs them in declaration order.
- TUHarness_DestroyCConversation in 0x00E54D80 is an emission helper, not a retail function (same convention as chunks 657-663).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00E54D80 CTCInventoryMap::ResetPrimitiveHandles -> CConversation::~CConversation (implicit).
- 0x00534EB0 std::__find_in_uint_array -> std::__find<CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent> > (the uint instantiation is 0x005348E0).
- 0x00B2B910 CFrameData::~CFrameData -> CCompositeSurfaceCache::~CCompositeSurfaceCache (the label was propagated from the byte-similar 0x009FE6C0).
- 0x009FE6C0 CFrameData::~CFrameData -> CGraphicInfoFrame::CFrameData::~CFrameData (nested class; label kept, spelling completed).
- 0x007FC6F0 / 0x007FC710 (catalog _global::Dtor_EndHeadEffectsBase_*) -> CTCKraken::InitTentaclesOnly / InitTentaclesAndHead (both RELOCATION_MATCH from the 0x007FC730 TU).
- 0x00A00B90 catalog leaf UnInitialise_00a00b90 -> CGraphicFrame::UnInitialise.
- 0x00A69AD0 catalog CViewport::ClearHandle -> CManagedTexture::Release; 0x00A69E20 catalog CEnginePrimitiveHandle::Clear -> CManagedTexture::~CManagedTexture (stores the CManagedTexture vtable 0x0129C880).
- 0x00461660 / 0x00461894 catalog CEngineLightingManager::UpdateShadowScene -> CPersistContext::TransferMap for CArmourDef's maps (descriptive one-argument form).
- 0x009FC550 catalog CMemoryBlock::~CMemoryBlock -> CResource::~CResource.

## Header debt
- engine/CPixelFormat.h, engine/CGraphicFrame.h: no methods; 0x009E3870 uses the layout-rename include (`#define CPixelFormat CPixelFormat_Layout`), the frame rows a local CGraphicFrame with the header's layout.
- engine/CThingPhysical.h is flattened (vptr as data): 0x006A6880 models CThing / CThingPhysical locally (HealthOverTimeEffects +0x98 = header offset).
- No engine headers for CArmourDef, CConversation, CTCKraken, CTCHeroMorph, CCompositeSurfaceCache, CGraphicInfoFrame::CFrameData, CManagedTexture, CManagedTextureOwner, CGraphicFrameData, CHealthOverTimeHelper: local models from the Ego_r layouts at STLport sizes.
- Every source repeats its STLport 4.6 subset (list / vector / map / __find); a shared rebuild/include/stlport46/ would remove it.
- The landed 0x009FE380 / 0x009FE440 / 0x00A00B90 / 0x009E3790 / 0x009E3970 rows use stand-in classes (CFrameMember, SubA/SubB, CPixelFormat_Methods, g_PixelFormatDescriptors): their symbols mangle wrong (NAME_FIXES for 0x009FE440 / 0x00A00B90; the CPixelFormat_Methods rows emit ?Initialise@CPixelFormat_Methods@@ / ?IsSigned@CPixelFormat_Methods@@ leaves).
