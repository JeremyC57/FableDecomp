# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xD | `?GetInstance@CManager@NUISystem@@SIPAV12@XZ` | 0x0058F022 | 0x0041E5F2 | reloc_bindings |
| +0x1E | `??_C@_0CA@LPMOEJHA@UI_SCROLLING_VIEWPORT_TEXT_AREA?$AA@` | 0x0058F033 | 0x01237550 | terminated literal payload |
| +0x26 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F03B | 0x0099EBF0 | reloc_bindings |
| +0x41 | `??0CWideString@@QAE@ABV0@@Z` | 0x0058F056 | 0x0099B720 | reloc_bindings |
| +0x53 | `?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ` | 0x0058F068 | 0x005CC772 | reloc_bindings |
| +0x5C | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0058F071 | 0x0099B8D0 | reloc_bindings |
| +0x64 | `??1CWideString@@QAE@XZ` | 0x0058F079 | 0x0099B510 | reloc_bindings |
| +0x85 | `?GFIntToWideString@@YI?AVCWideString@@J@Z` | 0x0058F09A | 0x00405F00 | reloc_bindings |
| +0x8E | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0058F0A3 | 0x0099B8D0 | reloc_bindings |
| +0x96 | `??1CWideString@@QAE@XZ` | 0x0058F0AB | 0x0099B510 | reloc_bindings |
| +0x9C | `??_C@_0BE@CGIODBDI@UI_ONE_OFF_SUBTITLE?$AA@` | 0x0058F0B1 | 0x0124E72C | terminated literal payload |
| +0xA4 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F0B9 | 0x0099EBF0 | reloc_bindings |
| +0xB1 | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F0C6 | 0x0041DB1D | reloc_bindings |
| +0xBA | `?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z` | 0x0058F0CF | 0x00429C6F | reloc_bindings |
| +0xC2 | `??1CCharString@@QAE@XZ` | 0x0058F0D7 | 0x0099EAE0 | reloc_bindings |
| +0xDB | `??_C@_0CJ@IGKMLEAL@UI_WEAPON_MENU_ENTRY_BRANCH_TEMP@` | 0x0058F0F0 | 0x01237208 | literal payload prefix |
| +0xE3 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F0F8 | 0x0099EBF0 | reloc_bindings |
| +0xEC | `??_C@_0BC@NNOGNHJ@PC_BUTTON_BASTARD?$AA@` | 0x0058F101 | 0x01236ED0 | terminated literal payload |
| +0xF1 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F106 | 0x0099EBF0 | reloc_bindings |
| +0x101 | `??_C@_00CNPNBAHC@?$AA@` | 0x0058F116 | 0x0122D70E | literal payload prefix |
| +0x107 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F11C | 0x0099EBF0 | reloc_bindings |
| +0x12A | `?CreateMenuEntryComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@MMABVCCharString@@MV5@MV5@@Z` | 0x0058F13F | 0x00428494 | reloc_bindings |
| +0x133 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F148 | 0x00429C15 | reloc_bindings |
| +0x13B | `??1CCharString@@QAE@XZ` | 0x0058F150 | 0x0099EAE0 | reloc_bindings |
| +0x168 | `?CreateMenuDesc@CTCInventory@@AAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@ABVCCharString@@@Z` | 0x0058F17D | 0x005820E7 | Both PDBs and landed menu description body |
| +0x18E | `??9CCharString@@QBE_NPBD@Z` | 0x0058F1A3 | 0x0099E960 | reloc_bindings |
| +0x19F | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F1B4 | 0x0041DB1D | reloc_bindings |
| +0x1A8 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F1BD | 0x00429C15 | reloc_bindings |
| +0x1BF | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F1D4 | 0x004291DE | reloc_bindings |
| +0x1D4 | `??_C@_0BJ@LFBNCFDC@UI_SELECTION_ARROW_RIGHT?$AA@` | 0x0058F1E9 | 0x01237150 | terminated literal payload |
| +0x1DB | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F1F0 | 0x0099EBF0 | reloc_bindings |
| +0x1EE | `??0CDeletion@NUISystem@@QAE@XZ` | 0x0058F203 | 0x0042BED4 | reloc_bindings |
| +0x204 | `??0CDeletion@NUISystem@@QAE@ABV01@@Z` | 0x0058F219 | 0x0042CD84 | reloc_bindings |
| +0x214 | `??1?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ` | 0x0058F229 | 0x0042ABCA | reloc_bindings |
| +0x22C | `??0?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE@XZ` | 0x0058F241 | 0x004DF86C | reloc_bindings |
| +0x255 | `??$_M_find@J@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBJJ@std@@@1@ABJ@Z` | 0x0058F26A | 0x00592F92 | reloc_bindings |
| +0x276 | `?insert_unique@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Nonconst_traits@U?$pair@$$CBJJ@std@@@2@@std@@_N@2@ABU?$pair@$$CBJJ@2@@Z` | 0x0058F28B | 0x004E539F | reloc_bindings |
| +0x296 | `??_C@_0M@HGIPBKAG@UI_SUB_LIST?$AA@` | 0x0058F2AB | 0x012303E8 | terminated literal payload |
| +0x29E | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F2B3 | 0x0099EBF0 | reloc_bindings |
| +0x2AD | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F2C2 | 0x0041DB1D | reloc_bindings |
| +0x2B6 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F2CB | 0x00429C15 | reloc_bindings |
| +0x2BE | `??1CCharString@@QAE@XZ` | 0x0058F2D3 | 0x0099EAE0 | reloc_bindings |
| +0x2C7 | `??_C@_0BB@EEGPNOOG@UI_BACK_FOR_LIST?$AA@` | 0x0058F2DC | 0x012302EC | terminated literal payload |
| +0x2CF | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F2E4 | 0x0099EBF0 | reloc_bindings |
| +0x2DC | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F2F1 | 0x0041DB1D | reloc_bindings |
| +0x2E5 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F2FA | 0x00429C15 | reloc_bindings |
| +0x2ED | `??1CCharString@@QAE@XZ` | 0x0058F302 | 0x0099EAE0 | reloc_bindings |
| +0x31D | `??_C@_0O@GFPMFDEM@UI_ITEM_TITLE?$AA@` | 0x0058F332 | 0x01251BCC | terminated literal payload |
| +0x325 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F33A | 0x0099EBF0 | reloc_bindings |
| +0x332 | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F347 | 0x0041DB1D | reloc_bindings |
| +0x33B | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F350 | 0x00429C15 | reloc_bindings |
| +0x343 | `??1CCharString@@QAE@XZ` | 0x0058F358 | 0x0099EAE0 | reloc_bindings |
| +0x359 | `??0CWideString@@QAE@ABV0@@Z` | 0x0058F36E | 0x0099B720 | reloc_bindings |
| +0x35E | `??_C@_15ELOONFKJ@?$AA?3?$AA?5?$AA?$AA@` | 0x0058F373 | 0x01236A50 | Exact terminated UTF-16 punctuation literal |
| +0x366 | `??YCWideString@@QAEABV0@PBG@Z` | 0x0058F37B | 0x0099B940 | reloc_bindings |
| +0x371 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0058F386 | 0x0099B8D0 | reloc_bindings |
| +0x38D | `?TITLE_NAME@?A0xe8439cb6@@3VCCharString@@A` | 0x0058F3A2 | 0x013B8B4C | Original anonymous static in both PDB initializers and the console inventory TU |
| +0x3BF | `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` | 0x0058F3D4 | 0x00410DE0 | reloc_bindings |
| +0x3DC | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0058F3F1 | 0x004365B0 | reloc_bindings |
| +0x400 | `??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z` | 0x0058F415 | 0x0040DCE0 | reloc_bindings |
| +0x410 | `??_C@_0BP@JIEFKECB@UI_LIST_ITEMS_ACTIONS_CANT_USE?$AA@` | 0x0058F425 | 0x01251BAC | terminated literal payload |
| +0x41A | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F42F | 0x0099EBF0 | reloc_bindings |
| +0x425 | `?GetInstance@CManager@NUISystem@@SIPAV12@XZ` | 0x0058F43A | 0x0041E5F2 | reloc_bindings |
| +0x42C | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F441 | 0x0041DB1D | reloc_bindings |
| +0x435 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F44A | 0x00429C15 | reloc_bindings |
| +0x455 | `??_C@_0CC@IODDAEJO@UI_MENU_ENTRY_ITEM_ACTION_NOT_US@` | 0x0058F46A | 0x01251B88 | literal payload prefix |
| +0x45D | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F472 | 0x0099EBF0 | reloc_bindings |
| +0x480 | `??_C@_0P@KCKKFKJB@UI_ACTION_ICON?$AA@` | 0x0058F495 | 0x01251B78 | terminated literal payload |
| +0x485 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F49A | 0x0099EBF0 | reloc_bindings |
| +0x49C | `??0CDeletion@NUISystem@@QAE@XZ` | 0x0058F4B1 | 0x0042BED4 | reloc_bindings |
| +0x4B2 | `??0CDeletion@NUISystem@@QAE@ABV01@@Z` | 0x0058F4C7 | 0x0042CD84 | reloc_bindings |
| +0x4C3 | `??1?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ` | 0x0058F4D8 | 0x0042ABCA | reloc_bindings |
| +0x4CC | `??_C@_0BB@EEGPNOOG@UI_BACK_FOR_LIST?$AA@` | 0x0058F4E1 | 0x012302EC | terminated literal payload |
| +0x4D4 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F4E9 | 0x0099EBF0 | reloc_bindings |
| +0x4E2 | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F4F7 | 0x0041DB1D | reloc_bindings |
| +0x4EB | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F500 | 0x00429C15 | reloc_bindings |
| +0x4F3 | `??1CCharString@@QAE@XZ` | 0x0058F508 | 0x0099EAE0 | reloc_bindings |
| +0x4F9 | `??_C@_0BD@EADPGCII@UI_SELECT_FOR_LIST?$AA@` | 0x0058F50E | 0x012302D8 | terminated literal payload |
| +0x501 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F516 | 0x0099EBF0 | reloc_bindings |
| +0x50F | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F524 | 0x0041DB1D | reloc_bindings |
| +0x518 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F52D | 0x00429C15 | reloc_bindings |
| +0x520 | `??1CCharString@@QAE@XZ` | 0x0058F535 | 0x0099EAE0 | reloc_bindings |
| +0x542 | `??_C@_0O@GFPMFDEM@UI_ITEM_TITLE?$AA@` | 0x0058F557 | 0x01251BCC | terminated literal payload |
| +0x54A | `??0CCharString@@QAE@PBDJ@Z` | 0x0058F55F | 0x0099EBF0 | reloc_bindings |
| +0x558 | `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` | 0x0058F56D | 0x0041DB1D | reloc_bindings |
| +0x561 | `??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z` | 0x0058F576 | 0x00429C15 | reloc_bindings |
| +0x569 | `??1CCharString@@QAE@XZ` | 0x0058F57E | 0x0099EAE0 | reloc_bindings |
| +0x586 | `?TITLE_NAME@?A0xe8439cb6@@3VCCharString@@A` | 0x0058F59B | 0x013B8B4C | Original anonymous static in both PDB initializers and the console inventory TU |
| +0x5AE | `?ContructItemLists@CTCInventory@@QAEXAAV?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z` | 0x0058F5C3 | 0x0058EF79 | Both PDBs and landed same-TU item list body |
| +0x5B6 | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F5CB | 0x004291DE | reloc_bindings |
| +0x5BE | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F5D3 | 0x004291DE | reloc_bindings |
| +0x5C6 | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F5DB | 0x004291DE | reloc_bindings |
| +0x5CE | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F5E3 | 0x004291DE | reloc_bindings |
| +0x5D6 | `??1CCharString@@QAE@XZ` | 0x0058F5EB | 0x0099EAE0 | reloc_bindings |
| +0x5DE | `??1CWideString@@QAE@XZ` | 0x0058F5F3 | 0x0099B510 | reloc_bindings |
| +0x5E6 | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F5FB | 0x004291DE | reloc_bindings |
| +0x5EE | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F603 | 0x004291DE | reloc_bindings |
| +0x5F6 | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F60B | 0x004291DE | reloc_bindings |
| +0x5FE | `??1?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE@XZ` | 0x0058F613 | 0x004DD2B1 | reloc_bindings |
| +0x606 | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F61B | 0x004291DE | reloc_bindings |
| +0x60E | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F623 | 0x004291DE | reloc_bindings |
| +0x616 | `??1CWideString@@QAE@XZ` | 0x0058F62B | 0x0099B510 | reloc_bindings |
| +0x61E | `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` | 0x0058F633 | 0x004291DE | reloc_bindings |
| +0x62A | `??_C@_0BG@LOPALFBL@UI_LIST_ITEMS_ACTIONS?$AA@` | 0x0058F63F | 0x01251B60 | terminated literal payload |

104 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
