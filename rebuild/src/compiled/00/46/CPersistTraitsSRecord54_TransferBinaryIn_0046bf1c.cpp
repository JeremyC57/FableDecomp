// CPersistTraits<SRecord54>::TransferBinaryIn(CDataInputStream*, SRecord54*) -- binary persist read of one 0x54-byte record: range-checked 64-bit position, straight copy out of the current source chunk when it holds the whole record, else the windowed reader
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046BF1C, length 86 bytes.
// Pragma: #pragma optimize("s", on) -- `push 15h ; pop ecx` before the `rep movsd`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Identity: the manifest label "COpinionTransientOffset::TransferBinaryIn" is propagated: that class is
// 0x18 bytes and its reader (landed 0x004793F8) fills `this`; here `this` is never read and the
// destination is the SECOND argument (`ret 8`). The only caller (0x0046BF0C, a CArray loader) passes
// (stream, &array[i]) with ecx = a local traits object and steps the element by 0x54 -- the
// CPersistTraits<T>::TransferBinaryIn(stream, value) member shape. The 0x54-byte element type is not
// recoverable from the bytes, so it is modelled by size.
// Shape: identical to 0x004793F8 apart from the destination: the 64-bit guard on position + 0x54
// (constant-zero high dword compared like any __int64), `>=` on the remaining chunk bytes, the memcpy
// as `rep movsd` of 0x15 dwords, then cursor / remaining / position updated in that order.
#include <string.h>

struct CDataInputStream
{
    long           Pad00;                   // +0x00
    long           ReadPosition;            // +0x04
    long           Pad08;                   // +0x08
    unsigned char* Cursor;                  // +0x0C
    long           Pad10;                   // +0x10
    long           RemainingInSourceChunk;  // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);   // 0x00993CA0
};

struct SRecord54
{
    unsigned char Bytes[0x54];
};

struct CPersistTraitsRecord54              // CPersistTraits<SRecord54>
{
    void TransferBinaryIn(CDataInputStream* stream, SRecord54* value);
};

void CPersistTraitsRecord54::TransferBinaryIn(CDataInputStream* stream, SRecord54* value)
{
    __int64 nextPosition = (unsigned long)(stream->ReadPosition + 0x54);
    if (nextPosition >= 0 && nextPosition <= 0x7FFFFFFF)
    {
        if (stream->RemainingInSourceChunk >= 0x54)
        {
            memcpy(value, stream->Cursor, 0x54);
            stream->Cursor += 0x54;
            stream->RemainingInSourceChunk -= 0x54;
            stream->ReadPosition += 0x54;
        }
        else
        {
            stream->ReadWithSrcChunkOverflow(value->Bytes, 0x54);
        }
    }
}