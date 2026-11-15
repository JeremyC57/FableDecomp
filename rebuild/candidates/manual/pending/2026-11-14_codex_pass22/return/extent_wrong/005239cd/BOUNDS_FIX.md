# Corrected whole-function envelope

Assigned `005239cd..00523b3b` is a 366-byte tail with 13 head bytes missing. The complete parent is `005239c0..00523b3b`, 379bytes, `CThingManager::RuntimeSaveEntities`. The parent prologue establishes stack/register/flag state consumed by the assigned slice. The final native instruction is `00523b38 ret 0xc`; linear disassembly covers every byte through that return.

Parent landed: **yes**. names/landed_sources.tsv and catalog_names.tsv.

The original landed source is `00/52/CThingManager_RuntimeSaveEntities_005239c0.cpp`. The supplied neighbor at the complete start covers this tail. Report EXTENT_WRONG without counting a duplicate new function.

Proof: `review/sol2_parent_envelopes/005239cd.disasm`; structured report: `review/sol2_parent_envelopes/REPORT.json`. No source/compile attempt has been made for this recovery yet.
