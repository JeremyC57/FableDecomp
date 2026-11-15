# Main review: 00BC6DB0 — accepted locally

Identity is CEnginePrimitiveManagerText::RenderSubPrimitive, not the older UI text label. Both raw PDBs, the original class layout/vtable and donor call graph support it.

Main independently compiled the selected source with RTM VC7.1 (main_review_01): all2217bytes and590instructions match after relocation masking. All77operands (34distinct symbols) were decoded with actual COFF addends and checked against canonical bindings, original PDB evidence or exact literal contents. No reachable emitted helper debt and no EH metadata.37named code bodies are inventoried; unreferenced inline COMDATs receive no separate match credit.

Main reran the full2048-case verbatim target+three original helper fixture with VC7.1 /Od /W3 /GR- without EH and GCC64 C++03. It checks masking/filtering, fixed/programmed shaders, constants, viewport-relative coordinates, font arguments and restoration of shader/layout/capture state. Engine/renderer/font boundaries are controlled fixtures, not a full game integration test. Production bodies were compared literally against the fixture. Main also reran25member-offset assertions,8sizes and3const-pointer type checks; the original SceneList reference is separately evidenced.

All7owned header changes were reviewed as old/new pairs.56old/default token profiles preserve previous declarations/assertion identifiers. Baseline30 independently recompiled all70accepted targets and confirmed every emitted named C++ code section and relocation inventory unchanged. Existing custom EH parent/funclet/data audits also passed on30. NAME_FIXES records3incorrect older binding labels, including the protected UseFixedFunction static and original primitive-manager global.

No repository commit or push. Worker histories and the independent main receipts are retained.
