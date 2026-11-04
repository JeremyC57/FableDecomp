// NScript::CQ_NewOakValeIntroScript::~CQ_NewOakValeIntroScript() -- implicit quest-script destructor: deregister WatchTimer and TalkIntermittentTimer, destroy the eight villager-speech CCharString vectors in reverse order, then ~CScriptBase
// Leaf: ??1CQ_NewOakValeIntroScript@NScript@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBEFC0, 450 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00DBEFA3 is the scalar deleting destructor 0x00DBEFA0, slot 0 of the RTTI-less vtable
//   0x012D7A28; that vtable is stored only by the ctor 0x00DAAC00 (landed as ??0CQ_NewOakValeIntroScript), whose only caller is
//   the Alloc 0x00DBEF70 (`push 10Ch ; call new`), stored at 0x00CD6E55 by the script-registration routine beside the save tag
//   "S_QNOVI" (0x012F789C); refs/script_recovery/conversion_queue.tsv maps S_QNOVI -> Q_NewOakValeIntro -> Alloc 0x00DBEF70. Ego_r.pdb
//   ??1 / ??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z; the body stores no vtable
//   of its own (compiler-generated). The catalog label CParticleEmitter_Dtor_7 is propagated and wrong.
// Layout: Ego_r.pdb CQ_NewOakValeIntroScript, 0x14 lower in retail (CScriptBase 0x40) and 4 lower per earlier Dinkumware vector:
//   lastVillagerSpeechIdx +0x98, VillagerSpeechMaleGood .. VillagerSpeechFemaleNeither +0x9C .. +0xF0 (STLport 12 bytes each),
//   WhichBadDeedsPerformed[5] +0xFC, TalkIntermittentTimer +0x104, WatchTimer +0x108; retail size 0x10C (Alloc). The PDB places
//   BarrelBrokenPos / WarehouseMeetPoint at the odd offsets Ego_r +0x8A / +0x99: the script headers' C3DVector is byte-aligned, so
//   the local C3DVector carries pack(1) (the true layout, as GOTCHAS 0xDB5260); without it every later member shifts by 4.
// Callees: ~CTimer inline (pDSTGame 0x0143E8F8 slot 88 DeregisterTimer, engine/CTimer.h), ~CCharString 0x0099EAE0
//   per element and free 0x00BFEA14 (STLport ~vector: _Destroy range + _Vector_base storage free), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined with an empty body (retail's TU defines it) to emit the vtable, ??_G and this implicit dtor; only
//   the dtor is graded. CLevelScriptBase<T> is a local copy without the dtor declaration (engine/CLevelScriptBase.h header debt,
//   chunks 665/673); CCharString is the one-pointer local model of the landed quest rows. Matched on the second probe.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
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
#include "engine/C3DVector.h"
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
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_NewOakValeIntroScript) == 0x10C);