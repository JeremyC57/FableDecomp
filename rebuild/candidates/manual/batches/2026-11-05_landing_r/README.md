# Landing 2026-11-05r: chunk 791 (4 of 6 rows landed, 2 parked)

One expert agent authored the rows. All four landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x009A1720 | CStreamingFontBank::PrepareToRender | RELOCATION_MATCH PASS (1 grade) |
| 0x00AB83A0 | CStaticFontBank::PrepareToRender | RELOCATION_MATCH PASS (1 grade) |
| 0x004963A0 | CFrontendGameComponent::Input | RELOCATION_MATCH PASS (12 grades; jump tables paired by hand) |
| 0x00E91F20 | NScript::CV_GuildMasterScript::GetGuildMasterSpeech | RELOCATION_MATCH PASS (5 grades; 29 table entries paired) |

- **Key spellings:**
  - The font-bank pair: the 22 inline CRenderStateManager setters through the GFGetSystemManager chain, on the new polymorphic
    opt-in class layouts. CStaticFontBank adds UV wrap, the out-of-line SetVertexFormat(0x144) and a modulate stage.
  - 0x004963A0: the event loop is `if (Start()==true) do {...} while (Next()==true)`. The GetMaxVisible* bodies are visible in the
    same file but not inlined, so VC7.1 knows ecx survives the call. GFWrapLimit<long,int,int> is a same-file
    `__declspec(noinline)` body (EXTRA_RULES chunk 422).
  - 0x00E91F20: the empty default string goes through an inline by-value pass-through.
  - vland warnings: RELOC_ALIAS `_rand` is the MSVCR71 import thunk (expected). RELOC_NAME on 0x01396DC8 (below).
- **Headers** (`hdr_README.md`), all opt-in branches; hdrcheck2 reads `default=same` and DIFF only for the new macros:
  - FABLE_RETAIL_CSTREAMINGFONTBANK (polymorphic, 6 slots, 0x3034) and FABLE_RETAIL_CSTATICFONTBANK (polymorphic, 0x2034);
  - FABLE_RETAIL_CINPUTEVENT_ACCESSORS (3 accessors) and FABLE_RETAIL_CINPUTMANAGER_EVENTSCANEND (1 declaration).
  - The agent rechecked the 11 landed includers with the overlay: 11/11 win, all PASS (`pending/.../SIBLINGS.tsv`).
  - Header debt: no retail-layout header yet for CFrontendGameComponent, CScriptDef or CV_RandomPopulationSimScript (modelled
    locally).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05r.tsv` (the six rows, plus 0x004959E0 CFrontendGameComponent::GetProfileNames).
  - 10 queue lines (tag `land0511r`):
    - 0x004944C0 ClearString (landed as ClearTextInput);
    - 0x00496EB0 `??$GFWrapLimit@JHH@@YIXAAJABH1@Z` with reference parameters;
    - the GetMaxVisible* Leaf lines 0x00494520 / 0x00494680 / 0x00494770 (private `ABE`, not `QBE`);
    - 0x00495830 GetSaveNames (landed as CFrontendProfileList::Refresh) and 0x004959E0 GetProfileNames;
    - SetMode 0x00495FE0's +0x1E44 member is CursorHighlight;
    - CSafeTrader::Main 0x00E4A280's pGlobalScriptDef +0x43C / +0x440 are TollBanditCloseToExit / TollBanditTimeToFade;
    - CHECK 0x01396DC8: the agent proposes CRenderStateManager::D3DTextureUVModeTable (already bound under that symbol, and an
      earlier codex label says the same) over the descriptive label g_textureUVModeToD3D. The symbol is not in the committed PDB
      function-name sets, so it waits for a cvdump check of the PDB data symbol.
- **Landing:** `vland --land` (`logs/land.log`); labels applied (`logs/apply_labels.log`); the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand791/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00E49020 CV_BanditTollScript::CTauntingBandit::Main | DIFFER(2424v2084), 11 attempts | Per-exit ~CTimer / ~CScriptThing / handle-dtor inlining (16 of 48 exit paths differ in the BanditsFlee / BanditsAngered blocks); TimerIndex kept in ebp |
| 0x00EC32F0 CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs | DIFFER(2040v2016), 10 attempts | VC7.1 keeps the empty vector's words in registers across a virtual call; retail reloads them at every push_back site |
