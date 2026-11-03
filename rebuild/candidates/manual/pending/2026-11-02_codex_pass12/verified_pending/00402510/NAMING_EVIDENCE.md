# GFMain identity, original names, and remaining native-only naming gaps

The assigned row is original global `long __fastcall GFMain(HINSTANCE__*,char*,int)`, not a class member. Exact leaf `?GFMain@@YIJPAUHINSTANCE__@@PADH@Z` is present in both original donor name sets and the current COFF symbol table. Original release/main.obj procedure records identify the three parameters and startup locals. Native extent00402510..00403480 is3952bytes, ending at the genuine return before the next aligned function. This final candidate reproduces all3952bytes with relocation operands masked.

## Original helper linkage

The original main.obj records distinguish S_GPROC32 external functions from S_LPROC32 file-static functions. `GFUninitialise`, `GFHandleSystemInitError`, `GFGetBuildNumber2` and `GFInitialise` are file-static; their genuine visible bodies are included before GFMain. Each emits COFF storage class3 with its real original name. Each entire helper body is also compared with retail, including its literals, globals and internal switch-table destinations:

- 00401b80 GFUninitialise:64bytes.
- 00401c00 GFHandleSystemInitError:808bytes.
- 00401f30 GFGetBuildNumber2:162bytes.
- 004022b0 GFInitialise:311bytes.

The enum parameter to GFHandleSystemInitError remains the original ESystemCode and original fastcall declaration. Its real static body allows VC7.1's local optimization to use the native stack argument. It is not replaced with a convenient different prototype. GFConfigDetection, GFFreeConfigDetection, GFHardDiskError, GFIsSystemErrorFailure, TestDirForValidFilePermissions and GetWindowTitle are all S_GPROC32 in both donors and retain external declarations.

## Important recovered identities

- GFInitialise caches PSoundManager, GPDisplayManager, GPRenderManager, GPShaderRenderManager, GPInputManager, GPFontManager and GPGraphicBankManager. The old labels treated several of these as unrelated primitives. Original typed data records and native getter/member offsets prove the actual types.
- GFInitialise clamps original GMaxTextureWidth/GMaxTextureHeight against GMaxDesiredTextureSize using CRenderManagerCore::GetMaxTextureWidth/GetMaxTextureHeight. These read D3DDeviceCaps.MaxTextureWidth/MaxTextureHeight at3a58+58/5c. They are texture capabilities, not display dimensions.
- The final value is GFrameLimit = GPDisplayManager->GetRefreshRate()/2. The original private long RefreshRate sits at1c8; the old half-screen-width label was incorrect.
- The shader singleton is private `CShaderRenderManager::SingletonInstance`, accessed through original public static Get(). The complete header preserves every named original member, native reduced container sizes, matrix padding and total3f0. Native constructor0098ab00, destructor0098abf0, the nested layout constructor, AllocateLights and the next adjacent callback global prove the offsets and extent. No alignment directive is invented.
- GFGetBuildNumber2 assigns original GVersionString at013b85e0. Both typed PDB global records and matching29-byte initializer/10-byte destructor establish it. Its original locals are CDiskFileWin32 build_file and CCharString str; the latter constructor takes CAFile&, not a pointer.
- GFUninitialise calls CUserProfileManager::Release at00406df0, not the old file-installer label. The native released private singleton is013b7d4c/50. CThreadedFile::Manager at013bc870 is a private static CThreadedFileManager. Final4051c0 is NUISystem::CIME::Shutdown.
- 009a0ef0 is CStreamingFontBank::SetUseFixedFunction(bool), proven by raw public/static type records and GetFontMaxHeight/SetAlphaMaskUVs/SetUseFixedFunction donor/native adjacency. Its protected static bool is013c5a7c. It is unrelated to CXMVPlayer::SetLeftAlignText at009bc890.
- 009a76d0 is member CBankFileManager::SetUsingRetailBanks(bool), not a namespace free function.
- 009cc410 is const CBankFile::FindIndexBySymbolCRC(unsigned long), returning unsigned long. Original class/type records plus native table lookup and its callers disprove the old LookupValue stand-ins.
- 00413c50 is NGlobalConsole::Initialise;0041a4c0 is NResourceDirectoryNames::GetUserDataDir. CreateCompiledDefs/CreateChecksums at0138e188/189 are protected static CDefinitionManager bools.

The full operand report checks557 operands across the five exact bodies. All42 main-body string references and all helper strings compare their complete emitted literal payloads to native bytes. This caught and corrected the four PARTICLE_MAIN/PARTICLE_FRONTEND alias literals, which have no GBANK_ prefix. Actual PE import names prove CreateFileW, CloseHandle, MessageBoxW and the scalar CRT allocation/deallocation/exit thunks. Internal switch references point to the correct native-relative case locations. NAME_FIXES.tsv records81 wrong bound spelling groups; descriptive names below are deliberately excluded from claims of recovered exact spelling.

## Honest naming limitations

The README expressly permits one descriptive address-suffixed name where original names cannot be proved. These eight symbols remain in that category, and must not be represented as recovered original spellings:

-0137548f StartupConsoleEnabled_0137548f: native startup gate for NGlobalConsole::Initialise; no matching original typed/module global name found.
-0138dd3b SaveSoundSymbols_0138dd3b and013bc9f0 LoadSoundSymbols_013bc9f0: native sound settings; original native sound implementation and several fields were removed from all available Anniversary PDB versions.
-01375754 PrepareParticles_01375754: disabled in static-map mode. Particle callers suggest a console particle-enable field, but no exact namespace anchor proves it; the tempting NGlobalConsole::EnableParticles spelling is not claimed.
-013750a4 RenderConfigurationFlag1_013750a4,013750aa RenderConfigurationFlag2_013750aa,013750ab UseFixedFontFunction_013750ab: native configuration fields copied from detection record13bd6e8 at70/8c/14. The first ORs flag1, inverse second ORs flag2, third enables the font setter. G-prefixed raw globals, original main module data and all function sets did not reveal names.
-009d81e0 ClearStartupLatch_009d81e0: native conditionally clears013964a8, adjacent009d81c0 sets it. VTune context is apparent but a precise original function name is absent from all queried public/type/module sets; no speculative VTune method name is substituted.

Search receipts include evidence/ROOT_STARTUP_UNPROVEN_SEARCH.md, STARTUP_NAME_AUDIT_ROOT.json, gfmain_module_data.json, gfmain_global_G_names.json, gfmain_filtered_globals.json and the original Ego_r/FableWin/Ego_d name sets. Native-only system sound configuration bytes38..57 and CSystemManager's removed sound counted pointer7c..83 retain explicitly documented opaque storage, not invented original member names. The latter is read directly at its proven native offset in GFInitialise. Known original members after it, including PFontManager+84, retain their exact names.

Wide-string parameter spelling deserves a separate ABI note: donor PDBs use wchar_t/PB_W; the native VC7.1 default wide ABI uses unsigned short/PBG. Original declarations, native operations and established native bindings prove this adaptation; PBG is not falsely claimed to appear verbatim in those donor function-only name lists.

## Evidence paths

- evidence/gfmain_function_records.json, gfmain_linkage_records.json: original main.obj procedures and locals.
- evidence/gfmain_types_selected.json, gfmain_method_types.json, gfmain_aux_types.json, gfmain_threadedfile_type.json: exact access, class key, signatures and full original members.
- evidence/gfmain_globals_near.json, gfmain_global_G_names.json, gfmain_additional_globals.json: original typed global order and types.
- workers/engine/evidence/gfinitialise/: original globals/types, build-string native binding, shader layout evidence, ADDITIONAL_LINKAGE.json and independent HELPER_AUDIT_GRADE19.json.
- RELOCATIONS.json: each of557 native/object operands, its exact symbol or explicit descriptive-name limitation, source/header hashes and helper extents.

## Behavior validation

The test pastes GFMain's final production body verbatim and uses observable filesystem, persistence, bank, display, text, registry and game collaborators. It executes2048 combinations covering language-file presence/open failure, bank modes and aliases, permissions and missing text, installation toggles, EULA/config failures, sound flags, antialiasing, static-map mode, rendering flags, successful/failed game startup and system initialization failure. Both64-bit GNU C++03 and real VC7.1 RTM /GR- pass. The test models native sound config words as32-bit unsigned int on the host, uses exactly representable floats, and does not rely on return-value elision. Byte/ABI verification is independent of the behavioral mocks.

## Final main review

Accepted shared-header attempt22 after fresh RTM compilation and2048 behavior cases on each compiler. Main independently decoded all557 operands and compared all five complete bodies; storage separately reviewed those operands and reopened the original PDB streams for identity/linkage. Its report is workers/storage/GFMAIN_INDEPENDENT_REVIEW.md. The shader model retains original members and proven native anchors, size and container corrections; its evidence is a partial native-field audit, not a claim every unused offset was directly observed. PSoundManager is owner-established. Eight exact original names remain unresolved as listed above. The original function-record receipt contains concatenated command output despite its .json suffix; a correctly labelled text copy is saved in reviews/00402510/ORIGINAL_FUNCTION_RECORDS.txt.

## Cross-row identity correction, main attempt23

The previously accepted list-destructor name at004046b0 was wrong. Original STLport4.6 `_List_base` owns node/sentinel destruction; native derived-list destructor00404710 jumps to004046b0. The GFMain-only declaration now uses the real empty derived destructor, so VC7.1 calls the genuine base destructor. Main independently recompiled and checked all557 operands and five complete bodies;2048 cases passed on each compiler. This corrects a detail missed in the earlier separate review. Original PDB procedure/module order also confirms0099e4c0 is the CCharString conversion, distinct from GetAsCharArray at00403a10. The eight unresolved descriptive names remain explicitly unresolved.

Selected main attempt25 also corrects the inherited CDefPointer class to original private Object and direct original methods without an invented Assign helper. Both original TPI sets verify its member set. All five genuine emitted function bodies and557 operands remain exact and independently checked;2048 cases rerun on each compiler. The8 unproven original names are unchanged.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

## CResource original field access correction

Both original TPI records prove CResource::ResourceList,PrevResource,NextResource,ResourceSize,LastUsedFrame are private at offsets8,12,16,20,24. The opt-in shared header restores that access, keeping every original field and the entire default branch unchanged. Source includes FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS. Actual VC7.1 preprocessing confirms those fields and the earlier private IVRefCount and private CDefPointer::Object declarations. See RESOURCE_ACTIVE_DECLARATIONS.json and the vendor original signature matrix at workers/vendor/BASE_DECLARATION_AUDIT/REPORT.json. Fresh byte and dual-compiler fixture checks pass; independent main acceptance is pending.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.
