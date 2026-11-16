// PKCBufferedReader::ReturnAllBuffers() -- hand back buffers (ReturnBuffer(0)) until the outstanding count at +0x68 is zero
// Leaf: ?ReturnAllBuffers@PKCBufferedReader@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B010, length 34 bytes.
// Callees: PKCBufferedReader::ReturnBuffer(bool*) @ 0x00C4AC70 (on `this`, a null pointer). Shape: the count is re-read after every
// call; the loop head is aligned with a `lea ebx,[ebx]` nop.
// Identity (NF42, round 5): `this` is the PKCBufferedReader of libpk/PKCBufferedReader.h (m_outstanding +0x68, the field the landed
// PollReadCompletion 0x00C4B160 / WaitForSlot 0x00C4AEB0 read); was the address-named class CBufferOwner_00c4b010 with a
// ReturnBuffer(long) stand-in. No libpk symbol exists for this method: ReturnAllBuffers is descriptive (the landed row's name), declared
// by the overlay opt-in FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS.
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS
#include "libpk/PKCBufferedReader.h"

void PKCBufferedReader::ReturnAllBuffers()
{
    while (m_outstanding != 0)
        ReturnBuffer(0);
}
