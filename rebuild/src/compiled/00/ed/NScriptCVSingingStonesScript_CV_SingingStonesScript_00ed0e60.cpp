// NScript::CV_SingingStonesScript::CV_SingingStonesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- CScriptBase(), Game (+0x40) / MasterScriptData (+0x44) by the inline CLevelScriptBase<T> ctor, this class's vtable (0x012F2B0C, no RTTI), the EffectList vector's three words at +0x7c..+0x84 cleared and the four ListText CCharStrings at +0x88 (a counted loop of constructors)
// Leaf: ??0CV_SingingStonesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED0E60, 85 bytes (+ int3 padding), thiscall, `ret 8`; calls CScriptBase::CScriptBase 0x00CB8110
//   (??0CScriptBase@@QAE@XZ) and CCharString::CCharString() 0x0099E4B0 (four times: the loop counter in ebx, the string pointer in edi, +4 per step).
// Identity (naming wave 35d): catalog NScript::CV_SingingStonesScript::CV_SingingStonesScript; its only caller is the Alloc 0x00ED39A0
//   (`push 9Ch` ; operator new ; call 0x00ED0E60; NScript::CV_SingingStonesScript::Alloc, registered as "V_SingingStones", save tag
//   "S_VSS": anniversary wave3/labels_alloc.tsv, labels_chunk713_2026-10-27.tsv); it stores vtable 0x012F2B0C, slot 0 of which is the
//   ??_G 0x00ED39D0 that calls ~CV_SingingStonesScript 0x00ED39F0; PDB
//   ??0CV_SingingStonesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Was: CVtable_012f2b0c::CVtable_012f2b0c(long a, long b) (no Leaf line; local stand-in bases CVtable_012c1648 for CScriptBase
//   0x00CB8110, a CKeyedBase_00ed0e60 novtable intermediate for the two stores, a local CCharString); same bytes.
// Model: the class model of the landed ~CV_SingingStonesScript row 0x00ED39F0
//   (rebuild/src/compiled/00/ed/NScriptCVSingingStonesScript_CVSingingStonesScript_00ed39f0.cpp), unchanged: Ego_r.pdb members at
//   Ego_r offset - 0x14, CLevelScriptBase<T> from engine/CLevelScriptBase.h with FABLE_RETAIL_CLEVELSCRIPTBASE (its inline ctor
//   stores the two pointers before this class's vtable store), EffectList an STLport vector<CScriptThing> whose inline default
//   constructor clears +0x7c/+0x80/+0x84. That row's TU already defines this ctor (emission only); this row grades it. The
//   implicit destructor, ??_G and vtable the TU also emits are not part of this graded body.
extern "C" void __cdecl free(void* block);    // 0x00BFEA14 (STLport allocator<CScriptThing>::deallocate)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "fable_string.h"                     // CCharString (~CCharString 0x0099EAE0)
#define FABLE_RETAIL_CLEVELSCRIPTBASE         // retail's implicit ~CLevelScriptBase<T> (hdr/ overlay, chunk 713)
#include "engine/CLevelScriptBase.h"          // CLevelScriptBase<T> : CScriptBase (Game +0x40, MasterScriptData +0x44)
#include "engine/CCountedPointer.h"           // CCountedPointer<T> (CScriptThing::PImp)
#include "engine/CScriptThing.h"              // CScriptThing : CBaseClass (vtable 0x01238C8C, virtual dtor slot 0)
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
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
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
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SingingStonesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {    // vtable 0x012F2B0C (no RTTI)
public:
    CV_SingingStonesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    long                      CurrentPlayListIndex;      // +0x48
    long                      PlayList[4];               // +0x4c
    long                      RudePlayList[4];           // +0x5c
    long                      YourPersonalPlayList[4];   // +0x6c
    std::vector<CScriptThing> EffectList;                // +0x7c (STLport, 12 bytes)
    CCharString               ListText[4];               // +0x88
    bool                      DoorManIntroComplete;      // +0x98
    bool                      DoorManAttackedByHero;     // +0x99
    bool                      DoorManHasBribe;           // +0x9a
    bool                      DoorManComplete;           // +0x9b
};
FABLE_STATIC_ASSERT(sizeof(CV_SingingStonesScript) == 0x9c);

CV_SingingStonesScript::CV_SingingStonesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00ED0E60 (this row)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}