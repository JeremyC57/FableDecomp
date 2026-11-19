# NF94 header overlay (round 8)

Every overlay is a copy of the CURRENT repo header (base copy in ../orig/) with `#ifdef` opt-ins only. Without the opt-in macros each header is
token-identical to the repo copy (the added lines are all inside `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` blocks). pin_asserts.py was run after
every edit.

| header | opt-in | added | rows using it | pin_asserts |
|---|---|---|---|---|
| engine/CCategory.h | FABLE_RETAIL_CCATEGORY_REMOVEITEM | `void RemoveItem(long itemDefIndex, long count);` (0x005BF221, Ego_r + FableWin ?RemoveItem@CCategory@NInventory@@QAEXJJ@Z), after the ADDITEM block | 0x0059C747 | pinned 0 (the header's asserts are all `_AT` or after no moved line) |
| engine/NHeroInformationScreens_CManager.h | FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_INITIALISESCROLLBARINDICES | `void InitialiseScrollBarIndices(long count);` (0x0066EA85, Ego_r + FableWin), after the DEREGISTERALL block | 0x0066EA85 | pinned 0 |
| engine/CTCTavernGameCardBlackjack.h | FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_GETHANDVALUE_INLINE | the inline body of GetHandValue (0x007DA230), after the class's last FABLE_STATIC_ASSERT_AT; needs CARDSMAINGAME or SCORETEXT | 0x007DA230 (scaffolding owner), 0x007DA900 | pinned 0 (all asserts above the block) |
| engine/CMainGameComponent.h | FABLE_RETAIL_CMAINGAMECOMPONENT_PPLAYERINTERFACE | `#include "boost/scoped_ptr.h"` + `class CGamePlayerInterface;` before `class CMainGameComponentInit;`; PPlayerInterface typed `boost::scoped_ptr<CGamePlayerInterface>` (+0x20, same 4 bytes) in an #ifdef/#else around the opaque line; needs FABLE_RETAIL_SCOPED_PTR | 0x00416359 | pinned 0 |
| engine/CThingMapManager.h | FABLE_RETAIL_CTHINGMAPMANAGER_GETALLACTIVETHINGMAPSINAREA | `#include "fable_containers.h"`, `class C2DBoxI;`, `void GetAllActiveThingMapsInArea(const C2DBoxI& area, std::vector<CThingMap*>& maps) const; // 0x006384B0` (Ego_r + FableWin) | 0x0049C6F0 | pinned 0 |
| engine/CGamePlayerInterface.h | FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSITEMBUTTONS_TYPED | QuickAccessItemButtons typed `std::vector<CCountedPointer<const CInputTypeBase> >` (same 0x10 bytes) on the default branch, with its includes | 0x00445950, 0x005819F2, 0x00581FD1, 0x00590D32 | pinned 0 |
| engine/CGamePlayerInterface.h | FABLE_RETAIL_CGAMEPLAYERINTERFACE_CONTEXTSENSITIVEBUTTONS | `long GetNumberOfContextSensitiveItemButtons() const; // 0x00445960` (Ego_r + FableWin) | 0x00590D32 | pinned 0 |

## Header users
- engine/CCategory.h: 49 landed includers (grep of rebuild/src/compiled + rebuild/include) are not affected (no landed row defines FABLE_RETAIL_CCATEGORY_REMOVEITEM; the default path is unchanged).
- engine/NHeroInformationScreens_CManager.h: 24 landed includers; none defines the new macro, so none changes.
- engine/CTCTavernGameCardBlackjack.h: 7 landed includers; none defines GETHANDVALUE_INLINE.
- engine/CMainGameComponent.h: 380 landed includers; none defines PPLAYERINTERFACE (the 3 rows I moved onto it use only GETPPLAYERINTERFACE).
- engine/CThingMapManager.h: 11 landed includers; none defines GETALLACTIVETHINGMAPSINAREA.
- engine/CGamePlayerInterface.h: 43 landed includers; none defines QUICKACCESSITEMBUTTONS_TYPED or CONTEXTSENSITIVEBUTTONS (cpp -P of the overlay and the base copy without the macros: identical output).
