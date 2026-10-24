// CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryIn(CMemoryDataInputStream*, std::list<COpinionTransientOffset>*) -- binary persist read of an opinion-offset list: clear it, read the count, then read and append each 0x18-byte element
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479369, 143 bytes, `__fastcall` (the traits object in ecx is never read), `ret 8`;
//   ebp frame of a size TU (`#pragma optimize("y", off)` + `("s", on)`: the 4-byte memcpy is a `movsd`, `leave`).
// Callees: list::clear @ 0x00450855 (frees the nodes; propagated label "CGuiControlTreePane::SortTreeRecursively"),
//   CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, COpinionTransientOffset() @ 0x006815F0 (out of line;
//   propagated label "CVsyncCallbackTimerInfo"), COpinionTransientOffset::TransferBinaryIn(CMemoryDataInputStream*) @ 0x004793F8
//   (reads the 0x18-byte element), list::push_back(const T&) @ 0x00479449. Sibling TransferBinaryOut is landed at 0x0047931E.
// Shape: the count is read through an inlined CPersistTraits<long>::TransferBinaryIn (its own `value` local, then
//   `*out = value`): only then does VC7.1 home it in the stream-argument slot ([ebp+8]) although the stream stays live in
//   ebx -- reading straight into the function's `count` gives it a frame word of its own (`sub esp,1Ch`). The
//   loop counts it down (`dec esi ; jne`) and builds a fresh element in the 0x18-byte frame each pass (no destructor).
// The stream uses engine/CMemoryDataInputStream.h with an inline Read (the header's `position` is long; the sum is
// taken unsigned, matching retail's zero high dword).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)((unsigned long)position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};


class COpinionTransientOffset {                                      // 0x18 bytes
public:
    unsigned char m_data[0x18];
    COpinionTransientOffset();                                       // 0x006815F0
    void TransferBinaryIn(CMemoryDataInputStream* stream);           // 0x004793F8
};

namespace std {
template <class T> class list {
public:
    void* _M_node;
    void clear();                                                    // 0x00450855
    void push_back(const T& value);                                  // 0x00479449
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, T* out) const;
};

template <> __forceinline void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, long* out) const
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = value;
}

template <> void CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryIn(CMemoryDataInputStreamReader* stream, std::list<COpinionTransientOffset>* out) const
{
    out->clear();
    long count;
    CPersistTraits<long>().TransferBinaryIn(stream, &count);
    for (long i = count; i > 0; --i) {
        COpinionTransientOffset value;
        value.TransferBinaryIn(stream);
        out->push_back(value);
    }
}