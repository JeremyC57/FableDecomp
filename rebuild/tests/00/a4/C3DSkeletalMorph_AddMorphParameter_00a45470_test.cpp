// Behaviour test for 0x00A45470: C3DSkeletalMorph::AddMorphParameter (back-pointer, push_back, drop the cached combined morph)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T* operator->() const { return Data; }
    void Clear()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};
namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
static int g_overflows = 0;
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    // test stand-in for the out-of-line 0x00A46CB0: grow to old*2 (at least 1), append x
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int, bool)
    {
        ++g_overflows;
        unsigned int old = size();
        unsigned int len = old ? 2 * old : 1;
        _Tp* ns = (_Tp*)malloc(len * sizeof(_Tp));
        _Tp* nf = ns;
        if (old != 0)
            memcpy(ns, this->_M_start, old * sizeof(_Tp));   // moved bitwise (the model has no destructor)
        nf += (__position - this->_M_start);
        _Construct(nf, __x);
        ++nf;
        free(this->_M_start);
        this->_M_start = ns;
        this->_M_finish = nf;
        this->_M_end_of_storage._M_data = ns + len;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
class C3DSkeletalMorph;
class C3DSkeletalMorphTarget {
public:
    int Id;
};
class C3DSkeletalMorphParameter {
public:
    virtual void v00() { }
    float             CurrentValue;
    C3DSkeletalMorph* SkeletalMorph;
};
class CBaseClassNonCopyable {
public:
    virtual void v00() { }
};
class C3DSkeletalMorph : public CBaseClassNonCopyable {
public:
    _STL::vector<CCountedPointer<C3DSkeletalMorphParameter> > MorphParameters;
    CCountedPointer<C3DSkeletalMorphTarget>                     CombinedMorph;
    void NotifyParamUpdate() { CombinedMorph.Clear(); }
    void AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter);
};
void C3DSkeletalMorph::AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter)
{
    parameter->SkeletalMorph = this;
    MorphParameters.push_back(parameter);
    NotifyParamUpdate();
}

static int g_targetsDeleted = 0;
static void __fastcall DeleteTarget(void* data) { delete (C3DSkeletalMorphTarget*)data; ++g_targetsDeleted; }

int main()
{
    C3DSkeletalMorph morph;
    // a cached combined morph with a single reference: dropped and deleted by the add
    CCPPointerInfo* targetInfo = new CCPPointerInfo;
    targetInfo->RefCount = 1;
    targetInfo->DeleteFunc = DeleteTarget;
    targetInfo->Data = new C3DSkeletalMorphTarget;
    morph.CombinedMorph.Data = (C3DSkeletalMorphTarget*)targetInfo->Data;
    morph.CombinedMorph.Info = targetInfo;

    C3DSkeletalMorphParameter param;
    param.SkeletalMorph = 0;
    CCPPointerInfo paramInfo;
    paramInfo.RefCount = 1;
    paramInfo.DeleteFunc = 0;
    paramInfo.Data = &param;
    CCountedPointer<C3DSkeletalMorphParameter> p;
    p.Data = &param;
    p.Info = &paramInfo;

    morph.AddMorphParameter(p);
    Check(param.SkeletalMorph == &morph);
    Check(morph.MorphParameters.size() == 1 && morph.MorphParameters._M_start[0].Data == &param && paramInfo.RefCount == 2);
    Check(morph.CombinedMorph.Data == 0 && morph.CombinedMorph.Info == 0 && g_targetsDeleted == 1);

    // more adds with no cached morph; the fourth takes the in-place push_back arm
    morph.AddMorphParameter(p);
    morph.AddMorphParameter(p);
    Check(morph.MorphParameters.size() == 3 && paramInfo.RefCount == 4 && _STL::g_overflows == 3);
    morph.AddMorphParameter(p);
    Check(morph.MorphParameters.size() == 4 && paramInfo.RefCount == 5 && _STL::g_overflows == 3);
    Check(morph.CombinedMorph.Info == 0 && g_targetsDeleted == 1);
    free(morph.MorphParameters._M_start);
    if (g_fail) {
        printf("00a45470_TEST FAIL\n");
        return 1;
    }
    printf("00a45470_TEST PASS\n");
    return 0;
}