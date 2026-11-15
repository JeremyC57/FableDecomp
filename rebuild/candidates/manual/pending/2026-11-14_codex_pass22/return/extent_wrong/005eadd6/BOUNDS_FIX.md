# Corrected whole-function envelope

Assigned `005eadd6..005eaf22` is a 332-byte tail with 22 head bytes missing. The complete parent is `005eadc0..005eaf22`, 354bytes, `CTCTavernGame::SetStateAfterYesNoAlt`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `005eaf1f ret 0x1c`; linear disassembly covers every byte through that return.

Parent landed: **yes**. names/landed_sources.tsv and catalog_names.tsv.

The original landed source is `00/5e/CTCTavernGame_SetStateAfterYesNoAlt_005eadc0.cpp`. The supplied neighbor at the complete start covers this tail. Report EXTENT_WRONG without counting a duplicate new function.

Proof: `review/sol2_parent_envelopes/005eadd6.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
