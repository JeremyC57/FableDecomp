// GFSerialiseVectorBinaryOut<float>(CDataOutputStream& stream, vector<float>& value) -- write the element count, then every float through CPersistTraits<float>
// Leaf: ??$GFSerialiseVectorBinaryOut@M@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464CED, 54 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; size TU
//         without a frame (`fld ; push ecx ; fstp [esp]` is the size-TU float argument push).
// Callees: CDataOutputStream::WriteLong(long) @ 0x00993EB0 (the count; catalogued "WriteCBYTE"), CDataOutputStream::WriteFloat(float)
//          @ 0x00993EE0 (each element; catalogued "WriteCBYTE", the landed 0x0045E863 calls it with floats).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryOut@M@@YIXAAVCDataOutputStream@@AAV?$vector@M...; the element is
//   loaded with `fld dword` and passed to WriteFloat. Paired with GFSerialiseVectorBinaryIn<float> 0x00464D23 (callers
//   0x00464CDA and 0x0056A013).
// Layout: engine/CDataOutputStream.h is data-only; the writer methods ride on a methods view; the vector is the retail
//   STLport {start, finish, end_of_storage}.
// Shape: the landed GFSerialiseVectorBinaryOut<NInventory::CItem> 0x005C12A1 template: count = size() (`sar 2`), WriteLong,
//   then `traits.TransferBinaryOut(stream, &value[i])` for i < count; CPersistTraits<float>'s transfer is inline
//   (`stream.WriteFloat(*value)`) and the uninitialised traits folds away.
#pragma optimize("s", on)
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStreamWriter : CDataOutputStream {
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
template <> struct CPersistTraits<float> {
    float DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const float* value) const { stream.WriteFloat(*value); }
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

template void __fastcall GFSerialiseVectorBinaryOut<float>(CDataOutputStreamWriter& stream, std::vector<float>& value);