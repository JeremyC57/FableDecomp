# Chunk 316 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Engine shader lookups: `M = g_pEngineData->GetXResource()->GetVertexShader("N")` with an INLINE getter reloads `mov ecx,[G]; mov ecx,[ecx+off]` per lookup; a named `CShaderResource* r = g_pEngineData->X;` read before the first lookup gives retail's edi-cached run (00bb6e00, 00bba480 vs 00bc4d90).
- STLport vector element stores after resize: `*(begin() + i)` with a SIGNED long index loads _M_start once and folds the offset; `__forceinline resize(n)` is needed or VC emits a call (00bc4d90, 00bc34d0, 00bbeaf0). 0x00B6CBD0 is resize.
- VC7.1 fully unrolls a 4-iteration `for (long i = 0; i < 4; ++i)` colour-fill loop over three or four quad arrays; write the loop, not the stores (00b86c00, 00b5c460).
- A retail fall-through from an outside-walk loop into the following clipping blocks is reproduced by leaving the `return` OUT of that branch in source (00bdcbb0, directions 0/1).
- An `unsigned short v[3]` local (a wchar_t-sized "string buffer") triggers the /GS cookie and frame growth, yet VC still enregisters two of its elements (00be2cd0, frame 0x520).
- In SetState chains the `test a,b / or a,b` operand roles flip at the block where the mask value's register weight changes; a named mask local moves the flip point (00bf01e0, still one block off).
- A copied CRGBColour's copy-ctor member order decides which colour byte stays in a register and which is spilled (00bf0b20).
- Compiler-generated manager destructors: declare no dtor on the manager, `__forceinline virtual ~CVertexShader()` on the wrappers; the base dtor becomes a tail `jmp` (00ba8c70, 00bd7ef0).

Review at landing:
- Rows that needed more than 4 grades: 00b86c00 (6 probes), 00bb8090 (5), 00bc4d90 (4), 00bb6e00, 00bdcbb0, 00be2cd0 (parked), 00bf01e0 / 00bf0b20 (7 probes each, parked).
- Three rows land under a different class than the catalog (00b86c00, 00b52250, 00b5c460): check the Leaf against the .obj symbol and the module the lander files them under.
- 00bc4d90 / 00bc34d0 / 00b99d90 use offsets 4 lower than the PDB headers (`// Layout:` comments carry the evidence); local base models (CEnginePrimitiveManager, CEnginePrimitiveGroupManager, CEngineComponent) sit next to possible engine headers -- check rebuild/include/engine before landing.
- The PeekLayerValue inline family (00b3f5e0, 00b40260, 00b40670, 00b40bd0, 00b412a0) should be attacked as one group with the landed CEngineMap inline.
