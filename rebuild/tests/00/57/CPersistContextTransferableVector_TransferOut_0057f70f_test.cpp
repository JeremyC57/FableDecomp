// behaviour test for 0x0057F70F CPersistContext_TransferableVector<long>::TransferOut: element `index` reaches the SLONG writer by value
#include <cstdio>
class CPersistContext { public: int id; };
class CCharString { public: long written; int calls; };
class CPersistTraitsSLONG {
public:
    static void TransferOut(CPersistContext& context, CCharString& out, const long& value)
    {
        out.written = value + context.id * 0;
        out.calls++;
    }
};
template <class T> class CPersistTraits {
public:
    void TransferOut(CPersistContext& context, CCharString& out, const T& value)
    {
        long temp = value;
        CPersistTraitsSLONG::TransferOut(context, out, temp);
    }
};
template <class T> struct Vec { T* _M_start; T* _M_finish; T* _M_end_of_storage; T& operator[](unsigned int n) { return *(_M_start + n); } };
template <class T> class CPersistContext_TransferableVector {
public:
    Vec<T>* TransferableVector;
    void TransferOut(CPersistContext& context, CCharString& out, long index)
    {
        CPersistTraits<T> traits;
        traits.TransferOut(context, out, (*TransferableVector)[index]);
    }
};
int main()
{
    long data[4] = { 7, -3, 123456, 42 };
    Vec<long> v; v._M_start = data; v._M_finish = data + 4; v._M_end_of_storage = data + 4;
    CPersistContext_TransferableVector<long> t; t.TransferableVector = &v;
    CPersistContext c; c.id = 1;
    CCharString s; s.written = 0; s.calls = 0;
    for (long i = 0; i < 4; ++i) {
        t.TransferOut(c, s, i);
        if (s.written != data[i] || s.calls != i + 1) { std::printf("0057f70f_TEST FAIL\n"); return 1; }
    }
    if (data[1] != -3) { std::printf("0057f70f_TEST FAIL\n"); return 1; }
    std::printf("0057f70f_TEST PASS\n");
    return 0;
}