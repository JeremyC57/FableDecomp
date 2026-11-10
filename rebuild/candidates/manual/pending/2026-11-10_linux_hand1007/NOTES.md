# chunk 1007 (CTCInventory, 16 rows) notes

Every grade ran with **`FABLE_HDR_OVERLAY=$A/hdr`** exported (through `p.sh`). The overlay adds only new nested opt-ins
(see `hdr/README.md`). Six rows match; eight are parked with their best source in `differ/`; two large rows were not attempted.

## Matched (src/ + test/)
| row | identity | grade | grades | lever |
|---|---|---|---|---|
| 0x00583E26 | `CTCInventory::OnItemSelected(long, bool&)` (vtable 0x01250DE4 slot 33) | RELOCATION_MATCH | 3 | the visible `IS_THING_ALIVE<CThing*>` body under `auto_inline(off)`; case 1 as a nested if with the return-to-inventory block written in BOTH elses (retail duplicates it) |
| 0x00587BFF | `CTCInventory::OpenPCSkillsMenu()` | RELOCATION_MATCH | 1 | the landed OpenPCOptionsMenu 0x0058AF10 shared-header spelling |
| 0x0058A763 | `CTCInventory::OpenPCStatusMenu()` | RELOCATION_MATCH | 1 | same spelling, first try |
| 0x0058A0C7 | `CTCInventory::ConstructItemEntry(long, long, CList*, CComponent*, CCountedPointer<CComponent>&)` | RELOCATION_MATCH | 7 | full shared-header rewrite in the landed FinalConstructWeaponItemEntries 0x005C3E91 spelling; visible STLport `map::operator[]` (FABLE_RETAIL_STLP_MAP) with `map[k] = ptr.Get()` |
| 0x0058E208 | `CTCInventory::ConstructItemEntryPC(...)` same params | RELOCATION_MATCH | 4 | port of 0x0058A0C7; `CSprite* sprite = static_cast<CSprite*>(icon->FindChild(...))` and `CSprite* fadedSprite = static_cast<CSprite*>(faded.Get())` cached as named locals |

0x0058A0C7 dropped the prior's local model entirely: its invented `CActionParamList` stand-in, `CActionParamLong`, and the
descriptive virtual names `AddChildAtEnd` / `AddSelectedAction` / `AddFocusAction` / `AddUnfocusAction` / `SetUserValue` /
`CList::AddEntry` are replaced by the shared `std::list<CActionParam*>`, `CActionParamSLONG` (vtable 0x012366F4) and the
PDB / rtti-vtable names AddChild (59), AddActionOnSelected (67), CMenuEntry AddActionOnUnselected (152) / AddActionOnVisible (153) /
AddThingActionParam (150), CList AddScrollingChildAndSetOffsets (152). The action types are the shared enumerators
UI_ACTION_ADD_MESH_CHILD 2000 / LOAD_ITEM_DESC 2003 / LOAD_ITEM_MENU_ENTRY_NAME 2006 / REMOVE_MESH_CHILD 2015 / UNLOAD_ITEM_DESC 2018.

## Parked (differ/)
| row | identity | grade | grades this pass | residue / what it needs next |
|---|---|---|---|---|
| 0x00584B86 | `CTCInventory::AssignThingPC(long, unsigned long, CComponent*)` | DIFFER(854v854), 60 difflines | 1 | the ability / item parameter as a named local in an inner block fixed the frame (0x34). Left: frame-slot colouring, the use parameter in the dead `thingID` slot [ebp+8], GetDef's arguments pushed after PeekWorld in retail. Still a local model: needs the shared CTCInventory / CDeletion / CMenuEntry / CDefPointer spelling (callees: SetThingAsQuickAccessItem 0x00584157, CTCBase::PeekWorld 0x00686A80, CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040, CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2, CCountedPointer<CAction>::Reset 0x005791E5, CAction::AddParam(CActionParam*) 0x0053FDB0, slots 62 SetDeletion, 74 AddActionOnDropped, 75 AddActionOnDroppedNowhere, 60 AddChildDynamically, CSprite 145 SetGraphicIndex) |
| 0x005850FD | `CTCInventory::GetStyleCardsListUIComponent(long, long, long, CComponent*)` (returns void) | DIFFER(1660v1678) | 1 | retail puts the hair-card arm out of line (after the def releases / after the epilogue) and jumps back; flipping the arms is worse (1656) |
| 0x00589402 | `CTCInventory::GetStyleCardsListUIComponentPC(...)` | DIFFER(1581v1582) | 0 | same block layout as 0x005850FD |
| 0x00589A30 | `CTCInventory::OpenPCMapMenu()` | DIFFER(549v549) | 2 | shared-header rewrite; one reload pair after the inline `repe cmpsb` (retail reloads `this` before `i`). See c2prio notes in the differ header |
| 0x0058F016 | `CTCInventory::ConstructItemCategoryList(...)` | DIFFER(1588v1587) | 1 | merge-loop start pointer in eax v retail edi (temp #51 coloured before the item pointer) |
| 0x0058F649 | `CTCInventory::OpenPCInventory()` | DIFFER(883v883) | 5 | shared-header rewrite; frame-slot colouring (retail gives the manager [ebp-4]) and one scheduling swap |
| 0x00590235 | `CTCInventory::GetItemsListUIComponent(CComponent*)` | DIFFER(1393v1393) | 1 | prior kept (127 difflines); the shared-header rewrite `probe/00590235_shared_lastgraded.cpp` (CONSOLEMENUS opt-in) is the same length, 308 difflines, a pure esi/edi/ebx permutation |
| 0x00590F0B | `CTCInventory::GetItemsListUIComponentPC(CComponent*)` | DIFFER(2831v2907) | 0 | prior (hand867), see its Residue / Next-idea lines |
| 0x00591B7F | `CTCInventory::UpdateContextSensitiveItems()` | DIFFER(729v735) | 0 | prior; escape-analysis residue |

## Not attempted
- 0x0058807D `CTCInventory::OpenPCInventoryMenu()` (4997 bytes) -- no draft.
- 0x0058F9BC `CTCInventory::OpenPCPhotojournal()` (2012 bytes, /GS) -- the screenshot scan calls about 15 unidentified string and
  file-find helpers (0x0040D2A0, 0x00406350, 0x0041A600, 0x0099BE70, 0x0099BF30, 0x00594803, 0x00999760 / 0x00999850,
  0x009992A0, 0x00594A08, 0x0099B800, 0x004294E2, 0x0099B720, 0x00997620). These need naming first. Strings 0x01251E1C,
  0x01251E00, 0x01237028, 0x01251DE8, 0x01251DC4, 0x01251DA0.

## Identities proven this pass (also in LABELS.tsv / NAME_FIXES.tsv)
- 0x005D9710 = `CTCInventoryItem::GetUseButtonActionType(long)` static __fastcall (the landed `LookUpInventoryItemDef` is wrong).
- 0x0060EEB0 = `CTCInventoryMap::ConstructPCMapList(bool, bool*)` (landed `BuildMapMenu` is wrong).
- 0x005BC6ED = `NInventory::CTCInventoryBase::CaptureBackBufferToTexture()` (landed `PrepareScreen` is descriptive).
- 0x00589402 = `GetStyleCardsListUIComponentPC` (returns void); 0x0058A724 = `ConstructStyleCardsScreen` (jmp to 0x005850FD).
- Carried-action-use slots: 28 `OnActionUse(CThing&)`, 30 `IsUsableWhileThingPerformingAction() const`. The landed 0x0058400A
  comment names slot 31 "OnActionUse", which looks wrong (not re-proven here).
- CTCInventoryBase slot 36 = OnItemHighlighted (a prior called it HighlightItem); NUISystem::CManager slot 21 = SetInventory and
  slot 26 = SetPlayerThing; CIEngine slot 76 = Enable3DSceneRendering(bool) (Ego_r `?Enable3DSceneRendering@CEngine@@UAEX_N@Z`;
  landed local models 0x005AE86B / 0x0056FAD7 call it "SetEnabled"); PSoundManager slot 46 = SetGroupVolume (landed 0x005AE86B
  calls it "RefreshCategory"). These are virtual slots, so they are listed here and not in NAME_FIXES.tsv.
- 0x007C8FD0 = `CTCFireballSpell::CancelMove()`.

## Header debt
- Overlay opt-ins to merge: see hdr/README.md.
- The differ priors 0x005850FD / 0x00589402 / 0x0058F016 / 0x00590235 / 0x00590F0B / 0x00591B7F / 0x00584B86 are still local
  models with descriptive slot names. They need the shared-header spelling, which is what made 0x0058A0C7 match.
