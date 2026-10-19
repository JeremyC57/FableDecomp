# Transfer combat-type definition settings

0046f7c8 (274 bytes): decorated manifest and RTTI establish global CCombatTypeDef, correcting an inherited namespace label. Actual CCombatTypeDef and CPersistContext shared declarations retained under aliases, with a documented native view of 12-byte STL containers. Donor types are also x86; the drift reflects container implementation sizes and a four-byte base difference, not a pointer-width conversion. Size pragma recovers the native register choice.

Main independently checked all21 native call targets and field addresses and passes optimized host and canonical VC7.1 byte/behavior tests. The expanded exact-source fixture distinguishes all eight helper types as well as context identity, call order and member offsets; native total size isac. Five HIGH authored grades. RELOCATION_MATCH helper bindings audited; root test-only helper-identity instrumentation leaves production unchanged.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
