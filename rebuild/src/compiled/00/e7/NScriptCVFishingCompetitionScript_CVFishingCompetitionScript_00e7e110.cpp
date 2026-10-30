// NScript::CV_FishingCompetitionScript::CV_FishingCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script constructor: CScriptBase base, the CLevelScriptBase members (Game, MasterScriptData), the script's vtable, then the empty Winner string and the empty SizeComment vector
// Leaf: ??0CV_FishingCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7E110, 53 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): stores vtable 0x012EA478, which the landed CV_FishingCompetitionScript::Alloc 0x00E81EB0 (registered
//   "S_VFC" / "V_FishingCompetition") also stores with this constructor inlined; slots 1 RegisterMain 0x00E7E150 and
//   2 Main 0x00E7E370 are landed under this class. FableWin.pdb
//   ??0CV_FishingCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. No caller (the
//   Alloc inlines its own copy). Label correction: the manifest row "CIcicleData::CIcicleData" is propagated.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CV_FishingCompetitionScript members at Ego_r offset - 0x14: the flags +0x48.., AttackCounter +0x54,
//   AttackDayCount +0x58, HitCount +0x5C, DoneIntro +0x60, RodHintGiven +0x61, PrizeNumber +0x64, Winner +0x68,
//   BiggestFish +0x6C, BiggestCompetitorFish +0x70, SizeComment STLport vector<long> +0x74 (0x80 bytes, the Alloc's size).
// Shape: the CV_AssassinAttacksScript constructor 0x00E376D0 model.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData; }
class CGameScriptInterfaceBase;
class CPersistContext;
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CV_FishingCompetitionScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CV_FishingCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CV_FishingCompetitionScript();
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
CV_FishingCompetitionScript::CV_FishingCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}