// CGameEvent::InitFromCompressedBuffer(const unsigned char* buffer) -- the inverse of CompressIntoBuffer: read the 16-bit type (bit 15 = Replacement), the player byte and the payload size, reset the event, append the payload into Data, and return the number of bytes consumed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F1870, length 124 bytes (+ int3 padding). `ret 4`.
// Callees: none (the payload copy is the inline memcpy: `rep movsd ; rep movsb`).
// Layout: engine/CGameEvent.h (Ego_r.pdb, packed, 0x28 bytes): Type +0, Player +4, Data +5, EndPos +0x25, Valid +0x26, Replacement +0x27
// (as the landed CompressIntoBuffer 0x009F1810, its exact inverse).
// Shape: the Replacement flag is the function's FIRST local (declared before the reads: its `xor al,al` lands between the player and
// size reads, and both header bytes are spilled to the shared frame word); the reset (EndPos = 0, Valid = true) precedes the append,
// whose destination &Data[EndPos] folds to Data while EndPos is re-read after the copy; the size widened once into a long (`movzx eax`)
// serves the copy, the EndPos add and the returned `size - buffer + cursor`.
#include <string.h>
#include "engine/CGameEvent.h"

struct CGameEventReader : CGameEvent {
    void Reset()
    {
        EndPos = 0;
        Valid = true;
    }
    void AddData(const unsigned char* data, long size)
    {
        memcpy(&Data[EndPos], data, size);
        EndPos = (unsigned char)(EndPos + size);
    }
    long InitFromCompressedBuffer(const unsigned char* buffer);
};

long CGameEventReader::InitFromCompressedBuffer(const unsigned char* buffer)
{
    bool replacement = false;
    const unsigned char* in = buffer;
    unsigned short type = *(const unsigned short*)in;
    in += 2;
    signed char player = (signed char)*in++;
    unsigned char size = *in++;
    if (type & 0x8000) {
        replacement = true;
        type &= 0x7fff;
    }
    Type = type;
    Player = player;
    Replacement = replacement;
    Reset();
    long length = size;
    AddData(in, length);
    in += length;
    return in - buffer;
}
