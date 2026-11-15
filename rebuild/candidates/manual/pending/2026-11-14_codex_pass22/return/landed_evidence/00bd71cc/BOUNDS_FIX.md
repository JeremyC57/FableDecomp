# Corrected whole-function envelope

Assigned `00bd71cc..00bd79f4` is a 2088-byte tail with 28 head bytes missing. The complete parent is `00bd71b0..00bd79f4`, 2116bytes, `CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `00bd79f1 ret 0xc`; linear disassembly covers every byte through that return.

Parent landed: **no**. docs/UNMATCHED_ROWS.tsv explicitly lists full parent as unmatched; no landed_sources entry.

README_CODEX.md permits recovery of an unlanded whole parent. This envelope is recoverable; do not author the tail or classify the function permanently blocked merely because its assigned start is wrong. Prior unmatched draft path is in docs/UNMATCHED_ROWS.tsv; recover original names/types and author the complete parent.

Proof: `review/sol2_parent_envelopes/00bd71cc.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
