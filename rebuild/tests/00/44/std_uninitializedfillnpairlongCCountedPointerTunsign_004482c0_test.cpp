// test for 004482c0 __uninitialized_fill_n<pair<long,CCountedPointer>*>
#include <stdio.h>
#include <new>
class CCountedValue;
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__cdecl* DeleteFunc)(void*);                   // +0x04
    void*         Data;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info) { if (Info) ++Info->RefCount; }
};
namespace std {
struct __false_type {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
typedef pair<long, CCountedPointer<CCountedValue> > CKeyedCountedPointer;
template CKeyedCountedPointer* __fastcall __uninitialized_fill_n<CKeyedCountedPointer*, unsigned int, CKeyedCountedPointer>(CKeyedCountedPointer*, unsigned int, const CKeyedCountedPointer&, const __false_type&);
}

int main()
{
    CCPPointerInfo info = { 1, 0, 0 };
    unsigned long xraw[3] = { 7, 0x55, (unsigned long)&info };
    const std::CKeyedCountedPointer& x = *(const std::CKeyedCountedPointer*)xraw;
    unsigned long d[12];
    std::CKeyedCountedPointer* f = (std::CKeyedCountedPointer*)d;
    std::CKeyedCountedPointer* e = std::__uninitialized_fill_n(f, 3u, x, std::__false_type());
    int ok = e == f + 3 && info.RefCount == 4 && d[0] == 7 && d[1] == 0x55 && d[8] == (unsigned long)&info && sizeof(std::CKeyedCountedPointer) == 12;
    xraw[2] = 0; ok = ok && std::__uninitialized_fill_n(f, 1u, x, std::__false_type()) == f + 1 && d[2] == 0 && info.RefCount == 4;
    ok = ok && std::__uninitialized_fill_n(f, 0u, x, std::__false_type()) == f;
    printf(ok ? "004482c0_TEST PASS\n" : "004482c0_TEST FAIL\n");
    return 0;
}