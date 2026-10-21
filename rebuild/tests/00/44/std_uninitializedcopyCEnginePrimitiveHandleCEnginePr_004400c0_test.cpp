#include <cstdio>
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
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CEnginePrimitiveHandle* __fastcall __uninitialized_copy<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const __false_type&);
}

CBaseClass::~CBaseClass() {}
void CEnginePersistentPrimitiveBase::v000() {}
void CEnginePrimitiveHandle::v001() {}
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 1;
    static double src[3]; static double dst[8];
    unsigned long* raw = (unsigned long*)src;
    raw[0] = 0x1234; raw[1] = (unsigned long)&prim; raw[2] = 0x1234; raw[3] = 0; raw[4] = 0x1234; raw[5] = (unsigned long)&prim;
    CEnginePrimitiveHandle* s = (CEnginePrimitiveHandle*)src;
    CEnginePrimitiveHandle* h = (CEnginePrimitiveHandle*)dst;
    CEnginePrimitiveHandle* e = std::__uninitialized_copy(s, s + 3, h, std::__false_type());
    unsigned long* d = (unsigned long*)dst;
    int ok = e == h + 3 && prim.RefCount == 3 && d[1] == (unsigned long)&prim && d[3] == 0 && d[5] == (unsigned long)&prim
        && d[0] != 0 && d[0] == d[2] && d[0] == d[4] && d[6] == 0;
    ok = ok && std::__uninitialized_copy(s, s, h, std::__false_type()) == h && prim.RefCount == 3;
    std::printf(ok ? "004400c0_TEST PASS\n" : "FAIL\n");
    return 0;
}