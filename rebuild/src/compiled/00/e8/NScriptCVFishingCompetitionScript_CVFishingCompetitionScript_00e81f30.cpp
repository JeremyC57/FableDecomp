// NScript::CV_FishingCompetitionScript::~CV_FishingCompetitionScript() -- the compiler-generated destructor: free the SizeComment block, destroy the Winner string, then ~CScriptBase
// Leaf: ??1CV_FishingCompetitionScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E81F30, 35 bytes, thiscall, tail `jmp ~CScriptBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00E81F10 of the RTTI-less vtable 0x012EA478, which the landed
//   CV_FishingCompetitionScript ctor 0x00E7E110 and Alloc 0x00E81EB0 store (slots 1 RegisterMain 0x00E7E150, 2 Main
//   0x00E7E370 landed under this class). FableWin.pdb ??1CV_FishingCompetitionScript@NScript@@UAE@XZ (donor 0x039F1040); the
//   destroyed members are Ego_r's SizeComment (vector<long>, retail +0x74) and Winner (CCharString, +0x68), the members the
//   ctor builds. No vtable store (not even CLevelScriptBase's): the destructor is the implicit one. The catalog label
//   "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14, CCharString::~CCharString 0x0099EAE0, CScriptBase::~CScriptBase 0x00CBD510.
// Layout: the landed ctor 0x00E7E110 model (Ego_r offsets - 0x14). CLevelScriptBase<T> is the local copy of
//   engine/CLevelScriptBase.h without its dtor declaration (retail's is implicit: the CLevelScriptBase<T> `??_G` 0x00CDD250
//   calls ~CScriptBase directly with no vtable store; the landed 0x00D83350 does the same; header debt in NOTES.md).
// Shape: members in reverse order (STLport ~vector<long>: trivial _Destroy + null-tested free; ~CCharString), then the base.
//   DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
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
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CV_FishingCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool BountyHuntActive;                              // +0x48
    bool OrganiserRanAway;                              // +0x49
    bool OrganiserKilled;                               // +0x4a
    bool CompetitionRunning;                            // +0x4b
    bool CompetitionWon;                                // +0x4c
    bool RodOfChampsGiven;                              // +0x4d
    bool CommentIRanAway;                               // +0x4e
    bool CompetitorsAttacked;                           // +0x4f
    bool CompetitorsJustGotAttacked;                    // +0x50
    long AttackCounter;                                 // +0x54
    long AttackDayCount;                                // +0x58
    long HitCount;                                      // +0x5c
    bool DoneIntro;                                     // +0x60
    bool RodHintGiven;                                  // +0x61
    long PrizeNumber;                                   // +0x64
    CCharString Winner;                                 // +0x68
    float BiggestFish;                                  // +0x6c
    float BiggestCompetitorFish;                        // +0x70
    std::vector<long> SizeComment;                      // +0x74
};
}
// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NScript::CV_FishingCompetitionScript* script)
{
    script->NScript::CV_FishingCompetitionScript::~CV_FishingCompetitionScript();
}