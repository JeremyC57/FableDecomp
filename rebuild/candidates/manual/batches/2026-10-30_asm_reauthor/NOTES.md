# asm_bake re-author pass -- notes (2026-10-30)

27 rows from `rows.tsv`. Result: 8 C++ RELOCATION_MATCH + 1 retail_asm RELOCATION_MATCH (all vland PASS, no
RELOC_IDENTITY), 11 re-author rows left DIFFER (best sources in `differ/`), 3 re-author rows not attempted, 4 CRT rows that
should come from the CRT / import library. `STATUS.tsv` has every row; `NAME_FIXES.tsv` the proven spellings.

## Hand-back
- `payload.json` (8 rows) and `retail_asm_payload.json` (00a5b850). Dry run:
  `LANDVERIFY_WORK=$S/asmre/lv python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/asmre/payload.json
  $S/asmre/oracle_final.tsv --recheck-landed` -> 8 x RELOCATION_MATCH PASS; the retail_asm payload -> RELOCATION_MATCH PASS.
  `--recheck-landed` is required: all 27 rows are already landed (as asm) and vland skips landed rows otherwise.
- Landing replaces the asm files: `00/c8/std_DestvalCFlasha_00c8db10.cpp` (rename: the function is libogg
  `ogg_stream_packetout`), the CWideString operator+ asm rows (0099bf30 / 0099be70), `00/42/CNewFrontendGameComponent_
  InitialiseDefs_0042f722.cpp`, the InitialiseEngine asm, `00/53/CList_InitialiseOffsets_00536b4f.cpp`,
  `00/41/Global_InitialiseConsoleVariables_00413c50.cpp` (raw _emit blob), `00/a5/global_GFInitVectorMath_00a5b850.cpp`.
  009c85a0: the C++ is already landed (`00/9c/NGameTextCDataBank_CDataBank_009c85a0.cpp`); only the asm twin
  `NGameText_CDataBank_Constructor_009c85a0.cpp` needs retiring.
- `oracle_final.tsv`: catalog rows + 6 added (0099bf30 177, 0099be70 177, 009a8840 1565, 009a8170 1732, 00413c50 4158,
  00401284 6). 00551340 corrected 428 -> 431 (the catalog row stops before `add esp,10h ; ret`).

## Names (two sources each; details in NAME_FIXES.tsv)
- 0x00C8DB10 is libogg `ogg_stream_packetout` (+ `_packetout` 0x00C8D9D0 static, `ogg_stream_packetpeek` 0x00C8DB20), not
  `std::_Dest_val<...CFlash>`.
- InitialiseEngine / InitialiseDefs are private (AAEXXZ); CompileDefs is a static member; CStartupDefinitionManager::Get()
  returns a reference; GetDef<T> takes the CDefPointer BY POINTER (FableWin). The def InitialiseEngine fetches is
  CEngineDef "ENGINE".
- 0x009EC890 is `CConsole::RunScript(const CWideString&)` (both PDBs), not ExecuteConfigFile.
- 0x005BD993 is `NInventory::CCategory::GetHighlightedItemMaxInCategory() const` (landed name GetMaxItemsPerSlot invented).
  The landed `CCategory_GetHighlightedItemCountInCategory_00713290` also looks mislabelled (not proven here).
- InitialiseConsoleVariables (landed name kept, fable_gfmain.h `FABLE_FASTCALL`; no PDB symbol -- it is inlined in Ego_r):
  the three command handlers are the PDB's `GConsoleSetLevel`, `GConsoleSetStartingHolySite`,
  `GConsoleEnableCodeSectionLoading`; the ten pool variables are the members of the default CEngineMemoryInit 0x0137549C
  (Ego_r member names). Three variables keep their existing bound global names although their console names disagree:
  0x013B8646 g_bLoadFromAnyDirectory ("UseRetailSaveGameSystem"), 0x013B8616 g_bRuntimeLevelFiles ("UseRetailBanks"),
  0x01375459 g_WADBuildEnabled ("AllowDataGeneration") -- worth a look by whoever owns those names; the C++ names are not
  provable from the console strings, so no NAME_FIXES entry. The other bool/long globals and 0x01375444 are descriptive.
  EGenericVarType values are written as numbers (1 long, 3 float, 4 bool, 5 string; no enumerator names in the PDBs).
- 0x0042DF9E Draw (differ): callee names from Ego_r (SetVWindow, ClearRenderTarget, GetVVisibleTelevisionScreenArea,
  BeginRender/EndRender/SwapScreens returning bool where the PDB says so, CFrontEndManager::GetInstance()->Draw(engine, 0),
  NUISystem::CManager::RenderErrorMessage(CIEngine&), CIME::Get().Render()). m_fLastRenderTime is +0xBC in retail (landed
  ctor 0x0042EA8F). The render-state block is CRenderStateManager::DeviceState (FogEnable +0x12C, TextureMipMapMode[0] +0x62C).
- 0x00536B4F: the def flag at +0x221 is CUIDef::PCStyle (landed IsPCStyle 0x00530740); slot 108 is LoadAndReturnDef() const.

## Header debt
- engine/CDefPointer.h (retail model) has no null constructor (FableWin `??0..QAE@W4ENullDef@@@Z`; enumerator name unproven):
  0042e204 carries a local CDefPointer with it.
- 00536b4f keeps the landed CList rows' local model (engine/CList.h is the Ego_r flattened layout; retail offsets differ) and a
  local CDefPointer<const CUIDef> over CUIDef's own ref-count base (vland warns SHARED_HEADER_AVAILABLE CDefPointer).
- 009c85a0 (already landed): SHARED_HEADER_AVAILABLE CResourceList warning, invented CNarratorMap word.
- 00413c50 models CConsoleInputBase / CConsoleCommandBase / CConsoleClasslessCommand / CConsoleVariable / CConsole locally
  (engine/CConsoleCommandBase.h exists but is a flattened 0x14-byte layout without the base / name member).

## Unmatched rows: what was tried (beyond the Grade lines in differ/)
- 0042DF9E Draw: retail_asm form works except one spill order: the asm must be in the function body (a __forceinline helper
  loses the ebp frame); `#pragma optimize("y",off)` + `("s",on)` like the rest of the TU; `unsigned long` control word so it
  shares [ebp-4] with the CRGBColour temporary; the byte `and` from `reinterpret_cast<unsigned char*>(&cw)[1] &= 0xF3`;
  `state.BookmarkMask |= CurrentBookmarkMask` (no local) fixed the first SetDesiredState. 6 grades used.
- 00597BF2: the same-TU map::operator[] stand-in (landed 0x00597006 model) fixed the [ebp+8] spills. VC7.1 evaluates the
  right operand of `==` first when both sides are calls.
- 009A8840 / 009A8170 (bank-file openers, 1.5-1.7 KB, inline CMemoryDataInputStream reads with the 64-bit position guard,
  CDiskFileWin32 / CFileDataInputStream / CRetailBank / threaded-file objects): analysed, not attempted for time. Reusable
  model: the landed CPersistTraits readers (e.g. 0x00755D00) carry the inline Read.
- 00C8C730 vorbisfile `_ov_d_seek_lap`: LTCG private convention (vf in esi for _ov_initset / _ov_initprime) -- lane gap.
