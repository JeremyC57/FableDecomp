// Behaviour test for 0x006724F0 GFSerialiseVectorBinaryOut<ENavigatorType>: the stream receives the count followed by
// every element as a signed long, in order.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

struct CDataOutputStreamWriter {                            // records WriteSLONG (0x00993EB0) calls
    long Written[16];
    int  Count;
    void WriteSLONG(long value) { Written[Count++] = value; }
};

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace nav {
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

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<ENavigatorType> {
    ENavigatorType DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const ENavigatorType* value) const
    {
        stream.WriteSLONG(*value);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, nav::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

int main()
{
    ENavigatorType items[3] = { (ENavigatorType)2, (ENavigatorType)-1, (ENavigatorType)7 };
    nav::vector<ENavigatorType> v;
    v._M_start = items; v._M_finish = items + 3; v._M_end_of_storage = items + 3;
    CDataOutputStreamWriter s;
    s.Count = 0;
    GFSerialiseVectorBinaryOut<ENavigatorType>(s, v);
    if (s.Count != 4 || s.Written[0] != 3 || s.Written[1] != 2 || s.Written[2] != -1 || s.Written[3] != 7) {
        std::printf("006724f0_TEST FAIL\n"); return 1;
    }
    nav::vector<ENavigatorType> e;
    e._M_start = e._M_finish = e._M_end_of_storage = 0;
    s.Count = 0;
    GFSerialiseVectorBinaryOut<ENavigatorType>(s, e);
    if (s.Count != 1 || s.Written[0] != 0) { std::printf("006724f0_TEST FAIL\n"); return 1; }
    std::printf("006724f0_TEST PASS\n");
    return 0;
}