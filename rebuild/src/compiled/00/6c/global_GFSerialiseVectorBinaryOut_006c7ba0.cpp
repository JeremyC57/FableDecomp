// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, const vector<pair<long, COpinionDeed*> >& value) -- write the count, then each pair's long and deed pointer
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7BA0, length 80 bytes (__fastcall stream, value).
// Callees: WriteLong 0x00993EB0, CPersistTraits<COpinionDeed*>::TransferBinaryOut 0x006C5310 (same-TU noinline stand-in).
// Model: engine/CDataOutputStream.h + writer adapter.
// Shape: the empty pair traits is declared at function scope; its inline TransferBinaryOut writes `first` then declares a NAMED
//   `CPersistTraits<COpinionDeed*> secondTraits` for the pointer (an unnamed temporary gets its own frame slot).
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteLong(long value);                             // 0x00993EB0
};
struct COpinionDeed;
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
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
template <class T> struct CPersistTraits {
    CPersistTraits() { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;
};
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<COpinionDeed*>::TransferBinaryOut(CDataOutputStreamWriter& stream, COpinionDeed* const* value) const
{
    stream.WriteLong((long)*value);
}
#pragma auto_inline(on)
template <> struct CPersistTraits<std::pair<long, COpinionDeed*> > {
    CPersistTraits() { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const std::pair<long, COpinionDeed*>* value) const
    {
        stream.WriteLong(value->first);
        CPersistTraits<COpinionDeed*> secondTraits;
        secondTraits.TransferBinaryOut(stream, &value->second);
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
template void __fastcall GFSerialiseVectorBinaryOut<std::pair<long, COpinionDeed*> >(CDataOutputStreamWriter& stream, std::vector<std::pair<long, COpinionDeed*> >& value);