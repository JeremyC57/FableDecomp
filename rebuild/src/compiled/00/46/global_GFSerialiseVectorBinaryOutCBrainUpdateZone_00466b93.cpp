// GFSerialiseVectorBinaryOut<CBrainUpdateZone>(CDataOutputStream& stream, vector<CBrainUpdateZone>& value) -- write the element count, then each zone's three longs through CPersistTraits<CBrainUpdateZone>
// Leaf: ??$GFSerialiseVectorBinaryOut@VCBrainUpdateZone@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00466B93, 88 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; size TU
//         without a frame (`push 0Ch ; pop ecx` divisor, the loop counter in the `push ecx` stack word).
// Callees: CDataOutputStream::WriteLong(long) @ 0x00993EB0 (the count and the three members; catalogued "WriteCBYTE").
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryOut@VCBrainUpdateZone@@@@YI...; PDB CBrainUpdateZone (12 bytes) =
//   { long Distance; long UpdateFrequency; long InterruptionFrequency; }. Its only caller is the persist-context binary
//   transfer 0x00466B65 (call site 0x00466B80), whose load arm calls GFSerialiseVectorBinaryIn<CBrainUpdateZone> 0x00466BEB
//   (traits TransferBinaryIn 0x00466C8E, Ego_r ?TransferBinaryIn@?$CPersistTraits@VCBrainUpdateZone@@@@). The catalog label
//   <CActionInputControl> is propagated.
// Layout: engine/CDataOutputStream.h with a methods view; retail STLport vector {start, finish, end_of_storage}.
// Shape: the landed 0x005C12A1 template: count = size() (`idiv` by 12), WriteLong, then the inline traits transfer per element
//   (three WriteLongs in member order); the zero-initialised traits is never read and folds away; the loop index becomes a
//   byte offset (ebp) and a down-counter. The size TU keeps the three-call traits transfer out of line unless __forceinline.
#pragma optimize("s", on)
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

class CBrainUpdateZone {
public:
    long Distance;                      // +0x00
    long UpdateFrequency;               // +0x04
    long InterruptionFrequency;         // +0x08
    CBrainUpdateZone() : Distance(0), UpdateFrequency(0), InterruptionFrequency(0) {}
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
template <> struct CPersistTraits<CBrainUpdateZone> {
    CBrainUpdateZone DefaultVal;
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CBrainUpdateZone* value) const
    {
        stream.WriteSLONG(value->Distance);
        stream.WriteSLONG(value->UpdateFrequency);
        stream.WriteSLONG(value->InterruptionFrequency);
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

template void __fastcall GFSerialiseVectorBinaryOut<CBrainUpdateZone>(CDataOutputStreamWriter& stream, std::vector<CBrainUpdateZone>& value);