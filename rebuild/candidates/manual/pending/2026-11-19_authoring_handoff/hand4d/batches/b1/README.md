# hand4d batch 1 (2026-11-19)
- 0x00540320 NUISystem::CEditBox::KeyPressed -- RELOCATION_MATCH + test PASS (strict vland). Flags /Zc:wchar_t. No overlay headers
  (repo opt-ins only: C2DBOXF, CWIDESTRING(_SUBSTRING), CIME(_ISIMEACTIVE), CTEXT_GETCHARWIDTHW, CTEXT_WINDOW_SLOTS).
  vland WARN RELOC_NAME only: ??BCWideString@@QBEPB_WXZ -> 0x0099AF00 labelled 'operator unsigned short const *' (the /Zc:wchar_t
  spelling of the same conversion; same as the earlier landed CEditBox rows).
