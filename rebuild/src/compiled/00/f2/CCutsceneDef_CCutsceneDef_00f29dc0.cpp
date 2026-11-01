// CCutsceneDef::~CCutsceneDef() -- implicit definition destructor: destroy the eight vector<CCharString> members in reverse order (Answer1 .. Macro: each element's ~CCharString, then free the storage), free the SubDefInfoMap vector storage (+0x29), then the inline CDefClassBase / CDefPointeeBase levels (CDefPointeeBase vtable) and ~CResource
// Leaf: ??1CCutsceneDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29DC0, 382 bytes, thiscall, tail `jmp 0x009FC550` (~CResource).
// Identity (proven): its only caller is 0x00F29DA3 inside the scalar deleting destructor 0x00F29DA0, slot 0 of vtable
//   0x012FB6E0 -- the vtable the landed CCutsceneDef::CCutsceneDef 0x00F29D00 stores (Ego_r.pdb ??0CCutsceneDef@@QAE@XZ). The
//   vtable's slots 10-16 / 21 / 22 / 24 are CParentDefClassBase's (0x004303B0 .. 0x004303E0) and slots 18-20 are the
//   CCutsceneDef overrides Transfer 0x00F2A1D0, Copy 0x00F29FF0, GetSizeofClass 0x00F29D90, so the retail TU order is
//   GetSizeofClass, ctor, ??_G, ??1, Copy, Transfer -- the Ego_r.pdb CCutsceneDef set (??0, ??1CCutsceneDef@@UAE@XZ, ??_E,
//   Alloc, Transfer, ??4, Copy) and FableWin.pdb's (??0, Copy, GetSizeofClass, Transfer, ??4, ??_E, ??1). The member offsets
//   are Ego_r.pdb's eight `vector<CCharString>` members Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1
//   at retail +0x3C..+0x90 (12-byte STLport vectors after the 0x39-byte packed CParentDefClassBase), destroyed last-first.
//   The catalog label "CTCInventoryMap::ResetPrimitiveHandles" is propagated and wrong (the 0x00F29DA0 ??_G is landed as a
//   CLandscapeBackgroundPatch `vector deleting destructor` stand-in calling this row as @CLandscapeBackgroundPatch_dtor_00f29dc0@4).
// Callees: CCharString::~CCharString 0x0099EAE0 (per element), free 0x00BFEA14 (STLport allocator<T>::deallocate),
//   CResource::~CResource 0x009FC550.
// Shape: CCutsceneDef and CParentDefClassBase declare no destructor (VC7.1 drops their dead vtable stores; only the
//   CDefPointeeBase store of the inline ~CDefClassBase remains); each STLport ~vector<CCharString> keeps start/finish in
//   edi/ebx for the _Destroy loop and ~_Vector_base re-reads _M_start for the null-tested free; the SubDefInfoMap's
//   pair elements are trivially destructible (__true_type: no loop). Model copied from the landed ~CCardDef 0x007D7950.
// Layout: CDefClassBase from engine/CDefClassBase.h (retail model, packed, 0x25); CParentDefClassBase (InstantiationName
//   CDefString +0x25, SubDefInfoMap CVectorMap +0x29, 0x39 bytes) is modelled locally as in ~CCardDef: engine/
//   CParentDefClassBase.h declares ~CParentDefClassBase out of line (0x00430300), and this body inlines it.
//   The TU emitter is the PDB default constructor (Ego_r ??0CCutsceneDef@@QAE@XZ, retail 0x00F29D00) with an empty body:
//   it emits the vtable, the ??_G and this implicit ??1 (the constructor body itself is the separate landed row).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    ~CCharString();                                        // 0x0099EAE0
private:
    void* PStringData;
};

class CDefString { public:
    long TablePos;                                         // +0x00
    CDefString() : TablePos(-1) { }
};

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _T1, class _T2> struct __type_traits<pair<_T1, _T2> > { typedef __true_type has_trivial_destructor; };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                          // +0x0c
    bool Dirty;                                            // +0x0d
    CVectorMap() : Dirty(false) { }
};

#pragma pack(push, 1)                                      // retail: the def chain is packed (CDefString at +0x25)
class CParentDefClassBase : public CDefClassBase {         // vtable 0x01230CBC
public:
    CParentDefClassBase();                                 // 0x00430370
    CDefString InstantiationName;                          // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >
        SubDefInfoMap;                                     // +0x29
};
#pragma pack(pop)

class CCutsceneDef : public CParentDefClassBase {          // vtable 0x012FB6E0
public:
    CCutsceneDef();                                        // 0x00F29D00 (Ego_r ??0CCutsceneDef@@QAE@XZ)
    virtual void Transfer(CPersistContext& context);       // slot 18 0x00F2A1D0 (Ego_r ?Transfer@CCutsceneDef@@UAEXAAVCPersistContext@@@Z)
    virtual void Copy(const CDefClassBase* other);         // slot 19 0x00F29FF0 (Ego_r ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z)
    virtual unsigned long GetSizeofClass();                // slot 20 0x00F29D90 (FableWin ?GetSizeofClass@CCutsceneDef@@UAEKXZ)
    std::vector<CCharString> Macro;                        // +0x3c
    std::vector<CCharString> SkipCond;                     // +0x48
    std::vector<CCharString> Lights;                       // +0x54
    std::vector<CCharString> LightScene;                   // +0x60
    std::vector<CCharString> SetupCond;                    // +0x6c
    std::vector<CCharString> Sound;                        // +0x78
    std::vector<CCharString> Answer0;                      // +0x84
    std::vector<CCharString> Answer1;                      // +0x90
};

CCutsceneDef::CCutsceneDef()
{
}