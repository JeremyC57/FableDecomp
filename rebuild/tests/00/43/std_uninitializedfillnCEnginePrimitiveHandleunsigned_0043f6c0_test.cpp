// test for 0043f6c0 __uninitialized_fill_n<CEnginePrimitiveHandle*>
#include <stdio.h>
#include <new>
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                       // +0x04
};
class CIVCountedPointer {                                // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* P;
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
class CEnginePrimitiveHandle : public CBaseClass {       // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                              // +0x04
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), Data(x.Data) {}
    virtual void v001();
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template CEnginePrimitiveHandle* __fastcall __uninitialized_fill_n<CEnginePrimitiveHandle*, unsigned int, CEnginePrimitiveHandle>(CEnginePrimitiveHandle*, unsigned int, const CEnginePrimitiveHandle&, const __false_type&);
}

CBaseClass::~CBaseClass() {}
void CEnginePersistentPrimitiveBase::v000() {}
void CEnginePrimitiveHandle::v001() {}
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 1;
    static double xbuf[2]; static double src[2];
    CEnginePrimitiveHandle* proto = (CEnginePrimitiveHandle*)src;
    static double dst[8];
    CEnginePrimitiveHandle* h = (CEnginePrimitiveHandle*)dst;
    unsigned long* raw = (unsigned long*)src; raw[0] = 0x1234; raw[1] = (unsigned long)&prim;
    CEnginePrimitiveHandle* e = std::__uninitialized_fill_n(h, 3u, *proto, std::__false_type());
    unsigned long* d = (unsigned long*)dst;
    int ok = e == h + 3 && prim.RefCount == 4 && d[1] == (unsigned long)&prim && d[5] == (unsigned long)&prim && d[0] != 0 && d[0] == d[2];
    ok = ok && std::__uninitialized_fill_n(h, 0u, *proto, std::__false_type()) == h && prim.RefCount == 4;
    raw[1] = 0; ok = ok && std::__uninitialized_fill_n(h, 1u, *proto, std::__false_type()) == h + 1 && d[1] == 0;
    printf(ok ? "0043f6c0_TEST PASS\n" : "0043f6c0_TEST FAIL\n");
    return 0;
}