// NScript::CV_MurderTwistFinalScript::~CV_MurderTwistFinalScript() -- implicit quest-script destructor: destroy seh_Guards, seh_Sheriff, Guards and Sheriff in reverse order, then ~CScriptBase
// Leaf: ??1CV_MurderTwistFinalScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6AB0, 214 bytes, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar
//   deleting dtor 0x00EB6A90 (slot 0 of the RTTI-less quest vtable 0x012EF978).
// Identity (proven): vtable 0x012EF978 is stored only by the ctor 0x00EAE910, whose only caller is the Alloc 0x00EB6A60
//   (`new` 0x88); CScriptInfoManager::RegisterScripts stores that Alloc after `push "S_VMTF"` (0x00CD9090);
//   refs/script_recovery/conversion_queue.tsv agrees (V_MurderTwistFinal / S_VMTF / 0x00EB6A60). Ego_r.pdb / FableWin.pdb carry
//   the ctor ??0CV_MurderTwistFinalScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. No vtable store in
//   the body = compiler-generated dtor. The catalog label (CParticleEmitter::~CParticleEmitter_0xb6ab0) is a placeholder.
// Layout: Ego_r.pdb CV_MurderTwistFinalScript at retail offsets (Ego_r - 0x14, then - 4 per STLport vector): SheriffState
//   +0x48, AssassinState +0x4c, TaskDeclined +0x50, PreemptiveAttackInProgress +0x51, Sheriff +0x54, Guards +0x60,
//   seh_Sheriff +0x6c, seh_Guards +0x7c (0x88 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: element dtors through the virtual slot 0 (flag 0) + free 0x00BFEA14 for both vectors; seh_Sheriff's implicit dtor
//   inline (PImp release, vtable 0x0126008C, ~CBaseObject 0x0099A430); ~CScriptThing inline (vtable 0x01238C8C, release,
//   ~CBaseClass 0x0099A2E0); operator delete 0x00BFE9BC; ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00EAE910), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h (the members this row needs; the malloc-backed allocator of
// this build: deallocate = free 0x00BFEA14)
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    typedef _Alloc allocator_type;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum ESheriffState { };
    enum EAssassinState { };
    CV_MurderTwistFinalScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    ESheriffState                                           SheriffState;                 // +0x48
    EAssassinState                                          AssassinState;                // +0x4c
    bool                                                    TaskDeclined;                 // +0x50
    bool                                                    PreemptiveAttackInProgress;   // +0x51
    CScriptThing                                            Sheriff;                      // +0x54
    std::vector<CScriptThing>                               Guards;                       // +0x60
    CScriptGameResourceObjectScriptedThingBase              seh_Sheriff;                  // +0x6c
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;                   // +0x7c
};
CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00EAE910
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
