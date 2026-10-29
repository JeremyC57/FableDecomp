# Hand chunk 562 (CPersistContext .. CTCTavernGame): 12 of 12 byte-exact

All 12 rows grade RELOCATION_MATCH (gall.sh), pass their behaviour tests under g++ and VC7.1 (tall.sh / tall.py), and pass
the vland dry run with no RELOC_CONFLICT or RELOC_DUPLICATE. There are no retail_asm rows.

## What matched (12)
- **CPersistContext::Transfer<signed char> 0x004CC5C0**: a sed copy of the landed Transfer<T> family (agent561 0x004106F0) with
  T = signed char. The RTTI `@C@@` on wrapper vtable 0x01239A80 proves T. TransferBinaryOut takes T by value (FableWin
  `QAEXAAVCDataOutputStream@@C@Z`).
- **CThingManager::WorldUpdateThings 0x0051FCC0** (PDB name; caller CWorld 0x004A5D1C):
  - Zero the counter, set FirstRenderOfFrame, and if the global gate is set, walk thing types 1..15.
  - Type 1 and type 5 each have their own gate flag. The shape is `if (type == 1) { if (!f1) continue; } else if (type == 5)
    { if (!f5) continue; }`, and VC7.1 merges the two flag tests into one `test al,al`.
  - Skip a type whose ClassTypeInfo entry is unused. Otherwise walk its update list, reading the next link first. Bump the
    counter for each thing.
  - For a thing that is not Dead, is Activated and is not InLimbo: call FrameUpdate, then the virtual IsFrameUpdated
    (slot 44), and RemoveFromUpdateList(*thing) when that returns false.
  - Tail call to UpdateDeaths.
  - CClassFactory comes from its shared header through the layout-rename pattern. Matched on the first grade.
- **NUISystem::CEditBox::~CEditBox 0x005408F0**: `CManager::GetInstance()->RemoveObserver(this)` (CObservable slot 5), then
  the implicit member dtors: three CWideStrings, then the two `list<CCountedPointer<CAction>>` clears (0x005356A0).
- **NUISystem::CMovie::~CMovie 0x005483C0**:
  - `Handle.Clear()`, then the parent viewport's ClearHandle (slot 91).
  - If this is the playing movie: Close the video system if Playing, forget PlayingMovie, run the inline FreeTextures (the
    engine's slot 70), then `delete g_pVideoSys; g_pVideoSys = 0`.
  - CVideoSys uses the layout-rename pattern over engine/CVideoSys.h.
- **NUISystem::CMesh::~CMesh 0x00550230**: `Handle.Clear();`. The model is CEnginePrimitiveHandle with Clear plus its inline
  virtual dtor (taken from agent561's ~CSprite 0x0041AB40).
- **NUISystem::CMorphingSprite::~CMorphingSprite 0x005541E0**: `for (it) delete *it; Handles.clear();`, then the
  map<ulong,ulong> dtor (`_M_erase` 0x0042FE07) and the list dtor. The tail is ~CSprite 0x0041AB40.
- **NUISystem::CScrollBarOutside::~CScrollBarOutside 0x00559520** and **NUISystem::CScrollBar::~CScrollBar 0x00559A10**: two
  `delete` of action lists each, in the order retail uses (Above/Under; Down/Up). They tail into ~CClickable 0x0055B760 and
  ~CDraggable 0x0055ABA0.
- **CTCBoastUI::~CTCBoastUI 0x0056AC80**: `RemoveFromGlobalTCs(); for (unsigned long i...) delete Components[i];
  Components.clear();`. Clear is STLport erase through `__copy_trivial`/memmove. Then the BoastGroups map dtor (`_M_erase`
  0x0056D440).
- **CTCQuestCompletionUI::~CTCQuestCompletionUI 0x005E41E0** (587 bytes):
  - The body follows the PDB inline `CWorld::SetAsStarting3DRendering() { FrameStarted3DRendering = GetFrame(); }`.
  - Writing `GetWorld().FrameStarted3DRendering = CWorld::GetFrame()` calls GetFrame before GetWorld, and gave
    DIFFER(587v587).
  - `CList::GPreviousSelected.clear()` uses an inline list clear.
- **CTCQuestCompletionUI::Copy 0x005E49E0** (vtable slot 12):
  - Clear Renown, then Gold, then Experience.
  - Refill each from the source via `map::operator[]` 0x005E9760.
  - Tail call to `CTCBase::Copy(other)` 0x006868D0.
  - The iterator needs a user default ctor.
- **CTCTavernGame::~CTCTavernGame 0x005EDA50** (implicit, with no vptr store): emitted through
  `game->CTCTavernGame::~CTCTavernGame()` in a one-line helper. The members run in reverse order (counted pointers, five
  CEnginePrimitiveHandle vectors, the component vector), then the tail jump to ~CTCBase.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00559520 catalog `CScrollableList::VectorDtor` is `NUISystem::CScrollBarOutside::~CScrollBarOutside`.
- 0x00559A10 catalog `CScrollableListWithButton::VectorDtor` is `NUISystem::CScrollBar::~CScrollBar`.
- 0x005E49E0 catalog `CTCQuestCompletionUI::vfunc_12` is `CTCQuestCompletionUI::Copy(const CTCBase&)`.
- 0x005EDA50 catalog `CChunkedFileChunk::DestructorCleanup` is `CTCTavernGame::~CTCTavernGame`.
- 0x00686830 manifest `~CChunkedFileChunk` is `CTCBase::~CTCBase`.
- 0x004374F0 manifest `C3DMeshInfo::HasPhysicsMesh` is `CPlayerGui::IsOpen` (vland RELOC_NAME warning).
- 0x005E8C00 catalog `_unattributed::FreeTree_CTreeOwner_005e8c00` is the STLport `_M_erase` of map<long,ulong>.
- The full list with mangled spellings is in NAME_FIXES.tsv (14 rows).

## Names
### Proven (PDB name set plus a second source: vtable/RTTI, caller, or callee chain)
- The 12 leaves, all present in Ego_r.pdb: `??$Transfer@C@CPersistContext@@QAEXPBDAACABC@Z`,
  `?WorldUpdateThings@CThingManager@@QAEXXZ`, `??1CEditBox@NUISystem@@UAE@XZ`, `??1CMovie@NUISystem@@UAE@XZ`,
  `??1CMesh@NUISystem@@UAE@XZ`, `??1CMorphingSprite@NUISystem@@UAE@XZ`, `??1CScrollBarOutside@NUISystem@@UAE@XZ`,
  `??1CScrollBar@NUISystem@@UAE@XZ`, `??1CTCBoastUI@@UAE@XZ`, `??1CTCQuestCompletionUI@@UAE@XZ`,
  `?Copy@CTCQuestCompletionUI@@UAEXABVCTCBase@@@Z`, `??1CTCTavernGame@@UAE@XZ`.
- Callees:
  - CThing::FrameUpdate 0x004C9D20, CThing::IsFrameUpdated (slot 44)
  - CThingManager::RemoveFromUpdateList 0x0051ECC0 (by reference) and CThingManager::UpdateDeaths 0x0051F070
  - NUISystem::CManager::GetInstance 0x0041E5F2
  - CTCBase::~CTCBase 0x00686830, RemoveFromGlobalTCs 0x00686DF0, GetWorld 0x00686A70, Copy 0x006868D0
  - CWorld::GetFrame 0x0049D870
  - CPlayerGui::IsOpen 0x004374F0 and Open 0x004374B0
  - CVideoSys::Close 0x00A3B380 and ~CVideoSys 0x00A3BC20
  - map<long,ulong>::operator[] 0x005E9760
  - ~CChangingStateComponent 0x0052CCF0, ~CClickable 0x0055B760, ~CDraggable 0x0055ABA0, ~CSprite 0x0041AB40
  - ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, ~CBaseIntelligentPointer 0x00A01C10
- Globals: g_pMainGameComponent 0x013B86A0, g_pPlayerGui 0x013B8790, g_pVideoSys 0x013B8ABC.
- CComponent virtual slots: Update (1), Draw (2), FindChild (3). FindChild is now spelled with its PDB signature,
  `UAEPAV12@VCCharString@@@Z`.
- STLport helpers, named by structure (STLport 4.6 sources, plus member types from the PDB and the retail callers):
  - `_Rb_tree<...>::_M_erase` 0x0042FE07 (map<ulong,ulong>), 0x0056D440 (map<CMenuEntry*,CQuestManager::CBoast>) and 0x005E8C00
    (map<long,ulong>)
  - `_List_base<CCountedPointer<NUISystem::CAction>>::clear` 0x005356A0
  - `_List_base<CQuestManager::CBoast>::clear` 0x004B8C00

### Probable
- `NUISystem::CList::GPreviousSelected` 0x013B8A98:
  - The name and its initializer order come from both PDBs and match the retail initializers 0x0121A6AC / 0x0121A6D0.
  - The atexit thunk sits in the CList TU.
  - The element type `CList*` is probable.
- CMovie statics: PlayingMovie 0x013B8AB4, PEngine 0x013B8AB8 and MovieTexture 0x013B8AC0. The spellings are already bound
  and used by the landed FreeTextures 0x00547B90, but no PDB static-member entry confirms them.

### Descriptive (no PDB name; these four addresses have no other references)
- `g_UpdateThingsEnabled_01375755`, `g_UpdateThingType1Enabled_0137575a`, `g_UpdateThingType5Enabled_01375759` (bool gates in
  WorldUpdateThings), `g_NoThingsUpdated_013b8a84` (long counter).
- Searched: the PDB function name sets (they hold functions only), the FSE address tables and manifest, the landed labels and
  bindings, and docs/. There was no hit.
- Pads: `CUnprovenSlotN` for CThing slots 0..43, CComponent slots 4..90, the CIEngine slots, and CManager slots 11..36
  (except 29).

## Review at landing
- 0x013B8AB8 has three bound spellings:
  - `?PEngine@CMovie@NUISystem@@2PAVCIEngine@@A`, used here and by the landed FreeTextures 0x00547B90
  - `?Engine@CMovie@NUISystem@@2PAVCIEngine@@A`, from the landed row 0x005482D0
  - `?g_pRenderDevice@@3PAUCRenderDevice@@A`, from the landed row 0x00547C90
- 0x013B8AC0 (CMovie::MovieTexture) is also bound as `?g_pMovieTexture@@3PAXA` (0x00547C90) and
  `?g_pMovieTexture@@3PAVCTexture@@A` (0x00547F60).
- vland raised no conflict for either address, but one address should carry one name. PEngine and MovieTexture are the
  CMovie-scoped spellings that FreeTextures already uses.
- 0x013B8A98: 13 invented spellings are bound. GPreviousSelected's element type is probable (see above).
- 0x005356A0 and 0x004B8C00 are also bound to `?base_OnReadFinished@@YIXPAUCActiveFile@@@Z` and `?BaseReadDone@@YIXPAUCActiveFile@@@Z`.
  These spellings come from the landed rows 0x005357E0 and 0x004BA590 and look like misattributions. They are listed in NAME_FIXES.tsv.

## Header debt
- engine/CMovie.h, CMorphingSprite.h and CTCBoastUI.h are flattened global structs (vptr as data, Dinkumware offsets), so the
  classes are modelled locally with the retail offsets.
- engine/CTCBase.h models the vptr as data, so CTCBase is modelled locally.
- engine/CClassFactory.h keeps ClassTypeInfo as opaque `unsigned char[0xc]`. The derived class reaches the vector through a
  cast accessor (layout-rename pattern).
- vland raised SHARED_HEADER_AVAILABLE for CIEngine, CObserver, CIVCountedPointeeBase, CQuestManager and CList. Each of those
  headers has its vptr as data and no virtuals, and CQuestManager.h has no nested CBoast. Virtual calls and nested types
  need local models.
- CObserver.h has size 0x18, but retail is 0x14 (the STLport set is 0xC).
- There are no headers for CThing, CThingManager, the NUISystem component chain (CComponent, CChangingStateComponent,
  CClickable, CDraggable, CSprite, CEditBox, CMesh, CScrollBar*), CTCQuestCompletionUI, CTCTavernGame or CMainGameComponent.
- Retail NUISystem member offsets are the Ego_r PDB offsets minus 0x30, and 8 less again for each Dinkumware list before the
  member. Retail CThing members from +0x88 are the Ego_r offsets minus 0x10.
