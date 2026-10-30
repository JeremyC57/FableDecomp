// NEnvironmentInternal::CEnvironmentThemeResourcePallette::Build(const CGameDefinitionManager& manager, const CEnvironmentDef& env, const CEnvironmentLookupTexture& lookup) -- rebuild the day sets: one CEnvironmentThemeSetDay per ENVIRONMENT_THEME_DAY def after index 0, each built from its def
// Grade: DIFFER(287v289): 48 diff lines (was 56) -- the inline GetDef/GetNumberOfDefs build their type-name CCharString as a named local (retail builds it before pushing index/&day); residue: frame 0x14 v 0x18 and register map (retail manager ebp, count edi + frame slot; ours manager ebx, count ebp)
// Leaf: ?Build@CEnvironmentThemeResourcePallette@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00888A60, 289 bytes incl. int3 padding (287 of code), thiscall, `ret 0Ch`.
// Identity: the catalog's "CEnvironmentThemeSetDay_BuildDay" is descriptive. Ego_r.pdb name set:
//   `?Build@CEnvironmentThemeResourcePallette@NEnvironmentInternal@@QAEXABVCGameDefinitionManager@@ABVCEnvironmentDef@@ABUCEnvironmentLookupTexture@2@@Z`;
//   the class is {CArray<CEnvironmentThemeSetDay> DaySets +0}, a CEnvironmentThemeSetDay is {CArray<CBlendedEnvironmentTheme>
//   Themes} (12 bytes with the retail STLport vector), built by the landed CEnvironmentThemeSetDay::Build 0x00888410.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CDefinitionManager::GetNumberOfDefsOfType
//   0x009AD3B0 (landed), CDefinitionManager::GetDef<CEnvironmentThemeDaySetDef>(const CCharString&, long, CDefPointer*)
//   const 0x00888E60, vector::resize(n, const T&) 0x0088A9C0, CEnvironmentThemeSetDay::Build 0x00888410 (landed), and the
//   /Gr `__fastcall` STLport helpers __copy 0x0088A5E0 (landed), __destroy_aux 0x006B6FF0 (landed, the day-set array) and
//   __destroy_aux 0x006B67A0 (landed, the blended-theme array of the resize temporary). String: "ENVIRONMENT_THEME_DAY".
// Shape: `DaySets.clear()` = erase(begin(), end()) (__copy over nothing, _Destroy, finish = i), `DaySets.resize(count - 1)`
//   (a zeroed CEnvironmentThemeSetDay temporary whose inline destructor folds to __destroy_aux(0, 0)), then the counted
//   loop from 1 with the def pointer released after each Build; the empty STLport tags sit in manager's argument slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void DestroyDef() const;                        // slot 1
    mutable long m_refCount;                                // +0x04
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->DestroyDef();
        }
    }
    T& operator*() const { return *Object; }
};

class CEnvironmentThemeDaySetDef : public CDefPointeeBase {
public:
    static const char* GetDefClassName() { return "ENVIRONMENT_THEME_DAY"; }
};
class CEnvironmentDef;

class CDefinitionManager {
public:
    long GetNumberOfDefsOfType(const CCharString& typeName);                                    // 0x009AD3B0
    template <class T> bool GetDef(const CCharString& type, long index, CDefPointer<const T>* def) const;   // 0x00888E60
    template <class T> bool GetDef(long index, CDefPointer<const T>* def) const
    {
        CCharString type(T::GetDefClassName());
        return GetDef<T>(type, index, def);
    }
    template <class T> long GetNumberOfDefs()
    {
        CCharString type(T::GetDefClassName());
        return GetNumberOfDefsOfType(type);
    }
};
class CGameDefinitionManager : public CDefinitionManager { };

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                        // 0x0088A5E0
template <class _ForwardIter>
void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&);       // 0x006B6FF0 / 0x006B67A0

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    explicit vector(const _Alloc& __a = _Alloc()) : _Base(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void resize(size_type __new_size, const _Tp& __x);                                               // 0x0088A9C0
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

// Same-TU stand-ins (never inlined) for the out-of-line resize 0x0088A9C0 and its fill-insert: VC7.1 then sees that the
// fill value is only read, keeps the zeroed temporary's pointers as constants and folds its destructor to
// __destroy_aux(0, 0), as retail does.
void __fill_insert_value(const void* __x, unsigned int __n);
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
{
    __fill_insert_value(*(void* const*)&__x, __n + (__pos - this->_M_start));
}
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        _M_fill_insert(end(), __new_size - size(), __x);
}
#pragma auto_inline(on)

} // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
};

namespace NEnvironmentInternal {
struct CEnvironmentLookupTexture;
class CBlendedEnvironmentTheme {                                    // 0x1F4 bytes
public:
    unsigned char Data[0x1F4];
    ~CBlendedEnvironmentTheme();
};
class CEnvironmentThemeSetDay {
public:
    CArray<CBlendedEnvironmentTheme> Themes;                        // +0x00
    void Build(const CGameDefinitionManager& manager, const CEnvironmentDef& env, const CEnvironmentLookupTexture& lookup,
               const CEnvironmentThemeDaySetDef& day);              // 0x00888410
};
class CEnvironmentThemeResourcePallette {
public:
    CArray<CEnvironmentThemeSetDay> DaySets;                        // +0x00
    void Build(const CGameDefinitionManager& manager, const CEnvironmentDef& env, const CEnvironmentLookupTexture& lookup);
};

void CEnvironmentThemeResourcePallette::Build(const CGameDefinitionManager& manager, const CEnvironmentDef& env,
                                              const CEnvironmentLookupTexture& lookup)
{
    long count = ((CGameDefinitionManager&)manager).GetNumberOfDefs<CEnvironmentThemeDaySetDef>();
    DaySets.clear();
    DaySets.resize(count - 1);
    for (long i = 1; i < count; ++i) {
        CDefPointer<const CEnvironmentThemeDaySetDef> day;
        manager.GetDef<CEnvironmentThemeDaySetDef>(i, &day);
        DaySets[i - 1].Build(manager, env, lookup, *day);
    }
}
}
