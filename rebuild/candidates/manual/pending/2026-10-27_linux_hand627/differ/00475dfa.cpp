// GFSerialiseVectorBinaryIn<C2DVector>(CMemoryDataInputStream& stream, std::vector<C2DVector>& out) -- read the element count, resize the vector, then read every 2D vector into place
// Grade: DIFFER(152v152) -- register allocation only: retail keeps the loop index in edi and the reloaded _M_start in esi (`mov [esi+edi*8],ecx`); ours swaps them
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475DFA, 152 bytes, __fastcall (stream ecx, out edx), plain `ret`.
// Identity: FableWin.pdb ??$GFSerialiseVectorBinaryIn@VC2DVector@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VC2DVector@@
//   V?$allocator@VC2DVector@@@std@@@std@@@Z (template on the element, CMemoryDataInputStream&, the STLport vector with allocator).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read's slow path), vector<C2DVector>::resize(
//   size_type, const C2DVector&) 0x00475E92, CDataInputStream::Read2DVector() 0x00995030 (Ego_r.pdb
//   ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ; landed as ReadC2DVector -- NAME_FIXES.tsv).
// Shape: the count read straight into its frame slot by the inlined Read (4-byte movsd); STLport resize(n) = resize(n, _Tp())
//   with an uninitialised C2DVector temporary whose slot the per-element Read2DVector result reuses; `out[i]` taken before
//   the read. Size TU: optimize("y",off) + ("s",on).
// Tried this round: real names/templates (this file), a named start-pointer local, operator[] as *(_M_start + n) / _M_start[n]
//   (index moves to edi but the element address is materialised: `lea esi,[eax+edi*8]`, 153 bytes); toy sweeps of index
//   type x operator[] spelling x size_type, while/post-increment loops, an inline GFSerialiseBinaryIn(stream, T&) helper,
//   local names. begin()-based indexing always gives index esi / start edi. Next: lane gap: index/base register choice.
// Leaf: ??$GFSerialiseVectorBinaryIn@VC2DVector@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_CDATAINPUTSTREAM_FRIEND class CMemoryDataInputStream
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C2DVector { public:
    float X;                                                // +0x00
    float Y;                                                // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class CDataInputStream { public:
    void*          __vftable;                               // +0x00
    unsigned long  StreamPos;                               // +0x04
    long           StreamSize;                              // +0x08
    unsigned char* CurrentSourcePtr;                        // +0x0c
    unsigned long  SourceChunkStreamPos;                    // +0x10
    long           SourceChunkBytesRemaining;               // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    C2DVector Read2DVector();                               // 0x00995030
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream { public:
    unsigned char* StreamSource;                            // +0x18
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
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
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    long count;
    stream.Read(&count, sizeof(count));
    out.resize(count);
    for (long i = 0; i < count; ++i)
        out[i] = stream.Read2DVector();
}

template void __fastcall GFSerialiseVectorBinaryIn<C2DVector>(CMemoryDataInputStream& stream, std::vector<C2DVector>& out);
