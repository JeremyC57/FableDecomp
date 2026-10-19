# Creature action construction, animation timing, drink and missile handling

Independently verified five production bodies: 00693B30 (305 bytes, grade6), 00693DC0 (299, grade6), 00693030 (259, grade6), 00847AC0 (283, grade2), and 008D5790 (316, grade2). All compile with VC7.1 to RELOCATION_MATCH and pass actual-source behavior fixtures. Host C++98/O2 and no-elision fixtures also pass. Native layout assertions remain enabled in VC; only native pointer-width/layout checks are disabled on the host. No root source grades were added.

Main audited all 71 masked relocation targets against retail bytes, both constructors' complete emitted 70-slot action vtables, their two typed-pointer destructor tables, and the complete native 70-slot Drink and FireMissile tables. Actual headers are included, with explicit donor/native layout differences. Earlier incomplete constructor tables and Drink's incorrect fixture constructor signature were corrected within the existing author budgets before acceptance.

The two constructors preserve base and pointer construction order, native three-pointer vectors, map sentinel initialization, animation setup and action ID increments. The default-animation constructor calls the actual four-argument CAnimationEntry constructor; the supplied-animation constructor uses the real seven-argument API. Fixtures check state at the SetAnimation boundary and sequential IDs across two objects. Native map allocation's non-null precondition is preserved.

SetAnimation copies and retains the intrusive animation component, snapshots FPS before callbacks, uses the real world mesh bank at +0x3c, and preserves the two virtual delay calls and callback-mutated timing before clamping. Tests cover negative/zero/positive duration, negative fractions, null/shared components and callback mutation. This address had an old scratch model but no previous canonical accepted row.

Drink uses the genuine seven-argument base construction in the fixture. Main checked the definition helper's short-lived CCarryableDef string, GetPSubDefInfo slot14 and counted return/assignment; native lookup manager is at +0x1c. Tests cover dead/expired drink, initialization invalidation, prior held-object replacement and last definition-owner cleanup. Intelligent-pointer records and intrusive definition records retain their distinct lifetimes. Required carrying/definition objects remain native preconditions.

ReleaseMissile preserves the real weapon component, reload/carry-slot/shot APIs, optional animation component and PROJECTILE_WEAPON_FIRE literal. Tests cover missing/dead held object, negative carry slot, null missile, missing animation, negative shot definition, aliased output pointers and untouched output sentinels. Root confirms the native 5DA920 helper is GetMissileLoadCarrySlot despite propagated incorrect names.

Compiler matches and isolated behavioral fixtures do not constitute a linked whole-game runtime test. Immutable author hashes, root compiler output, relocation/vtable audits and fixtures are preserved with the batch.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
