# Chunk 854: stopped mid-run (the user, 2026-11-08: "No more authoring")

The agent was told to stop and clean up; it wrote no finish.py output, payload or hand-over files. Saved as found:

| Row | State | Files |
|---|---|---|
| 0x00AB58E0 CMouseDX::Update | RELOCATION_MATCH (agent grade), test written | src/00ab58e0.cpp, test/00ab58e0.cpp |
| 0x00501D30 CWorldMap::GetLevelFilesFromWorldFile | RELOCATION_MATCH (agent grade), test written | src/00501d30.cpp, test/00501d30.cpp |
| 0x0083EFE0 LHNetworkLib::LHHttp2::ReceiveDocumentAsync | RELOCATION_MATCH (agent grade), **no test yet** | src/0083efe0.cpp |
| 0x00924C70 CAIStateGroup_KrakenAttack::Update | RELOCATION_MATCH (agent grade), **no test yet** | src/00924c70.cpp |
| 0x007602F0 NShape::IsShapeConvex (name from the agent, unverified) | WIP, best logged DIFFER(1123v1126) 130 lines; probe v8 reportedly register-only | probe/760_a4.cpp, probe/760_a8.cpp |
| 0x00507070 CWorldMap::BuildStaticMapGenerationVisibilityInfo (name from the agent, unverified) | WIP, 1 attempt, 474 lines | probe/507_a1.cpp |

To land the four matches: write the two missing tests, check the names against two sources, check the hdr/ opt-ins are additive against
rebuild/include, build a payload (address, module, name, source_cpp, test_cpp, pass_pattern, leaf), regrade with vland.py, then land.
Nothing here was graded by the coordinator.

## Continuing this chunk in another chat

Committed here: `src/` (4 matched sources), `test/` (2 tests), `hdr/` (the agent's header overlay, additive against rebuild/include
on 2026-11-08), `probe/` (every attempt, incl. the IsShapeConvex v1-v10 and BuildStaticMapGenerationVisibilityInfo tries), `STATUS.tsv`
(attempt log), `CHUNK_BRIEF.md` (the brief the agent worked from; its `$S/w22/agent854` paths are the old session's scratch), `rows.tsv`
and `mk854.py` (how the brief was cut). Not committed (retail bytes, regenerate them): the oracle and the disassembly.

1. On the Linux lane (docs/BUILDING.md): `T=tools/decomp_pipeline/linux_lane/lane_tools`;
   `python3 $T/chunk_pool/oracle_from_exe.py rebuild/candidates/manual/pending/2026-11-08_linux_hand854_stopped/rows.tsv chunk854`
   writes `$S/batch/chunk854_oracle.tsv` (exact lengths: 0x00AB58E0 1056, 0x0083EFE0 1074, 0x00501D30 1086, 0x007602F0 1126,
   0x00924C70 1152, 0x00507070 1432).
2. Grade a source with the hdr/ overlay (FABLE_HDR_OVERLAY must point at an existing directory, or cl.exe hangs), e.g. through a payload
   and `vland.py <payload> <oracle>` (read-only without `--land`).
3. Land the four matches (after the two missing tests): payload rows need address, module, name, source_cpp, test_cpp,
   pass_pattern (`<addr>_TEST PASS`), leaf (the PDB names are in CHUNK_BRIEF.md). Copy only the hdr/ files a matched row's macros
   use into rebuild/include, then follow the landing steps used by `batches/2026-11-08_landing_*`.
4. IsShapeConvex (0x007602F0): start from probe/760_a8.cpp (register assignment only, per the agent) and prove the name; then
   0x00507070 from probe/507_a1.cpp.
