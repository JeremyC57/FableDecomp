// NScript::CV_AssassinAttacksScript::CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script constructor: CScriptBase base, the CLevelScriptBase members (Game, MasterScriptData), the script's vtable, then the empty kill-region name, the five region records' names and the empty kill-count vector
// Leaf: ??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E376D0, 89 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): stores vtable 0x012E35B0, the RTTI-less level-script vtable whose slot 4 is the landed
//   CV_AssassinAttacksScript::OnPersist 0x00E39780 (hand508; its Alloc 0x00E39850 news 0xB8 bytes, registered "S_VAA");
//   FableWin.pdb ??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (the
//   level-script constructor signature of the landed CQ_GuildTrainingMeleeScript ctor 0x00D55D70). Only caller 0x00E3986B
//   (the Alloc). Label correction: the manifest row "CInputProcessManager::CInputProcessManager" is propagated.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0 (six times).
// Layout: Ego_r.pdb CV_AssassinAttacksScript members at Ego_r offset - 0x14 (retail CScriptBase is 0x40): RegionIndex
//   +0x48, AssassinObjectCount +0x4C, Activated +0x50, RegionOfMostRecentKill +0x54, RegionInfo RegionData[5] +0x58
//   (16 bytes: name, count, max, assassinActive), RegionKillCounts STLport vector<long> +0xA8 (0xC), QuestComplete
//   +0xB4, RewardsGiven +0xB5, GivenQuestCard +0xB6 (0xB8 bytes).
// Shape: the landed 0x00D55D70 level-script base; RegionData's inline default constructor builds only its name, so the
//   array is a five-step counted loop.
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
enum AA_LocationIdentifier { AA_LOCATION_NONE };
class CV_AssassinAttacksScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    class RegionData {
    public:
        CCharString name;                               // +0x00
        long count;                                     // +0x04
        long max;                                       // +0x08
        bool assassinActive;                            // +0x0c
    };
    CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CV_AssassinAttacksScript();
    AA_LocationIdentifier RegionIndex;                  // +0x48
    long AssassinObjectCount;                           // +0x4c
    bool Activated;                                     // +0x50
    CCharString RegionOfMostRecentKill;                 // +0x54
    RegionData RegionInfo[5];                           // +0x58
    std::vector<long> RegionKillCounts;                 // +0xa8
    bool QuestComplete;                                 // +0xb4
    bool RewardsGiven;                                  // +0xb5
    bool GivenQuestCard;                                // +0xb6
};
CV_AssassinAttacksScript::CV_AssassinAttacksScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}