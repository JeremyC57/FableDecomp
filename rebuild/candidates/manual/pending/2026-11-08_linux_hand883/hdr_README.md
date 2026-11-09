# hdr/ overlay -- chunk 883 (row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC)

Every change is an additive `#ifdef FABLE_RETAIL_*` opt-in. Without the macro, the default path is unchanged. The new
headers are new files, so nothing includes them unless a row asks for them. The parked source
`differ/006096e0.cpp` uses every opt-in listed here.

## New headers
| Header | What | Evidence |
|---|---|---|
| `engine/CEngineGraphic.h` | The def graphic reference, `pack(1)`, 14 bytes: BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type. It also declares the two FableWin.pdb constructors, `??0CEngineGraphic@@QAE@XZ` and `??0CEngineGraphic@@QAE@W4EEngineGraphicType@@KMM_N@Z` (declaration only). | Ego_r layout (struct_layouts_egor.tsv). With the constructors the class is not an aggregate, which matches retail's calls: the `__fastcall` GetEngineGraphic 0x005D96A0 takes its hidden result pointer in ecx (`lea ecx,[esp+10Ch]`, 0x0060B565), and the caller reads the result in place (`cmp dword ptr [eax],0`, 0x0060B571). |
| `engine/CAnimFilterBase.h` | The abstract anim filter: pure `operator()(const CAnimationEntry&) const` and `Clone() const`, and an inline virtual destructor. | vtable 0x0122FFCC = {purecall, purecall, sdtor 0x0041CCE4}. The row's temporary filter stores this vptr when it is destroyed (0x0060B460). |
| `engine/CAnimationSet.h` | `GetAnim(const CCharString&, ..., const CAnimFilterBase&)` 0x005DC340. Members: Anims (CVectorMap<unsigned long, CAnimationEntry>), DefaultFlags, DefaultTransitionTime, DefaultDelay, DefaultGroup. sizeof 0x20. | Ego_r layout and names. The call at 0x0060B447 passes `this` = appearance + 0x34. |
| `engine/CAppearanceDef.h` | `CAppearanceDef : CSubDefClassBase` with Graphic at +0x25 and Animation at +0x34, plus the inline `static CCharString __fastcall StatGetTypeName()` returning "CAppearanceDef". | Ego_r layout. RTTI `.?AVCAppearanceDef@@ : CSubDefClassBase`. `?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ` is in Ego_r and FableWin. The body is the landed GetSubDef<CAppearanceDef> 0x0042B0A2's. |

## Opt-ins in existing headers
| Header | Macro | What | Evidence |
|---|---|---|---|
| `engine/CAnimFilterNoGroup.h` | `FABLE_RETAIL_CANIMFILTER` | The real class, deriving from CAnimFilterBase: `operator()` 0x0041CD13, `Clone` 0x00429A2F, sizeof 4. The old flat struct stays in `#else`. | vtable 0x0122FFDC = {0x0041CD13, 0x00429A2F, sdtor 0x00429A45}. The temporary is built at 0x0060B43C. |
| `engine/CThingObjectDef.h` | `FABLE_RETAIL_CTHINGOBJECTDEF_GRAPHIC` | `CEngineGraphic Graphic; // +0x70` | `mov edx,[ecx+70h]` (BankIndex, 0x0060B3B5) and `cmp byte ptr [edx+7Dh],4` (Type, 0x0060B3E9). |
| `engine/CTCInventoryItem.h` | `FABLE_RETAIL_CTCINVENTORYITEM_UIDETAILS` | `static __fastcall` GetDetails 0x005D9630, GetEngineGraphic 0x005D96A0 and GetStartingRightHandedSet 0x005D9730, each returning by value. | Ego_r names. The calls at 0x0060B522, 0x0060B56C and 0x0060B3CE pass the index in edx and the result pointer in ecx. |
| `engine/CHeroLogBook.h` | `FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN` | CHeroLogBookEntry gets the inline `GetAbbreviatedName()` and `GetContent()`, and `friend class CTCInventoryMap;` for LogEntries. | Ego_r names. The inline CWideString copies are at 0x0060C445..0x0060C46B. |
| `engine/CTCInventoryMap.h` | `FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK` | CategoriesList (+0x144) becomes `std::map<ECategory, NUISystem::CComponent*>`. Adds public `ConstructLogbookListPC(CComponent*)` and protected `GetMenuEntryFromCategoryPC(ECategory, CComponent*)` (0x005FAC20). | Ego_r member and function names. The map operations are at 0x6166E0, 0x618BE0 and 0x615820. |
| `engine/NUISystem_CAction.h` | `FABLE_RETAIL_CACTION_INLINE_CTOR` | `CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) { }` | Expanded at 0x006097BE. It is called as the COMDAT 0x0042BE50 where the inline budget is used up (0x0060A97F). |
| `engine/CDefinitionManager.h` | `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_BODY` | The body of `GetDef<T>(long, CDefPointer<const T>*) const`, with retail's logic from the landed 0x0042B040: false when the index is not positive; otherwise the manager's counted def is assigned into `*out` and the temporary is released. | Retail calls this out of line here. With the body visible, VC7.1 releases the def handle without the null test and without the dead `Object = 0` store (0x0060B869..0x0060B87E). Declaration-only shows both. |
| `engine/CDefinitionManager.h` | `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY` | The body of `CDefClassBase::GetSubDef<T>`, with retail's logic from the landed 0x0042B0A2: the slot-14 `GetPSubDefInfo(T::StatGetTypeName())`, then `GetPDefFromGlobalIndex`, then `SetDynamicCast` into `*out`. It lives here because it needs CDefinitionManager complete, and it includes engine/CDefClassBase.h. | With the body visible, the appearance handle's release is `dec dword ptr [ecx+4] ; jne` (0x0060B489) instead of the load/dec/store shape. |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT` | STLport 4.6 `_M_create_node` builds the value with `_Construct(&p->_M_data, x)`. | Same code, but the extra inline layer makes the row's auto-inline budget run out at retail's point (scroll-action group 4). |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND` | STLport 4.6 `__stl_alloc_rebind`, and `_STLP_alloc_proxy::allocate` / `deallocate` going through it. | As above: with both opt-ins, group 4 calls 0x0042BE50, 0x0042AA29 and 0x00429B72 exactly as retail does. |
| `stlport/fable_map.h` | `FABLE_RETAIL_STLP_TREE_FIND_OUTOFLINE` | The non-const member-template `find` declaration, the twin of `FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE`. | The out-of-line call 0x00615820 at 0x0060C79D. |
| `stlport/fable_map.h` | `FABLE_RETAIL_STLP_MAP_COPY` | The `map(const map&)` declaration. | The out-of-line copy 0x00619C50 at 0x0060971C. |

Copies in this overlay: `stlport/fable_containers.h` and `stlport/fable_map.h` are whole copies of the repo files with the opt-ins
added. Three experimental opt-ins (`STLP_ALLOC_LAYER`, `STLP_LIST_CONVERT_ALLOCATOR`, `STLP_TREE_INCREMENT_TREEC`) were tried and
removed: none changed a byte.
