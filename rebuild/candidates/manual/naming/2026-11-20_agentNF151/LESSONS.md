# NF151 lessons (one line each)
- Compile every REFS row once at the start (work/probe_all.tsv, ~10 min for 115 rows): it turned 6 of 18 entries into measured done-already and listed the exact wrong binders.
- `#define private public` in a test that links the row object respells every private callee in the test object (LNK2019 on CreateMoreBlocks AAE): observe private state from test-defined members instead.
- An unknown `RenderManager->[+0x2908] != 0` read is a FableWin-only inline (IsSoftwareProcessingEnabled): grep FableWin names next to the row's PDB address before inventing a member.
- A binder that calls a private member from outside the class goes through the public inline wrapper retail expanded (SetVertexShaderConstantsDirect), never by making the declaration public.
- When a binder's local model declares several callees, check all of them against the PDBs: 0x009E1280 also mis-declared two other private callees, whose self rows were public too.
- Shader-enable stand-in rows port in one grade onto CShaderRenderManager.h + CScopedShadersEnable.h, copying the clean sibling row's include block.
