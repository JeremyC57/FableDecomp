// GFSerialiseVectorBinaryOut<CCrimeWaitingForBribe>(CDataOutputStream& stream, vector<CCrimeWaitingForBribe>& value) -- write the element count, then each bribe record's deed handle and deed type through CPersistTraits<CCrimeWaitingForBribe>
// Leaf: ??$GFSerialiseVectorBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B3B80, 77 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; speed TU.
// Callees: CDataOutputStream::WriteLong(long) 0x00993EB0 (the count and both members).
// Identity: the landed CPersistContext::TransferVector<CCrimeWaitingForBribe> 0x007B5470 calls it in mode 3 (binary
//   save); Ego_r.pdb CCrimeWaitingForBribe (8 bytes) = { long DeedHandle; EOpinionDeedType Type; }.
// Shape: the landed GFSerialiseVectorBinaryOut template (0x00467090) in a speed TU: count = size() (`sar 3`), WriteLong,
//   then per element `traits.TransferBinaryOut(stream, &value[i])` inlined to two WriteLong calls; STLport operator[]
//   is `*(begin() + n)`, so the element address is re-formed from the start pointer (`mov eax,[ebx] ; lea ebx,
//   [eax+edi*8]`) and the vector pointer is spilled to the `push ecx` word. The traits' DefaultVal is a POD with no
//   constructor, so the traits object needs no frame.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStreamWriter : CDataOutputStream {
};

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CCrimeWaitingForBribe> {
    CCrimeWaitingForBribe DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CCrimeWaitingForBribe* value) const
    {
        stream.WriteSLONG(value->DeedHandle);
        stream.WriteSLONG(value->Type);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

template void __fastcall GFSerialiseVectorBinaryOut<CCrimeWaitingForBribe>(CDataOutputStreamWriter& stream, std::vector<CCrimeWaitingForBribe>& value);