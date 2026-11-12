# 2026-11-12 landing fj -- 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions (wf17, after repair)

Source: `pending/2026-11-12_linux_handWF17/`. The row matched in workflow wf17 but modelled CTCPhysicsRigidBody locally (729 bytes
on the shared header's typed branch instead of 726). An Opus repair (`lane_tools/workflows/repair_and_verify_row.js`, 5 grades)
ported it onto the shared header: the typed branch of engine/CTCPhysicsRigidBody.h with the existing _BODY opt-in and the new
engine/CRigidBody.h, a new nested opt-in (RESOLVECOLLISIONS) declaring the private member, and a new STLport opt-in
`FABLE_RETAIL_STLP_VECTOR_INDEX_START` whose operator[] reads `_M_start` directly instead of through begin().
**Inferred:** that operator[] spelling is not STLport 4.6's text (`*(begin()+n)`); it is the same address arithmetic with one
inline level less, marked `Inferred:` in the source. The retail instructions that need it: the [esp+14h] index slot
(0x0075A7BA, 0x0075A7E0) kept beside the [esp+10h] byte offset (0x0075A7D4) and &config.AngularMomentum / &config.AngularVelocity
held in ebx / ebp (0x0075A7CB / 0x0075A7CE). With the default operator[] the control grade is DIFFER(729v726).
Mechanical verifier: ok. Evidence verifier: two documentation findings (stale NOTES bullets; the address range in the overlay
comment), fixed by the coordinator and regraded. RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. The test runs the real
member; its CMatrix3x4::operator* body is the landed 0x00A56530.
Headers (additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2): CTCPhysicsRigidBody.h, C3DVector.h
(CROSS_XZ_GROUP), CMatrix3x4.h (OPERATOR_MUL_VECTOR), stlport/fable_containers.h (VECTOR_INDEX_START).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Follow-up (queued, land1011fj, PORT): the landed sibling Simulate 0x0075AD50 still carries the same local model and should move
onto the header the same way.
