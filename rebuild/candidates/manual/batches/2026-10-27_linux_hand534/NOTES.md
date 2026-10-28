# Chunk 534 (wave 25, 12 re-queued rows of 527-920 bytes) -- notes

Result: 4 RELOCATION_MATCH (test PASS under g++ and VC7.1), 5 DIFFER parked with tests, 3 SKIP. No retail_asm rows.
vland dry run: WINS 4, no RELOC_CONFLICT / RELOC_DUPLICATE / RELOC_IDENTITY; 3 RELOC_NAME warnings (catalog labels,
all in NAME_FIXES.tsv) and 4 SHARED_HEADER_AVAILABLE warnings (header debt below).

| row | status | name |
|---|---|---|
| 0x00A3A410 | SKIP | CWADBank::AddFile (proven) |
| 0x00A3B9D0 | RELOCATION_MATCH | CVideoSys::Init |
| 0x00A3EF00 | SKIP | CSoundManagerLH constructor (descriptive) |
| 0x00A614B0 | DIFFER(634v634) | std::_Rb_tree<CWideString, pair<const CWideString, CChecksumCacheFile::CEntry>, ...>::insert_unique(iterator, const value_type&) |
| 0x00A68370 | RELOCATION_MATCH | CVolumeTexture::Initialise |
| 0x00A68690 | DIFFER(626v626) | CVolumeTexture::InitialiseSlicesFromTextureList |
| 0x00A868C0 | DIFFER(585v652) | CIPhysicsFacesSegmentBase::Calculate |
| 0x00AB1CC0 | DIFFER(612v612) | C3DAnimationBuilder::BuildBoneInfoTree |
| 0x00B61DD0 | DIFFER(521v532) | CEngineSkyRenderer::BuildCloudMesh |
| 0x00C400D0 | SKIP | over-spanned row (descriptive only) |
| 0x00DF9B80 | RELOCATION_MATCH | NScript::CQ_TraderConflictEvilScript::UpdateLiveEnemies |
| 0x00E12330 | RELOCATION_MATCH | NScript::CQ_WaspBossScript::WatchForCutscene |

## Names

### Proven (two sources each; exact PDB spelling confirmed with objdump -t)
- 0x00A3A410 `?AddFile@CWADBank@@QAEXABVCWideString@@@Z` -- Ego_r and FableWin both list AddFile in the CWADFile /
  CWADBankStateBlock run (retail TU 0x00A39B40-0x00A3A380: CWADFile methods, CWADBankStateBlock vtable 0x0129CFD0 slots,
  CCountedPointer<CFileInstaller::CFileInfo const> 0x00A3A380); one `const CWideString&` = `ret 4`; the body news an
  8-byte object with vtable 0x0129CFD0 (CWADBankStateBlock, RTTI) and drives CBankFile entry calls on `this`.
  The catalog's CreateWorldLevelWad is CWorldMap's two-argument function (`QAEXABVCWideString@@0@Z`, ret 8): wrong.
- 0x00A3B9D0 `?Init@CVideoSys@@QAEJABVCWideString@@PAVCTexture@@_N@Z` -- Ego_r + FableWin; manifest signature agrees.
- 0x00A614B0 `?insert_unique@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@...` (full
  leaf in differ/00a614b0.cpp) -- only caller is map<CWideString, CChecksumCacheFile::CEntry>::operator[] 0x00A617E0
  (landed) + STLport 4.6 _tree.c text (hint arms, `ret 0Ch`).
- 0x00A61360 insert_unique(const value_type&) and 0x00A61270 _M_insert -- called from the hinted/fallback arms with the
  STLport argument shapes; same TU.
- 0x00A60890 _Rb_tree_iterator<..., _Nonconst_traits<...>>::operator-- -- called for `--before`.
- 0x00A68370 `?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@JABVCPixelFormat@@KW4EVolumePool@@_N@Z` -- Ego_r + FableWin;
  donor order CalcByteLength, Uninitialise, ..., CreateTexture, operator=, InitialiseFromPreallocatedVolumeTexture,
  Initialise = 0xA67FF0, 0xA68060, ..., 0xA68250, 0xA682A0, 0xA682E0, 0xA68370.
- 0x00A68250 `?CreateTexture@CVolumeTexture@@IAE_NABVC3DExtentsI@@JKABVCPixelFormat@@W4EVolumePool@@_N@Z` -- same donor run;
  auto-inlined into 0x00A68370 from the same TU.
- 0x00A67FF0 `?CalcByteLength@CVolumeTexture@@IAEXXZ` (protected) -- Ego_r + FableWin; donor order.
- 0x00A68690 `?InitialiseSlicesFromTextureList@CVolumeTexture@@QAE_NPAVCTexture@@J@Z` -- Ego_r + FableWin; manifest agrees.
- 0x00A67EA0 `?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z` -- Ego_r; reloc binding already spells it.
- 0x00A67460 `?Uninitialise@CVolume@@QAEX_N@Z` -- Ego_r + FableWin CVolume set, one bool (`ret 4`).
- 0x009FA280 CTexture::Initialise, 0x009FA4E0 `?CopyFromTexture@CTexture@@QAEXABV1@K@Z`, 0x009FA140 Swizzle,
  0x009FA180 Unswizzle, 0x009FA450 Lock, 0x009F9DE0 Unlock, 0x009F9F70 Uninitialise -- Ego_r CTexture set + call shapes.
- 0x00A868C0 `?Calculate@CIPhysicsFacesSegmentBase@@KI_NABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@G@@JAAVCPlane@@AAV4@4@Z`
  -- Ego_r name set (protected static __fastcall, references) + landed Create 0x00A86CD0 binds this spelling.
- 0x00A86110 `?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z`
  -- Ego_r + hidden CPlane result in ecx at the call.
- 0x009B4A20 `?push_back@?$CArray@G@@QAEXABG@Z` -- Ego_r + FableWin; called for the out-of-line pushes in 0x00A868C0.
- 0x00AB1CC0 `?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABVC3DMesh2@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z`
  -- Ego_r + FableWin; sits after CBoneInfo::FindChildIndex 0x00AB15F0 and the CBoneInfo ctor 0x00AB1660 (donor order).
- 0x00AB15F0 `?FindChildIndex@CBoneInfo@C3DAnimationBuilder@@QBEJABVCCharString@@@Z` -- Ego_r + FableWin; donor order.
- 0x00AB3640 `?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z` -- passed to new CCPPointerInfo in
  0x00AB1CC0 + Ego_r CCountedPointer<T>::DeleteData pattern.
- 0x00AB2BD0 std::__destroy_aux<CCountedPointer<CBoneInfo>*> and 0x00AB3300 vector<...>::_M_insert_overflow -- STLport 4.6
  text + call shapes (fastcall `__destroy_aux` with __false_type ref; same-TU overflow body).
- 0x00B61DD0 `?BuildCloudMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z` -- Ego_r donor order BuildOuterSkyMesh,
  BuildBaseBandMesh, FrameStart, RenderStarField, BuildCloudMesh = retail 0xB61B30, 0xB61CD0, 0xB61DB0, 0xB61DC0, 0xB61DD0;
  landed BuildBuffers 0x00B620A0 calls it with (vertices, indices, 0x144, 0x49).
- 0x00B61B30 BuildOuterSkyMesh / 0x00B61CD0 BuildBaseBandMesh -- same run (`AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z`).
- 0x00DF9B80 `?UpdateLiveEnemies@CQ_TraderConflictEvilScript@NScript@@QAEXXZ` -- Ego_r donor order
  (CEntityScriptBinding<CQ_TraderConflictEvilScript>::CreateActiveEntityScript -> UpdateLiveEnemies) + FableWin name.
- 0x00E12330 `?WatchForCutscene@CQ_WaspBossScript@NScript@@QAEXXZ` -- landed CQ_WaspBossScript::Main 0x00E0EA40 spawns it
  under the thread name "WatchForCutscene" + Ego_r/FableWin name set.
- 0x00CB7940 `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ`, 0x008AC9D0 / 0x00CD3152 vector<CScriptThing>::erase (range /
  single), 0x00411570 `?Compare@?$CBasicString@D@@KIJPBD0@Z`, 0x00CD3D2E map<CCharString,
  CScriptGameResourceObjectScriptedThingBase>::operator[], 0x00CDBFB0 that map's dtor, 0x00CBFB7D RunCutsceneMacro,
  0x0099A380 / 0x0099A3B0 / 0x0099A430 CBaseObject protected ctor / operator= / dtor, 0x007E74D0
  ~CScriptGameResourceObjectScriptedThingBase, 0x004AA840 ~CScriptThing -- Ego_r (FableWin for 0x006E7AB0
  ~CCountedPointer<CScriptGameResourceObjectMovieBase>) + call shapes; exact symbols in NAME_FIXES.tsv.
- 0x0099AF00 `??BCWideString@@QBEPB_WXZ`, 0x0099AF10 `?GetAsCharArray@CWideString@@QBEPB_WXZ` -- Ego_r + FableWin; donor order
  operator 0x00B02200 right before GetAsCharArray 0x00B02220; needs /Zc:wchar_t.

### Probable
- 0x0099FDB0 GFGetTopBit (FableWin __fastcall; the inline top-bit loops in 0x00A68370 match its body, not re-proven by a
  caller here).
- 0x00AB34C0 C3DAnimationBuilder::CBoneInfo::~CBoneInfo (size and members match the 0x24-byte CBoneInfo; catalog says a
  _Dest_val instance).
- 0x00AB1660 C3DAnimationBuilder::CBoneInfo::CBoneInfo (Ego_r donor order; catalog CParticleMorphs_InitializeEntry).

### Descriptive (no PDB name exists or none provable)
- 0x00A3EF00 CSoundManagerLH constructor: stores vtable 0x0129D2C4 (RTTI .?AVCSoundManagerLH@@, base CASoundManager),
  calls the base ctor 0x00991910 with two arguments, then builds the member containers. Neither PDB has any
  CSoundManagerLH or CASoundManager name, so its signature and every member are unproven.
- 0x00C400D0: 64-way switch on (n-1) that picks table 0x0143BC1C or 0x0143BC20 and returns a 0x34-byte entry via
  0x00C3FF90. The row over-spans; see the label corrections.
- Globals: 0x0143782C (`g_CUnprovenCosThreshold_0143782c`, cos(0.15*pi)*13000, the cloud u/v extent; existing binding kept,
  name misleading), 0x0139A710 `g_CloudRingAngleStep_0139a710` (-0.35*pi/8), 0x0139A70C `g_CloudSegmentStep_0139a70c`
  (1/36), 0x0129BA3C `g_epsilon_0129ba3c` (1e-4f, GOTCHAS spelling; ~25 bound spellings across landed files),
  0x013D2825 / 0x013D2826 (CVideoSys::Init's disabled / cleared bss bools), 0x013D28E0 volume-texture byte counter,
  0x013CAA90 global allocator, 0x0129BA40 g_D3DFormatInfo.

## Label corrections (catalog)
- 0x00A3A410 CreateWorldLevelWad -> CWADBank::AddFile.
- 0x00A3EF00 CMainGameComponent_Initialize -> CSoundManagerLH constructor (descriptive).
- 0x00A614B0 BinaryTree_Insert_RBTree -> _Rb_tree::insert_unique(iterator, const value_type&); 0x00A61360
  LinkedList_FindInsertionPoint -> insert_unique(const value_type&); 0x00A61270 ListNode_Insert -> _M_insert;
  0x00A60890 TreeNode_Successor_RBTree -> _Rb_tree_iterator::operator--.
- 0x00A868C0 module `_global` -> CIPhysicsFacesSegmentBase (the landed Branch ctor 0x00A86B30 binds an invented
  CPhysicsFacesSegmentBranch::Calculate with pointer arguments); 0x00A86110 -> CIPhysicsFacesSegmentBase.
- 0x009B4A20 Vector_Append_uint16 -> CArray<unsigned short>::push_back.
- 0x00AB1CC0 BuildEntityScriptBindings_FromVertexIndices -> C3DAnimationBuilder::BuildBoneInfoTree.
- 0x00AB3640 CTCMapwho::OnDie -> CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData.
- 0x00AB3300 Vector_ReallocateAndInsert -> vector<CCountedPointer<CBoneInfo>>::_M_insert_overflow.
- 0x00AB15F0 module CBoneInfo -> C3DAnimationBuilder::CBoneInfo (nested).
- 0x00B61DD0 BuildBuffers_BuildGlowParticles -> CEngineSkyRenderer::BuildCloudMesh.
- 0x00C400D0 Animation_ComputeFrameOffset: the body ends at `ret` 0x00C4017F, followed by its jump table (0x00C40180)
  and index bytes (0x00C40190). The row (527 bytes) then runs past int3 padding into further functions from
  0x00C401E0, so the catalog must re-split it.
- 0x00DF9B80 CScriptBase_FilterCombatantsByType -> CQ_TraderConflictEvilScript::UpdateLiveEnemies.
- 0x00E12330 CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene -> CQ_WaspBossScript::WatchForCutscene.
- 0x00A67EA0 CPlayer::UninitModeQuestCompletionUI -> CVolumeTexture::GetVolumeLevel.
- 0x009FA180 / 0x009FA140 CPaintMapDialog::InitNumberBox (both) -> CTexture::Unswizzle / CTexture::Swizzle (bindings
  already right).
- 0x00CD3152 std::vector::pop_back -> vector<CScriptThing>::erase(iterator).
- 0x0099A3B0 CFourierAnalysis, 0x0099A430 ~CPhysicsMeshInfo, 0x0099A380 CBaseIntelligentPointer -> CBaseObject
  operator= / dtor / ctor.
- 0x007E74D0 ~CScriptGameResourceObjectMovieBase -> ~CScriptGameResourceObjectScriptedThingBase; 0x004AA840
  ~C3DClothPrimitive -> ~CScriptThing; 0x006E7AB0 _Dest_val<...> / CCountedPointer_Release_006e7ab0 ->
  ~CCountedPointer<CScriptGameResourceObjectMovieBase>; 0x00CDBFB0 StdMap_Destroy_API -> map dtor.
- 0x00E0F1D0 is landed / catalogued as CV_ChapelOfEvilScript::EndMission, but the reloc binding and the WaspBoss Main
  spawn name say CQ_WaspBossScript::WatchForTermination (review at landing).

## Header debt
- engine/CGameScriptInterface.h, CBaseObject.h and CPixelFormat.h exist (the verifier warns SHARED_HEADER_AVAILABLE), but
  they are layout-only: no virtuals and no methods. 0x00DF9B80 and 0x00E12330 call interface virtual slots (70, 84, 231;
  7, 8, 70, 72, 370, 379, 416, 475, 512). 0x00E12330 also needs CBaseObject's protected ctor, operator= and virtual dtor,
  and 0x00A68370 / 0x00A68690 need CPixelFormat methods. All of these keep local models. A header generator that emits
  the vtable slots and methods would let them include the shared headers.
- No headers for CVolumeTexture, CVolume, CTexture lock types, C3DMesh2 (local model: BoneCount 0x98, Bones 0x9C,
  BoneNameIndices 0xA4, BoneNameData 0xA8), C3DAnimationBuilder::CBoneInfo (0x24), CIPhysicsFacesSegmentBase, CPlane
  (needs a user ctor, non-POD: hidden result in ecx), CTVertexColTex1 (24 bytes).
- engine/CScriptThing.h has no virtuals; the scripts need slots 1 GetName, 2 GetDefName and protected virtual IsEqualTo at
  slot 78.
- Retail CScriptBase is 0x40 bytes (Ego_r layout - 0x14); the script models put the interface at +0x40.
