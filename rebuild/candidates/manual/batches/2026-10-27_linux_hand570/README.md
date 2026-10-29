# Chunk 570 (2026-10-29): 4 of 5 ~1 KB released pass 9 rows landed

Landed (RELOCATION_MATCH + PASS): 0x00E295E0 `NScript::CQS_MeetSisterScript::CMeetSisterSister::Main`, 0x00EC3D40
`NScript::CV_RockTrollFirstEncounterScript::Main`, 0x00EF87D0 `NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main`
(names: Ego_r.pdb + the vtable slot each landed Alloc stores), and 0x00A5BEF0 `NVectorMath::GFInterpolateVectors` as grade
retail_asm (retail_asm/, landed separately with `vland.py --land`, flags `/Zi`; the SSE loop is retail's hand `__asm` -- an
xmmintrin spelling gives a different frame -- the x87 fallback is C++; vland warned RELOC_UNCHECKED, multi-block TU).
Parked (pending/2026-10-27_linux_hand570/differ): 0x00F3E6B0 `CTriangleStripifier2::BuildVertexAndPolyMaps` (928v920,
register allocation in the neighbour scan). Catalog labels "HandleSisterArenaDialogue" and "ProcessTriangles" were wrong.
NAME_FIXES.tsv: 9 rows queued. LESSONS.md: GOTCHAS lines.
