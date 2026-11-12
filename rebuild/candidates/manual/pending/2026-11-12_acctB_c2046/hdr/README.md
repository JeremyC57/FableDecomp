# Overlay headers of chunk 2046

## engine/CQ_HobbeCaveScript.h -- new opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT (default text unchanged)
- Copied from rebuild/include/engine/CQ_HobbeCaveScript.h (HEAD at 2026-11-12); one `#ifdef` block added after the class, inside
  `namespace NScript`: the definition of the nested entity script `CQ_HobbeCaveScript::CHobbeTorrent : CEntityScriptBase`.
- Evidence (two sources each):
  - layout: Ego_r.pdb CHobbeTorrent (ghidra_out/struct_layouts_egor.tsv, 32 bytes: ParentClass `CQ_HobbeCaveScript *` +0x14,
    PMasterData +0x18, NumberToGenerate long +0x1C) = retail: Main 0x00D92380 reads [this+0x14] -> parent and dec [this+0x1C];
    the landed OnPersist 0x00D91930 persists "NumberToGenerate" at +0x1C; the landed Alloc 0x00D8F3F0 allocates 0x20 and stores
    ParentClass at +0x14.
  - members: Ego_r.pdb + FableWin.pdb ?Alloc / ?Init / ?OnPersist / ?Main@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@ (Main / Init /
    OnPersist private EAE, GetParentScript public UBE, Alloc static SI); retail vtable 0x012D5960 (slot 1 Main 0x00D92380, slot 4
    OnPersist 0x00D91930).
  - ctor: the landed Alloc row's inline constructor (same text).
- Users: src/00d92380.cpp. No landed includer defines the macro, so landed rows keep their bytes.
- No other overlay header; row 2 uses repo headers and existing opt-ins only.
