# Chunk 663 -- 10 of 10 landed (2026-11-01, NA008: the first "authorable" never-attempted chunk, no twins)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA008; ten 100-215 byte rows all carrying the propagated label
~CPatchTesselationEdgeStrip). Landed via `coordinator/land_chunk.sh 663` (vland --land 10/10 RELOCATION_MATCH + PASS, multi-check 0;
1-3 probes each): 0x006BA6D0 ~CFaction, 0x00850360 NCameraModes::~CPhysicsForceModelBase (via boost::checked_delete 0x00850510),
0x0082E780 ~CScriptedMapBrush, 0x0051A0A0 NThingMap::~CThingMapManager, 0x00550D50 NUISystem::~CTable, 0x00B47B50
~CEngineLightingManager, 0x006ED480 ~CCombatManager, 0x007B9D30 ~CTCTavernTable, 0x007783F0 ~CTCShop, 0x00B94B50 ~CEngineScene.
Implicit dtors (CFaction, CPhysicsForceModelBase, CThingMapManager, CTCTavernTable, CTCShop) use the ungraded TU-harness pattern
accepted in chunks 657-660. No row parked.
24 NAME_FIXES queued (c663). Labels: ghidra_out/labels_chunk663_2026-10-27.tsv (36). Asks for the owner / coordinator: a shared STLport
4.6 header set under rebuild/include (every source repeats the container text); header debt for the flattened engine headers named in
NOTES.md. Lessons in GOTCHAS (1 line) and LESSONS.md.
