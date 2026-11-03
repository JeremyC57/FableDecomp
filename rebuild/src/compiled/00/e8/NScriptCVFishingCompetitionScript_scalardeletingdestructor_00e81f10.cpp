// NScript::CV_FishingCompetitionScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_FishingCompetitionScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E81F10, 30 bytes, thiscall, `ret 4`; calls 00e81f30=NScript::CV_FishingCompetitionScript::~CV_FishingCompetitionScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012EA478 (no RTTI), the vtable the landed constructor 0x00E7E110 stores last
//   (??0CV_FishingCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, FableWin.pdb); Ego_r.pdb ??_ECV_FishingCompetitionScript@NScript@@UAEPAXI@Z; FableWin.pdb ??_ECV_FishingCompetitionScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CV_FishingCompetitionScript@NScript@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012ea478::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00E7E110 (rebuild/src/compiled/00/e7/NScriptCVFishingCompetitionScript_CVFishingCompetitionScript_00e7e110.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added. The constructor body is not part of this graded body.
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
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
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