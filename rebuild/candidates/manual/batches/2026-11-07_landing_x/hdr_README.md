# Chunk 849 header overlay

Every edited file was copied fresh from rebuild/include and differs from it only by added lines: each change is an opt-in branch
(`#ifdef FABLE_RETAIL_<X>` ... `#else` <existing text, unchanged> `#endif`, or an added `#ifdef` block). No existing line was changed or
reflowed, and new declarations sit inside their `#ifdef`. A landed includer that does not define the new macros compiles the same text
as before. Checked at hand-back with `diff` against rebuild/include: every file still shows `-0` lines.
Includers rechecked: 40 landed rows read-only with this overlay (`vland.py --recheck-landed`, logs/recheck_siblings.log). Each of them
includes one of the edited headers, or defines an opt-in macro whose branch was wrapped (FABLE_RETAIL_STLP_LIST / _STLP_SET / _STLP_MAP /
_CTHING_GETTC_EXPANDED / _CTHINGSEARCHTOOLS_ALLTHINGSBODY / _CTCHEROSTATS). All 40 grade RELOCATION_MATCH PASS (SIBLINGS.tsv).

## Edited repo headers (opt-in only)

| header | macro | what | evidence | row |
|---|---|---|---|---|
| stlport/fable_containers.h | FABLE_RETAIL_STLP_VECTOR_SWAP | STLport `swap(_Tp&,_Tp&)` and an inline `vector::swap` (three std::swap calls) | retail expands `particle_list.swap(next_particle_list)` in place (0x00AE30C7..0x00AE30DC) | 0x00AE2E20 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_SORT2 | declares the two-iterator `sort(first,last)` | retail calls sort<pair<long,unsigned long>*> 0x00AE7DC0 out of line (/Gr fastcall) | 0x00AE2E20 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_BACK_INSERTER | STLport back_insert_iterator / back_inserter, and a declaration of `copy` as __fastcall | 0x0057EBDE -> 0x00581376 and 0x0057EC1C -> 0x005813A0 (result through a hidden pointer in ecx); the landed 0x007B3760 defines the same model locally | 0x0057E2F1 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE | STLport `distance` / `__distance` (const-reference arguments, the loop walks a copy of first) and `list::size()` as `size_type __result = distance(begin(), end()); return __result;`. This alternative is nested inside the existing FABLE_RETAIL_STLP_LIST size() line | 0x0057EBA3: head in eax, walk in ecx; the existing for-loop form swaps those two registers (8 lines) | 0x0057E2F1 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_SET_EMPTY | `_Rb_tree::empty()` and `set::empty()` (STLport `_M_node_count == 0`) | 0x005016E6 tests the local set's count in place | 0x00501450 |
| engine/CArray.h | FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH | `CArray::push_back` as the inline fast path (construct at finish), falling back to vector::push_back | Ego_r ?push_back@?$CArray@K@@QAEXABK@Z; retail calls _M_insert_overflow 0x00464ACE directly at the seed site 0x00AE2F36 | 0x00AE2E20 |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND | non-const LowerBound as an ordinary inline body in the debug donor's spelling (`count=size()`, `left=begin()`, count/2, Compare on the keys, `left=++mid`), plus the key/key overload of CKeyPairCompareLess. The const LowerBound stays the default noinline body | 0x00501645..0x00501687 expands it (the `cdq ; sub ; sar` halving, `cmp [esi+eax*8],0C2h`); 0x00501597 calls the const one, 0x004365B0. The overloads and the donor body come from the Codex pass 13 TPI read | 0x00501450 |
| engine/CThingFilter_HasTC.h | FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE | operator() declared only | ??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z out of line at 0x006A80A0 (landed). If the inline body is visible, VC7.1 hoists the filter temporary's store above the region loop | 0x00501450 |
| engine/CPersistContext.h | FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE | `IsSaving() const` declared only | 0x0057E8E0 / 0x0057EB91 call 0x004AEF50 (?IsSaving@CPersistContext@@QBE_NXZ, bound) | 0x0057E2F1 |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_SERIALISE (nested in FABLE_RETAIL_CTCHEROSTATS, a new `#elif` before the opaque `#else`) | typed members at the generated default offsets with Ego_r names (map / vector / CCharString / list), the nested MirroredCriminalStatus (Ego_r 68 bytes; ctor 0x0057A5DD), and the protected virtual OnSerialise. Its includes (fable_map.h, fable_string.h, CHeroMarriageInfo.h, EHeroTrainingStatus.h) come in only under the macro | struct_layouts_egor.tsv; the row's offsets (Morality [ebx+28h] .. Marriages [ebx+1D8h]); sizeof 0x1DC asserted | 0x0057E2F1 |
| engine/CTCInventory.h | FABLE_RETAIL_CTCINVENTORY | retail class: CTCInventoryBase + NHeroInformationScreens::CBase bases (RTTI), members from +0x134 as generated (the CComponent* members typed), the six PDB methods (OpenInventoryShortcut, DeleteMenuComponents 0x00587A41, OpenPCInventoryMenu 0x0058807D, OpenPCMapMenu 0x00589A30, OpenPCSkillsMenu 0x00587BFF, OpenPCStatusMenu 0x0058A763); sizeof 0x228, PTop_Level_List +0x218. The whole file is wrapped: retail branch, then `#else` with the generated text unchanged | RTTI; PDB names; jump table 0x0058EF31 | 0x0058E99F |
| engine/CMemoryFile.h | FABLE_RETAIL_CMEMORYFILE_DUMPTOFILE | `void DumpToFile();` | 0x009E9330 | 0x009CF3A0 |
| fable_retail_banks.h | FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS | GetDevHeaderFilePath 0x009A7E20 / GetRetailHeaderFilePath 0x009A7ED0 (const, CWideString from const CCharString&), and `NBankFileManager::GBankFileManager` 0x013CA79C | Ego_r dynamic initializer / atexit names for NBankFileManager::GBankFileManager | 0x009CF3A0 |
| engine/NUISystem_CYesNo.h | FABLE_RETAIL_CYESNO_CTOR | `CYesNo(long)` 0x0041CADC, Yes/No callbacks; sizeof 0x164 | 0x0041D21B allocation size and ctor call | 0x0041D21B |
| engine/NUISystem_{CFrontEndScreen,CFrontEndButton,CListArrow,CSwappingStateComponent,CTextContainer,CZoomingComponent,CSpellContainer,CDynamicList}.h | FABLE_RETAIL_<CLASS>_MEMBERS | Ego_r members 0x30 lower (+4 per vector), sizeof 0x16C / 0x1B4 / 0x190 / 0x170 / 0x16C / 0x184 / 0x17C / 0x180 | operator new sizes in 0x0041D21B | 0x0041D21B |

## New headers

- engine/CHeroStatsDef.h: Ego_r members and order on the packed 0x39-byte CParentDefClassBase, so the first member is at +0x3C and
  sizeof is 0x138. Evidence: MaximumStamina is read at +0x3C (0x0057E448), and the landed 0x0057DE14 reads RenownLossOnDeath at +0x54
  and MoneyLossOnDeath at +0x58.
- engine/EHeroTrainingStatus.h: the enum. Its enumerator spellings come from Codex's PDB read only, so they are single-source.
- engine/CTCDRegionExit.h: CTCDriverBase base, StatGetInterfaceType returning TCI_REGION_EXIT (100), GetNameOfRegionConnectingTo, and the
  Ego_r members (sizeof 0x38). PEntranceConnectedTo is typed at +0x2A only under FABLE_RETAIL_CINTELLIGENTPOINTER.
- engine/CTCActionUseScriptedHook.h: CTCBase base, TCI 194, the inline const GetPEntranceConnectedTo, and the Ego_r members (sizeof 0x50).
- engine/CThingFilter_HasTCOfLeafType.h: an empty template with the unary_function base. Its const operator()(CThing&) is declared only.
- engine/CPackedUIntArray.h, engine/CBankFileEntryUpdateData.h: the 0x009CF3A0 helpers (PackedInts / Size / Bits / Bias with
  operator[] const 0x00A629F0; the 32-byte entry update record with `friend class CBankFile`).
- engine/NUISystem_{CDraggableInto,CNavButton,COk,CControllerDisconnect,CSpellContainerList}.h: RTTI base, ctor(long) at 0x00558EC0 /
  0x00558B90 / 0x0041CB70 / 0x0041CBE4 / 0x00544B70, Ego_r members 0x30 lower, sizeof 0x170 / 0x194 / 0x160 / 0x160 / 0x1A8.

Not edited: engine/CBankFile.h (HOLD; 0x009CF3A0 models the retail class locally, see NOTES.md header debt) and rebuild_abi.h.
