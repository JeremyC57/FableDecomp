# NF63 header overlay (hdr/)

Each file is a copy of the CURRENT repo header (re-diffed at hand-back) with `#ifdef` opt-ins only. Every default path is
token-identical to the repo: without the opt-in macro the preprocessed text is unchanged (the edits are new `#ifdef` blocks, or an
`#ifdef X ... #else <the original lines> #endif` around existing lines). Macro names are member-specific so they merge with NF61 / NF62.

- `engine/CEngineShadowRenderer.h` (copy of rebuild/include/engine/CEngineShadowRenderer.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CENGINESHADOWRENDERER_BUILDDEPTHPACKING`
- `engine/CMap.h` (copy of rebuild/include/engine/CMap.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CMAP_THEMESIZEZ`
- `engine/CSurface.h` (copy of rebuild/include/engine/CSurface.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CSURFACE_SETPIXEL`
- `engine/CThingTrackNode.h` (copy of rebuild/include/engine/CThingTrackNode.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CTHINGTRACKNODE_DRAW`
- `libpk/CBankProcessor.h` (copy of rebuild/include/libpk/CBankProcessor.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CBANKDATA_PREPARE`, `FABLE_RETAIL_CBANKPROCESSOR_STEPS`
- `libpk/CPlaybackSlot.h` (copy of rebuild/include/libpk/CPlaybackSlot.h; removed lines vs repo: 1): opt-ins `FABLE_RETAIL_CPLAYBACKSLOT_UPDATE`
- `libpk/CPooledResource.h` (copy of rebuild/include/libpk/CPooledResource.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CPOOLEDRESOURCE_DEACTIVATE`
- `libpk/CSoundEventGroup.h` (copy of rebuild/include/libpk/CSoundEventGroup.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CSOUNDEVENT_ISACTIVE`, `FABLE_RETAIL_CSOUNDEVENT_KEYFIELDS`
- `libpk/CSoundEventManager.h` (copy of rebuild/include/libpk/CSoundEventManager.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CSOUNDEVENTMANAGER_STOPFIELD18`, `FABLE_RETAIL_CSOUNDEVENTMANAGER_UPDATEPLAYBACKS`
- `libpk/CSoundEventSet.h` (copy of rebuild/include/libpk/CSoundEventSet.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL`, `FABLE_RETAIL_CSOUNDEVENTSET_FINDWITHFIELD`
- `libpk/PKCTRList.h` (copy of rebuild/include/libpk/PKCTRList.h; removed lines vs repo: 0): opt-ins `FABLE_RETAIL_PKCTRLIST_DTOR`

Notes on the two edits that touch existing lines:
- `libpk/CPlaybackSlot.h`: the one-line struct is split so the `#ifdef FABLE_RETAIL_CPLAYBACKSLOT_UPDATE` block sits before its
  closing `};`; without the macro the token sequence is identical to the repo line.
- `libpk/CSoundEventGroup.h` (`FABLE_RETAIL_CSOUNDEVENT_KEYFIELDS`) and `libpk/CSoundEventSet.h` (`FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL`):
  the original member lines are kept verbatim in the `#else` branch; the opt-in branch has the same size and offsets.
