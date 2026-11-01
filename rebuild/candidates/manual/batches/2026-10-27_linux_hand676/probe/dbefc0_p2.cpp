// NScript::CQ_NewOakValeIntroScript::~CQ_NewOakValeIntroScript() -- probe
// Leaf: ??1CQ_NewOakValeIntroScript@NScript@@UAE@XZ
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CScriptBase.h"

class CCharStringData;
class CCharString { public:
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base { public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase { public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                         // +0x40
    T*                        MasterScriptData;             // +0x44
};

#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };                 // Ego_r: byte-aligned (BarrelBrokenPos at +0x8A, WarehouseMeetPoint at +0x99)
#pragma pack(pop)

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_NewOakValeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_NewOakValeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool        VictimShake;                                // +0x48
    long        StopTimeIndex;                              // +0x4C
    bool        AttackOver;                                 // +0x50
    bool        DadFound;                                   // +0x51
    bool        DadFinishedIntro;                           // +0x52
    bool        DadOfferedRewards;                          // +0x53
    long        GoodDeedsPerformed;                         // +0x54
    long        BadDeedsPerformed;                          // +0x58
    long        GUIGoodDeedCounter;                         // +0x5C
    long        GUIBarrelCounter;                           // +0x60
    long        GUIBullyHealthCounter;                      // +0x64
    long        GuardsDealtWithBadDeeds;                    // +0x68
    bool        BullySubdued;                               // +0x6C
    bool        BullyRanOff;                                // +0x6D
    bool        GivenHeroTeddy;                             // +0x6E
    bool        HeroAttackedVictim;                         // +0x6F
    bool        SpokeAboutFindingTeddy;                     // +0x70
    bool        HeroDiscoveredInfidelity;                   // +0x71
    bool        BarrelManLeftHeroInCharge;                  // +0x72
    bool        BarrelManSpokenToHeroOnReturn;              // +0x73
    bool        BarrelBrokenInstantaneous;                  // +0x74
    bool        BarrelBrokenPersistent;                     // +0x75
    C3DVector   BarrelBrokenPos;                            // +0x76 (Ego_r 138: packed in the PDB)
    bool        InstructionGiven_Barrels;                   // +0x82
    bool        ReceiveKiss;                                // +0x83
    bool        ReceiveHug;                                 // +0x84
    C3DVector   WarehouseMeetPoint;                         // +0x85
    bool        TeddyRuined;                                // +0x91
    bool        GuardsSpokenOnce;                           // +0x92
    bool        TalkingToWoman;                             // +0x93
    bool        GivenSweets;                                // +0x94
    bool        GivenTheresaChocs;                          // +0x95
    bool        VictimComplainsAboutLosingTeddy;            // +0x96
    long        lastVillagerSpeechIdx;                      // +0x98
    std::vector<CCharString> VillagerSpeechMaleGood;        // +0x9C
    std::vector<CCharString> VillagerSpeechMaleBad;         // +0xA8
    std::vector<CCharString> VillagerSpeechMaleBoth;        // +0xB4
    std::vector<CCharString> VillagerSpeechMaleNeither;     // +0xC0
    std::vector<CCharString> VillagerSpeechFemaleGood;      // +0xCC
    std::vector<CCharString> VillagerSpeechFemaleBad;       // +0xD8
    std::vector<CCharString> VillagerSpeechFemaleBoth;      // +0xE4
    std::vector<CCharString> VillagerSpeechFemaleNeither;   // +0xF0
    bool        WhichBadDeedsPerformed[5];                  // +0xFC
    CTimer      TalkIntermittentTimer;                      // +0x104
    CTimer      WatchTimer;                                 // +0x108
};
CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00DAAC00
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
