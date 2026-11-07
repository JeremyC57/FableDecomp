# Chunk 853 notes

Result: 1 matched (0x0043A570 RELOCATION_MATCH), 4 parked (0x0050A650 md6, 0x00E19560 md59, 0x00DF5000 md92, 0x0066B370 md439),
1 SKIP/split (0x00CE6CF0 = two functions, both byte-matched alone in split/). All tests PASS under g++ -std=c++98 and VC7.1.

## 0x0043A570 CPlayerGui::~CPlayerGui (MATCHED, src/0043a570.cpp)
- Name proven (catalogue placeholder `NHeroInformationScreens_Destructor` is wrong): first store is the CPlayerGui vtable 0x0123177C; the
  sole caller is ??_GCPlayerGui 0x00444F20 (call, then `test [esp+8],1` / operator delete); Ego_r + FableWin `??1CPlayerGui@@UAE@XZ`.
- Shape: user body `PGuiDef.Release(); PGuiDef.Object = 0;` then the implicit member destruction in reverse order, the inline base
  CTBaseSingletonNoDefaultAlloc<CPlayerGui> dtor last (vtable store 0x01231720).
- Key spelling: the static's release reads the count through a reference (header opt-in CDEFPOINTER_RELEASE_COUNTREF), as its own atexit
  dtor 0x01228B70 does; the default in-place `--Object->IVRefCount` gives `dec [ecx+4]`.
- Callees: _List_base<CScreenMessage>::clear 0x0043FE40, _List_base<CScriptScreenText>::clear 0x0043FDF0 (both mislabelled and bound under
  `@CActiveFile_Helper*@4`, NAME_FIXES), CTexture::Uninitialise 0x009F9F70, ~CBaseClass 0x0099A2E0, ~CSubtitleRenderer 0x00443E20,
  free 0x00BFEA14, operator delete 0x00BFE9BC.
- Symbol conflicts (NAME_FIXES): PGuiDef 0x013B878C is bound with a non-const pointee and as g_PlayerGuiDef / g_playerGuiDef; the base
  vtable 0x01231720 as `??_7CTBaseSingletonNoDefaultAlloc@@6B@`.

## 0x0050A650 CWorldMap::GetMapNavigationAreaInit (parked DIFFER(2233v2233) md6, differ/0050a650.cpp)
- Retry of chunk 850's SRC_FAIL from its n2 probe (Codex pass-13 attempt-8 body). Leaf IBE + box by pointer (Ego_r 0x004DF950, FableWin
  0x01C905D0); the reloc_bindings spelling QAE / ABVC2DBoxI is wrong (NAME_FIXES).
- Fixes found here: C2DBoxF corner ctor must be inline (existing C2DBOXF_CORNERS); new nested opt-in C2DBOXF_COPY (authored copy ctor,
  FableWin 0x01A664E0) fixes the frame (0x80 -> 0x7c); FABLE_RETAIL_CARRAY fixes the Maps index register order.
- Residue (= Codex's 4-byte residue): the final region_exits free reloads _M_start into ebx (retail eax), and the empty-loop edge skips the
  reload. Five loop/iterator spellings tried after a5, none moves it.
- All 18 relocations pair with retail; none is bound at another address.
- Descriptive: the CThingPhysical slot-57 view `UseHighDetailQuadTree` (+0xe4) is a row-local vtable view (header debt below).

## 0x00E19560 NScript::CQ_WhiteBalverineWWScript::SpawnBalverines (parked DIFFER(2159v2078) md59, differ/00e19560.cpp)
- Shape: heal 250, quarter / threshold from GetHealth, `for (long round = 1; round <= 3; )` with `++round; threshold -= quarter;` at the
  bottom (this fixes the frame slot order); row-local inline ~CScriptThing (the documented split lever) brings 2.0 KB of temporaries in line.
- Residue: the seh_soldier CScriptGameResourceObjectScriptedThingBase ctor comes out inline (retail calls 0x007E72A0); the in-while terminate
  exit expands ~CScriptThing for spawn_marker (retail calls 0x004AA840 and tail-merges the exits). /d2inl* diagnostics (never kept) move one
  or the other, never both: an inline-budget residue with no per-site spelling.
- Name: 0x01375748 is `NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations` (established spelling, via
  engine/fable_script_animation_defaults.h); the earlier probe's g_ScriptAnimFlag is dropped.

## 0x00DF5000 NScript::CQ_TentacleKrakenBossFightScript::PhaseOne (parked DIFFER(3039v3071) md92, differ/00df5000.cpp)
- Name confirmed from the body and the PDB: PhaseOne (the never_attempted ANALYSIS label DoMission_PrisonEscape is stale; the catalogue
  and reloc_bindings already say PhaseOne, so no LABELS line). New shared header engine/CQ_TentacleKrakenBossFightScript.h.
- Key spellings: `CRGBColour(0, 0, 255, 255)` for the Kraken health bar; implicit CScriptThing copy/assign; __forceinline operator= on
  CScriptThing / ScriptedThingBase as in the landed scripts.
- Residue: the in-loop ~CScriptThing temporaries split (retail expands the first, calls the later ones), ~map<CCharString,
  ScriptedThingBase> expanded (retail calls 0x00CDBFB0), one frame slot pair swapped. A row-local inline ~CScriptThing expands every site
  in this 3 KB function (4025 bytes): the lever that worked in SpawnBalverines does not reach here.

## 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents (parked DIFFER(1805v1808) md439, differ/0066b370.cpp)
- Catalogue placeholder sub_0066b370 and binding `?UpdateSounds@CTCSoundPlayer@@QAEXXZ` (landed FrameUpdate 0x0066BBF0) are wrong: Ego_r
  0x005749A0 / FableWin 0x01E7E870 `?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ`, sole caller FrameUpdate's tail jump (NAME_FIXES, LABELS).
- Shape: early-outs (sound enabled, 0x01375758 flag, LOD distance, GetTC<CTCAnimationComplex>), sound identifier from the entity sound
  sub-def, surface / engine-theme / last material, then for each anim set (GetAllCombinationAnims + the animator) the best-alpha anim
  (hoisted count, unrolled x4 as retail) and its events between the times: bit 31 to the attached emitter's handler, otherwise the
  criteria chain voice/extra -> material/extra -> material -> criteria -> event name.
- Key spellings: `(*it)->` with no CAnimSet* local (retail reloads *it each use); hoisted `anims` begin pointer and `count`;
  `event->ID & 0x80000000` (gives jns).
- Residue: global register assignment (this in edi, retail esi), theme/material CDefPointer releases as load/dec/store (retail dec [ecx+4]),
  sound_def Object not CSE'd.
- Callees with evidence: GetStrongestEngineThemeAt 0x0063ADF0 (Ego_r AAJ spelling; landed PAJ spellings wrong, NAME_FIXES);
  GetAllCombinationAnims 0x0070D100, GetSoundEventManager 0x006F90C0, CManager::GetAnimEventsBetweenTimes 0x006F99B0 (all Ego_r + FableWin,
  labels corrected in LABELS.tsv); InternalPlaySoundEvent 0x0066B120 (landed spelling; also bound as PlayAttachedSampleByName_0066b120).
- DESCRIPTIVE: `g_bPlayAnimationSoundEvents` 0x01375758 (a .data bool = 1, read only at 0x0066B384; no PDB name found, probably an
  NGlobalConsole variable); CThingSurfaceView (CThing slots 38 / 40) and CSoundEventHandlerView (the CThingSoundEmitter +0x98 object,
  slot 12) are vtable views; CEntitySoundDef / CEngineThemeDef / CMaterialDef / CAnimSet are row-local models (Ego_r member names).
- Pre-existing multi-name globals (not changed): 0x0129D25C is bound as g_InvalidSoundHandle (3JA / 3KA / 3KB), g_InvalidSoundHandle_0129D25C,
  g_invalidSampleID; 0x0137E974 as g_bSoundEnabled and g_soundsEnabledFlag. No PDB name for either; the row uses the landed spellings.

## 0x00CE6CF0 NScript::CGameflowScript::Init (SKIP: row holds two functions, split/)
- 0x00CE6CF0 Init, 16 bytes: `PostSavePosition = EGP_INTRO; CoreQuestWaiting = 0;` then `jmp +0` into the next function. MATCH alone.
- 0x00CE6D00 DeclareGossipCategories, 2221 bytes: 54 AddRumourCategory calls (Ego_r 0x00CCE770). RELOCATION_MATCH alone, first try.
- The catalogue row must be recut (0x00CE6CF0 length 16, new row 0x00CE6D00 length 2221); split/<addr>.cpp + .test.cpp are ready to land
  after the recut. The landed 0x00D3B410 "CGameflowScript::DeclareGossipCategories" is really CQ_GuildTrainingScript::GossipSetup (LABELS,
  NAME_FIXES).

## Header debt
- CThing vtable slots 38 GetSurfaceThing-like / 40 GetSurfaceMaterial-like (descriptive) and the CThingSoundEmitter +0x98 handler object
  (slot 12 HandleEvent-like) are not in the shared headers; 0x0066B370 uses row-local views. Real names need the CThing RTTI slot list.
- CThingPhysical slot 57 (+0xe4, UseHighDetailQuadTree) likewise a row-local view in 0x0050A650.
- CAnimSet / CAnimateThing / NAnimationEvents::CManager / CSmallVector have no shared header (row-local in 0x0066B370).

## Lane note
- A source with an unclosed namespace (lost `}`) spins cl.exe exactly like `#error`; my errchk.sh now also checks brace balance.
