# Check before landing chunk 733 (coordinator, 2026-11-03)
`hdr/engine/NUISystem_CUIScreenUnitConverter.h` makes the class statics public so they mangle `@@2`, the spelling landed code
already uses. Access must come from the PDB (TPI field list / S_GDATA32 of `NUISystem::CUIScreenUnitConverter::BackBufferWidth`
/ `BackBufferHeight`, 0x013B876C / 0x013B8770), not from existing spellings. If the PDB says private, the statics stay private
(`@@0`) and the landed `@@2` binders are queued PRIORITY instead.
