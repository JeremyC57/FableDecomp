// CPSCRenderSprite::CPSCRenderSprite -- body rounds floats with bare dword fistp (no __ftol2), the original's inline-asm rounding helper
// Grade: DIFFER(lane gap: inline-asm fistp) -- 16 bare dword `fistp` sites (float store/reload, then fistp/fild); VC7.1 C++ emits __ftol2 or (/QIfist) a qword fistp, so the body is asm_bake family per GOTCHAS; parked without a compiled attempt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFB7C0, 1040 bytes.
