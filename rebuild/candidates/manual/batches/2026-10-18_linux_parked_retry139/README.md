# Surface blit, draggable update and ranged-weapon actions

Five independently verified bodies:009BF350 (310 bytes, grade2),0055A3E0 (298, grade7 including one compiler failure),008D5970 (291, grade8),0078B6B0 (268, grade1),0084A5F0 (293, grade2). All produce RELOCATION_MATCH and pass exact-source host C++98/O2, no-elision and VC7.1 behavior tests. Main audited69 body relocations,210 native action-vtable cells, two UI slots, three literals and two floating constants. Actual headers remain included with documented donor/native differences and VC assertions active. No root source grades.

Surface blit preserves saved rendering state, mutable render gate, EndScene/BeginScene failure semantics, stdcall D3D9 slots34/41/42 and owning GetSurfaceLevel return. Twelve scenarios check compatible fallback, format/width/height short circuits, ordering, all scene failures, gate mutation and balanced surface references. Native CSurface cleanup restores the actual vtable and calls Release.

Draggable Update uses the real ChangePosition slot31 and cursor slot153. Thirteen scenarios cover click-held/no-drag states, immediate/animated return, callback completion, epsilon equality, NaN movement and byte-layer wrap. Repeated vector comparisons and Time-delta behavior preserve retail evaluation order; no artificial padding or alignment was introduced to alter compiler scheduling.

FireMissileTest uses the genuine seven-argument constructor, full70-slot action base and const interface lookup4365B0. It rechecks Shot after MakeNoise, converts accuracy to float and calls fastcall CreateShot with its real ten arguments and temporary counted special-definition reference. Nine scenarios exercise null/dead/expired actor, zero/callback-cleared shot, missing carrying, null target and explicitly labeled escaped-reference cleanup stress.

HeroLoadRangedWeapon tests live weapon, optional weapon/animation components, ammunition null/expiry, and callback weapon replacement before the fresh animation lookup. Actual constructor78AE30 calls base693B30 with duration-1/fraction1; real LOAD and AIMING literals and Play/AddQueuedAnimation signatures are retained. Nine scenarios pass.

PickupGameObject retains the actual carryable-definition lookup and intrusive lifetime, active/passive slot choice, absent-slot behavior, real fastcall DropWeapon and reloaded PObject before insertion. Eleven scenarios include null/dead/expired object, wrong type, absent carrying, nonweapon, empty/occupied/absent held item, callback replacement and last definition owner. Native6A1230 is the carry-slot-empty query, not the propagated inventory label. The real constructor7EB030 and full70-slot table underpin the fixture.

All frozen hashes, compiler/landing output and masked-target/virtual audits are preserved. This is isolated compiler and fixture verification, not a linked whole-game runtime test.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
