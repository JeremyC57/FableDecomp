# CCreatureDef::Transfer naming and layout evidence

The exact leaf ?Transfer@CCreatureDef@@UAEXAAVCPersistContext@@@Z occurs in both original PDB name sets. Native .rdata0125b484 is its CCreatureDef vtable slot; the complete native body ends with ret4. The original FableWin local is named persist; the selected source uses that name.

Ego_r_TYPES/FableWin_TYPES retain independent raw TPI class keys, signatures and member fields. Their debug vector/map widths differ (16 versus20); both contract independently to retail12bytes and yield all37 identical retail offsets. LAYOUT_PROOF reports the compiler assertions. CWoundMorphs/CExpressionSet own their actual CVectorMap members and complete nested entry storage. Both original raw enum tables agree on every EBodyLocation/EExpressionType constant.

RELOCATION_AUDIT audits all140 native operand sites. Existing canonical bindings are reused. Seven absent bindings are proved from original PDB families, native pointed-to owner fields and the matching binary-in/out pair. Release CPersistContext::Transfer<CRandomAppearanceMorph> drops the debug key argument, as the landed00676740 neighbour and this caller prove. Both PDBs identify the method family, even though neither emits this release one-argument spelling.

NAME_FIXES records three independent existing spelling corrections:00757500 is CWoundMorphs::TransferBinaryOut,00465820 transfers CDefString rather than float, and00465852 has an obsolete stand-in declaration. These corrections do not assert a target byte match.

All original package row/prior/neighbour sources were inspected; prior is byte-identical to best (SHA25647aa564a455251a21ea510995de85228e1c9270700486e0cae6d7b01a352688c). The historical ctor neighbour has a wrong RadiusWeight tail assumption; the current model follows both raw TPI layouts and the actual Transfer member addresses.
