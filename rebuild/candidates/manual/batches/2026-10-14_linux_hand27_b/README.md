# Batch `2026-10-14_linux_hand27_b` — chunk 27 re-land: 00c6e660 (1 of 1)

The seven-argument virtual call mislabelled "CDataOutputStream::WriteCBYTE" (`CRenderCall_00c6e660::Call`): the zeroed
two-word record it passes by address holds two FLOATS -- their zeroes are immediate stores, so no zero register is created
and the four `push 0` stay immediates (an integer record made the lane share `xor eax,eax` across the pushes: 36 v 45 bytes).
See `_hand27/README.md` for the rest of the chunk.
