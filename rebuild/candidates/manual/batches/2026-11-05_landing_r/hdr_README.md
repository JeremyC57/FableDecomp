# chunk 791 header overlay (hdr/engine)

Every change is an opt-in `#ifdef FABLE_RETAIL_*` branch; the default (`#else` / outside the `#ifdef`) text is byte-for-byte the repo's,
so existing includers preprocess to the same translation unit. Each branch carries a `// Retail opt-in` line (gen_class_headers.py RETAIL_MARK).

| Header | Macro | What changed | Evidence | Used by |
|---|---|---|---|---|
| engine/CStreamingFontBank.h | FABLE_RETAIL_CSTREAMINGFONTBANK (with FABLE_RETAIL_CFONTBANK) | polymorphic retail class over CFontBank: 6 virtual slots (??_G 0x009A4E40, GetStringExtents 0x009A14E0, GetFontMaxHeight 0x009A0EA0, PrepareToRender 0x009A1720, Preload 0x009A2000, Render 0x009A25D0), Ego_r members, size 0x3034 asserted | RTTI .?AVCStreamingFontBank@@ vtable 0x0129AB54 (rtti-vtables.tsv); FableWin.pdb / Ego_r.pdb ?PrepareToRender@CStreamingFontBank@@UBEX_N@Z; landed ~CStreamingFontBank 0x009A1090 offsets | src/009a1720.cpp |
| engine/CStaticFontBank.h | FABLE_RETAIL_CSTATICFONTBANK (with FABLE_RETAIL_CFONTBANK) | polymorphic retail class over CFontBank: slots 0-3 and 5 (slot 4 keeps CFontBank::Preload), Ego_r members, size 0x2034 asserted | RTTI .?AVCStaticFontBank@@ vtable 0x0129EAD4; FableWin.pdb / Ego_r.pdb ?PrepareToRender@CStaticFontBank@@UBEX_N@Z; landed ~CStaticFontBank 0x00AB7A70 offsets | src/00ab83a0.cpp |
| engine/CInputEvent.h | FABLE_RETAIL_CINPUTEVENT_ACCESSORS | declarations only (no layout change): GetDeviceNumber 0x00A03B60, GetJoystickButton 0x00A03B90, GetJoystickPosition 0x00A043A0 (C2DVector by value) | Ego_r.pdb / FableWin.pdb ?GetDeviceNumber@CInputEvent@@QBEJXZ, ?GetJoystickButton@CInputEvent@@QBEJXZ, ?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ; retail call sites in 0x004963A0 (hidden result slot at 0x00496433) | src/004963a0.cpp |
| engine/CInputManager.h | FABLE_RETAIL_CINPUTMANAGER_EVENTSCANEND | declaration only: EventScanEnd(CInputEventScanInfo&) const 0x009F4AC0 | Ego_r.pdb / FableWin.pdb ?EventScanEnd@CInputManager@@QBEXAAVCInputEventScanInfo@@@Z; called after the scan loop in 0x004963A0 | src/004963a0.cpp |

## Includers rechecked (read-only, overlay on)
All 11 landed sources that include one of these headers (recheck.py + `vland.py --recheck-landed`, LANDVERIFY_WORK under this directory,
deleted after): 0x006286F0 GFPlayFullScreenXBoxMovie, 0x009A1090 ~CStreamingFontBank, 0x009A4160 FindGlyph, 0x009A0EA0 / 0x009A14E0
CStreamingFontBank GetFontMaxHeight / GetStringExtents, 0x009F55C0 CInputManager::FlagEventAsProcessed, 0x00A04410 CInputEvent(),
0x00A04160 CInputEvent::GetChecksum, 0x00AB5FC0 ~CKeyboardDX, 0x00AB7A70 ~CStaticFontBank, 0x00AB7A50 CStaticFontBank::GetFontMaxHeight:
11 of 11 wins (10 RELOCATION_MATCH, 0x00A04410 MATCH), every test PASS. No header was included transitively by another repo header.
