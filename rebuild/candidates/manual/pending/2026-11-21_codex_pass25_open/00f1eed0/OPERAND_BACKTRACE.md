# PlayWave final operand backward trace

The selected source remains `qp19_stdlib_checkpoint`: 9262/9262 bytes, one raw SIB difference at 0x00F1FFBA. The instruction at 0x00F1FFB8 forms the address of the `HUDType` argument to the first creature counter's `AddQuestInfoCounterList` call. Both encodings compute the same address. This is an open byte mismatch, not a source fix or a match.

The two source expressions are:

```cpp
// First element of the selected wave's creature vector:
Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures.begin()
// Byte displacement of the selected creature:
i * sizeof(CArenaCreatureDef)
```

The final address adds `offsetof(CArenaCreatureDef, HUDType)` to their sum. In the retail ABI those sizes are 0x38 and 0x30. This is exactly the address of `Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[i].HUDType` in the selected production statement. These explanatory expressions were not added as locals or substitutions to the production source.

## Compiler evidence

A task-local instrumented copy of the actual VC7.1 c2.dll logs node hashes, comparison operands, imported local names and pre-lowering operand trees. It does not change the reconstruction. The emitted PlayWave section is byte-identical to the selected ordinary compiler result: SHA256 `b73b6160611788965582e546079e25f9b59e328b6eb391e1f727033d787befcd`. The preservation receipt and full private trace are under `runtime/playwave_c2/binary_trace`; no instrumented compiler or donor bytes are part of the handback.

The final address node has displacement 0x30 and exactly two leaves:

| Leaf | Identity | Meaning at the address node | Canonicalization key |
| --- | --- | --- | --- |
| Imported source local | Kind 4, symbol 0xb2e, imported name `_this` | Selected wave's `Creatures._M_start` | 0x165c1 |
| Compiler temporary | Kind 3, symbol 0x1d29 | Strength-reduced byte index `i * 0x38` | 0x14a40 |

The pre-lowering trace proves the receiver's preceding assignment in the same statement. It first combines the round index and round-vector start with the Waves offset, then combines the wave index and wave-vector start with the Creatures offset. The final nested load is assigned to symbol 0xb2e before the HUD address is formed. Thus the value's meaning is the creature vector's first-element pointer; it is not the round index, wave index, Game receiver, or another caller local.

The trace separately proves temporary 0x1d29 is initialized to zero at relative source line 193 (the first counter-loop header), used in the HUD address at relative line 199, and advanced/tested at the same loop header. Native generation places it at `[esp+0x64]`, advances it by 0x38 and terminates at 0xa8. The counter-array and TotalCreatures indices advance independently by four bytes. This excludes a hidden source declaration or a different creature index as the explanation.

The canonicalizer compares keys in descending order. The low key of the source local comes from its symbol ordinal with the local hash shift; the temporary uses the temporary shift. Those measured keys account for the compiled operand order. The trace does not reveal the original retail compilation's symbol ordinals.

The original imported spelling `_this` is directly observed; assigning that ordinal specifically to the source text of vector::operator[] versus its nested begin helper before cloning/CSE is still not independently established. Both refer to this same selected creature container. The data flow after expansion, including the exact container and pointer value, is established.

## Donor source and types

The matching debug donor's PlayWave source line 1411 performs three nested real non-const vector `operator[]` calls, in order:

| Container | Debug implementation entry |
| --- | --- |
| `vector<CArenaRoundDef>` | 0x03B098E0 |
| `vector<CArenaWaveDef>` | 0x03B08D80 |
| `vector<CArenaCreatureDef>` | 0x03B08CD0 |

The caller then selects HUDType and passes it to virtual slot 0x524. Its first counter index is the second declared local named `i`, a signed `long` at debug frame offset -224. The loop starts at zero, increments by one and compares with three. All three vector subscripts take an unsigned-int size_type argument (the donor's mangled parameter is `I`). The selected source has exactly that signed caller index and the container's normal conversion to size_type. There is no donor support here for at(), a raw pointer caller expression, a separate named receiver, or a different index ordinal.

The donor library is the Visual Studio 2010 Dinkumware library: the assertion records `d:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\vector` and `vector subscript out of range`. It has an assertion-bearing subscript body whose successful tail directly adds the scaled index to its first-element pointer. It does not call begin(). Its round/creature element strides are 0x40, its wave stride is 0x44, and HUDType is at 0x34; those debug class/container layouts differ from the retail STLport ABI. The actual STLport 4.6.2 `_vector.h` spells its release subscript as `*(begin() + __n)`. The selected package implements that form.

Two source-grounded library diagnostics were tested without altering the selected row:

- qp20 uses the existing original STLport reference/const_reference typedef and `__n` spelling: byte-identical to the selected object, one hunk.
- qp24 uses the existing direct-start subscript opt-in, corresponding to the donor library's arithmetic after debug checks: 9262/9262 bytes but 128 hunks. It is not selected.

Earlier discarded postincrement spellings of the creation and counter loops were also byte-neutral. There is no evidence supporting a new local or arbitrary declaration change to adjust the symbol pool.

## Evidence still missing

A permissible correction needs the original retail translation unit's exact expanded helper/configuration text or another donor-confirmed helper difference that explains the symbol-pool allocation preceding this statement. The debug donor proves the caller's access chain and index type, but its different library does not provide the exact release STLport inline-local sequence. The retail result alone does not identify which earlier helper body, declaration visibility, or expansion phase would change the measured local/temporary ranks. No such difference was established in this pass.

The other open issues remain separate: the donor gift loop differs from the selected retail gift conditional, and the inherited unused CDefString conversion COMDAT has a conflicting emitted body. Neither has been claimed as solved by this operand trace.

## Selected source and test receipts

Production source equals the qp19 candidate byte-for-byte. Both overlay headers equal the qp19 snapshots. The complete production PlayWave function equals the standalone test body byte-for-byte, SHA256 `930bd33d64a44c00bd1b461b5152d19d9aeb7f34a936d011f60708e332f18e58`. Native C++03 and VC7.1 /Od /W3 tests pass. All 321 relocated operands are audited; the raw comparison leaves only 0x00F1FFBA, including verification of all relative branch destinations.

A separate build of the current source used the full upstream STLport 4.6.2 library from the project reference materials. The engine's 12-byte vector<bool> requires the ordinary template instantiation in place of upstream's bit-packed specialization. With that representation chosen, every project ABI assertion passed. VC7.1 emitted precisely the same 9,262 leaf bytes and 321 symbol/offset bindings as with the project's excerpts. The known mismatch therefore persists even with the original vector implementation, allocator implementation and complete library declarations in this test.
