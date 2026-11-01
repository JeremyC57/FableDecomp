// NScript::CV_BanditCampPathScript::~CV_BanditCampPathScript() -- implicit quest-script destructor: destroy Bandit2, Bandit1 (CScriptThing) and the Bandits vector<CScriptThing> in reverse order, then ~CScriptBase
// Leaf: ??1CV_BanditCampPathScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E477F0, 171 bytes, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar
//   deleting dtor 0x00E477D0 (slot 0 of the RTTI-less quest vtable 0x012E4A9C).
// Identity (proven): vtable 0x012E4A9C is stored only by the ctor 0x00E46BD0, whose only caller is the Alloc 0x00E477A0
//   (`new` 0x70); CScriptInfoManager::RegisterScripts stores that Alloc after `push "S_VBCP"` (0x00CD8238) in the block that
//   names "V_BanditCampPath" (0x012DA044); refs/script_recovery/conversion_queue.tsv agrees (V_BanditCampPath / S_VBCP /
//   0x00E477A0). Ego_r.pdb / FableWin.pdb carry ??1CV_BanditCampPathScript@NScript@@UAE@XZ and the ctor
//   ??0CV_BanditCampPathScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. No vtable store in the body =
//   compiler-generated dtor. The catalog label (CActionMoveTo::~CActionMoveTo) is propagated and wrong.
// Layout: Ego_r.pdb CV_BanditCampPathScript at retail offsets (Ego_r - 0x14 for the 0x40 retail CScriptBase, and the
//   12-byte STLport vector vs Ego_r's 16): OverheardBanditConversation +0x48, Bandits +0x4c, Bandit1 +0x58, Bandit2 +0x64
//   (0x70 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: ~CScriptThing inline (vtable 0x01238C8C, CCountedPointer release, ~CBaseClass 0x0099A2E0); the vector's
//   element dtors go through the virtual ~CScriptThing (vtable slot 0, flag 0), then free 0x00BFEA14; ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E46BD0), which emits the vtable, ??_G and this implicit dtor.
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
class CV_BanditCampPathScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BanditCampPathScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                      OverheardBanditConversation;              // +0x48
    std::vector<CScriptThing> Bandits;                                  // +0x4c
    CScriptThing              Bandit1;                                  // +0x58
    CScriptThing              Bandit2;                                  // +0x64
};
CV_BanditCampPathScript::CV_BanditCampPathScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E46BD0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
