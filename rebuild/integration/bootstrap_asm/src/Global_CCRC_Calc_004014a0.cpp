#include "fable_crc.h"

// CCRC::Calc(fable_u32 seed, const void* data, fable_u32 byteCount) -- table-driven CRC-32 over a byte run
// Retail: Fable.exe (TLC, VC7.1) @ 0x004014A0, 56 bytes. Callees: none (reads g_FableCRCTable_0129A168).
//
// The engine's standard CRC-32 step. It folds `byteCount` bytes of `data` into `seed` using the 256-entry
// lookup table at g_FableCRCTable_0129A168, one byte per iteration: index the table with the low byte of
// the running value XOR the input byte, then shift the running value right by eight and XOR. Feeding the
// previous result back in as `seed` lets callers checksum a buffer in pieces, which is how the string
// hashing and save-game validation code use it.
//
// An empty run returns `seed` untouched.
//
// Matching: the `byteCount != 0` guard around the loop is separate from the loop test so the
// `test esi,esi; je` precedes the counted loop; `seed` lives in eax throughout.
fable_u32 FABLE_FASTCALL
FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount)
{
    if (byteCount != 0)
    {
        const fable_u8* bytes =
            static_cast<const fable_u8*>(data);
        // Classic reflected CRC-32 update, one byte at a time.
        for (fable_u32 index = 0; index < byteCount; ++index)
        {
            seed =
                g_FableCRCTable_0129A168[
                    bytes[index] ^ (seed & 0xFF)] ^
                (seed >> 8);
        }
    }
    return seed;
}
