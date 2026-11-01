# 2026-10-27_linux_hand671 — chunk 671 (NA016, never-attempted pool)

Landed 2026-11-01 via `land_chunk.sh 671`: 10/10 RELOCATION_MATCH, bindings multi-check 0. Five destructors (labels right) and five
mislabelled rows (three "OnDie" implicit dtors, ~CEngineLocalDetailGenerator, CInputEvent::GetChecksum). CV_SwordInTheStoneScript named
RTTI-less via ctor -> Alloc -> registration strings. NAME_FIXES.tsv (24 rows) queued as c671.
Decisions (coordinator under owner delegation 2026-11-01): 0x00A8D700 is spelled ??_GC3DMesh2@@UAEPAXI@Z because retail's body is the scalar
deleting form (the donor PDBs' ??_E is the donor build's shape, not retail's); four TUs define the PDB ctor with an empty body only to emit the
implicit dtor (chunks 664/665/667 practice). Globals g_pEngineWorldMap / g_pRenderStateManager / g_pGameScriptInterface keep the established
landed spellings until a PDB data-symbol dump. Header debt (NOTES.md): 17 flattened engine headers; engine/CLevelScriptBase.h still declares a
destructor retail lacks (migration wave 10). Lane fix: $S/tools/rstr.py hardcodes an old scratchpad path.
