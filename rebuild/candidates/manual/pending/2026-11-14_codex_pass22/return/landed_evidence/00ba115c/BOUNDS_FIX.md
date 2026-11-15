# Corrected whole-function envelope

Assigned `00ba115c..00ba1a68` is a 2316-byte tail with 28 head bytes missing. The complete parent is `00ba1140..00ba1a68`, 2344bytes, `CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `00ba1a65 ret 0xc`; linear disassembly covers every byte through that return.

Parent landed: **no**. docs/UNMATCHED_ROWS.tsv explicitly lists full parent as unmatched; no landed_sources entry.

README_CODEX.md permits recovery of an unlanded whole parent. This envelope is recoverable; do not author the tail or classify the function permanently blocked merely because its assigned start is wrong. Prior unmatched draft path is in docs/UNMATCHED_ROWS.tsv; recover original names/types and author the complete parent.

Proof: `review/sol2_parent_envelopes/00ba115c.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
