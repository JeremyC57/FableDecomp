# Transfer combat dialogue definition fields

00456dfb (242 bytes, /O1): manifest decorated signature and native RTTI table1233ebc slot18 establish CCombatDialogueDef::Transfer(CPersistContext&). Main disassembled the two scalar context helpers431061/431102 to confirm float and long transfer semantics. The native sizeof getter456df5 returns8c, and twenty member-reference sites establish fields3c..88. Generated donor header retained; it is four bytes larger, and the reason for that layout difference remains unresolved.

Main rebuilt a verbatim-source fixture with actual donor-header expansion and out-of-line context stubs, replacing the worker's parallel inline-body test. It independently passes hostO2 and VC7.1 checks for all twenty member addresses, types, order, mutations and native offsets/size. Three authored grades including the /O1 flag variant; repeated unchanged-source tests are separate verification. RELOCATION_MATCH helper identities audited. Comment-only evidence correction does not change emitted source semantics.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.

Post-landing header correction: the shared context declaration exists in fable_persist_context.h rather than engine/. Source now includes that actual declaration and derives a field-free method adapter. Authored grade4 remains RELOCATION_MATCH with /O1; main rebuilt the verbatim-source fixture and independently rechecked native+host PASS. Existing catalog entry updated without changing coverage.
