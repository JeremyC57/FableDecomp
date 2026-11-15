# 2026-11-15 landing ge -- 0x00BF0B20 CEnginePrimitiveRenderUtil::SetupOutlineStencilStates (1,290 bytes; agentP6F, Fable model)

RELOCATION_MATCH, behaviour test PASS (16,384 cases), strict headers, no header change. Codex pass 22 parked it 3 lines off
(0x00BF0B36..3D: retail loads the B byte before storing G); agentP6 measured a scheduler tie (B load and G store both priority
0x17400, the lower IL index issues first) but found no lever in three sessions.

Fix: the two `width *= 1.0f/mesh_scale;` statements are folded into the vertex-shader upload arguments
(`width_world_space*(1.0f/mesh_scale)*0.01f`, same operations in the same order). Each float-local assignment left a no-op tuple on the
fmul chain, and that tuple is what tied the G store with the B load.
