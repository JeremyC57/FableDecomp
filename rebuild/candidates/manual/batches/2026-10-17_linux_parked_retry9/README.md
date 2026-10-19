# Parked-row retry batch 9

Four recoveries, 793 bytes. Every source passed independent canonical parity and actual-source behavior tests under VC7.1/Wine and g++. Final results: landing_evidence.txt.

- 0045619e: CHeroStatsDef destructor, 166 bytes. RTTI vtable 0x012339dc leads through deleting wrapper 0x00456182 to this body. Eight owned allocations are freed around two member cleanup calls; the base destructor runs last. Tests check exact release/helper ordering and the null case.
- 00458111: CSpecialAbilitiesDivineWrathDef destructor, 231 bytes. RTTI vtable 0x0123487c leads through deleting wrapper 0x004580f5.
- 004582d6: CSpecialAbilitiesUnholyPowerDef destructor, 231 bytes. RTTI vtable 0x012348e4 leads through deleting wrapper 0x004582ba. Both ability destructors free fourteen pointers, then destroy the definition base. Tests exercise every free in order, including base cleanup, and all-null instances. The quarantined PDB headers conflict with these retail vector layouts; local models preserve observed offsets. The apparent 0x70 base model in the hero fixture is an opaque prefix through the first cleanup member, not a claim that the actual definition base has that extent.
- 008f5fa0: CHeightMap::AreaChanged, 165 bytes. Uses shared CHeightMapCell header for the target and retail map layout corroborated by landed CHeightMap::Update. Tests exercise rectangle queueing, all dirty cells and surrounding bytes, preservation of other bits, invalid/empty rectangles, the final queue slot, full/overfull queues, and untouched map state. Host fixture uses actual aligned cell objects.

Propagated landscape/strip labels were corrected before landing using RTTI and deleting wrappers. Neither naming nor nearby source alone was treated as ownership proof.
