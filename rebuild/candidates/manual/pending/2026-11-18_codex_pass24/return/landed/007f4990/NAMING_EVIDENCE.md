# 0x007F4990 naming and relocation audit

Ego_r 0x007EDF50 and FableWin 0x027B4690 agree on `?GetNPCShortName@CTCHasName@@QAE?AVCWideString@@XZ`. The final RTM object emits this leaf. Ego_d identifies this function beside GetNPCNameAndFullTitle and gives the distinct scopes for name and default_name_converted. Retail calls the two adjacent CThingManager name-map methods and takes the copied string result in both branches.

All 24 operands resolve to the addresses below. Applying them reproduces all 449 retail bytes.

| Operand offset | Retail address | Exact emitted symbol | Evidence |
|---|---|---|---|
| +0x015 | 0x00686AE0 | `?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ` | NAME_FIXES_QUEUE |
| +0x01C | 0x0051F010 | `?DoesNPCNameEntryExistForThing@CThingManager@@QAE_N_K@Z` | NAME_FIXES_QUEUE |
| +0x037 | 0x00686AE0 | `?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ` | NAME_FIXES_QUEUE |
| +0x03E | 0x0051F040 | `?GetNameForThing_OnlyCalledFromTCHasName@CThingManager@@QAE?AVCWideString@@_K@Z` | Exact Ego_r and FableWin names and landed function catalog |
| +0x04E | 0x0099B720 | `??0CWideString@@QAE@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x057 | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x067 | 0x013B86A0 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | ESTABLISHED_SPELLINGS |
| +0x074 | 0x0099AED0 | `??0CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x082 | 0x009CC190 | `?EntryExists@CBankFile@@QBE_NK@Z` | reloc_bindings |
| +0x098 | 0x009CC210 | `?GetEntryDataType@CBankFile@@QBEKK@Z` | NAME_FIXES_QUEUE |
| +0x0B1 | 0x009C9070 | `?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z` | NAME_FIXES_QUEUE |
| +0x0BF | 0x009C7A40 | `?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ` | reloc_bindings |
| +0x0C9 | 0x0099B7D0 | `??4CWideString@@QAEABV0@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x0D2 | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x0ED | 0x009C8890 | `?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z` | reloc_bindings |
| +0x103 | 0x00BFEB16 | `_rand` | reloc_bindings |
| +0x11D | 0x009C9070 | `?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z` | NAME_FIXES_QUEUE |
| +0x12B | 0x009C7A40 | `?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ` | reloc_bindings |
| +0x135 | 0x0099B7D0 | `??4CWideString@@QAEABV0@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x13E | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x192 | 0x00686AE0 | `?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ` | NAME_FIXES_QUEUE |
| +0x199 | 0x00521A90 | `?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z` | Exact Ego_r and FableWin names and landed function catalog |
| +0x1A9 | 0x0099B720 | `??0CWideString@@QAE@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x1B2 | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
