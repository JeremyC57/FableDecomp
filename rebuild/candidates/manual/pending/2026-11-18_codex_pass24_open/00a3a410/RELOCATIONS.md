# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x1B | `?ToCharString@CWideString@@QBE?AVCCharString@@XZ` | 0x00A3A42A | 0x0099B2C0 | reloc_bindings |
| +0x23 | `?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z` | 0x00A3A432 | 0x009CCDC0 | reloc_bindings |
| +0x2E | `??1CCharString@@QAE@XZ` | 0x00A3A43D | 0x0099EAE0 | reloc_bindings |
| +0x48 | `??2@YAPAXI@Z` | 0x00A3A453 | 0x00BFEA1A | reloc_bindings |
| +0x55 | `??_7CWADBankStateBlock@@6B@` | 0x00A3A45F | 0x0129CFD0 | CWADBankStateBlock retail RTTI and its four virtual entries. Native allocation initializes this vptr at 0x00A3A45F. |
| +0x63 | `??0?$CCountedPointer@VCWADBankStateBlock@@@@QAE@PAVCWADBankStateBlock@@@Z` | 0x00A3A46E | 0x00A3A380 | Native raw-pointer constructor builds the 12-byte count block with the state-block deleter. Ego_d records the concrete derived specialization. |
| +0x6A | `??_C@_00CNPNBAHC@?$AA@` | 0x00A3A475 | 0x0122D70E | literal payload prefix |
| +0x73 | `??0CCharString@@QAE@PBDJ@Z` | 0x00A3A47E | 0x0099EBF0 | reloc_bindings |
| +0xA1 | `?ToCharString@CWideString@@QBE?AVCCharString@@XZ` | 0x00A3A4A9 | 0x0099B2C0 | reloc_bindings |
| +0xA9 | `?AddEntry@CBankFile@@QAEXABVCCharString@@K0V?$CCountedPointer@VCBankStateBlock@@@@@Z` | 0x00A3A4B1 | 0x009CD620 | reloc_bindings |
| +0xB2 | `??1CCharString@@QAE@XZ` | 0x00A3A4BA | 0x0099EAE0 | reloc_bindings |
| +0xBE | `?ToCharString@CWideString@@QBE?AVCCharString@@XZ` | 0x00A3A4CF | 0x0099B2C0 | reloc_bindings (nearby instruction) |
| +0xC6 | `?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z` | 0x00A3A4D7 | 0x009CCDC0 | reloc_bindings (nearby instruction) |
| +0xD1 | `??1CCharString@@QAE@XZ` | 0x00A3A4E2 | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0xDA | `??1CCharString@@QAE@XZ` | 0x00A3A4C3 | 0x0099EAE0 | reloc_bindings |
| +0xFC | `??3@YAXPAX@Z` | 0x00A3A4FB | 0x00BFE9BC | reloc_bindings |
| +0x112 | `?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z` | 0x00A3A50B | 0x00999110 | reloc_bindings |
| +0x124 | `??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z` | 0x00A3A51D | 0x0099AD80 | reloc_bindings |
| +0x13C | `?GetEntryDataSize@CBankFile@@QBEKK@Z` | 0x00A3A53A | 0x009CC240 | reloc_bindings |
| +0x155 | `??0CDateAndTime@@QAE@XZ` | 0x00A3A54E | 0x009E9AB0 | reloc_bindings |
| +0x161 | `??0CDateAndTime@@QAE@XZ` | 0x00A3A55A | 0x009E9AB0 | reloc_bindings |
| +0x16D | `??0CDateAndTime@@QAE@XZ` | 0x00A3A566 | 0x009E9AB0 | reloc_bindings |
| +0x183 | `?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z` | 0x00A3A57C | 0x009CBE90 | reloc_bindings |
| +0x19C | `?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ` | 0x00A3A595 | 0x0099A5A0 | reloc_bindings |
| +0x1A6 | `??9CDateAndTime@@QBE_NABV0@@Z` | 0x00A3A59F | 0x009E9B20 | pass 24 naming evidence |
| +0x1CE | `?GetEntryDataSize@CBankFile@@QBEKK@Z` | 0x00A3A5C7 | 0x009CC240 | reloc_bindings |
| +0x1FF | `?_M_fill_insert@?$vector@DV?$allocator@D@std@@@std@@QAEXPADIABD@Z` | 0x00A3A5F8 | 0x004110D0 | reloc_bindings |
| +0x219 | `?GetEntryDataSize@CBankFile@@QBEKK@Z` | 0x00A3A612 | 0x009CC240 | reloc_bindings |
| +0x224 | `?ReadEntryData@CBankFile@@QAEXKKPAXK@Z` | 0x00A3A61D | 0x009CC2A0 | reloc_bindings |
| +0x25A | `?_M_fill_insert@?$vector@DV?$allocator@D@std@@@std@@QAEXPADIABD@Z` | 0x00A3A653 | 0x004110D0 | reloc_bindings |
| +0x27A | `?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z` | 0x00A3A673 | 0x0099A960 | reloc_bindings |
| +0x284 | `?SetPosition@CDiskFileWin32@@UAEXJ@Z` | 0x00A3A67D | 0x0099ABF0 | reloc_bindings |
| +0x2BB | `_free` | 0x00A3A6B4 | 0x00BFEA14 | reloc_bindings |
| +0x2CC | `_free` | 0x00A3A6C5 | 0x00BFEA14 | reloc_bindings |
| +0x2F3 | `??0CFileDataInputStream@@QAE@AAVCAFile@@K@Z` | 0x00A3A6EC | 0x00994700 | reloc_bindings |
| +0x2FC | `??0CMemoryDataOutputStream@@QAE@XZ` | 0x00A3A6F5 | 0x00996AF0 | reloc_bindings |
| +0x30D | `?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ` | 0x00A3A706 | 0x0099A5A0 | reloc_bindings |
| +0x320 | `?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z` | 0x00A3A719 | 0x00996CC0 | reloc_bindings |
| +0x32E | `??0CMemoryDataInputStream@@QAE@ABVCMemoryDataOutputStream@@@Z` | 0x00A3A727 | 0x00995EE0 | reloc_bindings |
| +0x33C | `?GetLength@CMemoryDataOutputStream@@QBEKXZ` | 0x00A3A735 | 0x00994B40 | reloc_bindings |
| +0x353 | `?UpdateEntry@CBankFile@@QAEXKAAVCDataInputStream@@K0K_N@Z` | 0x00A3A74C | 0x009D1320 | Ego_r 0x00B1CB80 and FableWin 0x02F95040. Native calls the landed update with two input streams and their sizes at 0x00A3A74C. |
| +0x35C | `??1CDataStream@@UAE@XZ` | 0x00A3A755 | 0x00993B10 | reloc_bindings |
| +0x365 | `??1CMemoryDataOutputStream@@UAE@XZ` | 0x00A3A75E | 0x00996B30 | reloc_bindings |
| +0x371 | `??1CFileDataInputStream@@UAE@XZ` | 0x00A3A76A | 0x00994780 | reloc_bindings |
| +0x37D | `??_7CDiskFileWin32@@6B@` | 0x00A3A773 | 0x0122D06C | reloc_bindings |
| +0x382 | `?Close@CDiskFileWin32@@UAEXXZ` | 0x00A3A77B | 0x0099A920 | reloc_bindings |
| +0x38B | `??1CWideString@@QAE@XZ` | 0x00A3A784 | 0x0099B510 | reloc_bindings |
| +0x394 | `??1CBaseClassNonCopyable@@MAE@XZ` | 0x00A3A78D | 0x0099A300 | reloc_bindings |
| +0x39D | `??1CWideString@@QAE@XZ` | 0x00A3A796 | 0x0099B510 | reloc_bindings |

49 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
