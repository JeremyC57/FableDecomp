# Chunk 857 lessons (GOTCHAS one-liners)

- VC7.1 canonicalises a one-expression sum such as `X*X+Y*Y` whatever the operand order; to get retail's load order split it (`float s = Y*Y; s += X*X;`) (0x00A1C010).
- A same-TU inline callee (CNavigatorAStar::IsRouteClear) expands into the caller with every `return` turned into a jump; write its real body in the row instead of a declaration (0x00A1C010).
- `FABLE_RETAIL_C2DBOXI_COPY` only works with `FABLE_RETAIL_C2DBOXI` also defined; with it, `C2DBoxI old = display.GetViewport();` copies field by field from the base (retail), without it VC7.1 copies through a `lea` pointer (0x009D8CF0, md 81 -> 32).
- The C3DMesh2 TU is /Gr: there VC7.1 inlines vector<CCountedPointer<C3DBone2>>::_M_insert_overflow like retail; an `inline` keyword alone does not (0x00A8B990).
- The CRenderStateManager bookmark-test load order (`mov ecx,[state+8]` before or after `mov edx,[this+0x2814]`) changes with distant code (the viewport copy flipped one block) and not with the helper's operand order; treat it as a lane gap (0x009D8CF0).
- Retail Fable.exe's wide strings print as one character in the lane disassembly; read them as UTF-16 from the exe (tmp tool bin/ws.py) before writing paths (0x0049A0B0).
- Catalog names in this chunk were wrong for 4 of 8 rows (forge autonames): check the PDB name sets and callers before trusting the catalog.
