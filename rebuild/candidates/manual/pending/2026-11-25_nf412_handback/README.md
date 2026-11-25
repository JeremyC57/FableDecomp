# NF412 hand-back (2026-11-25): GetClassTypeName forwarders call their real statics

142 forwarder rows (retail vftable slot 22, 16-byte `mov ecx,[esp+4]; call static; ...; ret 4`) respelt to
`?GetClassTypeName@<Class>@@UBE?AVCCharString@@XZ` calling the landed `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ`
(nf406_b1 form). Landed as `2026-11-25_landing_nf412_b1..b4` (b1 50, b2 50, b3 17, b4 25), RELOCATION_MATCH PASS -> PASS.
/FORCE link of all 142 with and without RegisterTCs 0x004EE23F: duplicates 0 -> 0; 142 unresolved stand-in callees -> 0.

Coordinator applied: 142 self_symbols lines (old lines checked equal to each `current_line`), 12 table adds and 2 deletes,
closed T3 `nf414 0x004DA926 callers`, added the nf412 lines.
RULINGS r1 (coordinator decision): respell the 3 remaining stand-in statics (0x004D3EF9, 0x004D41DF, 0x0049D450) first, then their 3 callers.
