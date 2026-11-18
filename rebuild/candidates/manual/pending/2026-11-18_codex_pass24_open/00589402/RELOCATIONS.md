# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xD | `?GetInstance@CManager@NUISystem@@SIPAV12@XZ` | 0x0058940E | 0x0041E5F2 | reloc_bindings |
| +0x1C | `??_C@_0M@CJFIGKEH@PC_UI_FRAME?$AA@` | 0x0058941D | 0x0124EFA8 | terminated literal payload |
| +0x21 | `??0CCharString@@QAE@PBDJ@Z` | 0x00589422 | 0x0099EBF0 | reloc_bindings |
| +0x5B | `?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z` | 0x0058945C | 0x005BF482 | reloc_bindings |
| +0x67 | `?IsBook@CTCInventoryItem@@SI_NJ@Z` | 0x00589468 | 0x005D9970 | reloc_bindings |
| +0x74 | `?erase@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEPAU?$pair@JJ@2@PAU32@@Z` | 0x00589475 | 0x00593139 | reloc_bindings |
| +0xA2 | `?IsExpression@CTCInventoryItem@@SI_NJ@Z` | 0x005894A3 | 0x005D9830 | reloc_bindings |
| +0xB5 | `?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z` | 0x005894B6 | 0x005D96F0 | reloc_bindings |
| +0xDD | `?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z` | 0x005894DE | 0x005D9A10 | reloc_bindings |
| +0xEB | `?GetTattooCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCTattooDef@@@@J@Z` | 0x005894EC | 0x005D9AA0 | reloc_bindings |
| +0x101 | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` | 0x00589561 | 0x006869D0 | reloc_bindings |
| +0x108 | `?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z` | 0x00589568 | 0x009AD9E0 | reloc_bindings |
| +0x11F | `??$GetSubDef@VCAppearanceModifierDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@Z` | 0x0058957B | 0x0057FF92 | reloc_bindings |
| +0x149 | `?push_back@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEXABU?$pair@JJ@2@@Z` | 0x005895A5 | 0x005947B6 | reloc_bindings |
| +0x181 | `?IsInvalidCustomTattoo@CTattooDef@@QBE_NXZ` | 0x00589503 | 0x00581765 | First category tattoo validity call |
| +0x193 | `?push_back@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEXABU?$pair@JJ@2@@Z` | 0x00589515 | 0x005947B6 | First category tattoo vector insertion |
| +0x1DE | `?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z` | 0x005895E2 | 0x005D9A10 | reloc_bindings (nearby instruction) |
| +0x1EC | `?GetTattooCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCTattooDef@@@@J@Z` | 0x005895F0 | 0x005D9AA0 | reloc_bindings (nearby instruction) |
| +0x202 | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` | 0x005899B4 | 0x006869D0 | Native default category obtains the game definition manager |
| +0x209 | `?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z` | 0x005899BB | 0x009AD9E0 | Native default category fetches the hair object definition |
| +0x21C | `??$GetSubDef@VCAppearanceModifierDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@Z` | 0x005899D2 | 0x0057FF92 | Native default category calls the same-TU appearance modifier specialization |
| +0x246 | `?push_back@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEXABU?$pair@JJ@2@@Z` | 0x005899FC | 0x005947B6 | Native default category inserts the classified appearance modifier |
| +0x27A | `?IsInvalidCustomTattoo@CTattooDef@@QBE_NXZ` | 0x00589607 | 0x00581765 | Default category tattoo validity call |
| +0x28C | `?push_back@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEXABU?$pair@JJ@2@@Z` | 0x00589619 | 0x005947B6 | Default category tattoo vector insertion |
| +0x2E0 | `??0CWideString@@QAE@XZ` | 0x0058966D | 0x0099AED0 | reloc_bindings |
| +0x2EC | `??_C@_0BH@KIKGPLH@PC_MENU_ENTRY_HAIRCUTS?$AA@` | 0x00589679 | 0x01250DA8 | terminated literal payload |
| +0x2F1 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058967E | 0x0099EBF0 | reloc_bindings |
| +0x2FD | `??_C@_0BC@DEDJBLGC@PC_HAIRCUTS_TITLE?$AA@` | 0x0058968A | 0x01250D94 | terminated literal payload |
| +0x307 | `??0CCharString@@QAE@PBDJ@Z` | 0x00589694 | 0x0099EBF0 | reloc_bindings |
| +0x31C | `?ConstructStyleEntryPC@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0ABVCCharString@@0@Z` | 0x005896A9 | 0x0058315B | Both PDBs and landed same-TU style-entry body |
| +0x324 | `??1CCharString@@QAE@XZ` | 0x005896B1 | 0x0099EAE0 | reloc_bindings |
| +0x330 | `??_C@_0BD@CFFCMKFG@PC_HAIRCUTS_BUTTON?$AA@` | 0x005896BD | 0x01250D80 | terminated literal payload |
| +0x335 | `??0CCharString@@QAE@PBDJ@Z` | 0x005896C2 | 0x0099EBF0 | reloc_bindings |
| +0x348 | `??_C@_0BB@IMKBABAL@PC_TEXT_HAIRCUTS?$AA@` | 0x005896D5 | 0x01250D6C | terminated literal payload |
| +0x34D | `??0CCharString@@QAE@PBDJ@Z` | 0x005896DA | 0x0099EBF0 | reloc_bindings |
| +0x367 | `??4CWideString@@QAEABV0@ABV0@@Z` | 0x005896F4 | 0x0099B7D0 | reloc_bindings |
| +0x36F | `?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ` | 0x005896FC | 0x005CC772 | reloc_bindings |
| +0x378 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x00589705 | 0x0099B8D0 | reloc_bindings |
| +0x380 | `??1CWideString@@QAE@XZ` | 0x0058970D | 0x0099B510 | reloc_bindings |
| +0x38B | `?GFIntToWideString@@YI?AVCWideString@@J@Z` | 0x00589718 | 0x00405F00 | reloc_bindings |
| +0x394 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x00589721 | 0x0099B8D0 | reloc_bindings |
| +0x39C | `??1CWideString@@QAE@XZ` | 0x00589729 | 0x0099B510 | reloc_bindings |
| +0x3BB | `??_C@_0BF@GMIPIGFH@PC_HAIRCUTS_SUBTITLE?$AA@` | 0x00589748 | 0x01250AF8 | terminated literal payload |
| +0x3C0 | `??0CCharString@@QAE@PBDJ@Z` | 0x0058974D | 0x0099EBF0 | reloc_bindings |
| +0x3E8 | `??_C@_0BK@OGGFFEED@PC_MENU_ENTRY_FACIAL_HAIR?$AA@` | 0x00589775 | 0x01250D50 | terminated literal payload |
| +0x3ED | `??0CCharString@@QAE@PBDJ@Z` | 0x0058977A | 0x0099EBF0 | reloc_bindings |
| +0x3F9 | `??_C@_0BF@DJHLBFPO@PC_FACIAL_HAIR_TITLE?$AA@` | 0x00589786 | 0x01250D38 | terminated literal payload |
| +0x403 | `??0CCharString@@QAE@PBDJ@Z` | 0x00589790 | 0x0099EBF0 | reloc_bindings |
| +0x418 | `?ConstructStyleEntryPC@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0ABVCCharString@@0@Z` | 0x005897A5 | 0x0058315B | Both PDBs and landed same-TU style-entry body |
| +0x420 | `??1CCharString@@QAE@XZ` | 0x005897AD | 0x0099EAE0 | reloc_bindings |
| +0x42C | `??_C@_0BG@NMOGFHDH@PC_FACIAL_HAIR_BUTTON?$AA@` | 0x005897B9 | 0x01250D20 | terminated literal payload |
| +0x431 | `??0CCharString@@QAE@PBDJ@Z` | 0x005897BE | 0x0099EBF0 | reloc_bindings |
| +0x442 | `??_C@_0BE@HNODOMNI@PC_TEXT_FACIAL_HAIR?$AA@` | 0x005897CF | 0x01250D0C | terminated literal payload |
| +0x44A | `??0CCharString@@QAE@PBDJ@Z` | 0x005897D7 | 0x0099EBF0 | reloc_bindings |
| +0x465 | `??4CWideString@@QAEABV0@ABV0@@Z` | 0x005897F2 | 0x0099B7D0 | reloc_bindings |
| +0x46D | `?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ` | 0x005897FA | 0x005CC772 | reloc_bindings |
| +0x476 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x00589803 | 0x0099B8D0 | reloc_bindings |
| +0x47E | `??1CWideString@@QAE@XZ` | 0x0058980B | 0x0099B510 | reloc_bindings |
| +0x489 | `?GFIntToWideString@@YI?AVCWideString@@J@Z` | 0x00589816 | 0x00405F00 | reloc_bindings |
| +0x492 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0058981F | 0x0099B8D0 | reloc_bindings |
| +0x49A | `??1CWideString@@QAE@XZ` | 0x00589827 | 0x0099B510 | reloc_bindings |
| +0x4B6 | `??_C@_0BI@JONHMKPM@PC_FACIAL_HAIR_SUBTITLE?$AA@` | 0x00589843 | 0x01250AB4 | terminated literal payload |
| +0x4BB | `??0CCharString@@QAE@PBDJ@Z` | 0x00589848 | 0x0099EBF0 | reloc_bindings |
| +0x4E3 | `??_C@_0BG@HHJJFPJI@PC_MENU_ENTRY_TATTOOS?$AA@` | 0x00589870 | 0x01250CF4 | terminated literal payload |
| +0x4E8 | `??0CCharString@@QAE@PBDJ@Z` | 0x00589875 | 0x0099EBF0 | reloc_bindings |
| +0x4F4 | `??_C@_0BB@BKLFIOFJ@PC_TATTOOS_TITLE?$AA@` | 0x00589881 | 0x01250CE0 | terminated literal payload |
| +0x4FE | `??0CCharString@@QAE@PBDJ@Z` | 0x0058988B | 0x0099EBF0 | reloc_bindings |
| +0x513 | `?ConstructStyleEntryPC@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0ABVCCharString@@0@Z` | 0x005898A0 | 0x0058315B | Both PDBs and landed same-TU style-entry body |
| +0x51B | `??1CCharString@@QAE@XZ` | 0x005898A8 | 0x0099EAE0 | reloc_bindings |
| +0x527 | `??_C@_0BC@JEHHKPOH@PC_TATTOOS_BUTTON?$AA@` | 0x005898B4 | 0x01250CCC | terminated literal payload |
| +0x52C | `??0CCharString@@QAE@PBDJ@Z` | 0x005898B9 | 0x0099EBF0 | reloc_bindings |
| +0x53D | `??_C@_0BA@IPCFDHEI@PC_TEXT_TATTOOS?$AA@` | 0x005898CA | 0x01250CBC | terminated literal payload |
| +0x545 | `??0CCharString@@QAE@PBDJ@Z` | 0x005898D2 | 0x0099EBF0 | reloc_bindings |
| +0x560 | `??4CWideString@@QAEABV0@ABV0@@Z` | 0x005898ED | 0x0099B7D0 | reloc_bindings |
| +0x568 | `?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ` | 0x005898F5 | 0x005CC772 | reloc_bindings |
| +0x571 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x005898FE | 0x0099B8D0 | reloc_bindings |
| +0x579 | `??1CWideString@@QAE@XZ` | 0x00589906 | 0x0099B510 | reloc_bindings |
| +0x584 | `?GFIntToWideString@@YI?AVCWideString@@J@Z` | 0x00589911 | 0x00405F00 | reloc_bindings |
| +0x58D | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0058991A | 0x0099B8D0 | reloc_bindings |
| +0x595 | `??1CWideString@@QAE@XZ` | 0x00589922 | 0x0099B510 | reloc_bindings |
| +0x5B1 | `??_C@_0BE@KMIJDNFM@PC_TATTOOS_SUBTITLE?$AA@` | 0x0058993E | 0x01250A7C | terminated literal payload |
| +0x5B6 | `??0CCharString@@QAE@PBDJ@Z` | 0x00589943 | 0x0099EBF0 | reloc_bindings |
| +0x5D9 | `??1CWideString@@QAE@XZ` | 0x00589966 | 0x0099B510 | reloc_bindings |
| +0x5E6 | `_free` | 0x00589973 | 0x00BFEA14 | reloc_bindings |
| +0x5F4 | `_free` | 0x00589981 | 0x00BFEA14 | reloc_bindings |
| +0x602 | `_free` | 0x0058998F | 0x00BFEA14 | reloc_bindings |
| +0x610 | `_free` | 0x0058999D | 0x00BFEA14 | reloc_bindings |

87 verified operand bindings.
