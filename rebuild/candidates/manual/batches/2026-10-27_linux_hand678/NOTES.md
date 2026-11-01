# Hand chunk 678 (NScript::CQ_SecretPassageScript .. libvorbis res1_forward): 9 of 10 byte-exact

All nine matched rows are RELOCATION_MATCH (graded with gall.sh). Their tests print `<addr>_TEST PASS` under g++ (tall.sh)
and under VC7.1 in Wine (tall.py). One row is parked.

## What matched (9)
- **NScript::CQ_SecretPassageScript::~CQ_SecretPassageScript 0x00DF10D0**: the compiler-generated dtor of a quest
  script. Shape: a local `CLevelScriptBase<CQ_SunnyvaleMasterData>` with no dtor declaration; the PDB ctor is defined
  with an empty body so the TU emits ??_G and ??1. The STLport `_Rb_tree::clear()` is inlined with an out-of-line private
  `_M_erase` (the TravellingHeroes tree text). Members follow the Ego_r layout with the script base 0x14 below
  (size 0x12C).
- **NScript::CV_BookCollectingScript::~CV_BookCollectingScript 0x00E57AA0**: same recipe. The vectors are STLport
  `_Vector_base` with `__destroy_aux` and type traits; `vector<bool>` is 8 bytes smaller than in the PDB; a local
  `CConversation` model has a virtual dtor and is 92 bytes.
- **CDisplayManager::~CDisplayManager 0x009C01C0**: the user dtor stores the vtable. Shape points:
  - boost 1.30 `scoped_ptr::reset` is `this_type(p).swap(*this)`, which clears the pointer before the delete;
  - CSurface has an inline virtual dtor and CTexture a plain one, both calling Uninitialise;
  - STLport `_List_base` with `clear()` defined out of class (as in `_list.c`), which keeps ~_List_base out of line;
  - CBusyResourceManager and CVsyncCallbackTimer dtors are out of line.
- **CTCPhysicsRigidBody::Move 0x0075AEC0** (vtable slot 87). Shape points:
  - a user `CMatrix3x4() {}` and a user C3DVector copy ctor give the non-POD hidden result in ecx and the
    `fld X, fld Y, mov Z` copies;
  - `TransformVector` works in place on a named copy of CMPosition, reading x and y first.
- **C3DAnimFileLoader::~C3DAnimFileLoader 0x00A996D0**: an empty user dtor. Nine engine/CCountedPointer.h members and a
  `vector<CChunkedFileChunk*>`; the base CChunkedFileLoader dtor 0x00AA7540 is out of line.
- **CBankFile::~CBankFile 0x009CE180**: `~CBankFile() { Close(); }` with a same-TU `Close() { CloseEntries(); }`. The
  checksum cache is `unsigned long Body[6]`, so it is dword-aligned at +0xF8.
- **libvorbis res1_forward 0x00CA1060** (`@res1_forward@28`, vendor 1.0.1 res0.c, verbatim). `_01forward` is a same-TU
  `static __declspec(noinline) __stdcall` inside `extern "C"`, so VC picks retail's private convention. The externs are
  `__fastcall`. The row matches with and without /Gr.
- **CTCDExperienceOrb::OnCollected 0x0077A180**. Shape points:
  - a same-TU `CDefClassBase::GetSubDef<T>` template body keeps the sound def in esi across the calls (the release
    becomes `dec [esi+4]; jne`) and lets the interface key share the dead collector argument slot;
  - lookups go through `const CInterfaceMap* map = &Components` (the const LowerBound 0x004365B0 out of line, the
    non-const one inline);
  - the pickup loop re-reads `GetDef().PickupParticleEffectsPerExperiencePoints.begin()/end()`;
  - "PICKUP_SOUND" converts implicitly through `CCharString(const char*, long = -1)`.
- **CTCContainerRewardHero::RewardHeroWithItems 0x0074EE90**. Shape points:
  - the retail TU compiled the callees ahead of this body, so CVectorMap::LowerBound (the landed 0x0040F020 text) and
    a GetRewardItems stand-in are same-TU non-inline bodies under `auto_inline(off)`. The stand-in holds only the real
    body's first two steps: `gold = 0;` and `items.erase(begin, end)`, which is 0x0074FB20;
  - those bodies give the ebx zero web, the spilled receiver and retail's frame slots;
  - writing `gold <= 0 && items.empty()` first, then the gold arm, then the items arm gives retail's jump-threaded
    block order;
  - the stats lookup is the never-written-local form;
  - the font and the text bank come through the Ego_r inline accessors
    `CMainGameComponent::PeekCutSceneMainFont()` and `GetTextBank()`.

## Parked for Fable 5.1 (1)
- 0x009D06F0 CBankFile::OpenReadOnly DIFFER(385v385): retail puts the FindContainedBank-failed exit (`lea ecx,[esp+0xc] ; call ~CCountedPointer<CRetailBank> ; return false`) inline after `test al,al ; jne`, and the version check `jne`s back to it; ours merges the two exits at the version check's position (8 lines, block order only) | tried: two early returns, merged `||`, nested ifs, `&&` success block, named bool, if/else-if chain, bool result local, operand swap, version local, goto to one label (before and after the success block), /Ob1 /Os /EHsc /Oy- (16 probes, probe/009d06f0.log) | next: same-TU noinline bodies for CBankFileManager::FindContainedBank and ~CCountedPointer<CRetailBank> (branch-weight and clobber facts), or build it in one TU with the neighbouring CBankFile rows.

## Skipped
None.

## Proven
0x00DF10D0 ??1CQ_SecretPassageScript@NScript@@UAE@XZ -- ??_G 0x00DF10B0 is slot 0 of vtable 0x012DDC30, which ctor 0x00DEC7F0 stores; Alloc 0x00DF1080 (push 12Ch) calls that ctor and is registered at 0x00CD74E8 with "Q_SecretPassage"/"S_QSP"; Ego_r ??1 + layout
0x00E57AA0 ??1CV_BookCollectingScript@NScript@@UAE@XZ -- ??_G 0x00E57A80 is slot 0 of vtable 0x012E5EFC, which ctor 0x00E54310 stores; Alloc 0x00E57A50 (push 0B8h) is registered at 0x00CD849C with "V_BookCollecting"/"S_VBC"; Ego_r layout
0x009C01C0 ??1CDisplayManager@@UAE@XZ -- stores vtable 0x0129B3F8 (RTTI .?AVCDisplayManager@@, slot 0 = its caller ??_G 0x009C30B0); Ego_r member offsets
0x0075AEC0 ?Move@CTCPhysicsRigidBody@@UAEXXZ -- slot 87 of CTCPhysicsRigidBody vtable 0x0126996C, the same slot as CTCPhysicsBase::Move 0x006B1360; Ego_r name set
0x00A996D0 ??1C3DAnimFileLoader@@UAE@XZ -- stores vtable 0x0129E060 (RTTI, ??_G 0x00A9A7A0); second caller is C3DAnimFileXLoader's ??_G 0x00AA4A20; Ego_r
0x009CE180 ??1CBankFile@@UAE@XZ -- stores vtable 0x0129B54C (RTTI CBankFile, ??_G 0x009D47A0); derived bank dtors chain to it; Ego_r layout
0x009D06F0 ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z -- slot 1 of CBankFile vtable 0x0129B54C; Ego_r name set (catalog label right)
0x00CA1060 @res1_forward@28 -- libvorbis 1.0.1 res0.c res1_forward: slot 6 (forward) of residue1_exportbundle 0x013ABDE4; passes _encodepart 0x00CA0620 to _01forward; the verbatim vendor body byte-matches
0x0077A180 ?OnCollected@CTCDExperienceOrb@@QAEXAAVCThing@@@Z -- Ego_r name set; callers 0x006AD6F0 / 0x007C1991 pass the collecting thing; Ego_r CTCDExperienceOrb / CExperienceDef layouts
0x0074EE90 ?RewardHeroWithItems@CTCContainerRewardHero@@QAE_NXZ -- Ego_r + FableWin name sets; with 0x0074EC77 it is one of the two callers of GetRewardItems 0x0074E860, a CTCContainerRewardHero member; the string TXT_SIGN_NOTHING and CChestDef::DisplayMessageOnEmpty fit
0x0074E860 ?GetRewardItems@CTCContainerRewardHero@@IAEXAAV?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@AAJ@Z -- Ego_r/FableWin signature; ret 8 with gold zeroed first (`mov [ebx],0`); push_back 0x0074FE50 callers (landed note)
0x00487DC0 ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ -- `add ecx,2Ch ; jmp GetPItem` = Ego_r CPlayer::ControlledCreature +0x2C; FableWin name (chunk 652)
0x004E34B2 ??1?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ -- destroys the GetRewardItems vector at both exits of 0x0074EE90; label file chunk 246
0x00A358F0 ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z -- Ego_r + FableWin, manifest label, hidden-result __fastcall shape in 0x0075AEC0
0x00A61B10 ??1CBusyResourceManager@@QAE@XZ -- CDisplayManager::BusyResourceManager (+0x2B0) member dtor in 0x009C01C0; body clears the two CPendingQuery lists
0x009C2220 ??1?$_List_base@PAVCDeviceResetCallback@@V?$allocator@PAVCDeviceResetCallback@@@std@@@std@@QAE@XZ -- DeviceResetCallbacks list (+0x264) dtor in 0x009C01C0; STLport _List_base body
0x00AA7540 ??1CChunkedFileLoader@@UAE@XZ -- base dtor at the end of 0x00A996D0 and of ~C3DAnimFileXLoader
0x00CBD510 ??1CScriptBase@@UAE@XZ -- base dtor ending both quest-script dtors
(Further callee names with evidence are in NAME_FIXES.tsv.)

## Label corrections
- 0x00DF10D0: "CParticleEmitter::~CParticleEmitter" is NScript::CQ_SecretPassageScript::~CQ_SecretPassageScript.
- 0x00E57AA0: "CParticleEmitter::~CParticleEmitter_MeshFree" is NScript::CV_BookCollectingScript::~CV_BookCollectingScript.
- 0x009C01C0: "CInputManagerDX::~CInputManagerDX" is CDisplayManager::~CDisplayManager.
- 0x009CE180: "NHeroInformationScreens::CBase::~CBase" is CBankFile::~CBankFile.
- 0x00CA1060: "Environment_AccumulateVertexData" is libvorbis res1_forward.
- Callee labels: see NAME_FIXES.tsv. Highlights:
  - 0x00487DC0 GetHeroCreature is GetPControlledCreature;
  - 0x004E34B2 CActiveFile::OnReadFinished is ~vector<pair<long,CCharString> >;
  - 0x0065FBA4 CVectorMap_LowerBound_Faction is CSoundMap::GetPSoundPair;
  - 0x0066B280 CAnimateThing_PlayRandomSound is CTCSoundPlayer::PlayAttachedSample;
  - 0x00686D60 GetGameSubsystem is CTCBase::PeekComponent;
  - 0x00CA0BA0 Environment_GetThemeIndex is libvorbis _01forward.

## Descriptive names (not proven)
- g_BankFileManager 0x013CA79C: the spelling is not established (g_bankFileManager also seen).
- g_pPlayerGui 0x013B8790: majority spelling (g_pGameUI_013b8790 also seen).
- g_pMainGameComponent 0x013B86A0: GOTCHAS wave 25 spelling; no PDB global.
- g_framesPerSecond 0x01375550 (long): majority spelling.
- 0x009CD850 CBankFile::CloseEntries and 0x009A7CA0 CBankFileManager::GetDevBankFilePath are evidence-best (see NAME_FIXES).

## Header debt
These rows use local models next to existing engine headers, because the headers are flattened Ego_r byte arrays with no
methods or virtual layout:
- CLevelScriptBase (both quest scripts);
- CSurface / CTexture (0x009C01C0);
- CBankFile (0x009CE180 and the parked 0x009D06F0; engine/CBankFile.h is a flat `__vftable` header);
- CTCPhysicsRigidBody (0x0075AEC0; vtable-shaped CTCPhysicsBase);
- CTCBase / CPlayer / CPlayerManager / CPlayerGui / CSubtitleRenderer / CTCHeroStats (0x0074EE90);
- CThing / CVectorMap (0x0077A180, 0x0074EE90).
vland also flags SHARED_HEADER_AVAILABLE (as warnings) for the local CDefinitionManager (0x0077A180, 0x0074EE90),
CTCHeroExperience (0x0077A180), CPixelFormat (0x009C01C0) and CTCPhysicsBase (0x0075AEC0).

rebuild/include/fable_string.h lacks the PDB default `long length = -1` on CCharString(const char*, long), so the two rows
that convert a literal implicitly (0x0077A180, 0x0074EE90) declare a local CCharString. CDefClassBase, CSubDefClassBase,
CDefPointer and CCountedPointer come from the engine headers.
