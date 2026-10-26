// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, const vector<pair<CCharString, long> >& value) -- write the count, then each named long's string and value
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D3A30, length 132 bytes (__fastcall stream, value; /EHsc-style frame).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, WriteLong 0x00993EB0, WriteNullTerminatedString 0x00995AE0.
// Model: engine/CDataOutputStream.h + writer adapter. The element is CNamedLong = pair<CCharString, long>.
// Shape: the traits specialisation's DefaultVal is built by IMPLICIT conversion (`DefaultVal("", 0)`), which gives retail's `lea`
//   re-address of the temporary; an explicit CCharString("") reuses eax and swaps a register.
#include "engine/CDataOutputStream.h"
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteLong(long value);                             // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
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
typedef std::pair<CCharString, long> CNamedLong;
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CNamedLong> {
    CNamedLong DefaultVal;
    CPersistTraits() : DefaultVal("", 0) { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CNamedLong* value) const
    {
        stream.WriteNullTerminatedString(value->first);
        stream.WriteLong(value->second);
    }
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CNamedLong>(CDataOutputStreamWriter& stream, std::vector<CNamedLong>& value);