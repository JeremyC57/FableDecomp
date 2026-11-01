// NScript::CV_BodyGuardScript::~CV_BodyGuardScript() -- implicit quest-script destructor: deregister the four HenchmanDeadRespawnTimer, destroy the HenchmanList vector<CScriptThing>, deregister the four chat/commentary timers, then ~CScriptBase
// Leaf: ??1CV_BodyGuardScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E62780, 163 bytes, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar
//   deleting dtor 0x00E62760 (slot 0 of the RTTI-less quest vtable 0x012E6E4C).
// Identity (proven): vtable 0x012E6E4C is stored only by the ctor 0x00E5D140, whose only caller is the Alloc 0x00E62730
//   (`new` 0x94); CScriptInfoManager::RegisterScripts stores that Alloc after `push "S_VBGu"` (0x00CD8403);
//   refs/script_recovery/conversion_queue.tsv agrees (V_BodyGuard / S_VBGu / 0x00E62730). Ego_r.pdb / FableWin.pdb carry the
//   ctor ??0CV_BodyGuardScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. No vtable store in the body =
//   compiler-generated dtor. The catalog label (CParticleEmitter::~CParticleEmitter_Minimal) is a placeholder.
// Layout: Ego_r.pdb CV_BodyGuardScript at retail offsets (Ego_r - 0x14, then - 4 after the STLport vector): TotalWages +0x48,
//   HeroCannotAffordHelp +0x4c, HeroWieldingWeapon +0x4d, HeroStoodStillCounter +0x50, HenchmanLastIdleChatTimer +0x54,
//   HenchmanLastBoredChatTimer +0x58, CommentaryTimer +0x5c, HenchmanFreakedOutThePlayerTimer +0x60, HenchmanList +0x64,
//   ConversationIndex +0x70, HenchmanDeadRespawnTimer[4] +0x74, HenchmanGlobalStatus[4] +0x84 (0x94 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through g_pGameScriptInterface 0x0143E8F8;
//   the array is the inline countdown loop, no EH vector iterator), the vector's element dtors through the virtual slot 0
//   (flag 0) + free 0x00BFEA14, ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E5D140), which emits the vtable, ??_G and this implicit dtor.
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
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
class CCounter {
public:
    CCounter& operator=(float);
    bool operator>(float) const;
    float GetCounter() const;
    long RelevantWorldFrame;                                             // +0x00 (Ego_r.pdb)
};
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
class CV_BodyGuardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BodyGuardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    long                      TotalWages;                               // +0x48
    bool                      HeroCannotAffordHelp;                     // +0x4c
    bool                      HeroWieldingWeapon;                       // +0x4d
    CCounter                  HeroStoodStillCounter;                    // +0x50
    CTimer                    HenchmanLastIdleChatTimer;                // +0x54
    CTimer                    HenchmanLastBoredChatTimer;               // +0x58
    CTimer                    CommentaryTimer;                          // +0x5c
    CTimer                    HenchmanFreakedOutThePlayerTimer;         // +0x60
    std::vector<CScriptThing> HenchmanList;                             // +0x64
    long                      ConversationIndex;                        // +0x70
    CTimer                    HenchmanDeadRespawnTimer[4];              // +0x74
    long                      HenchmanGlobalStatus[4];                  // +0x84
};
CV_BodyGuardScript::CV_BodyGuardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E5D140
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}