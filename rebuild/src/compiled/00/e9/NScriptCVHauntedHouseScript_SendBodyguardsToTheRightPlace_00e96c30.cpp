// NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2) -- collect the things named "BodyGuard"; with one bodyguard teleport it to marker1, with two teleport the first to marker1 and the second to marker2
// Leaf: ?SendBodyguardsToTheRightPlace@CV_HauntedHouseScript@NScript@@QAEXVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E96C30, 413 bytes, thiscall, `ret 8` (two by-value CCharString arguments, destroyed by
//   the callee at the end).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?SendBodyguardsToTheRightPlace@CV_HauntedHouseScript@NScript@@QAEXVCCharString@@0@Z
//   is the only CV_HauntedHouseScript member taking two CCharStrings by value; the row's only caller is
//   CV_HauntedHouseScript::CM_HeroInsideHouse::Main 0x00E96660 (site 0x00E96888, `this` = ParentClass +0x14, two CCharString
//   argument copies built in the argument area); the body fetches "BodyGuard" things. Catalog label DoMission_BodyGuardSimple
//   (descriptive, wrong). FableWin.pdb also has a CQ_BreakSiegeScript twin of the method (another body).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, ~CScriptThing 0x004AA840, std::vector<CScriptThing>::~vector 0x008AC970 (out of line on the terminating
//   exit only), free 0x00BFEA14; CGameScriptInterface slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 472
//   EntityTeleportToThing (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "BodyGuard" 0x012CB3F0.
// Layout: CScriptBase retail 0x40, Game +0x40; STLport 4.6 vector<CScriptThing> (12 bytes, 12-byte elements: the
//   `imul 0x2AAAAAAB ; sar 1` divide by 12).
// Shape: the vector local is the STLport 4.6 template (allocator-proxy base, inline ~vector = _Destroy loop through the
//   element's virtual destructor + null-tested free); VC7.1 inlines the destructor on the fall-through exit and calls the
//   COMDAT copy 0x008AC970 on the shared IsActiveThreadTerminating exit.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:        // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    virtual ~CScriptThing();                            // 0x004AA840
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp& reference;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }   // 0x008AC970 for CScriptThing
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
};
}
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CV_HauntedHouseScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2);
};

void CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2)
{
    std::vector<CScriptThing> bodyguards;
    Game->GetAllThingsWithScriptName("BodyGuard", bodyguards);
    if (bodyguards.size() == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName(marker1), false);
    }
    if (bodyguards.size() == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName(marker1), false);
        Game->EntityTeleportToThing(bodyguards[1], Game->GetThingWithScriptName(marker2), false);
    }
}
}   // namespace NScript