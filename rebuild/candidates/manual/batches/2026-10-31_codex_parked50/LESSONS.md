# Parked50 lessons — 2026-10-31

These are observed outcomes of this pass, not guarantees for other rows. Exact trials are under history/.

| Row | Successful change | Qualification |
|---|---|---|
| 006f50b0 | Genuine STLport tree types, CCombatantInfo struct key/private layout, shared ownership and visible equal_range/distance context | The distance helper is cdecl here, corroborated by native pushes/caller cleanup; do not apply /Gr mechanically to every STL function. Trial02's flag parser consumed following source text; trial03 and fresh final verification supersede it. |
| 0044c6c2 | Correct four typed const-def handles and original manager/allocator hierarchy | Supplied baseline already had relocation-aware parity. Recovery here is type/name confidence plus constructor behavior verification. |
| 007f9010 | Original float current/last frame information, list model and original fastcall destruction signature | 131 bytes; 144 behavioral cases per compiler. |
| 009395f0 | Same-TU search context that reads the whole input filter/comparator, with original Brain references | 122 bytes. Search body is analysis-only; fixture replaces it with a contract mock. |
| 008c6160 | Real class tag/accessor and action virtual-prefix/PCreature layout | 87 bytes; original type/layout correction eliminated the codegen mismatch. |
| 007cbf60 | Signed indexed vector traversal plus original fastcall template signatures and shared def ownership | 241 bytes. Correcting inline declarations matters even when the only final calls are elsewhere. |
| 008bf1c0 | Direct nested GetPos arguments and real GFGetAngleXYTo/physics virtual signature | 178 bytes. Do not retain the invented AngleBetweenPositions helper identity. |
| 006772d0 | Real CDefString default constructors and all scalar/bitfield initialization in the constructor initializer list | 115 bytes. Default string construction is interleaved with later members in the required order. Tests independently check every native non-vptr byte over 256 fill patterns, including untouched bits. |
| 00845b10 | Declare the address-taken LowerBound key inside the successful HasTC arm, separately from its non-address-taken interface constant | 165 bytes. This prevents the key initialization being hoisted above the guard. The actual owner is CTCTavernGameMaster, with GetGame/GetIsBeingPlayed and EnterGame slot29. |
| 0085fb30 | Fully written failure exits plus HasTC written as if(bit) return true; return false | 135 bytes. The branch form avoids premature boolean materialization in this context. |
| 00c0acc0 | Define cursor/count after the float guard, then use a guarded do-while with explicit count/mask exits | 74 bytes. Allocation and loop rotation both mattered. Original identity remains unproven; native ABI and behavior are verified. |

## Useful improvements that did not match

- 00933db0: incremental target-radius then own-radius accumulation removed a four-byte x87 spill pair and reached 347/347 with the corrected KickCreature constructor and typed intelligent pointers. Fourteen normalized diff lines remain.
- 007e4530: branch-return HasTC and complete exits restored 159/159; only the final component pointer register remains different.
- 006d8790: original tree-find context and direct bit-test form restore 124/124 and EBX for the found iterator. The timer-store/parent-load scheduling and scratch registers still differ.
- 00660ae2: the original typed find loop exposes the hidden-result iterator slot, but a load through the returned pointer remains one byte shorter than retail. Do not replace the Torch specialization with another task type just to match bytes.
- 00809030: using the constructor's incoming target reference after Normalise instead of rereading the stored reference improves 172 to 169 bytes, still versus 166. The supplied exact-length model had incorrect references/interface types.
- 009e1dd0: the visible real extent-copy body makes code too small. The recovered global name is correct even though this particular compiler context is not.

## Non-wins worth retaining

Processor target changes /G7 and /G5 did not solve the tested short scheduling residues. Named values/references, complete exit chains, local scopes and same-file callee contexts were not universally helpful. Some full-object contexts reduced or increased required stores. All failed forms remain available; don't repeat them without a different reason.

No prohibited source trick, byte padding or out-of-scope target was used to force acceptance. A behavior PASS does not establish byte parity; a relocation-masked parity result does not establish name/ABI correctness by itself. The accepted rows passed all three checks separately.
