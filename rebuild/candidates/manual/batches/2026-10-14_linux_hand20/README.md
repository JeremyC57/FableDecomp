# Batch `2026-10-14_linux_hand20` — delegated hand chunk 20 (CChunkedFile .. CComponent): 38 bodies (38 landed)

Twentieth delegated hand chunk of the Wine lane (`$S/w22/chunk20.tsv`, 40 small-module bodies of
17–96 bytes authored by a background agent against the live VC7.1 compiler, re-proved through the
lander's own parity and behaviour checks): a 20-byte combat-action node pool and its recycler in
the 0x00C3Cxxx module (`CCombatActionPool_00c3c9b0::PushActive/PushFree`,
`CCombatActionManager_00c3d610::RecycleAction`), the `CTCBribable` bribe-amount getter, four
STLport `std::vector` copy constructors and a 60-byte-element vector destructor (propagated
"ResetRings" labels), `vector::at` / `vector<char>::clear()` instances, `CCombatSequence*`
getters confirmed by vtables 0x01278FEC / 0x01261D74 (the base is 0x28 bytes, so the generic
header's +0x2c def slot is +0x28 in retail) and the `CComponent` sprite accessors confirmed by the
`NUISystem::CSprite` vtable.

Label corrections are in the batch sources. Parked (scheduling only): 0x008B0AB0 (push-edi
placement between vector zero stores), 0x0080C140 (dead `fxch` pair before `fcompp`).

All graded `RELOCATION_MATCH` / `MATCH` + `PASS` on 2026-10-14.
