## BOUNDS_FIX.md

# Corrected whole-function envelope

Assigned `005eb08d..005eb180` is a 243-byte tail with 13 head bytes missing. The complete parent is `005eb080..005eb180`, 256bytes, `CTCTavernGame::SetStateAfterOK`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `005eb17d ret 0xc`; linear disassembly covers every byte through that return.

Parent landed: **yes**. names/landed_sources.tsv and catalog_names.tsv.

The original landed source is `00/5e/CTCTavernGame_SetStateAfterOK_005eb080.cpp`. The supplied neighbor at the complete start covers this tail. Report EXTENT_WRONG without counting a duplicate new function.

Proof: `review/sol2_parent_envelopes/005eb08d.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
