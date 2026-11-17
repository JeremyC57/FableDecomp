# NF76 header overlay (2026-11-17)

Every header here is a copy of the repo header at the moment I first changed it (base copies in `../orig/`), with `#ifdef` opt-ins
only. Without the opt-in macro each file preprocesses to the same tokens as the repo header (the only non-additive edits wrap an
existing body in `#ifdef X <declaration> #else <body unchanged> #endif`, or turn `#ifdef A` into `#if defined(X) ... #elif
defined(A)`). After each edit I ran `pin_asserts.py rebuild/include hdr <header>`; the counts are below (a moved
`FABLE_STATIC_ASSERT` is respelled `FABLE_STATIC_ASSERT_AT(<old line>, ...)`).

| header | opt-ins added | rows | pinned |
|---|---|---|---|
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_LIST_SIZE_DECL`: list::size() declared only (`#if defined(...SIZE_DECL)` / `#elif defined(FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE)` in place of the old `#ifdef`) | 0x006D53C0 | 0 |
| engine/GFRandom.h | `FABLE_RETAIL_GFRANDOM_OUTOFLINE`: GFRandom declared only (`#ifdef` decl `#else` the inline body unchanged `#endif`) | 0x006D53C0 | 0 |
| engine/CTCSinglePersonOccupiable.h | `FABLE_RETAIL_CTCSINGLEPERSONOCCUPIABLE_ISOCCUPIED`: IsOccupied() const declaration (0x0087C3F0) + inline StatGetInterfaceType (172) | 0x00970CD0 | 0 |
| engine/CANavQuadTreeNode.h | `FABLE_RETAIL_CANAVQUADTREENODE_GENERATOR_CTOR`: **adds the base class** `: public CBaseClassNonCopyable` (RTTI; the class line is split over `#ifdef` lines, same tokens without the macro) and includes engine/CBaseClassNonCopyable.h, plus the inline Ego_r (C2DVector, E, E) ctor | 0x00A79930 | 0 |
| engine/CNavNavigableLeafNode.h | `FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_GENERATOR_CTOR`: the inline (C2DVector, E, E, E) ctor, **and the inline CSmallVector default ctor in the template's non-ITERATORS branch (gated by this CNavNavigableLeafNode macro)** | 0x00A79930 | 0 |
| engine/CNavSwitchableLeafNode.h | `FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_GENERATOR_CTOR`: the generator ctor declaration | 0x00A79930 | 7 |
| engine/CBankFile.h | `FABLE_RETAIL_CBANKFILE_PEEKENTRYUPDATEDATA` (needs ADDENTRYMULTIPLEFILES): FableWin inline PeekEntryUpdateData | 0x00B41FA0, 0x00B42530 | 0 |
| engine/CBankFileEntryUpdateData.h | `FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_READINFO`: public inline GetInfoSize() + ReadInfo declaration | 0x00B41FA0, 0x00B42530 | 0 |
| engine/CEngineWorldMap.h | `FABLE_RETAIL_CENGINEWORLDMAP_LOADWATERDATA`: private LoadWaterData declaration | 0x00B41FA0 | 0 |
| engine/CEngineWaterRenderer.h | `FABLE_RETAIL_CENGINEWATERRENDERER_LOADNONPATCHDATA_DECL`: LoadNonPatchData declaration on the flattened default layout | 0x00B41FA0 | 0 |
| engine/CEngineMap.h | `FABLE_RETAIL_CENGINEMAP_CLOSESTATICMAP`: CloseStaticMap declaration | 0x00B42530 | 0 |
| engine/CEngineStaticMapFileBlockManager.h | `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_SETBANKFILEENTRY`: SetBankFileEntry declaration | 0x00B42530 | 1 |
| engine/CLocalDetailCacheMap.h | `FABLE_RETAIL_CLOCALDETAILCACHEMAP_OPENSTATICMAP`: OpenStaticMap declaration | 0x00B42530 | 8 |
| engine/CCountedPointer.h | `FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY`: copy ctor declared only (`#ifdef` decl `#else` the body unchanged `#endif`) | 0x00B42530 | 0 |
| engine/CTCHeroStats.h | `FABLE_RETAIL_CTCHEROSTATS_SPENTEXPERIENCE` (needs the SERIALISE member set): public inline `AddSpentExperience(long)` (TotalSpentExperience += amount) and `GetExperienceSpentOn()` (Ego_r.pdb names; expanded in the row) | 0x005778CA | 0 |
| engine/CTCInventoryAbilities.h | `FABLE_RETAIL_CTCINVENTORYABILITIES_UPGRADEABILITY`: `UpgradeAbility(EHeroAbility)` declaration (0x005EEEE6) | 0x005778CA | 0 |
| stlport/fable_deque.h | `FABLE_RETAIL_STLP_STACK_TOP_POP`: deque `back()` / `pop_back()` declared only (when the PUSH_BACK branch is off) and the STLport `stack<T, deque<T> >` adaptor with inline `top()` / `pop()` (STLport 4.6; back / pop_back are the Ego_r.pdb / FableWin.pdb names of 0x0059B039 / 0x0059AEE5, see NAME_FIXES) | 0x00599E3F | 0 |
| engine/CTBaseSingleton.h | `FABLE_RETAIL_CTBASESINGLETON_ISALLOCATED`: static inline `IsAllocated()` (`PClass.Data != 0`); `FABLE_RETAIL_CTBASESINGLETON_GET_DECL`: `Get()` declared only (`#ifdef` decl `#else` the inline body unchanged `#endif`) | 0x00599E3F | 0 |
| engine/CVirtualKeyboard.h | `FABLE_RETAIL_CVIRTUALKEYBOARD_INLINE_STATE`: inline `IsDone()` / `WasCanceled()` | 0x00599E3F | 10 |
| engine/CFrontEndManager.h | `FABLE_RETAIL_CFRONTENDMANAGER_UPDATE` now also declared in the DESTRUCTOR (full-layout) branch: `Update(float)` 0x00599E3F; `FABLE_RETAIL_CFRONTENDMANAGER_VIRTUALKEYBOARD`: `LaunchVirtualKeyboard()` 0x00596917, `GotoNextScreen(CComponent*, bool)` 0x00596763, protected `CreateNewProfile(CWideString&)` 0x0059697A | 0x00599E3F | 0 |
| engine/CNewFrontendGameComponent.h | `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME`: inline `StartGame()` (`SetQuit(); m_bCreateGame = true;`, body reconstructed from the retail bytes, see NOTES) and `LoadGame(CWideString&)` declaration (0x00594F36) | 0x00599E3F | 0 |
| engine/NUISystem_CAction.h | `FABLE_RETAIL_CACTION_ADDPARAM_INLINE`: `AddParam(CActionParam*)` inline (`UIActionParams.push_back(param)`, the 0x0053FDB0 body; `#ifdef` inline `#else` the declaration unchanged `#endif`) | 0x00549B20 | 0 (its assert is already `_AT`) |

All 22 base copies in `../orig/` were compared with `rebuild/include` at the end of the run (cmp: all identical), so no re-pin was needed.
