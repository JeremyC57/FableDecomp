// Behaviour test for 0x0067F9B0: CPhysicalPrimitiveInit::StatRegisterClass() -- register the three hit-location primitive prototypes (sphere, cylinder, mesh): each new primitive is wrapped in a counted pointer and pushed onto CPhysicalPrimitiveInit::RegisteredPrimitives
// A copy of the source model and logic (compiled separately from src/0067f9b0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CDefString {
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

namespace NHitLocations {
class CPhysicalPrimitiveBase {
public:
    virtual ~CPhysicalPrimitiveBase() { }
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;
};
class CPhysicalPrimitiveSphere : public CPhysicalPrimitiveBase {   // vtable 0x0125B9FC
public:
    CDefString BaseName;                               // +0x04
    float      Radius;                                 // +0x08
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
class CPhysicalPrimitiveCylinder : public CPhysicalPrimitiveBase { // vtable 0x0125BA4C
public:
    CDefString BaseName;                               // +0x04
    float      Radius;                                 // +0x08
    float      Height;                                 // +0x0c
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
class CPhysicalPrimitiveMesh : public CPhysicalPrimitiveBase {     // vtable 0x0125B9B0
public:
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
}

struct CCPPointerInfo {
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    static void __fastcall DeleteData(void* data);     // 0x0067F650 for CPhysicalPrimitiveBase
    CCountedPointer(T* data) : Data(data)
    {
        Info = data ? new CCPPointerInfo(data, DeleteData) : 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __false_type _IsPODType;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
}

// Same-TU stand-in for vector<CCountedPointer<CPhysicalPrimitiveBase> >::_M_insert_overflow 0x0067FF30 (compiled in this
// TU): it only reads through `x` and never touches the tag, so VC7.1 keeps the pushed temporary's words in registers.

class CPhysicalPrimitiveInit {
public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
    static std::vector<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> > RegisteredPrimitives;   // 0x013BA888
    static void __fastcall StatRegisterClass();
};

void __fastcall CPhysicalPrimitiveInit::StatRegisterClass()
{
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveSphere));
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveCylinder));
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveMesh));
}

// ---- test stand-ins ----
static int g_lastAllocKind = 0, g_dataDeletes = 0, g_overflows = 0;
namespace NHitLocations {
CPhysicalPrimitiveBase* CPhysicalPrimitiveSphere::Alloc() const { g_lastAllocKind = 1; return const_cast<CPhysicalPrimitiveSphere*>(this); }
CPhysicalPrimitiveBase* CPhysicalPrimitiveCylinder::Alloc() const { g_lastAllocKind = 2; return const_cast<CPhysicalPrimitiveCylinder*>(this); }
CPhysicalPrimitiveBase* CPhysicalPrimitiveMesh::Alloc() const { g_lastAllocKind = 3; return const_cast<CPhysicalPrimitiveMesh*>(this); }
}
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data) { ++g_dataDeletes; delete (T*)data; }

template <class _Tp, class _Alloc>
void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
{                                                          // STLport growth: old + max(old, n)
    ++g_overflows;
    unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* p = this->_M_start; p != __position; ++p, ++__new_finish) _Construct(__new_finish, *p);
    for (unsigned int k = 0; k < __fill_len; ++k, ++__new_finish) _Construct(__new_finish, __x);
    for (_Tp* p = __position; p != this->_M_finish; ++p, ++__new_finish) _Construct(__new_finish, *p);
    for (_Tp* p = this->_M_start; p != this->_M_finish; ++p) p->~_Tp();
    std::free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}

std::vector<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> > CPhysicalPrimitiveInit::RegisteredPrimitives;

static int Fail(int line) { std::printf("0067f9b0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)
typedef CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PrimitivePtr;

int main()
{
    std::vector<PrimitivePtr>& list = CPhysicalPrimitiveInit::RegisteredPrimitives;
    list._M_start = list._M_finish = list._M_end_of_storage._M_data = 0;
    CPhysicalPrimitiveInit::StatRegisterClass();
    CHECK(list._M_finish - list._M_start == 3 && g_overflows == 3);           // 0 -> 1 -> 2 -> 4
    for (int i = 0; i < 3; ++i) {
        CHECK(list._M_start[i].Data != 0 && list._M_start[i].Info != 0 && list._M_start[i].Info->RefCount == 1);
        CHECK(list._M_start[i].Info->Data == list._M_start[i].Data);
        list._M_start[i].Data->Alloc();
        CHECK(g_lastAllocKind == i + 1);                                         // sphere, cylinder, mesh in order
    }
    CHECK(static_cast<NHitLocations::CPhysicalPrimitiveSphere*>(list._M_start[0].Data)->BaseName.TablePos == -1);
    CHECK(g_dataDeletes == 0);
    CPhysicalPrimitiveInit::StatRegisterClass();                                 // one push fits, two grow again
    CHECK(list._M_finish - list._M_start == 6 && g_overflows == 4);
    list._M_start[5].Data->Alloc();
    CHECK(g_lastAllocKind == 3 && list._M_start[3].Info->RefCount == 1);
    for (PrimitivePtr* p = list._M_start; p != list._M_finish; ++p) p->~PrimitivePtr();
    CHECK(g_dataDeletes == 6);
    std::free(list._M_start);
    list._M_start = list._M_finish = list._M_end_of_storage._M_data = 0;
    std::printf("0067f9b0_TEST PASS\n");
    return 0;
}