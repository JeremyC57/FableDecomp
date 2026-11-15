# Corrected whole-function envelope

Assigned `00d2988b..00d29da7` is a 1308-byte tail with 11 head bytes missing. The complete parent is `00d29880..00d29da7`, 1319bytes, `NScript::CQ_EndGameScript::CHookCoastTeleporter::Main`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `00d29da6 ret `; linear disassembly covers every byte through that return.

Parent landed: **no**. docs/UNMATCHED_ROWS.tsv explicitly lists full parent as unmatched; no landed_sources entry.

README_CODEX.md permits recovery of an unlanded whole parent. This envelope is recoverable; do not author the tail or classify the function permanently blocked merely because its assigned start is wrong. Prior unmatched draft path is in docs/UNMATCHED_ROWS.tsv; recover original names/types and author the complete parent.

Proof: `review/sol2_parent_envelopes/00d2988b.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. Whole-parent source recovery is active; attempts and current state are recorded in ROW.json and PROBES.md.
