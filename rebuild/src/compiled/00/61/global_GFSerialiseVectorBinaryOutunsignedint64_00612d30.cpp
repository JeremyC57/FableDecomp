// _global::GFSerialiseVectorBinaryOut<unsigned __int64>(CDataOutputStream& stream, vector<unsigned __int64>& value) -- write the element count, then every 64-bit value
// Leaf: ??$GFSerialiseVectorBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00612D30, 60 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Caller: 0x00619323 (the save half of a CPersistContext::TransferBinary over a vector<unsigned __int64>).
// Callees: CDataOutputStream::WriteSLONG 0x00993EB0 (the count; 4-byte writer through vtable slot 4) and the 8-byte writer
//   0x00993EC0 (`push 8 ; lea edx,[esp+8] ; call [eax+10h] ; ret 8`: the value by value, low word pushed last).
// Shape: the generic template of the landed 0x005C12A1 / 0x0046AFF4 with the unsigned __int64 traits inlined
//   (`CPersistTraits<unsigned __int64>::TransferBinaryOut` = one 8-byte write); speed TU (aligned loop head padded with
//   `lea esp,[esp]`, no pragma): the stream in ebp, the vector in ebx, the count in esi, the index in edi, the element
//   re-addressed through `begin()` each pass (`*(begin() + i)`).
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; the vector is STLport's three words.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteUHUGE(unsigned __int64);   // 0x00993EC0
};

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<unsigned __int64> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const unsigned __int64* value) const { stream.WriteUHUGE(*value); }
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
template void __fastcall GFSerialiseVectorBinaryOut<unsigned __int64>(CDataOutputStreamWriter& stream,
                                                                       std::vector<unsigned __int64>& value);