# NF65 header overlay (hdr/)

Each file is a copy of the CURRENT repo header (re-diffed at hand-back) with `#ifdef` opt-ins only. Default path check: each overlay
and its repo header were run through `cpp -P` with no opt-in macro defined (rebuild/include on the path); "default tokens" below says
whether the two preprocessed texts are identical after whitespace normalisation (the __LINE__-numbered FABLE_STATIC_ASSERT typedef
names are normalised: added lines shift them; nothing references them). Macro names are member-specific (merge with NF62 / NF64).

- `engine/CAIStateGroup_JackOfBladesStage2.h` (copy of rebuild/include/engine/CAIStateGroup_JackOfBladesStage2.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_JACK_STAGE2_ONACTIVATE`
- `engine/CAnimateThing.h` (copy of rebuild/include/engine/CAnimateThing.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CANIMATETHING_CLEARQUEUES`
- `engine/CEngineMap.h` (copy of rebuild/include/engine/CEngineMap.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINEMAP_OPENDEVELOPMENTSTATICMAP`
- `engine/CEngineScreenEffectDisplacementRenderer.h` (copy of rebuild/include/engine/CEngineScreenEffectDisplacementRenderer.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_OLDRENDERTARGET`
- `engine/CEngineWaterRenderer.h` (copy of rebuild/include/engine/CEngineWaterRenderer.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINEWATERRENDERER_OPENSTATICMAPFORUSE_DECL`
- `engine/CEngineWorldMap.h` (copy of rebuild/include/engine/CEngineWorldMap.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS`, `FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE`, `FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST`
- `engine/CStaticMapBankFile.h` (copy of rebuild/include/engine/CStaticMapBankFile.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS`
- `engine/CStaticMapBankStateBlock.h` (copy of rebuild/include/engine/CStaticMapBankStateBlock.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CSTATICMAPBANKSTATEBLOCK_CLASS`
- `libpk/CBankProcessor.h` (copy of rebuild/include/libpk/CBankProcessor.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CBANKPROCESSOR_ADDEVENTTRIGGER`, `FABLE_RETAIL_CBANKPROCESSOR_ADDSAMPLE`, `FABLE_RETAIL_CBANKPROCESSOR_NUMBERTRIGGERS`, `FABLE_RETAIL_CEVENTTRIGGERREF_NUMBERCTOR`, `FABLE_RETAIL_CEVENTTRIGGERREF_TYPESETTERS`, `FABLE_RETAIL_SSAMPLEHEADER_FORMAT`
- `libpk/CItem.h` (copy of rebuild/include/libpk/CItem.h; removed lines vs repo: 1; default tokens identical: yes): opt-ins `FABLE_RETAIL_CHASHEDITEM_CTOR`, `FABLE_RETAIL_CITEMDISPATCHER_FINDITEM`, `FABLE_RETAIL_CITEMTABLE_INSERT`, `FABLE_RETAIL_CNAMEDITEM_CTOR`
- `libpk/CScriptHost.h` (copy of rebuild/include/libpk/CScriptHost.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CSCRIPTHOST_CREATESAMPLE`

Notes on edits that touch existing lines (each keeps the repo lines verbatim in the `#else` branch):
- `engine/CEngineWorldMap.h` (`FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE`): wraps the retail branch's `private:` + `_pad_0x30` / `_pad_0x2c`
  padding; the opt-in branch declares the private CreateNewBankMap and splits the padding into VisibleMapList (when not typed) +
  StaticMapBankName (4) + the typed `CStaticMapBankFile StaticMapFile` (+0x34, 0x164); GameWorld stays at +0x198. It sits inside the
  `FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD` branch, so a row must define that macro too (0x00B3FF70 does).
- `engine/CStaticMapBankFile.h` / `engine/CStaticMapBankStateBlock.h`: the generated layout class goes to the `#else` branch; the opt-in
  branch is the polymorphic retail class with the same size (0x164 / 0x8).
- `libpk/CBankProcessor.h`: `FABLE_RETAIL_SSAMPLEHEADER_FORMAT` (SSampleHeader record tail), `FABLE_RETAIL_CBANKPROCESSOR_ADDSAMPLE` (member
  line `m_pData / m_pManager / m_pLog`, manager typed CScriptHost*) and `FABLE_RETAIL_CBANKPROCESSOR_NUMBERTRIGGERS` (member line
  `m_pCriteriaTriggers / m_pNumberTriggers`, registry typed CItemDispatcher*) wrap the original lines; same offsets.
- `libpk/CItem.h`: `FABLE_RETAIL_CITEMTABLE_INSERT` replaces `virtual void v03();` in its opt-in branch (same slot); the other opt-ins
  (`FABLE_RETAIL_CHASHEDITEM_CTOR`, `FABLE_RETAIL_CNAMEDITEM_CTOR`, `FABLE_RETAIL_CITEMDISPATCHER_FINDITEM`) are new blocks (the two ctor
  opt-ins add the item members, layout 0x10 / 0x14, only when defined).
- `engine/CEngineWorldMap.h` (`FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS`, inside the `_STATICMAPFILE` block): StaticMapBankName becomes a
  CWideString (the opaque 4-byte line kept in `#else`); in the GameWorld branch's non-`_GAMEWORLDMAP` padding `_pad_0x19c[0x24]` is split
  into 0xc + StaticMapOpenState (+0x1A8, CEngineMap::STATIC_MAP_OPEN_STATE) + 0x14 (the original line kept in `#else`). StaticMapOpenState
  therefore exists only when `_GAMEWORLD` is defined and `_GAMEWORLDMAP` is not.
- `engine/CEngineScreenEffectDisplacementRenderer.h` (`FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_OLDRENDERTARGET`): the opaque
  `OldRenderTarget[0x24]` line moves to `#else`; the opt-in types it as engine/CRenderTarget.h's CRenderTarget (same size and offset).
- `engine/CStaticMapBankFile.h`: the opt-in class declares the FableWin.pdb override GetDataFormatVersion (CBankFile's pure slot).
- `engine/CEngineMap.h`, `engine/CEngineWaterRenderer.h` (on the flattened default class), `engine/CAIStateGroup_JackOfBladesStage2.h`,
  `engine/CAnimateThing.h`, `libpk/CScriptHost.h`: new declaration-only blocks.
