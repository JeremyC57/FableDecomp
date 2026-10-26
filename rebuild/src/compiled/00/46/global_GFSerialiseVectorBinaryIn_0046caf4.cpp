// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<EActionRegisteredType>& out) -- read the count, resize the vector to it (new elements default-constructed), then read each element through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn@W4EActionRegisteredType@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@W4EActionRegisteredType@@V?$allocator@W4EActionRegisteredType@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046CAF4, 144 bytes, `__fastcall` (stream in ecx, the vector in edx, homed at [ebp-0Ch]), plain `ret`; ebp frame (a size-optimised /Os module: unaligned entry).
// Identity (proven): GFSerialiseVectorBinaryIn<EActionRegisteredType>(CMemoryDataInputStream&, vector<EActionRegisteredType>&) (FableWin.pdb name set); evidence: its caller is CPersistContext::Transfer(vector<EActionRegisteredType>&) 0x0046CA94, called by CCombatSequenceDef::Transfer 0x0046C6F0 (slot 18 of vtable 0x01234BBC, RTTI .?AVCCombatSequenceDef@@) on this+0x54 (Ego_r.pdb CCombatSequenceDef::ActionList at +0x58, vector<EActionRegisteredType>)
// Label: the manifest's "GFSerialiseVectorBinaryIn<CRGBColour>" is a propagated label: this row reads vector<EActionRegisteredType> (family J062, each row its own instantiation).
// Callees: vector<EActionRegisteredType>::resize(size_type, const EActionRegisteredType&) 0x0046CB84, CPersistTraits<EActionRegisteredType>::TransferBinaryIn 0x0045872C,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (FableWin.pdb, private).
// Shape: FableWin.pdb ??$GFSerialiseVectorBinaryIn@<T>: one traits object for the call (CPersistTraits<T> has a constructor and a destructor), the count
//   through the inline ReadSLONG (CDataInputStream::Read: 64-bit range check, memcpy from the source chunk, else ReadWithSrcChunkOverflow), resize with a
//   default element, then each element read in place; the count stays in its stack slot (no register left), the index in esi.
// Model: the traits body (0x0045872C) was compiled earlier in the retail TU, so it is visible here (an explicit member specialization is the device): the
//   traits object needs no store and shares the homed vector pointer's slot, as in GFSerialiseMapBinaryIn (families J047 / J060).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CDataInputStream CDataInputStreamPdb
#include "engine/CDataInputStream.h"
#undef CDataInputStream
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class CDataInputStream : public CDataInputStreamPdb {      // engine/CDataInputStream.h is data-only: the reader is added here
public:
    __forceinline void Read(void* buffer, long size)       // FableWin.pdb ?Read@CDataInputStream@@QAEXPAXJ@Z (inline here)
    {
        __int64 newPosition = (__int64)(unsigned long)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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
    __forceinline long ReadSLONG() { long value; Read(&value, sizeof(value)); return value; }                          // FableWin.pdb ?ReadSLONG@CDataInputStream@@QAEJXZ
    unsigned long ReadULONG() { unsigned long value; Read(&value, sizeof(value)); return value; }        // FableWin.pdb ?ReadULONG@CDataInputStream@@QAEKXZ
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    unsigned char* StreamSource;                           // +0x18
};
enum EActionRegisteredType { };                 // Ego_r.pdb (enumerators not needed)
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    ~CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
};
#pragma auto_inline(off)
template <> void CPersistTraits<EActionRegisteredType>::TransferBinaryIn(CMemoryDataInputStream& stream, EActionRegisteredType& value)
{
    EActionRegisteredType temp;
    stream.Read(&temp, sizeof(temp));
    value = temp;
}
#pragma auto_inline(on)
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp& reference;
    typedef unsigned int size_type;
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
    void resize(size_type __new_size, const _Tp& __x);     // 0x0046CB84
};
}
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count = stream.ReadSLONG();
    out.resize(count, T());
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<EActionRegisteredType >(CMemoryDataInputStream&, std::vector<EActionRegisteredType >&);