# Hand chunk 665 (CCombatManager .. NScript quest scripts): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (q.py and the read-only vland run: 10 wins, behaviour PASS under g++ and VC7.1). Every
catalog label was a propagated guess; all ten identities below are proven (none descriptive). Only two globals in
0x009A1090 are descriptive.

## What matched (10)
- **zlib 1.2.1** -- 0x00C051C0 `_tr_init` (trees.c, C under zlib's names, `extern "C" __cdecl`): the landed init_block row
  (0x00C03EE0) already carried the trees.c TU text with `_tr_init` as its same-TU caller; reused as is. Version from the
  retail strings " deflate 1.2.1 Copyright 1995-2003 Jean-loup Gailly " / " inflate 1.2.1 ... Mark Adler " and the shared
  deflate_state layout of the landed neighbours. tr_static_init() is empty in 1.2.1 (no GEN_TREES_H) and folds away.
- **NScript quest scripts** (implicit dtors, tail jump to ~CScriptBase 0x00CBD510) -- 0x00D2F640 ~CQ_EndGameFocalSitesScript,
  0x00D27290 ~CQ_DragonBossFightScript, 0x00D83350 ~CQ_HeroSoulsBriarScript, 0x00DD1F40 ~CQ_OrchardFarmRaidScript. Shape:
  Ego_r members at retail offsets (0x14 below Ego_r), CTimer dtor inline (engine/CTimer.h, DeregisterTimer slot 88), real
  STLport map/vector text, member arrays as inline countdown loops (no /EHsc). The one lever: CLevelScriptBase<T> must have
  an IMPLICIT dtor -- the header's out-of-line declaration calls a nonexistent ??1CLevelScriptBase, and an inline user
  `~CLevelScriptBase() {}` adds a vtable store (probe 1 of 0x00D2F640, 114v108).
- **CTC components** -- 0x007AFF70 ~CTCBoss (implicit: no vtable store) and 0x007C5660 ~CTCGhostSword (user dtor, empty body:
  first store is its own vtable). engine/CDefPointer.h + the landed CIntelligentPointer / CTCBase models; matched first try.
- **CStreamingFontBank** -- 0x009A1090 ~CStreamingFontBank: user dtor + inline user ~CFontBank (vtable 0x0129AB38 store,
  Name dtor, jmp ~CBaseClassNonCopyable). The glyph-cache sweep is one entry pointer advanced inside two counted loops
  (256 x 8); `cache[i][j]` indexing is folded into one linear loop with fontIndex hoisted (probe 1).
- **CCombatManager** -- 0x006ED200 Update: STLport map iteration with `_Rb_global<bool>::_M_increment` expanded inline from
  its _tree.c body; CombatDialogueManager needs its real (4-aligned) members or it lands at +0x35 (probe 2).
- **CThreadedFileManager** -- 0x0098E690 ~CThreadedFileManager: `{ Uninitialise(); }` over the real STLport _tree.h/_tree.c
  text: VC7.1 inlines one level of the CPendingObjectSortKey tree's `_M_erase` into the implicit CPendingObjectMap[2][7] loop
  and keeps the call for the Objects map, exactly as retail.

## Proven
- 0x006ED200 ?Update@CCombatManager@@QAEXXZ -- sole caller CWorld::UpdateFrame 0x004A5A40 via CWorld+0x4C (Ego_r.pdb
  CWorld::PThingCombatManager, scoped_ptr<CCombatManager>); walks Combatants (+0x10, map<unsigned __int64, CCombatantInfo>,
  value at node+0x18) calling ?UpdateAttackingCombatantsList@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z 0x006ED030 (landed)
  with true, then DialogueManager +0x38 ?Update@CombatDialogueManager@@QAEXXZ 0x008AF780; name in Ego_r.pdb + FableWin.pdb.
- 0x00C051C0 __tr_init (zlib 1.2.1 trees.c `_tr_init`) -- retail version strings 1.2.1; called by z_deflateReset 0x00C01B60
  (landed, binds `__tr_init`); tail-jumps to init_block 0x00C03EE0 (landed, same TU text); deflate_state offsets
  l_desc +0xb10, d_desc +0xb1c, bl_desc +0xb28, last_eob_len +0x16ac, bi_buf +0x16b0, bi_valid +0x16b4.
- 0x00D2F640 ??1CQ_EndGameFocalSitesScript@NScript@@UAE@XZ -- sole caller ??_G 0x00D2F620 = slot 0 of vtable 0x012CBEB4,
  stored only by ctor 0x00D2A820, whose only caller Alloc 0x00D2F5F0 (new 0x94) is registered under "Q_EndGameFocalSites" /
  "S_QEGFS" (0x00CD5E9D..0x00CD5ECB); both PDB name sets carry the ??1; members match Ego_r (TeleportHSPs/RegionDialogueNames
  CCharString[5], JackTimer/WorldTimer).
- 0x00D27290 ??1CQ_DragonBossFightScript@NScript@@UAE@XZ -- ??_G 0x00D27270 = slot 0 of vtable 0x012CB70C, stored by ctor
  0x00D25410 whose Alloc 0x00D27240 (new 0x98) is registered under "Q_DragonBossFight" / "S_QDBF"; members match Ego_r (two
  vector<CScriptThing>, map<CCharString,bool> cs_flags, two CTimers).
- 0x00D83350 ??1CQ_HeroSoulsBriarScript@NScript@@UAE@XZ -- ??_G 0x00D83330 = slot 0 of vtable 0x012D41BC, ctor 0x00D80C30,
  Alloc 0x00D83300 (new 0x74) registered under "Q_HeroSoulsBriar" / "S_QHSB"; members match Ego_r (seh_BriarRose
  CScriptGameResourceObjectScriptedThingBase, csargs map<CCharString,CCharString>, two CTimers).
- 0x00DD1F40 ??1CQ_OrchardFarmRaidScript@NScript@@UAE@XZ -- ??_G 0x00DD1F20 = slot 0 of vtable 0x012DAF78, ctor 0x00DCC040,
  Alloc 0x00DD1EF0 (new 0x10C) registered under "Q_OrchardFarmRaid" / "S_QOFR"; members match Ego_r (Teams
  CCrateTeamManager[2] -> 0x00CDB3C0 twice, FailReasons CWideString[4], CrateList, TextSystemScriptCode, three CTimers).
- 0x007AFF70 ??1CTCBoss@@UAE@XZ -- sole caller ??_G 0x007AFF50 = slot 0 of vtable 0x0126D494, RTTI .?AVCTCBoss@@
  (: CTCBase : CBaseClassNonCopyable : CBaseClass); no vtable store = implicit; Ego_r members PBossDef / HelperCreatures / Effects.
- 0x007C5660 ??1CTCGhostSword@@UAE@XZ -- sole caller ??_G 0x007C66B0 = slot 0 of vtable 0x0126E494, RTTI .?AVCTCGhostSword@@;
  first store = that vtable (user dtor); Ego_r members PDef / Minions / MinionEffects / CurFadeInTimer / PItemRemoved.
- 0x009A1090 ??1CStreamingFontBank@@UAE@XZ -- sole caller ??_G 0x009A4E40 = slot 0 of vtable 0x0129AB54, RTTI
  .?AVCStreamingFontBank@@ (: CFontBank); then CFontBank's vtable 0x0129AB38 (bound ??_7CFontBank@@6B@); Ego_r offsets
  glyphBankCache +0x14, GlyphBankMemory +0x28, fontIndex +0x302C, PTexture +0x3030 used verbatim.
- 0x0098E690 ??1CThreadedFileManager@@UAE@XZ -- callers ??_G 0x00991330 (slot 0 of vtable 0x0129A164, RTTI
  .?AVCThreadedFileManager@@) and the atexit thunk 0x0122A4D0 of the global 0x013BC870; calls
  ?Uninitialise@CThreadedFileManager@@QAEXXZ 0x0098E0D0; Ego_r members DeviceQueue CPendingObjectMap[2][7] / Objects.

Descriptive (no PDB data-symbol names): 0x013BDA50 `g_StreamingFontBanks_013bda50` (CStreamingFontBank*[8], indexed by
fontIndex), 0x013BDA70 `g_StreamingGlyphCache_013bda70` (256 x 8 x 16-byte entries; struct SStreamingGlyphCacheEntry_013bda70
with FontIndex:4 / Flags:12 / Glyph -- field names descriptive; Ego_r's StreamingFontCacheEntry is a different, newer layout).
Also descriptive: CQ_OrchardFarmRaidScript::MissionFailed's type (Ego_r `<unnamed-type-MissionFailed>`, modelled as an
unnamed enum; 4 bytes, does not reach the bytes).

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
- 0x006ED200 CombatDialogueManager_UpdateDialogueFrame -> CCombatManager::Update
- 0x00C051C0 __tr_init -> zlib 1.2.1 `_tr_init` (label right; module _global, C linkage)
- 0x00D2F640 CParticleEmitter_Dtor_3 -> NScript::CQ_EndGameFocalSitesScript::~CQ_EndGameFocalSitesScript
- 0x00D27290 CParticleEmitter_Dtor_2 -> NScript::CQ_DragonBossFightScript::~CQ_DragonBossFightScript
- 0x00D83350 CParticleEmitter_Dtor_6 -> NScript::CQ_HeroSoulsBriarScript::~CQ_HeroSoulsBriarScript
- 0x00DD1F40 CParticleEmitter_Dtor_8 -> NScript::CQ_OrchardFarmRaidScript::~CQ_OrchardFarmRaidScript
- 0x007AFF70 CChunkedFileChunk_Destructor_WithVectorCleanup -> CTCBoss::~CTCBoss
- 0x007C5660 Destroy_CChunkedFileChunk -> CTCGhostSword::~CTCGhostSword
- 0x009A1090 CTextureScreen_dtor -> CStreamingFontBank::~CStreamingFontBank
- 0x0098E690 CThreadedFileManager_Destructor -> CThreadedFileManager::~CThreadedFileManager
- Neighbours (NAME_FIXES.tsv): 0x00D25410 manifest/landed CQ_NewOakValeIntroScript ctor is ??0CQ_DragonBossFightScript;
  the ??_G rows 0x00D27270 / 0x00D2F620 / 0x00D83330 / 0x00DD1F20 (landed as CLandscapeBackgroundPatch vector deleting
  dtors, catalog CVtable_xxx) are ??_GCQ_<quest>Script@NScript@@UAEPAXI@Z; 0x00CBD510 = ~CScriptBase (landed
  CRecordSet_00cbd510); 0x007ACC50 = map<CCharString,bool> _M_erase (landed FreeTree_CTreeOwner_007acc50); 0x0098FD80 =
  map<unsigned long, CFileObject*> _M_erase (landed CUnprovenKey_0098fd80); 0x00CDB3C0 nested
  CQ_OrchardFarmRaidScript::CCrateTeamManager dtor (landed global ??1CCrateTeamManager@@QAE@XZ); 0x0099A300 =
  ~CBaseClassNonCopyable (same as chunk 662).

## Header debt
- engine/CLevelScriptBase.h: `virtual ~CLevelScriptBase();` declared out of line; retail's is implicit (??_G 0x00CDD250 /
  0x00CDD270 call ~CScriptBase directly, no store). The four quest rows use a local copy without the declaration. Fix: drop
  the declaration (the PDB ??1?$CLevelScriptBase is the implicit member).
- engine/CTCBase.h (flattened, no vtable), engine/CStreamingFontBank.h / CFontBank.h / CTexture.h (flattened, no member
  functions; vland WARN SHARED_HEADER_AVAILABLE CTexture): local models with the headers' names/offsets. CTexture needs
  `~CTexture() { Uninitialise(); }` (0x009F9F70), CFontBank an inline user virtual dtor.
- CTCBoss::PhaseStartPos sits at Ego_r +0x22 (packed C3DVector); modelled at +0x24 (no new packing pragma; no byte effect).
