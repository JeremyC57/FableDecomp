// CGameCameraManager::InitManualCameraList() -- rebuild the manual camera list from the manager def's CameraList mode indices (each def that loads is pushed), then point the current and initial list iterators at the previously selected mode's new position, else at the def's InitialMode
// Leaf: ?InitManualCameraList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069A010, 400 bytes (389 without padding), thiscall, plain `ret`. Ego_r.pdb
//   `?InitManualCameraList@CGameCameraManager@@AAEXXZ` (private); called from CGameCameraManager::SetCameraManagerDef 0x0069A850
//   right after PDef is replaced (followed by InitSpecialCameraList 0x0069A1A0).
// Callees: the current mode def's virtual slot 10 (+0x28, a CDefString returned through a hidden pointer; descriptive name
//   GetDefName), CDefStringTable::GetString(CDefString) 0x009D49B0 on g_DefStringTable 0x013CA828, CDefinitionManager::
//   GetDefGlobalIndexFromName 0x009AD410 (on World->DefinitionManager, +0x74 -> +0x10), CCharString::~CCharString 0x0099EAE0,
//   STLport __copy<CDefPointer<const CCameraModeDef>*, .., int> 0x0069CB80 and __destroy_aux 0x0069D2B0 (the inline
//   vector::clear = erase(begin(), end())), CDefinitionManager::GetDef<CCameraModeDef> 0x0056D0C0 (COMDAT-folded, catalog
//   "GetDef<CThingPhysicalSwitchDef>"), vector<CDefPointer<const CCameraModeDef> >::_M_insert_overflow 0x0069DBB0 (the inline
//   push_back's growth path).
// Label: catalog "CGameCameraManager_PopulateSwitchList" -- this is CGameCameraManager::InitManualCameraList (PDB name; it
//   writes CameraList +0x108 / CameraListIndex +0x114 / InitialModeCameraListIndex +0x118 from CCameraManagerDef::CameraList).
// Layout: retail CGameCameraManager World +0x74, CameraList +0x108 (12-byte STLport vector), CameraListIndex +0x114,
//   InitialModeCameraListIndex +0x118, PDef +0x134 (as the 0x00699BF0 destructor model); retail CCameraManagerDef CameraList
//   +0x3c, SpecialityCameraList +0x48, InitialMode +0x54 (Ego_r +0x40/+0x50/+0x60 with 16-byte vectors); CWorld
//   DefinitionManager +0x10.
// Shape: the def pointee keeps its count at +4 (`dec [ecx+4] ; jne ; call [vt+4]`). GetDef and _M_insert_overflow are template
//   members compiled in this TU: same-TU noinline stand-ins leave the loop's CDefPointer uncaptured, so its object stays in ecx
//   from the push_back copy / reload into the release. The previous mode's name is a NAMED block-scope CCharString (its slot is
//   reused by the loop index; the returned eax is not reused for the push); the CDefString temporary shares the default index
//   slot. defaultIndex is declared before initialIndex (store order and slots), the copy constructor reads the source object
//   once, the loop value *it is a local (ebp), and the final index is the `initialIndex < 0 ? defaultIndex : initialIndex`
//   ternary stored through the chain `CameraListIndex = InitialModeCameraListIndex = ...` (+0x118 first).
inline void* operator new(unsigned int, void* __p) { return __p; }

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
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
    public:
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
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                                       unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (__x.Object != 0);
}
#pragma auto_inline(on)

// Same-TU stand-in for CDefinitionManager::GetDef<T> (0x0056D0C0, a template member compiled in the caller's TU): it fills
// the out pointer and never keeps its address, so the caller's def pointer is not treated as captured.
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<T>* out) const
{
    if (index < 0)
        return false;
    out->Object = (T*)(this + index);
    return true;
}
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