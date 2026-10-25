// Behaviour test for 0x0069A010: CGameCameraManager::InitManualCameraList() -- rebuild the manual camera list from the manager def's CameraList mode indices (each def that loads is pushed), then point the current and initial list iterators at the previously selected mode's new position, else at the def's InitialMode
// A copy of the source model and logic (compiled separately from src/0069a010.cpp) with test stand-ins for the callees.
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

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
class CDefString {
public:
    long Index;
    CDefString(long index = -1) : Index(index) { }
};
class CDefStringTable {
public:
    CCharString GetString(CDefString name);            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;             // slot 10 (+0x28)
};
class CDefPointeeBase : public CDefVirtual {
public:
    mutable long RefCount;                             // +0x04
};
class CCameraModeDef : public CDefPointeeBase { };

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) { T* object = other.Object; Object = object; if (object) ++object->RefCount; }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <>
void __fastcall __destroy_aux<CDefPointer<const CCameraModeDef>*>(CDefPointer<const CCameraModeDef>* __first,
    CDefPointer<const CCameraModeDef>* __last, const __false_type&);   // 0x0069D2B0
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <>
CDefPointer<const CCameraModeDef>* __fastcall __copy<CDefPointer<const CCameraModeDef>*, CDefPointer<const CCameraModeDef>*, int>(
    CDefPointer<const CCameraModeDef>* __first, CDefPointer<const CCameraModeDef>* __last,
    CDefPointer<const CCameraModeDef>* __result, const random_access_iterator_tag&, int*);   // 0x0069CB80
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

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
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
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
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
    template <class T> bool GetDef(long index, CDefPointer<T>* out) const;   // 0x0056D0C0 (COMDAT-folded)
};
class CGameDefinitionManager : public CDefinitionManager { };

#pragma auto_inline(off)
#pragma auto_inline(on)

// Same-TU stand-in for CDefinitionManager::GetDef<T> (0x0056D0C0, a template member compiled in the caller's TU): it fills
// the out pointer and never keeps its address, so the caller's def pointer is not treated as captured.
#pragma auto_inline(off)
#pragma auto_inline(on)
class CWorld {
public:
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;         // +0x10
};
class CCameraManagerDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    std::vector<long> CameraList;                      // +0x3c
    std::vector<long> SpecialityCameraList;            // +0x48
    long InitialMode;                                  // +0x54
};

class CGameCameraManager {
public:
    void* __vftable;                                   // +0x00
    unsigned char _pad04[0x74 - 0x04];
    CWorld* World;                                     // +0x74
    unsigned char _pad78[0x108 - 0x78];
    std::vector<CDefPointer<const CCameraModeDef> > CameraList;   // +0x108
    std::vector<CDefPointer<const CCameraModeDef> >::iterator CameraListIndex;              // +0x114
    std::vector<CDefPointer<const CCameraModeDef> >::iterator InitialModeCameraListIndex;   // +0x118
    unsigned char _pad11c[0x134 - 0x11c];
    CDefPointer<const CCameraManagerDef> PDef;         // +0x134
    void InitManualCameraList();
};

void CGameCameraManager::InitManualCameraList()
{
    CGameDefinitionManager* defManager = World->DefinitionManager;
    long currentMode = -1;
    if (CameraListIndex != CameraList.end()) {
        CCharString name = g_DefStringTable.GetString((*CameraListIndex)->GetDefName());
        currentMode = defManager->GetDefGlobalIndexFromName(name);
    }
    CameraList.clear();
    long defaultIndex = -1;
    long initialIndex = -1;
    long index = 0;
    for (std::vector<long>::const_iterator it = PDef->CameraList.begin(); it != PDef->CameraList.end(); ++it) {
        long mode = *it;
        CDefPointer<const CCameraModeDef> modeDef;
        if (defManager->GetDef(mode, &modeDef)) {
            CameraList.push_back(modeDef);
            if (mode == currentMode)
                initialIndex = index;
            if (mode == PDef->InitialMode)
                defaultIndex = index;
        }
        ++index;
    }
    CameraListIndex = InitialModeCameraListIndex = CameraList.begin() + (initialIndex < 0 ? defaultIndex : initialIndex);
}

// ---- test stand-ins ----
static int g_destroyed = 0;
CCharString::~CCharString() { }
CCharString CDefStringTable::GetString(CDefString name)
{
    CCharString s; s.PStringData = (void*)(long)name.Index;    // the "string" carries the def's global index
    return s;
}
CDefStringTable g_DefStringTable;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() const { ++g_destroyed; }
void CDefVirtual::v02() { } void CDefVirtual::v03() { } void CDefVirtual::v04() { } void CDefVirtual::v05() { }
void CDefVirtual::v06() { } void CDefVirtual::v07() { } void CDefVirtual::v08() { } void CDefVirtual::v09() { }
CDefString CDefVirtual::GetDefName() const { return CDefString(-1); }
class TestModeDef : public CCameraModeDef {
public:
    long Id;
    virtual CDefString GetDefName() const { return CDefString(Id); }
};
static TestModeDef* g_modes[8];
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { return (long)name.PStringData; }
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<T>* out) const
{
    if (index < 0 || index >= 8 || !g_modes[index]) return false;
    out->Object = (T*)g_modes[index];
    ++out->Object->RefCount;
    return true;
}
typedef CDefPointer<const CCameraModeDef> ModePtr;
namespace std {
template <>
void __fastcall __destroy_aux<ModePtr*>(ModePtr* __first, ModePtr* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <>
ModePtr* __fastcall __copy<ModePtr*, ModePtr*, int>(ModePtr* __first, ModePtr* __last, ModePtr* __result, const random_access_iterator_tag&, int*)
{
    for (int __n = (int)(__last - __first); __n > 0; --__n, ++__first, ++__result) {
        ModePtr* dst = __result;
        if (dst->Object != __first->Object) { dst->~ModePtr(); new (dst) ModePtr(*__first); }
    }
    return __result;
}
}
template <class _Tp, class _Alloc>
void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
{                                                          // STLport growth: old + max(old, n)
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

static int Fail(int line) { std::printf("0069a010_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)
static long IdAt(CGameCameraManager& m, int i) { return static_cast<const TestModeDef*>(m.CameraList._M_start[i].Object)->Id; }
static long IdAtIndex(CGameCameraManager& m) { return static_cast<const TestModeDef*>(m.CameraListIndex->Object)->Id; }

int main()
{
    static TestModeDef modes[8];
    for (int i = 0; i < 8; ++i) { modes[i].Id = i; modes[i].RefCount = 1; g_modes[i] = (i == 3 || i == 7) ? 0 : &modes[i]; }
    CCameraManagerDef def; def.RefCount = 1;
    long list1[] = { 1, 4, 5, 2 };
    def.CameraList._M_start = list1; def.CameraList._M_finish = list1 + 4; def.CameraList._M_end_of_storage._M_data = list1 + 4;
    def.InitialMode = 4;
    CGameDefinitionManager defs;
    CWorld world; world.DefinitionManager = &defs;
    static CGameCameraManager manager;
    manager.World = &world; manager.PDef.Object = &def;
    manager.CameraList._M_start = manager.CameraList._M_finish = manager.CameraList._M_end_of_storage._M_data = 0;
    manager.CameraListIndex = manager.CameraList._M_finish;          // no current selection

    manager.InitManualCameraList();                            // nothing selected: start at the def's InitialMode
    CHECK(manager.CameraList._M_finish - manager.CameraList._M_start == 4);
    CHECK(IdAt(manager, 0) == 1 && IdAt(manager, 1) == 4 && IdAt(manager, 2) == 5 && IdAt(manager, 3) == 2);
    CHECK(manager.CameraListIndex == manager.CameraList._M_start + 1 && manager.InitialModeCameraListIndex == manager.CameraListIndex);
    for (int i = 0; i < 8; ++i) if (g_modes[i]) CHECK(modes[i].RefCount == ((i == 1 || i == 2 || i == 4 || i == 5) ? 2 : 1));

    manager.CameraListIndex = manager.CameraList._M_start + 2;          // the player picked mode 5
    manager.InitManualCameraList();                            // rebuilt: the selection survives
    CHECK(manager.CameraList._M_finish - manager.CameraList._M_start == 4 && IdAtIndex(manager) == 5);
    CHECK(manager.InitialModeCameraListIndex == manager.CameraListIndex && modes[5].RefCount == 2 && modes[1].RefCount == 2);

    long list2[] = { 6, 0, 3, 4 };                             // the def changes; 3 fails to load, 5 is gone
    def.CameraList._M_start = list2; def.CameraList._M_finish = list2 + 4; def.CameraList._M_end_of_storage._M_data = list2 + 4;
    def.InitialMode = 0;
    manager.InitManualCameraList();
    CHECK(manager.CameraList._M_finish - manager.CameraList._M_start == 3);
    CHECK(IdAt(manager, 0) == 6 && IdAt(manager, 1) == 0 && IdAt(manager, 2) == 4);
    CHECK(IdAtIndex(manager) == 0 && modes[5].RefCount == 1 && modes[1].RefCount == 1 && modes[6].RefCount == 2);
    CHECK(g_destroyed == 0);
    std::printf("0069a010_TEST PASS\n");
    return 0;
}