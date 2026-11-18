# CThingCreatureDef::Transfer

The VC7.1 object exports `?Transfer@CThingCreatureDef@@UAEXAAVCPersistContext@@@Z`. The source-order record in Ego_d identifies `CThingCreatureDef::Transfer`. The submitted extent is 1450 bytes.

- egor donor procedure at 006a39c0
- fablewin donor procedure at 01fb3ed0

- abs .rdata at 0x0125B324 (a data reference: in .rdata usually a vtable slot, see names/rtti-vtables.tsv; in .text an EH or jump table)

At 00671B06 the byte temporary and flag storage enter AL and CL in the opposite order while updating IsIndestructible. The corresponding shift and mask reverse as well.

The fixture checks each transferred field in order, each of eleven flag names and defaults, bit writeback, and sorted/unique versus binary-preserved navigator arrays.
