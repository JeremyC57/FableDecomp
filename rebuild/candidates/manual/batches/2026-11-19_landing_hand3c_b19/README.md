# Landing hand3c b19 (2026-11-19): CCRCSymbolMap::LoadBinary (0x00A38C20)

**Result:** strict vland --land, RELOCATION_MATCH + `00a38c20_TEST PASS` on the first grade; no flags.

**Source:** FableWin 0x031DB670 statement for statement, with Ego_d locals. No header changes and no new opt-ins.
- Shared headers only: CDiskFileWin32, CArray<unsigned char>(n) (FABLE_RETAIL_CARRAY + GETBYTESIZE), and CMemoryDataInputStream's inline ReadULONG / ReadSLONG.
- SLONGMap is a CVectorMap, whose resize at 0x00A39AE0 is already landed.
- One row-local stand-in: CCRCSymbolMap (vtable, plus SLONGMap at +4, with Ego_r names). No header exists for it yet.

The parked prior's "constant 0/4 register-caching" residue was an artefact of its local model.
