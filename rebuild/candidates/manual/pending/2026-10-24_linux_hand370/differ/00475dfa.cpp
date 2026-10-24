// GFSerialiseVectorBinaryIn<C2DVector>(CDataInputStream& stream, std::vector<C2DVector>& out) -- read the element count, resize the vector, then read every 2D vector into place
// Grade: DIFFER(152v152) -- register allocation only: retail keeps the loop index in edi and the reloaded _M_start in esi (`mov [esi+edi*8],ecx`); this source swaps them. Fixed on the way: `resize(n)` must be __forceinline in the size TU (else an out-of-line call), and the store must index the reloaded start (`*(begin()+n)` or `begin()[i]`, not a `*(_M_start+n)` reference, which materialises `lea esi,[eax+edi*8]`). The CPersistTraits/ReadCount template-family shape (landed 0x005C2AD8) is worse (141v152).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475DFA, 152 bytes, __fastcall (stream ecx, out edx), plain `ret`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined Read's slow path),
// vector<C2DVector>::resize(size_type, const C2DVector&) @ 0x00475E92 (STLport: erase the tail or insert copies),
// CDataInputStream::ReadC2DVector() @ 0x00995030 (landed; by value through a hidden pointer).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: the count is read straight into its frame slot by the inlined CDataInputStream::Read (4-byte `movsd`, the
// position from the bounds test reused); STLport `resize(n)` = `resize(n, _Tp())` with an uninitialised C2DVector
// temporary (user default constructor), whose slot the per-element ReadC2DVector result reuses; `out[i]` is taken
// before the read. Same TU settings as the other GFSerialise templates: `optimize("y",off)` + `("s",on)`.
// Leaf: GFSerialiseVectorBinaryIn
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct C2DVector {
    float X;                                                // +0x00
    float Y;                                                // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    C2DVector ReadC2DVector();                              // 0x00995030
};

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00475E92
    __forceinline void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
};
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CDataInputStreamReader& stream, std::vector<T>& out)
{
    long count;
    stream.Read(&count, sizeof(count));
    out.resize(count);
    for (long i = 0; i < count; ++i)
        out[i] = stream.ReadC2DVector();
}

template void __fastcall GFSerialiseVectorBinaryIn<C2DVector>(CDataInputStreamReader& stream, std::vector<C2DVector>& out);
