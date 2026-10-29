// CPODVector_0046fee9::TransferBinaryIn(CDataInputStream*) -- read a vector of 0x18-byte POD elements: the count, a resize filled with zeroed elements, then the raw element block
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046FEE9, 231 bytes, `ret 4` (__fastcall on the vector itself, the stream on the stack).
// Label: the catalog name "CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryIn" is propagated:
//   `this` is the vector, not a traits object -- its only caller, the persist-context transfer helper 0x0046FE40,
//   calls it as `value->TransferBinaryIn(inputStream)` with the value in ecx (the write twin is 0x0046FE8A). The
//   element type is unknown; it is modelled by size (0x18 bytes) and the class named by address.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path, x2),
//   vector::resize(unsigned long, const T&) @ 0x0046FFD0.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: the WoundMorphs read idiom (landed 0x00757990): count read into the dead stream-argument slot, resize with a
//   value-initialised element temporary (`rep stosd` x6 at [ebp-1Ch]), data = `size() > 0 ? first : (void*)this`,
//   `if (bytes > 0)` Read(data, bytes) with the `size <= remaining` test and the intrinsic memcpy; `this` is spilled
//   to [ebp-4] across the first read. Size-optimised TU: #pragma optimize("s", on) (ebp frame, `leave`).
#pragma optimize("s", on)
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct SPODElement_0046fee9 { long _words[6]; };

struct CPODVector_0046fee9 {            // STLport vector, 12 bytes
    SPODElement_0046fee9* _M_start;
    SPODElement_0046fee9* _M_finish;
    SPODElement_0046fee9* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    __forceinline SPODElement_0046fee9* data() { return size() > 0 ? _M_start : (SPODElement_0046fee9*)this; }
    void resize(unsigned long count, const SPODElement_0046fee9& value);   // 0x0046FFD0
    void TransferBinaryIn(CDataInputStream* stream);
};

void CPODVector_0046fee9::TransferBinaryIn(CDataInputStream* stream)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    unsigned long count;
    in->Read(&count, sizeof(count));
    resize(count, SPODElement_0046fee9());
    SPODElement_0046fee9* elements = data();
    long bytes = size() * sizeof(SPODElement_0046fee9);
    if (bytes > 0)
        in->Read(elements, bytes);
}