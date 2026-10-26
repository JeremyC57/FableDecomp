// GFSerialiseVectorBinaryOut<CHeroMarriageInfo>(CDataOutputStream& stream, vector<CHeroMarriageInfo>& value) -- write the marriage count, then every record through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F109, 67 bytes, __fastcall (stream ecx, vector edx) `ret`. Called by the
//   CHeroMarriageInfo vector's persist transfer (0x0058165D); the record writer is the landed
//   CPersistTraits<CHeroMarriageInfo>::TransferBinaryOut 0x0057A3CD. Retail keeps the hero's marriages in a vector
//   (Ego_r.pdb: list).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (the count, by value), the traits' TransferBinaryOut(stream,
//   const CHeroMarriageInfo*) @ 0x0057A3CD (thiscall on the 0x18-byte traits local).
// Shape: as the landed 0x005C12A1 (vector<CItem>): `CPersistTraits<T> traits; long count = value.size();
//   stream.WriteLong(count); for (long i = 0; i < count; ++i) traits.TransferBinaryOut(&stream, &value[i]);` --
//   the traits holds a DefaultVal record (0x18 frame bytes, no constructor code: CHeroMarriageInfo has no default
//   ctor), the element address strength-reduced to `begin() + 18h*i`, the count counted down. Size TU without a
//   frame pointer (unlike 0x005C12A1).
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"
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
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStream* stream, const T* value);           // 0x0057A3CD
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(&stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CHeroMarriageInfo>(CDataOutputStreamWriter& stream,
                                                                     std::vector<CHeroMarriageInfo>& value);