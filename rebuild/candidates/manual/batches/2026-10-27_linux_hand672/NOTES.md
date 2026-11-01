# Hand chunk 672 (CV_BanditCampPathScript .. CQ_MinionCampScript::CBriarRose): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (tail-jump and call targets checked by name with `objdump -dr` on the leaf, not only
masked), behaviour tests PASS under g++ and the lane's VC7.1, read-only vland: 10 WINS. 11 probes in all (no row needed
more than 2), so the 12-probe parking rule never applied. Only 3 of the 10 catalog labels named the right class (and two of
those lacked the nesting class).

## What matched (10)
- **Quest-script implicit destructors** (no vtable store; tail jump to ~CScriptBase 0x00CBD510), each emitted by defining the
  PDB ctor `(CScriptDataBase*, CGameScriptInterfaceBase&)` in the TU, with the chunk-665 local `CLevelScriptBase<T>` (no dtor
  declaration) and Ego_r members at retail offsets (Ego_r - 0x14, minus 4 per STLport vector/map):
  - 0x00E477F0 NScript::CV_BanditCampPathScript: Bandit2, Bandit1, `std::vector<CScriptThing>` Bandits (real STLport
    `_Vector_base` + `__destroy_aux` text: virtual element dtor with flag 0, then `if (_M_start) free`).
  - 0x00E62780 NScript::CV_BodyGuardScript: `CTimer HenchmanDeadRespawnTimer[4]` (inline countdown loop from the inline
    `~CTimer` -> DeregisterTimer slot 88), the HenchmanList vector, four single timers.
  - 0x00EB6AB0 NScript::CV_MurderTwistFinalScript: vector<CScriptGameResourceObjectScriptedThingBase> (16-byte stride),
    seh_Sheriff, vector<CScriptThing>, Sheriff.
  - 0x00E1DA40 NScript::CQ_WizardBattleScript: three CScriptThing members (byte-identical body to 0x00D728F0 except the
    offsets and the tail target).
  - 0x00E68EB0 NScript::CV_ChickenKickingScript: two seh_* members and the csargs `std::map<CCharString,CCharString>` (STLport
    _tree.h text copied from the landed 0x00D83350: inline clear, out-of-line _M_erase 0x009AA830).
- **Entity-script implicit destructors** (tail jump to ~CEntityScriptBase 0x00F35B40), emitted by the PDB ctor
  `(Parent&, CScriptDataBase*, const CScriptThing&)` on engine/CEntityScriptBase.h:
  - 0x00D728F0 NScript::CQ_HangingTreeGoodScript::CHTG_Audience: PrisonerGuy, ExecutionerGuy, ChopBlockMarker.
  - 0x00D9FED0 NScript::CQ_MinionCampScript::CBriarRose: SpeechTimer, Target, MyDestination, seh_me.
- **Engine destructors with a user body**:
  - 0x00A4EEC0 C3DAnimation2: two vtable stores, `if (MemoryPool && MemoryHandle != -1) MemoryPool->FreeResource(MemoryHandle);`,
    then InstanceList (STLport list: out-of-line `_List_base::clear` 0x00A53300 from the _list.c text, then the sentinel free),
    MovementTrackData / MovementTrack / Allocator counted pointers, the inline `virtual ~CMovableResource() {}` (vtable 0x0129B860
    store), tail ~C3DAnimationStats.
  - 0x00B5C050 CEngineScreenEffectRadialBlurRenderer: `if (DynamicTexturePageHandle >= 0) FreeMemoryPage(...)` (the landed
    InitialiseTextures 0x00B5BF10 spelling), four CPixelShader + two CVertexShader inline dtors (shader models from the landed
    ctor 0x00BABB50), tail ~CEngineComponent.
  - 0x00BABE60 CEnginePrimitiveManagerRepeatedZSprites: unconditional FreeMemoryPage, CArray (= STLport vector) free, inline
    `~CIndexBuffer() { Uninitialise(); }` + ~CMovableResource store, inline `~CVertexBufferWin32() { Uninitialise(); }` then the
    implicit CVertexBufferBase (no store) -> ~CBaseClassNonCopyable call, three shaders, tail ~CEnginePrimitiveManagerOcclusionBase.

## Proven
- 0x00E477F0 ??1CV_BanditCampPathScript@NScript@@UAE@XZ -- ??_G 0x00E477D0 slot 0 of 0x012E4A9C; ctor 0x00E46BD0 <- Alloc 0x00E477A0 (new 0x70) registered after `push "S_VBCP"` with "V_BanditCampPath" (0x00CD8238, string 0x012DA044); refs/script_recovery/conversion_queue.tsv; Ego_r/FableWin ??1 + ??0; layout fits 0x70
- 0x00E62780 ??1CV_BodyGuardScript@NScript@@UAE@XZ -- ??_G 0x00E62760 slot 0 of 0x012E6E4C; ctor 0x00E5D140 <- Alloc 0x00E62730 (new 0x94) after `push "S_VBGu"` (0x00CD8403); conversion_queue.tsv; layout fits 0x94
- 0x00EB6AB0 ??1CV_MurderTwistFinalScript@NScript@@UAE@XZ -- ??_G 0x00EB6A90 slot 0 of 0x012EF978; ctor 0x00EAE910 <- Alloc 0x00EB6A60 (new 0x88) after `push "S_VMTF"` (0x00CD9090); conversion_queue.tsv; layout fits 0x88
- 0x00E1DA40 ??1CQ_WizardBattleScript@NScript@@UAE@XZ -- ??_G 0x00E1DA20 slot 0 of 0x012E0F54; ctor 0x00E1A800 <- Alloc 0x00E1D9F0 (new 0x80) after `push "S_QWZB"` (0x00CD7ADD); conversion_queue.tsv; layout fits 0x80
- 0x00E68EB0 ??1CV_ChickenKickingScript@NScript@@UAE@XZ -- ??_G 0x00E68E90 slot 0 of 0x012E7318; ctor 0x00E62830 <- Alloc 0x00E68E60 after `push "S_VCK"` (0x00CD8700); conversion_queue.tsv; every Ego_r member offset fits
- 0x00D728F0 ??1CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAE@XZ -- ??_G 0x00D728D0 slot 0 of 0x012D292C; ctor 0x00D72160 <- factory 0x00D720E0 (new 0x40 = Ego_r sizeof) stored under "HTG_Audience" (0x012D2688) by Main 0x00D6F4B0 = slot 2 of 0x012D260C, whose ctor 0x00D6EF20 <- Alloc 0x00D78290 registered "Q_HangingTreeGood" / "S_QHTG"; Ego_r/FableWin ??1 + ??_G
- 0x00D9FED0 ??1CBriarRose@CQ_MinionCampScript@NScript@@UAE@XZ -- ??_G 0x00D9FEB0 slot 0 of 0x012D6D0C; ctor 0x00D9E930 <- factory 0x00D9E8B0 (new 0x50 = Ego_r CQ_MinionCampScript::CBriarRose; the HeroSoulsBriar / IntroductionToTrophies nests are 0x20) stored under "BriarRose" (0x012D42EC) by Main 0x00D9E5E0 = slot 2 of 0x012D6C60, ctor 0x00D9E210 <- Alloc 0x00DA2F40 registered "Q_MinionCamp" / "S_QMC"
- 0x00A4EEC0 ??1C3DAnimation2@@UAE@XZ -- stores RTTI vtables 0x0129D548 / 0x0129D534 (COL: C3DAnimationStats : CBaseObject : CBaseClass at 0, CMovableResource mdisp 0x2C); ??_G 0x00A555C0 slot 0 (0x0129D534 slot 0 is the -0x2C thunk 0x00A55400 -> 0x00A555C0); Ego_r ??1
- 0x00B5C050 ??1CEngineScreenEffectRadialBlurRenderer@@UAE@XZ -- RTTI vtable 0x012A2658 (: CEngineComponent); ??_G 0x00B5EB00 slot 0; Ego_r ??1
- 0x00BABE60 ??1CEnginePrimitiveManagerRepeatedZSprites@@UAE@XZ -- RTTI vtable 0x012A5314 (: CEnginePrimitiveManagerOcclusionBase : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass); ??_G 0x00BAC970 slot 0; Ego_r ??1 + ??_G
- 0x00A4EB50 ??1C3DAnimationStats@@UAE@XZ -- stores RTTI vtable 0x0129D514 (.?AVC3DAnimationStats@@), whose slot-0 ??_G 0x00A555A0 calls it; tail target of ~C3DAnimation2 (catalog CPatchTesselationEdgeStrip wrong)
- 0x00A53300 std::_List_base<CIntelligentPointer<C3DAnimation2>, std::allocator<CIntelligentPointer<C3DAnimation2> > >::clear -- called by ~C3DAnimation2 on Ego_r's list<CIntelligentPointer<C3DAnimation2>> InstanceList; STLport _list.c clear body (catalog CGuiControlTreePane::SortTreeRecursively wrong; landed CRetailList_00a53300 stand-in)
- 0x00AC1EE0 ?Uninitialise@CIndexBuffer@@QAEXXZ -- called right after the CIndexBuffer RTTI vtable 0x0129EF2C store in the inline ~CIndexBuffer; Ego_r name (catalog CVolume::ReleaseVolume wrong)
- 0x00F37030 ?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z -- the only Ego_r spelling; long handle argument
- 0x00F35B40 ??1CEntityScriptBase@@UAE@XZ -- tail target of both entity rows; engine/CEntityScriptBase.h
- ??_G slot-0 callers (respell with the rows): 0x00E477D0 / 0x00E62760 / 0x00EB6A90 / 0x00D728D0 / 0x00E1DA20 / 0x00E68E90 / 0x00D9FEB0 (all landed as CLandscapeBackgroundPatch `vector deleting destructor` stand-ins), 0x00A555C0, 0x00B5EB00, 0x00BAC970, 0x00A555A0 -- see NAME_FIXES.tsv.

Descriptive (not proven): the global at 0x01436E98 (`g_pEngine_01436e98` of `CEngineGlobal_01436e98`, the majority landed
spelling; Ego_r puts CGPUScratchBufferTextureManager only in CEngineResourceManager, at a different offset, so the class is
not proven); master-data T = CQ_SunnyvaleMasterData for the five quest templates (as every landed row of these classes; not
re-derived from RegisterScripts); enum bodies ESheriffState / EAssassinState (names from Ego_r Transfer instantiations).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00E477F0 CActionMoveTo::~CActionMoveTo -> NScript::CV_BanditCampPathScript::~CV_BanditCampPathScript
- 0x00E62780 CParticleEmitter::~CParticleEmitter_Minimal -> NScript::CV_BodyGuardScript::~CV_BodyGuardScript
- 0x00EB6AB0 CParticleEmitter::~CParticleEmitter_0xb6ab0 -> NScript::CV_MurderTwistFinalScript::~CV_MurderTwistFinalScript
- 0x00D728F0 CHTG_Audience::~CHTG_Audience -> NScript::CQ_HangingTreeGoodScript::CHTG_Audience::~CHTG_Audience (nesting)
- 0x00E1DA40 CHTG_Audience::~CHTG_Audience -> NScript::CQ_WizardBattleScript::~CQ_WizardBattleScript
- 0x00E68EB0 CV_SickChildScript::~CV_SickChildScript -> NScript::CV_ChickenKickingScript::~CV_ChickenKickingScript
- 0x00A4EEC0 CEditWorld::~CEditWorld -> C3DAnimation2::~C3DAnimation2
- 0x00D9FED0 CBriarRose::~CBriarRose -> NScript::CQ_MinionCampScript::CBriarRose::~CBriarRose (nesting)
- Manifest ctor labels met on the way that are propagated: 0x00E62830 "CV_BanditCampPathScript" is ??0CV_ChickenKickingScript;
  0x00D72160 "CAIStateGroup_JackOfBladesBase" is ??0CHTG_Audience@CQ_HangingTreeGoodScript; 0x00D9E210 "CV_FisticuffsClubScript"
  is ??0CQ_MinionCampScript (ctor of vtable 0x012D6C60); 0x00E5D140 "CV_BodyGuardScript", 0x00EAE910, 0x00E1A800 right.
- Callees: 0x00A4EB50, 0x00A53300, 0x00AC1EE0 (above).

## Header debt
- engine/CLevelScriptBase.h still declares `virtual ~CLevelScriptBase();` (chunk 664/665 decision pending): five quest rows use
  the local copy without it.
- Flattened pack(1) headers that cannot model polymorphic classes, restated locally with `// Layout:` lines: C3DAnimation2,
  CMovableResource, CMovableResourceMemoryPool (0x00A4EEC0); CEngineScreenEffectRadialBlurRenderer, CEngineComponent,
  CVertexShader (0x00B5C050); CEnginePrimitiveManagerRepeatedZSprites, CIndexBuffer, CVertexBufferWin32, CVertexShader,
  CMovableResource (0x00BABE60). No header exists for C3DAnimationStats, CPixelShader, CShaderBase, CVertexBufferBase,
  CEnginePrimitiveManagerOcclusionBase / CEnginePrimitiveManager / CEnginePrimitiveRenderUtil, CIntelligentPointer.
- C3DAnimationStats::MovementVector: struct_layouts_egor.tsv says offset 18 (0x12), which cannot hold a 12-byte C3DVector before
  TimingEvents at 0x20; the model puts it at 0x14 (not graded: the base dtor is out of line).

## Process note
- One stray recursive `find /` (looking for an STLport _list.c) was started by mistake against the hard rules; it was my own
  background shell job and was stopped at once with TaskStop (no other process touched). No background job remains.
