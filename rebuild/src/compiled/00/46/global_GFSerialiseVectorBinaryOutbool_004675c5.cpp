// GFSerialiseVectorBinaryOut<bool>(CDataOutputStream& stream, vector<bool>& value) -- write the element count, then every flag through CPersistTraits<bool>
// Leaf: ??$GFSerialiseVectorBinaryOut@_N@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004675C5, 49 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; size TU
//         without a frame, like its float twin 0x00464CED.
// Callees: CDataOutputStream::WriteLong(long) @ 0x00993EB0 (the count; catalogued "WriteCBYTE"), CDataOutputStream::WriteBool(bool)
//          @ 0x00993E30 (each element; catalogued WriteEBOOL, landed: writes the flag as one 0/1 byte).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryOut@_N@@YIXAAVCDataOutputStream@@AAV?$vector@_N...; the count is the
//   byte difference (1-byte elements, retail vector<bool> is a plain byte vector) and each element is `movzx` -> WriteBool.
//   Paired with GFSerialiseVectorBinaryIn<bool> 0x004675F6 (callers 0x004675B2 and 0x00CDCFBA). The catalog label
//   <C2DVector> is wrong.
// Layout: engine/CDataOutputStream.h is data-only; the writer methods ride on a methods view; the vector is the retail
//   STLport {start, finish, end_of_storage}.
// Shape: the landed GFSerialiseVectorBinaryOut<NInventory::CItem> 0x005C12A1 template: count = size(), WriteLong, then
//   `traits.TransferBinaryOut(stream, &value[i])` for i < count; CPersistTraits<bool>'s transfer is inline
//   (`stream.WriteBool(*value)`) and the uninitialised traits folds away.
#pragma optimize("s", on)
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteEBOOL(bool);   // 0x00993E30
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
template <> struct CPersistTraits<bool> {
    bool DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const bool* value) const { stream.WriteEBOOL(*value); }
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

template void __fastcall GFSerialiseVectorBinaryOut<bool>(CDataOutputStreamWriter& stream, std::vector<bool>& value);