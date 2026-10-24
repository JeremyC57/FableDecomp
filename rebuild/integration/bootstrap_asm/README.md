# Bootstrap asm fixtures (not reconstruction)

`__asm`/`naked` bodies that `rebuild/build_bootstrap.ps1` compiles into the Windows GFMain runtime
fixtures. They call the fixture's `Fable*` shim symbols, so they are kept as they are; they are not
catalog rows. The landed, byte-exact reconstructions of the same addresses are genuine C++:

| address | fixture | landed source |
|---|---|---|
| 0x00403B10 | `src/CSystemManagerInit_Constructor_00403b10.cpp` | `rebuild/src/compiled/00/40/CSystemManagerInit_CSystemManagerInit_00403b10.cpp` |
| 0x0099A920 | `src/CDiskFileWin32_Close_0099A920.cpp` | `rebuild/src/compiled/00/99/CDiskFileWin32_Close_0099a920.cpp` |
| 0x0099AD80 | `src/CDiskFileWin32_Constructor_0099AD80.cpp` | `rebuild/src/compiled/00/99/CDiskFileWin32_CDiskFileWin32_0099ad80.cpp` |
| 0x009C9070 | `src/NGameText_CDataBank_GetPGroupEntry_009c9070.cpp` | `rebuild/src/compiled/00/9c/NGameTextCDataBank_GetPGroupEntry_009c9070.cpp` |
| 0x0042DEC5 | `src/CNewFrontendGameComponent_InitialiseInput_0042dec5.cpp` | same name under `rebuild/src/compiled/00/42/` |
| 0x00499CE0 | `src/CProgressDisplay_Constructor_00499ce0.cpp` | same name under `rebuild/src/compiled/00/49/` |
| 0x0059A9E0 | `src/FrontEndHistoryDequeMapAllocator_Allocate_0059a9e0.cpp` | same name under `rebuild/src/compiled/00/59/` |
| 0x00988290 | `src/CShaderRenderManager_ResetWorldTransform_00988290.cpp` | same name under `rebuild/src/compiled/00/98/` |
| 0x0098DFD0 | `src/CThreadedFile_Constructor_0098dfd0.cpp` | same name under `rebuild/src/compiled/00/98/` |

The second group were the catalog rows themselves until the 2026-10-24 de-bake replaced them with genuine C++
at the same paths; the bootstrap keeps compiling these asm copies.

Moved here 2026-10-24 out of `rebuild/src/compiled` and `rebuild/tests`, where the 0x0099A920 pair
differed from the landed files only by letter case and collided on a Windows checkout. Retiring them
means pointing the fixture at the genuine sources and providing their real callees instead of the shims.

0x00401FE0 `EULA::DoEULAThings` joined them 2026-10-24 when chunk 334 landed it as genuine C++ at the
catalog path; the fixture's naked asm body moved here unchanged.
0x009AC4D0 `NBankFileManager::CRetailBank` ctor and 0x009AC530 `map<CCharString,CContainedBank>::operator[]`
joined them 2026-10-24 when chunk 339 landed both as genuine C++.
- NResourceDirectoryNames_GetSaveDir_0041a540.cpp moved here 2026-10-24 when the address landed as genuine C++.
- NResourceDirectoryNames_GetSaveDir_0041a540_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- NResourceDirectoryNames_GetMyDocumentsDir_0041a430.cpp moved here 2026-10-24 when the address landed as genuine C++.
- NResourceDirectoryNames_GetMyDocumentsDir_0041a430_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CPersistContext_TextConstructor_009badd0.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CPersistContext_TextConstructor_009badd0_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CFileInstallerSingleton_Get_00404440.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_CCRC_Calc_004014a0_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_WinMain_00403480.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFFreeConfigDetection_00401b20_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GetFontBankName_00415260.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFInitialiseSetupProgressDisplay_00413120.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFUninitialise_00401b80.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFUninitialise_00401b80_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFInitialiseSetupProgressDisplay_00413120_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_WinMain_00403480_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GetFontBankName_00415260_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GFFreeConfigDetection_00401b20.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_CCRC_Calc_004014a0.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CFileInstallerSingleton_Get_00404440_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GetStreamingFontBankName_00415440.cpp moved here 2026-10-24 when the address landed as genuine C++.
- Global_GetStreamingFontBankName_00415440_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CAFile_GetProjectPath_00997510_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CAFile_GetProjectPath_00997510.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CAFile_SetCurrentPath_009974f0.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CAFile_SetCurrentPath_009974f0_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CWideString_AddLiteralAndWideString_0099bff0.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CWideString_AddLiteralAndWideString_0099bff0_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CBankFileAsync_SetEnableFailureHandling_009d5240_test.cpp moved here 2026-10-24 when the address landed as genuine C++.
- CBankFileAsync_SetEnableFailureHandling_009d5240.cpp moved here 2026-10-24 when the address landed as genuine C++.
