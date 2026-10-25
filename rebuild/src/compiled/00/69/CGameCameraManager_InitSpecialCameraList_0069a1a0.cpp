// CGameCameraManager::InitSpecialCameraList() -- rebuild the special camera list: clear it, look up every speciality camera mode def the manager def names and append each one found, then point the special list index at its start
// Leaf: ?InitSpecialCameraList@CGameCameraManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069A1A0, 228 bytes (+ int3 padding to 256), thiscall, plain `ret`.
// Identity: the catalog's "CGameCameraManager_PopulateListFromData" is a descriptive auto label. FableWin.pdb lists the
//   private ?InitSpecialCameraList@CGameCameraManager@@AAEXXZ next to InitManualCameraList; the body reads
//   CCameraManagerDef::SpecialityCameraList (PDef +0x134, the def's vector<long> at +0x48) into SpecialCameraList
//   (+0x11C) and sets SpecialCameraListIndex (+0x128). Single caller 0x0069A894.
// Callees: std::__copy(first, last, result, const random_access_iterator_tag&, int*) 0x0069CB80 and
//   std::__destroy_aux(first, last, const __false_type&) 0x0069D2B0 (landed; clear() = erase(begin(), end())),
//   CGameDefinitionManager::GetDef<CCameraModeDef>(long, CDefPointer&) 0x0056D0C0 (landed),
//   vector::_M_insert_overflow 0x0069DBB0 (push_back's regrowth), the def's virtual Destroy (slot 1).
// Layout: CGameCameraManager in its retail layout -- engine/CGameCameraManager.h carries the Ego_r.pdb debug-STL offsets
//   (SpecialCameraList +0x130 there), retail has 4-byte lists, 12-byte vectors and no InitialModeCameraListIndex:
//   World +0x74, SpecialCameraList +0x11C, SpecialCameraListIndex +0x128, PDef +0x134. CWorld::DefinitionManager +0x10.
// Shape: the definition manager is a local read before the loop; per id the global index read into a local first (else
//   the id takes ecx and the GetDef `this` load sinks below both pushes), a fresh CDefPointer (zeroed after the argument
//   pushes), GetDef, push_back (null-tested placement copy with AddRef, else _M_insert_overflow(finish, x, tag, 1, true)),
//   and the pointer's destructor (null test, `dec [p+4] ; jne`, virtual slot 1). Both STLport tags share one frame word.
//   GetDef and _M_insert_overflow are same-TU noinline stand-ins: with bare declarations VC7.1 assumes both may rewrite
//   the def local and re-reads it from the frame after each call (254 bytes); retail keeps it in ebp.
inline void* operator new(unsigned int, void* p) { return p; }

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    void AddRef() const { ++RefCount; }
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    CDefPointer& operator=(const CDefPointer& other);
};

class CCameraModeDef : public CDefBase { };

class CGameDefinitionManager {
public:
    template <class T> bool GetDef(long globalIndex, CDefPointer<T>& out);   // 0x0056D0C0
};

class CWorld {
public:
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;              // +0x10
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x0069CB80
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0069D2B0

template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                         // 0x0069DBB0 (stand-in below)
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs((const _Tp*)__last, (const _Tp*)this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> { };

class CCameraManagerDef : public CDefBase {
public:
    unsigned char _pad08[0x48 - 0x08];
    CArray<long> SpecialityCameraList;                      // +0x48
};

// Same-TU stand-ins (auto_inline off, noinline) for the template bodies retail compiled into this TU: GetDef 0x0056D0C0
// writes only the out pointer, _M_insert_overflow 0x0069DBB0 only reads the inserted value, so VC7.1 keeps the def in a
// register across both.
CDefBase* LookUpDef_69a1a0(long globalIndex);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long globalIndex, CDefPointer<T>& out)
{
    CDefBase* def = LookUpDef_69a1a0(globalIndex);
    if (!def)
        return false;
    *(CDefBase**)&out = def;
    return true;
}
namespace std {
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (__x.Object != 0);
}
}
#pragma auto_inline(on)

class CGameCameraManager {
public:
    unsigned char _pad00[0x74];
    CWorld* World;                                          // +0x74
    unsigned char _pad78[0x11C - 0x78];
    std::vector<CDefPointer<const CCameraModeDef> > SpecialCameraList;   // +0x11C
    std::vector<CDefPointer<const CCameraModeDef> >::iterator SpecialCameraListIndex;   // +0x128
    bool UsingCameraManually;                               // +0x12C
    bool BlockModeChange;                                   // +0x12D
    CDefPointer<const CDefBase> PCameraManagerSet;          // +0x130
    CDefPointer<const CCameraManagerDef> PDef;              // +0x134
    void InitSpecialCameraList();
};

void CGameCameraManager::InitSpecialCameraList()
{
    SpecialCameraList.clear();
    CGameDefinitionManager* definitions = World->DefinitionManager;
    for (CArray<long>::const_iterator it = PDef.Object->SpecialityCameraList.begin();
         it != PDef.Object->SpecialityCameraList.end(); ++it) {
        long globalIndex = *it;
        CDefPointer<const CCameraModeDef> def;
        if (definitions->GetDef(globalIndex, def))
            SpecialCameraList.push_back(def);
    }
    SpecialCameraListIndex = SpecialCameraList.begin();
}