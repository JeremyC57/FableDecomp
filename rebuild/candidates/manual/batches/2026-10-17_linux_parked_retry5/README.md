# Parked-row retry batch 5

Four candidates passed the independent canonical VC7.1/Wine dry run and actual-implementation behavior fixtures under both g++ and VC7.1. Final landing evidence is recorded in landing_evidence.txt.

- 004581f8: CSpecialAbilitiesUnholyPowerDef constructor; 00458033: CSpecialAbilitiesDivineWrathDef constructor; 188 bytes each. Volatile 32-bit scalar stores preserve retail ordering before the padded vector members. The quarantined derived header has a different vector layout. The flattened generated base header describes a 0x3d-byte layout and lacks the external constructor declaration; these isolated translation units retain the observed 0x3c-byte base model. The verifier reports the base-header availability warning.
- 0074e080: CTCContainerRewardHero::AdjustTrajectoryVector; 183 bytes. Corrected `(int bias, C3DVector* vector)` signature and x87 expression grouping. Tests call the actual implementation for axis-aligned and mixed vectors with varied bias, using independent square-root expectations.
- 0094c850: CAIStateGroup_GameMaster::InitCreaturePosition; 169 bytes. An explicit temporary scope fixes pointer cleanup ordering; a separately named placement receiver fixes call setup. Tests cover missing/dead buildings, no seat, placement failure/success, temporary destruction, and changing the creature during the virtual seat query. Shared CAI base/brain headers are reused; a retail work-building view documents its offset difference from the donor.

Only q-confirmed and independently verified sources are included. No assembly bakes.

Owner correction for 004581f8: retail stores vtable 0x012348e4, whose RTTI identifies CSpecialAbilitiesUnholyPowerDef; slot-0 deleting wrapper 0x004582ba calls destructor 0x004582d6. The DivineWrath sibling instead uses vtable 0x0123487c and wrapper 0x004580f5. Renamed source/test/catalog/payload/oracle in place without a duplicate landing. Canonical corrected-source parity and behavior recheck: owner_correction_evidence.txt. The historical landing log retains the old propagated label.
