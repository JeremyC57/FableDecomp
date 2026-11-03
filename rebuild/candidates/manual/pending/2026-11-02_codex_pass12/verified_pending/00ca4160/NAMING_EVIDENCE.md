# Original vendor identity

The original Xiph libvorbis lsp.c defines vorbis_lpc_to_lsp and the same cheby, Laguerre_With_Deflation, Newton_Raphson and comp helpers in TU order. Retail allocates the same four polynomials, uses the same odd/even recurrence and calls those neighboring routines in exactly that order. The neighboring Newton_Raphson landed source independently identifies these two callers and private static conventions. This proves identity without relying on the false manifest label.

Original code is C compiled with the library /Gr convention; qsort remains the CRT cdecl import. Static original helper bodies supply their private register conventions rather than invented wrappers. Exact helper symbols and all relocated operands are to be audited before acceptance.

Final CRT name review: __alloca_probe and __chkstk are genuine compiler aliases at00bfea30. Original RTM chkstk.obj defines both at the same section/offset, and all61 helper bytes match native. Ego_r names __chkstk; FableWin names __alloca_probe. Neither spelling is a wrong-name correction; reciprocal NAME_FIXES lines were removed. Preserve the compiler-emitted symbol. See evidence/name_conflicts/CRT_ALIAS.json.
