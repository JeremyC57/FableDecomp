## BOUNDS_FIX.md

# Corrected whole-function envelope

Assigned `0074b77c..0074b8fb` is a 383-byte tail with 12 head bytes missing. The complete parent is `0074b770..0074b8fb`, 395bytes, `CTCCreatureGenerator::InitialiseCreatureGenerator`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `0074b8f8 ret 4`; linear disassembly covers every byte through that return.

Parent landed: **yes**. names/landed_sources.tsv and catalog_names.tsv.

The original landed source is `00/74/CTCCreatureGenerator_InitialiseCreatureGenerator_0074b770.cpp`. The supplied neighbor at the complete start covers this tail. Report EXTENT_WRONG without counting a duplicate new function.

Proof: `review/sol2_parent_envelopes/0074b77c.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
