// NScript::CV_SingingStonesScript::~CV_SingingStonesScript() -- implicit village-script destructor: the four ListText strings (count-down loop), the EffectList vector (each CScriptThing's virtual dtor, then the block freed), then ~CScriptBase
// Leaf: ??1CV_SingingStonesScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED39F0, 89 bytes (+ int3 padding), thiscall; tail `jmp ~CScriptBase` 0x00CBD510.
// Identity (proven, two sources): the only caller 0x00ED39D3 is the scalar deleting destructor 0x00ED39D0, slot 0 of the RTTI-less vtable
//   0x012F2B0C; that vtable is stored only by the ctor 0x00ED0E60, whose only caller is the Alloc 0x00ED39A0 (`push 0x9C` ; operator
//   new ; ctor), stored by the script-registration routine next to the save tag "S_VSS" (0x012F764C); refs/script_recovery/
//   conversion_queue.tsv lists that tag and allocator as the village script V_SingingStones, i.e. NScript::CV_SingingStonesScript (FableWin.pdb
//   ??0CV_SingingStonesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z and ??1CV_SingingStonesScript@NScript@@UAE@XZ). The body stores
//   no vtable of its own: the compiler-generated destructor. The catalog label is propagated and wrong (NAME_FIXES.tsv).
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (ListText[4] by an inline count-down loop), the virtual ~CScriptThing per EffectList
//   element (`push 0 ; call [eax]`: STLport _Destroy calls `p->~_Tp()` through the vtable), free 0x00BFEA14 (~_Vector_base),
//   ??1CScriptBase@@UAE@XZ 0x00CBD510 (tail).
// Layout: Ego_r.pdb members at Ego_r offset - 0x14 (retail CScriptBase 0x40 vs Ego_r 0x54; CLevelScriptBase<T> Game +0x40 /
//   MasterScriptData +0x44) (CurrentPlayListIndex +0x48, PlayList / RudePlayList / YourPersonalPlayList long[4] +0x4C/+0x5C/+0x6C, EffectList STLport
//   vector<CScriptThing> +0x7C (12 bytes), ListText[4] +0x88, the four DoorMan bools +0x98; sizeof 0x9C = the Alloc's `push 9Ch`). CLevelScriptBase<T> comes from engine/CLevelScriptBase.h with the retail opt-in
//   FABLE_RETAIL_CLEVELSCRIPTBASE (hdr/ overlay, chunk 713): retail's ~CLevelScriptBase<T> is compiler-generated, so the implicit
//   derived dtor tail-jumps ~CScriptBase; with the header's default out-of-line declaration it would jump to ??1?$CLevelScriptBase.
//   T = NScript::CQ_SunnyvaleMasterData as in the landed village-script rows (0x00E376D0, 0x00ED7640); T changes no byte or symbol
//   here and is not separately proven for this script.
// TU: the PDB ctor ??0CV_SingingStonesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined (emission only, body not
//   reconstructed: the landed row is CVtable_012f2b0c 0x00ED0E60, a descriptive stand-in for this ctor), which emits the vtable, the scalar deleting dtor and this implicit dtor, as retail's TU does.
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

CV_SingingStonesScript::CV_SingingStonesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00ED0E60
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}