# Naming evidence

- Ego_r 0x00BD28A0 and FableWin 0x032FC840 give the exact leaf `?CreateDegenerateTriangleIndexBuffer@C3DPrimitive2@@AAE_NXZ`.
- The debug index_counts, degenerate_indices, total_indices, new_index_buffer and per-block scoped locals agree with native buffer counts, upload, and ownership transfer.
- Shared C3DPrimitive2, N3DPrimitive::CStaticBlock and scoped_ptr preserve their already established layouts. The native degenerate block flag is byte +9.

## Behavioral fixture

The fixture executes the selected method body across all 31 selection patterns for zero through four static blocks. In-memory index buffers check the copied runs, rebased starts, cleared blocks, allocation flags and byte count, lock ordering, ownership transfer, and destruction.
