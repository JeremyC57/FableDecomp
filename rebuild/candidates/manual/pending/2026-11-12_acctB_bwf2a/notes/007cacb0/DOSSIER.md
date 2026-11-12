# DOSSIER 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate (2974 B) -- identity scout bwf2, 2026-11-12

Companion: $W/OUTLINE.md (frame map + block-by-block pseudo-C++; sections C1-C7 at its top correct the prior scout outline).

## 1. Retail shape (xdis 007cacb0 0xbae / dis_all.txt)
- thiscall, one `bool` arg never read, `ret 4`. Prologue `sub esp,74h ; push esi ; mov esi,ecx ; mov al,[esi+25h] ; test al,al ;
  je 0x007CB847` THEN `push ebx ; push ebp ; push edi` (shrink-wrapped). Saved order esi, ebx, ebp, edi. Frame 0x74.
- No `push ebp ; mov ebp,esp` (/Oy on: no `optimize("y",off)`), no fs:0 SEH frame (no /EHsc although CWideString / CCharString
  locals have dtors), no /GS cookie, no size-optimisation idioms (`optimize("s")` not indicated). => no `// Flags:` line, no pragma.
  Same as every landed CTCHeroReceiveItems_* sibling (grep: none has Flags or `#pragma optimize`).
- 3 epilogues (0x007CAF1B, 0x007CAF93, 0x007CB844) + the early-out tail at 0x007CB847. No jump table, no cold block.

## 2. Identity (manifest label is RIGHT)
- `?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z` -- Ego_r.pdb 0x007C9230 (egor_pdb_names.tsv:18476, egor_signatures.tsv:19414);
  FableWin.pdb 0x02726CD0 (7339 B / 1646 i, naming_stragglers/FableWin.exe.signatures.tsv) + 0x017E4965 thunk.
- rtti-vtables.tsv:6213 `CTCHeroReceiveItems 0126ebfc 26 007cacb0`; xref.py: only reference is .rdata 0x0126EC64 (= 0x0126EBFC+26*4).
  Slot 26 = CTCBase `virtual void GlobalUpdate(bool);` (engine/CTCBase.h:105). Signature `virtual void GlobalUpdate(bool paused)`,
  public, non-const, returns void. Parameter name not in PDB name sets (use CTCBase.h's; unnamed is fine).

## 3. Callees (address | landed row | landed name | PDB spelling | flag)
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | PeekGuiDef | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (Ego_r 0x0044B230) | ok; CPlayerGui.h:223 FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF |
| 0x00A01B10/B50/B90/C10 | CBaseIntelligentPointer_* | ctor / GetPItem / SetPItem / ~ | ??0 / ?GetPItem@..QBEPAVCBaseObject@@XZ / ?SetPItem@..QAEXPAVCBaseObject@@@Z / ??1..UAE@XZ | ok (engine/CIntelligentPointer.h) |
| 0x007CABD0 | 00/7c/CTCHeroReceiveItems_AddItemToInventory_007cabd0.cpp | AddItemToInventory | ?AddItemToInventory@CTCHeroReceiveItems@@AAEXAAVCThing@@_N@Z (private) | ok; same TU, defined earlier, NOT inline -> called |
| 0x00BFEA14 | global_free_00bfea14.cpp | free | _free | STLport node deallocate |
| 0x004CC340 | 00/4c/global_ISTHINGALIVECIntelligentPointerCThing_004cc340.cpp | IS_THING_ALIVE | ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z | ok (only 1st PContainer site calls it) |
| 0x00686A70 | CTCBase_GetWorld_00686a70.cpp | GetWorld | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ | ok |
| 0x00687540 | CMessageEventManager_AddMessage_00687540.cpp | AddMessage | ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z | ok; args (65, ConstantFPS, C3DVector(0,0,0), 0, 0, 0.0f, 0.0f, (long)PContainer.Get()) |
| 0x0040F020 / 0x004365B0 | CVectorMap_LowerBound_* | LowerBound (non-const / const) | CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound | ok: non-const => GetTC, const => PeekTC |
| 0x0041C930 | CSoundInit_CSoundInit_0041c930.cpp | ??0CSoundInit@@QAE@XZ | same | ok; do NOT define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR (retail calls it) |
| vtbl +0x24 / +0x10 | CASoundManager slot 9 Get2DEmitter (descriptive, CASoundManager.h:141) / CASoundEmitter slot 4 PlaySoundEvent (CASoundEmitter.h:39) | | | |
| 0x0099AED0 / 0x0099B510 | CWideString ctor/dtor | ??0CWideString@@QAE@XZ / ??1CWideString@@QAE@XZ | ok |
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long=-1) / ~CCharString | | ok (the `push -1` is the default length) |
| 0x009C95E0 | NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp | GetTextBySymbol | ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z | ok |
| 0x00405F00 | global_GFIntToWideString_00405f00.cpp | GFIntToWideString | ?GFIntToWideString@@YI?AVCWideString@@J@Z (Ego_r 0x00404F90) | no shared declaration in rebuild/include -> declare in row |
| 0x0099BE70 / 0x0099BF30 | global_operator_* | ??H@YI?AVCWideString@@ABV0@0@Z / ??H@YI?AVCWideString@@ABV0@PBG@Z | Ego_r: ..ABV0@0@Z / ..ABV0@PB_W@Z | FLAG: landed PBG v Ego_r PB_W (wchar_t setting; landing debt, not row's) |
| 0x0099B8D0 / 0x0099B7D0 | CWideString operator+= / operator= | ??YCWideString@@QAEABV0@ABV0@@Z / ??4.. | ok |
| 0x00BFEA70 | global_ftol2 | __ftol2 | | plain (long) casts x3 |
| 0x005D9070 / 0x005D92F0 | CTCInventoryItem_GetDescription / _GetStartingRightHandedSet | | ?GetDescription@CTCInventoryItem@@QBE?AVCWideString@@XZ / ?GetStartingRightHandedSet@CTCInventoryItem@@QBE?AVCRightHandedSet@@XZ | repo CTCInventoryItem.h has only the STATIC overloads -> overlay _DESCRIPTION |
| vtbl +0xC0 (0x00702FF0) | CThingObject_GetProperties_00702ff0.cpp | ?GetProperties@CThingObject@@UBEKXZ | ok; via inline HasProperty (FableWin ?HasProperty@CThingObject@@QBE_NK@Z) |
| 0x00686D40 | CTCBase_GetPlayerManager | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ | ok |
| 0x00449970 | CPlayerManager_GetMainPlayer | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ | ok; then inline GetPPlayerCharacter (CPlayer +0x34 GetPItem) |
| 0x007CBDC0 | CTCGold_GetGold_007cbdc0.cpp | GetGold | ?GetGold@CTCGold@@QBEJXZ (Ego_r 0x007CA450) | ok; next TU (CTCGold.cpp), not visible |
| 0x0055F3B0 | CTCHeroOnlineScoreboard_UpdateScoreForActivity | | ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z | ok |
| 0x0057B327 | CTCHeroStats_AddMoney | | ?AddMoney@CTCHeroStats@@QAEXJ_N@Z | ok |
| 0x009C7B70 | global_ConstructString_009c7b70.cpp | ConstructString | ?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z | ok (NGameText_CDataBank.h FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS) |
| 0x004BBE10 | CRightHandedSet_Set_004bbe10.cpp | **Set** | **?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z** (Ego_r 0x004BF6E0, FableWin 0x01D0D780) | FLAG NAME_FIX: no PDB has CRightHandedSet::Set; same body/signature |
| 0x00A22F80 | CRightHandedSet_Rotate | ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z | ok (CRightHandedSet.h _ROTATE) |
| 0x00686D60 | CTCBase_PeekComponent | ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ | ok |
| 0x00492E4B | CSubtitleRenderer_SetText_00492e4b.cpp | ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@KM_N2@Z | ok; CSubtitleRenderer.h FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT; args (font, text, 0x62, 0.0f, true, true) |
| 0x0048DAE0 | global_GFLimit_0048dae0.cpp | ??$GFLimit@MMM@@YIXAAMABM1@Z | ok |
| 0x009BEDC0 | CDisplayManager_GetRenderTargetDimensions | ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ | CDisplayManager.h:63 |
| 0x004C7590 | CThing_SetAsRenderedThisFrame | ?SetAsRenderedThisFrame@CThing@@QAEXK@Z | CThing.h (FABLE_RETAIL_CTHING) |
Globals: PSoundManager 0x013B8394; CMainGameComponent::PMainGameComponent 0x013B86A0 (read via inline Get()); ConstantFPS 0x01375550 (inline
GetConstantFPS); GPDisplayManager 0x013B8390; CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790 (SubtitleRenderer +0x18);
g_ItemUp 0x01383A5C (descriptive, file-static of this TU, as landed PrepareToShowItems); CIntelligentPointer<CThingObject> vtable 0x0125A858.
Strings: "TEXT_GUI_FISH_LENGTH" 0x0126ECD4, "..._LENGTH_UNITS" 0x0126ECB8, "TEXT_GUI_FISH_WEIGHT" 0x0126ECA0, "..._WEIGHT_UNITS" 0x0126EC84,
"TXT_OBJT_GOLD" 0x0126EC6C; wide L".  " 0x0126EC7C, L": " 0x01236A50, L"." 0x0124C4A8. Floats 1.0f 0x0122DED8, 640.0f 0x01238168, 480.0f 0x01238874.

## 4. Members (struct_layouts_egor.tsv CTCHeroReceiveItems, sizeof 184 Ego_r = 0xAC retail; retail = Ego_r - 12 from PCurrentItem on)
Items list +0x0C, PickedUpItems vector<long> +0x10, PCurrentItem CIntelligentPointer<CThingObject> +0x1C, PersistCurrentItem +0x24,
ReceivingItems +0x25, DisplayingItems +0x26, ForceClear +0x27, SendMessageW +0x28 (PDB spelling), PContainer CIntelligentPointer<CThing>
+0x29 (byte-packed), AmountThroughForAreaResize float +0x34, FullSizeTimer long +0x38, InitialAreaItemTook / OldAreaItemTaking /
CurrentAreaItemTaking C2DBoxF +0x3C/+0x4C/+0x5C, OldItemRHSet / CurrentItemRHSet CRightHandedSet +0x6C/+0x84, ItemPrimitiveHandle /
BackgroundPrimitiveHandle CEnginePrimitiveHandle +0x9C/+0xA4. CTCBase: ParentThing +0x04.
- Repo engine/CTCHeroReceiveItems.h offers only FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS (Items, PickedUpItems, OnSerialise) and
  _VSLOTS. Everything from PCurrentItem on: hdr_prior opt-in FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE (declares GlobalUpdate,
  private AddItemToInventory, inline IsReceivingItems/IsDisplayingItems, full member list + offset asserts) -- must be RE-BASED onto
  the current repo header ($W/hdr/ is empty).
- Landed siblings model the class LOCALLY although a shared header exists: OnKill 0x007CA630, PrepareToShowItems 0x007CA670, dtor
  0x007CA8E0 (also CRightHandedSet, C2DBoxF, CIntelligentPointer stand-ins in PrepareToShowItems). Use shared headers.
- Other classes: CThing UsingComponents bitset words +0x20.., Components CVectorMap +0x44, Dead bit +0x91.0 (CThing.h
  FABLE_RETAIL_CTHING_GETTC_EXPANDED for GetTC/PeekTC); CTCFish Length +0x10 / Weight +0x14 (inline GetLength/GetWeight -- needed for
  retail's early length copy); CTCFishingSpot Enabled +0x24; CTCDiggingSpot Enabled +0x0C; CTCOwnedEntity POwner +0x0D (inline
  GetPOwner, CTCOwnedEntity.h:36); CTCBuyableHouse Rented +0x12 (GetRented, CTCBuyableHouse.h:25); CTCHeroStats RentMoney +0x54
  (hdr_prior _RENTMONEY: AddRentMoney / GetRentMoney inline); CPlayerGuiDef ReceiveItemsSoundCriteria CCharString +0x644 (repo header
  flattens it to `CCharStringData* ReceiveItemsSoundCriteria_PStringData`; hdr_prior _CONTAINERITEM types it), NumberOfSecondsFor
  ContainerItemToReachFullSize +0x8C0, ContainerItemDisplayAreaTLPos_X/_Y +0x8C4/+0x8C8, BRPos_X/_Y +0x8CC/+0x8D0, ..ToRotate +0x8D4,
  ..ToStayAtFullSize +0x8D8 (CPlayerGuiDef.h:628-634, flattened floats, usable as-is); CMainGameComponent PSampleBank +0x10,
  PTextBank +0x14 (GetTextBank/GetSampleBank inline), PCutSceneMainFont +0x1614C; CWorld PMessageEventManager +0x60 (inline
  GetMessageEventManager, CWorld.h:251).

## 5. Callee-body visibility
- Inline in retail, expanded here: CEnginePrimitiveHandle::Clear x2 (CEnginePrimitiveHandle.h FABLE_RETAIL_CENGINEPRIMITIVEHANDLE +
  FABLE_RETAIL_CIVCOUNTEDPOINTER), STLport list pop_front / erase / begin / end / empty / front (fable_containers.h FABLE_RETAIL_STLP_LIST,
  erase body lines 957-966 = retail's unlink order), GetTC/PeekTC/HasTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED), IS_THING_ALIVE on
  PCurrentItem x2 + 2nd PContainer site (ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE), CIntelligentPointer ctor(T*)/operator=/Get
  (FABLE_RETAIL_CINTELLIGENTPOINTER (+_ASSIGN)), GFGetInterpolatedValue x4 (engine/GFGetInterpolatedValue.h), CWorld::
  GetMessageEventManager, CMainGameComponent Get/GetConstantFPS/GetTextBank/GetSampleBank/PeekCutSceneMainFont, CTBaseSingletonNoDefault
  Alloc<CPlayerGui>::Get + GetSubtitleRenderer, CTCFish GetLength/GetWeight, SetAsEnabled x2, HasProperty, GetPOwner, GetRented,
  AddRentMoney/GetRentMoney, CPlayer::GetPPlayerCharacter, C2DBoxF / CRightHandedSet implicit copies.
- Called although inline-able (VC7.1 per-function inline budget in a 2974-byte function; PrepareToShowItems in the same TU EXPANDS
  Init): CRightHandedSet::Init 0x004BBE10, GFLimit<MMM> 0x0048DAE0, CSoundInit() 0x0041C930, IS_THING_ALIVE<CIP<CThing>> 0x004CC340
  (1st PContainer site only). Lesson (agent2050 brief): call-v-expand follows static frequency -- write naturally first; if c2 expands
  a call site retail calls, look for one more branch level / a different helper (e.g. `!IS_THING_DEAD`) before reaching for noinline.
- Same TU, defined earlier, non-inline -> called: AddItemToInventory 0x007CABD0. Next TU (not visible): CTCGold::GetGold 0x007CBDC0.
- No STLport out-of-line helper (map::operator[], tree ctor/dtor, list clear) is called by this function.

## 6. Sibling rows to copy idioms from
- CTCHeroReceiveItems_PrepareToShowItems_007ca670.cpp (same TU, 0x007CA670): two-statement box copies, `OldItemRHSet.Set(g_ItemUp,
  C3DVector(0,0,-1)); CurrentItemRHSet = OldItemRHSet;`, `PCurrentItem = CIntelligentPointer<CThingObject>(0);` -- exactly the
  0x007CADA3 and 0x007CB635 sequences here. No Flags, no pragma, local models.
- CTCHeroReceiveItems_OnKill_007ca630.cpp: the Items iterator loop (begin v cached header, end() re-read), `CThingObject* item =
  it->first.Get(); if (IS_THING_ALIVE(item))`; opt-ins FABLE_RETAIL_CTCBASE, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CINTELLIGENTPOINTER
  under `#pragma pack(push,1)`.
- CTCHeroReceiveItems_GiveItems_007ca9e0.cpp / OnSerialise_007cb8b0.cpp: FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS,
  FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CCHARSTRING with engine/CTCHeroReceiveItems.h.
- agent2039 src/006aca30.cpp (RELOCATION_MATCH, not yet landed): the identical gold/rent/scoreboard sub-block with its define list
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED, _CTCHEROSTATS_RENTMONEY, _CTCBUYABLEHOUSE ...) and hdr overlay (agent2039/hdr/engine/CTCGold.h,
  CTCHeroOnlineScoreboard.h, CTCHeroStats.h).
- CTCContainerRewardHero_RewardHeroWithItems_0074ee90.cpp / CTCChest_DisplayLockedMessage_00750110.cpp: the GetTextBySymbol ->
  CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(PeekComponent().PeekCutSceneMainFont(), text, ...) tail;
  NInventory PlayInventoryMovementSound 0x005BD33C (CSoundInit out-of-line ctor + member store); NPlayerGui_GFAnchorToBottomRight_00643d0c
  (`(float)Width + pos.X - 640.0f`); CGameCameraBase_GetRenderCamera_006fd5e0 (GFGetInterpolatedValue); prior scout stub
  $W/prior/scout/stub.cpp has a full compiling define/include list (6 of its macros exist only in hdr_prior).

## 7. NAME_FIXES for the coordinator
- 0x004BBE10 ?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z (landed "Set"; PrepareToShowItems 0x007CA670 local `Set`).
- 0x0099BF30 Ego_r ??H@YI?AVCWideString@@ABV0@PB_W@Z v landed PBG (project-wide wchar_t question; informational).

## 8. Re-verification pass (identity scout re-run, 2026-11-12, against the CURRENT repo)
- Re-read dis_all.txt 0x007CACB0..0x007CAD1D and 0x007CB351..0x007CB84B against OUTLINE.md: block addresses, call targets, TCI keys
  (0x1A, 4, 0xCC, 0x33, 0xD5, 0x10D), frame slots and the box/RHSet copy orders all check out. Sections 1-7 above stand.
- HEADER DRIFT since hdr_prior (the author must re-base; $W/hdr/ is still EMPTY):
  * engine/CRightHandedSet.h now has `void Init(up,forward){Up=up;Forward=forward;}` with an INLINE body under
    FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE (user: NParticleEngineCPSCRenderSprite_PreparePrimitives_00afcc30.cpp). Retail CALLS Init
    here (0x007CB65C call 0x004BBE10) -> do NOT define _CQUATERNION_PARTICLE_UPDATE; add hdr_prior's declaration-only
    FABLE_RETAIL_CRIGHTHANDEDSET_INIT branch to the overlay (mutually exclusive with _CQUATERNION_PARTICLE_UPDATE; same Init spelling).
    _SET (inline `Set`) must stay undefined too (it is the wrong name and inline).
  * engine/CTCHeroReceiveItems.h now has _VSLOTS (slot 23 IsComponentSerialisable, ends with `public:`) which hdr_prior lacks; overlay =
    current file + hdr_prior's _GLOBALUPDATE blocks (includes C2DBoxF.h / CRightHandedSet.h / CEnginePrimitiveHandle.h, GlobalUpdate
    decl, IsReceivingItems/IsDisplayingItems, private AddItemToInventory, members +0x1C..+0xA4, offset asserts) and _PICKEDUP if wanted.
  * engine/CTCHeroStats.h: NO current branch has both RentMoney (+0x54, only in the _SERIALISE branch line 193) and AddMoney (only
    _BOASTS line 121 / _BRIBE+_ADDMONEY line 148). Re-base hdr_prior's / agent2039 hdr's FABLE_RETAIL_CTCHEROSTATS_RENTMONEY branch
    (agent2039/hdr/engine/CTCHeroStats.h:275) with AddMoney decl + inline AddRentMoney/GetRentMoney.
  * Still absent from repo: CTCFish.h, CTCFishingSpot.h, CTCDiggingSpot.h, CTCGold.h, CTCHeroOnlineScoreboard.h (use hdr_prior / agent2039
    hdr copies), FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY, _CTCINVENTORYITEM_DESCRIPTION, _CPLAYERGUIDEF_CONTAINERITEM (repo
    CPlayerGuiDef.h:456 still flattens +0x644 to `CCharStringData* ReceiveItemsSoundCriteria_PStringData`), _CTCHEROSTATS_RENTMONEY,
    _CTCHERORECEIVEITEMS_GLOBALUPDATE, _CRIGHTHANDEDSET_INIT -- all present in $W/hdr_prior.
  * Present in repo and usable as-is: FABLE_RETAIL_CTHING_GETTC_EXPANDED (CThing.h), FABLE_RETAIL_IS_THING_ALIVE (ThingAlive.h; the
    _NOINLINE variant is per-TU -> wrong here), FABLE_RETAIL_CENGINEPRIMITIVEHANDLE, FABLE_RETAIL_STLP_LIST (fable_containers.h),
    FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF, FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT, FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS,
    FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE, CTCOwnedEntity.h GetPOwner (line 36), CTCBuyableHouse.h GetRented (line 25),
    NGameText_CDataBank.h GetTextBySymbol (line 53, under _NGAMETEXT_GROUPENTRY at line 48).
- GFLimit<float,float,float> out-of-line precedent: CSubtitleRenderer_FormatTextForVWindow_0048f50c.cpp:30 declares the template
  body-less, `template <class T, class TMin, class TMax> void __fastcall GFLimit(T& value, const TMin& minimum, const TMax& maximum);`
  -> retail-style call 0x0048DAE0. Do the same and keep engine/GFLimit.h (inline body, line 6) out of the include closure. Call shape
  0x007CB705..0x007CB730: ecx=&AmountThroughForAreaResize, edx=&0.0f temp (+0x5C), push &1.0f temp (+0x64) -> `GFLimit(Amount, 0.0f, 1.0f)`.
- Rotate tail 0x007CB7E4..0x007CB835: ecx = &CurrentItemRHSet is the copy SOURCE pointer reused as Rotate's this; `push ecx` reserves the
  float slot (fstp [esp]), then push &g_ItemUp. FullSizeTimer: load hoisted to 0x007CB7E4 before the copy, `dec ebp ; mov [esi+38h],ebp`
  at 0x007CB811 inside it -- write `OldItemRHSet = CurrentItemRHSet; FullSizeTimer--;` first, swap if c2 schedules differently.
