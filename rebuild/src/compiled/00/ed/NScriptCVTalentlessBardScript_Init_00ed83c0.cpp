// NScript::CV_TalentlessBardScript::Init() -- the talentless bard level script's Init: clear the bard flags and the sung-about count, start the bard wander timer from the script def's BardWanderResetTime, fill the eleven quest name / song text-tag pairs of QuestInfo, size SongThingAvailable to the twelve song slots and clear each (yielding to a thread termination)
// Leaf: ?Init@CV_TalentlessBardScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED83C0, 480 bytes, thiscall, plain `ret`. Slot 3 (Init) of the RTTI-less level-script
//   vtable 0x012F3428 (word 0x012F3434), whose slot 4 is the landed OnPersist 0x00ED85A0 and which the landed constructor
//   0x00ED76E0 stores.
// Identity (proven, two sources): (1) the vtable slot (CScriptBase slots 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist);
//   (2) Ego_r.pdb holds ?Init@CV_TalentlessBardScript@NScript@@UAEXXZ and it is the one CV_TalentlessBardScript virtual the catalog
//   had no row for (Main 0x00ED7830, RegisterMain 0x00ED7790, OnPersist 0x00ED85A0 are landed); the members written are the
//   Ego_r.pdb CV_TalentlessBardScript members at retail offsets (Ego_r - 0x14, - 0x18 after the SongItems vector) and the
//   quest / text-tag literals fill its QuestData QuestInfo[12]. Catalog row "_gapscan::sub_00ed83c0" carried no name.
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (??4CCharString@@QAEABV0@PBD@Z), std::vector<bool>::resize 0x00467686
//   (STLport, one byte per bool), CScriptBase::IsActiveThreadTerminating 0x00CB7940; pDSTGame 0x0143E8F8 slot 89 SetTimer (the
//   inline CTimer::operator=(long)); pGlobalScriptDef 0x0143E90C (+0x7c8 = BardWanderResetTime: Ego_r +0x828 less the 24 vectors
//   shrunk 16 -> 12 before it, as the landed CTB_Bard::Init 0x00ED7E00 places BardWanderDistanceMin/Max at +0x7c0/+0x7c4).
// Data: quest names 0x012C5A18 "Q_WaspBoss" .. and their TEXT_QST_163_* tags 0x012F3658 ...; QuestInfo indices 9, 5, 6, 10, 7,
//   1, 2, 8, 11, 0, 3 (index 4 stays empty: the retail source order, no enum names in either PDB).
// Layout: retail vector<bool> is a 12-byte byte vector (the loop stores bytes through _M_start and compares the byte distance
//   _M_finish - _M_start; the landed OnPersist's 0x14 bit-vector model is header debt), so RandomSpeechTimer is at +0xe0.
// Shape: the flag stores are sunk to their retail order by writing them in that order; the timer assignment is the inline
//   CTimer::operator= through the global interface; the clear loop re-reads begin / end each pass (`SongThingAvailable[i] = false`
//   with `i < size()`), the termination test first; the trailing `if (IsActiveThreadTerminating()) return;` keeps only the call.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                      // +0x0
    _Tp* _M_finish;                                     // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {   // STLport 4.6, 0xC
public:
    typedef unsigned int size_type;
    typedef _Tp& reference;
    size_type size() const { return (size_type)(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
    void resize(size_type __new_size, const _Tp& __x);  // 0x00467686 (vector<bool>: one byte per element)
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CScriptThing.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4); the inline operator=(long) is
//   the interface's SetTimer through the global pointer (retail expands it here).
#include "engine/CTimer.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
extern CGameScriptInterfaceBase* pDSTGame;              // 0x0143E8F8
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
// Layout: Ego_r.pdb CScriptDef BardWanderResetTime +0x828 (2088) less the 24 vectors shrunk 16 -> 12 before it = retail +0x7c8.
class CScriptDef : public CParentDefClassBase {
    unsigned char _pad_0x39[0x7c8 - 0x39];
public:
    long BardWanderResetTime;                           // +0x7c8
};
extern CDefPointer<const CScriptDef> pGlobalScriptDef;  // 0x0143E90C
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class QuestData {                                   // Ego_r.pdb QuestData@CV_TalentlessBardScript@NScript@@, 8 bytes
    public:
        CCharString name;
        CCharString tag;
    };
    bool bHurtBard;                                     // +0x48
    bool bBard_finished;                                // +0x49
    unsigned long lNumber_of_things_sung_about;         // +0x4c
    bool bRewardGiven;                                  // +0x50
    CScriptThing stBard;                                // +0x54
    bool BardIntroComplete;                             // +0x60
    bool AttackedByHero;                                // +0x61
    bool BardFollowing;                                 // +0x62
    CTimer BardWanderTime;                              // +0x64
    std::vector<CCharString> SongItems;                 // +0x68
    QuestData QuestInfo[12];                            // +0x74
    std::vector<bool> SongThingAvailable;               // +0xd4
    CTimer RandomSpeechTimer;                           // +0xe0
    virtual void Init();                                // slot 3
};

void CV_TalentlessBardScript::Init()
{
    AttackedByHero = false;
    BardFollowing = false;
    BardIntroComplete = false;
    bBard_finished = false;
    bRewardGiven = false;
    bHurtBard = false;
    lNumber_of_things_sung_about = 0;
    BardWanderTime = pGlobalScriptDef->BardWanderResetTime;
    QuestInfo[9].name = "Q_WaspBoss";
    QuestInfo[9].tag = "TEXT_QST_163_QUEST_WASP_BOSS";
    QuestInfo[5].name = "Q_OrchardFarmRaidGood";
    QuestInfo[5].tag = "TEXT_QST_163_QUEST_ORCHARD_FARM_GOOD";
    QuestInfo[6].name = "Q_OrchardFarmRaidEvil";
    QuestInfo[6].tag = "TEXT_QST_163_QUEST_ORCHARD_FARM_EVIL";
    QuestInfo[10].name = "Q_WhiteBalverineKnotholeGlade";
    QuestInfo[10].tag = "TEXT_QST_163_QUEST_WHITE_BALV";
    QuestInfo[7].name = "Q_PrisonEscapeRescueMother";
    QuestInfo[7].tag = "TEXT_QST_163_QUEST_PRISON";
    QuestInfo[1].name = "Q_BanditCamp";
    QuestInfo[1].tag = "TEXT_QST_163_QUEST_BANDIT_CAMP";
    QuestInfo[2].name = "Q_OpeningGraveyardSecretPassage";
    QuestInfo[2].tag = "TEXT_QST_163_QUEST_GRAVEYARD";
    QuestInfo[8].name = "Q_TraderEscort";
    QuestInfo[8].tag = "TEXT_QST_163_QUEST_DARKWOOD_TRADERS";
    QuestInfo[11].name = "Q_WizardBattle";
    QuestInfo[11].tag = "TEXT_QST_163_QUEST_WIZARD_BATTLE";
    QuestInfo[0].name = "Q_Arena";
    QuestInfo[0].tag = "TEXT_QST_163_QUEST_ARENA";
    QuestInfo[3].name = "Q_EndGameBossBattle";
    QuestInfo[3].tag = "TEXT_QST_163_QUEST_JACK";
    SongThingAvailable.resize(12, false);
    for (unsigned int i = 0; i < SongThingAvailable.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[i] = false;
    }
    if (IsActiveThreadTerminating())
        return;
}
}