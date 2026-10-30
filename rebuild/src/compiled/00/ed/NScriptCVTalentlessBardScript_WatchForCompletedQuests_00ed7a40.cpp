// NScript::CV_TalentlessBardScript::WatchForCompletedQuests() -- Talentless Bard watcher thread: mark the songs of four quests already completed, then wait for each quest-completed message and mark its song (Scythe info clears song 3, the Dragon Boss fight without the mask sets it)
// Leaf: ?WatchForCompletedQuests@CV_TalentlessBardScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7A40, 546 bytes, thiscall, plain `ret`.
// Identity (proven): the CSpawnedFunc<CV_TalentlessBardScript> built in the landed NScript::CV_TalentlessBardScript::Main
//   0x00ED7830 stores this address as its member function (site 0x00ED795B, its only reference), next to the
//   "WatchForCompletedQuests" literal 0x012F3458; Ego_r.pdb ?WatchForCompletedQuests@CV_TalentlessBardScript@NScript@@QAEXXZ
//   (donor 0x00F4E7D0).
//   Catalog label `TalentlessBard_OnScript` is wrong (chunk 580 already recorded the name).
// Callees: CCharString() 0x0099E4B0 / copy 0x0099EC30 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570 (inline
//   CCharString::operator==), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CV_TalentlessBardScript::GetQuestIndex
//   0x00ED7C70 (landed, by-value CCharString); CGameScriptInterfaceBase slots 7 NewScriptFrame, 26 MsgOnAnyQuestCompleted,
//   286 IsQuestCompleted.
// Literals: "QS_ScytheInfo" 0x012C4684, "Q_DragonBossFight" 0x012C45B4, "" 0x0122D70E.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT, CScriptBase 0x40); CV_TalentlessBardScript members at Ego_r.pdb
//   - 0x14, - 0x18 after the SongItems vector (retail STLport vectors are 0xC): QuestInfo QuestData[12] +0x74 (name +0,
//   tag +4), SongThingAvailable +0xD4, RandomSpeechTimer +0xE0 (the ctor 0x00ED76E0 zeroes three words at +0xD4 and Alloc
//   allocates 0xE4: the vector<bool> is a plain 12-byte vector indexed one byte per element, as the landed
//   ProcessSongsQuestComplete 0x00ED9B40 reads it). CQ_SunnyvaleMasterData::HeroWoreMask +0x133 (Ego_r, no shift).
// Shape: generated-script form (a termination check before every statement); the outer `while (!IsActiveThreadTerminating())`
//   is rotated by VC7.1 (one test before the loop, one at the bottom); the null test of questName is threaded once through
//   both literal compares because Compare's body is in the TU (GOTCHAS chunk 576, landed 0x00F088E0).
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
// The Compare body (landed 0x00411570) in this TU under auto_inline(off), as the landed 0x00F088E0 / 0x004FDCB0 do.
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CCountedPointer.h"
namespace std {
template <class T> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {   // STLport 4.6 _alloc.h: the end-of-storage pointer + the allocator
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
}
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _before[0x133];                       // +0x000 .. +0x133 (opaque here)
    bool HeroWoreMask;                                  // +0x133
};
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class QuestData {                                   // Ego_r.pdb, 8 bytes
    public:
        CCharString name;                               // +0x0
        CCharString tag;                                // +0x4
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
    long GetQuestIndex(CCharString name);               // 0x00ED7C70
    void WatchForCompletedQuests();
};

void CV_TalentlessBardScript::WatchForCompletedQuests()
{
    CCharString questName;
    if (Game->IsQuestCompleted(QuestInfo[9].name)) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[9] = true;
    }
    if (Game->IsQuestCompleted(QuestInfo[5].name)) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[5] = true;
    }
    if (Game->IsQuestCompleted(QuestInfo[6].name)) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[6] = true;
    }
    if (Game->IsQuestCompleted(QuestInfo[10].name)) {
        if (IsActiveThreadTerminating())
            return;
        SongThingAvailable[10] = true;
    }
    while (!IsActiveThreadTerminating()) {
        while (!Game->MsgOnAnyQuestCompleted(questName)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        long index = GetQuestIndex(questName);
        if (index != -1) {
            if (IsActiveThreadTerminating())
                return;
            SongThingAvailable[index] = true;
        }
        if (questName == "QS_ScytheInfo") {
            if (IsActiveThreadTerminating())
                return;
            SongThingAvailable[3] = false;
        } else if (questName == "Q_DragonBossFight") {
            if (!MasterScriptData->HeroWoreMask) {
                if (IsActiveThreadTerminating())
                    return;
                SongThingAvailable[3] = true;
            }
        }
        Game->NewScriptFrame();
    }
}
}   // namespace NScript