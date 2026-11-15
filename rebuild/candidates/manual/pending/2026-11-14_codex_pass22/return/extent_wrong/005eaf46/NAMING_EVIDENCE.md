## BOUNDS_FIX.md

# Corrected whole-function envelope

Assigned `005eaf46..005eb076` is a 304-byte tail with 22 head bytes missing. The complete parent is `005eaf30..005eb076`, 326bytes, `CTCTavernGame::SetStateAfterYesNo`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `005eb073 ret 0x14`; linear disassembly covers every byte through that return.

Parent landed: **yes**. names/landed_sources.tsv and catalog_names.tsv.

The original landed source is `00/5e/CTCTavernGame_SetStateAfterYesNo_005eaf30.cpp`. The supplied neighbor at the complete start covers this tail. Report EXTENT_WRONG without counting a duplicate new function.

Proof: `review/sol2_parent_envelopes/005eaf46.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
