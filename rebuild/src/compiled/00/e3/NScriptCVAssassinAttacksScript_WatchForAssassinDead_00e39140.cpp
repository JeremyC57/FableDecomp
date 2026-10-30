// NScript::CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThing assassin) -- the per-assassin "WatchForAssassinDead" spawned thread of the Assassin Attacks quest: every script frame ask the assassin who killed it; once someone has, hand the hero the OBJECT_QUEST_CARD_ASSASSIN_ATTACKS card (first kill only), record the kill region, clear that region's active-assassin flag and, when the killer is the hero, bump the region's kill count and award the assassin experience; stop when the assassin dies unkilled or the thread is terminated
// Leaf: ?WatchForAssassinDead@CV_AssassinAttacksScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E39140, 967 bytes (`ret 0Ch`: the by-value CScriptThing, destroyed by the callee).
// Identity: Ego_r.pdb / FableWin.pdb ?WatchForAssassinDead@CV_AssassinAttacksScript@NScript@@QAEXVCScriptThing@@@Z; its
//   spawner stores 0x00E39140 in a CSpawnedFunc1<CV_AssassinAttacksScript, CScriptThing> (vtable 0x012E37BC) named
//   "WatchForAssassinDead" (0x012E37D8) at 0x00E39081.
// Callees: CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0, CCharString copy
//   0x0099EC30, ~CCharString 0x0099EAE0, CCharString::operator= 0x0099EFB0, CBasicString<char>::Compare 0x00411570,
//   GetRegionIdForRegion 0x00E38290; inline CScriptThing wrappers MsgWhoKilledMe (slot 19), IsNull (77), IsAlive (75),
//   GetName (1, null arm CScriptThing::NullString 0x0143E8EC); Game slots 7 NewScriptFrame, 15 GetRegionName,
//   128 GiveHeroExperience, 295 GiveHeroQuestCardDirectly, 655 GetActiveQuestName.
// Data: "OBJECT_QUEST_CARD_ASSASSIN_ATTACKS" 0x012E3814, "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E;
//   pGlobalScriptDef 0x0143E90C +0x3E4 = Ego_r CScriptDef::AssassinExperience (+0x418; 13 Dinkumware vectors ahead).
// Layout (retail = Ego_r.pdb - 0x14, STLport vector 12 bytes): RegionOfMostRecentKill +0x54, RegionInfo[5] +0x58
//   (RegionData {name, count +4, max +8, assassinActive +0xC}), RegionKillCounts +0xA8, GivenQuestCard +0xB6.
// Shape: ~CScriptThing is __forceinline; retail expands it (with the by-value parameter's) at the loop-terminate exit and
//   the two terminate checks after a kill, and calls 0x004AA840 at the other four exits: those `return`s are written
//   under `#pragma inline_depth(0)`.
//   The loop is `for(;;) { if (IsActiveThreadTerminating()) return; ...; Game->NewScriptFrame(); }` with the killed and
//   died-alive exits written as `break` to the function end: that gives retail's rotated loop and shared cleanup tail.
#include <string.h>
class CCharString;
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
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    struct InlineCopy { };                              // tag: the copy retail expands inline
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(other), PImp(other.PImp) {}
    __forceinline virtual ~CScriptThing() {}            // 0x004AA840 where it stays out of line
    __forceinline virtual const CCharString& GetName() const   // slot 1 (+0x04) 0x004AA8B0
    {
        if (!PImp.Data)
            return NullString;
        return PImp.Data->GetName();
    }
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018();
    virtual CScriptThing MsgWhoKilledMe() const         // slot 19 (+0x4c) 0x004ABE60
    {
        if (!PImp.Data)
            return CScriptThing(*this, InlineCopy());
        return PImp.Data->MsgWhoKilledMe();
    }
    virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049();
    virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059();
    virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069();
    virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const                        // slot 75 (+0x12c) 0x004AB130
    {
        return PImp.Data != 0 && PImp.Data->IsAlive();
    }
    virtual void t076();
    virtual bool IsNull() const                         // slot 77 (+0x134) 0x004AB150
    {
        return PImp.Data == 0 || PImp.Data->IsNull();
    }
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    static CCharString NullString;                      // 0x0143E8EC
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDef { public:
    unsigned char _pad[0x3e4];
    long AssassinExperience;                            // +0x3e4
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_AssassinAttacksScript : public CScriptBase {
public:
    enum AA_LocationIdentifier { AA_FIRST_LOCATION };
    class RegionData { public:                          // 16 bytes
        CCharString name;                               // +0x00
        long        count;                              // +0x04
        long        max;                                // +0x08
        bool        assassinActive;                     // +0x0c
        RegionData();
    };
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    AA_LocationIdentifier     RegionIndex;              // +0x48
    long                      AssassinObjectCount;      // +0x4c
    bool                      Activated;                // +0x50
    CCharString               RegionOfMostRecentKill;   // +0x54
    RegionData                RegionInfo[5];            // +0x58
    std::vector<long>         RegionKillCounts;         // +0xa8
    bool                      QuestComplete;            // +0xb4
    bool                      RewardsGiven;             // +0xb5
    bool                      GivenQuestCard;           // +0xb6
    AA_LocationIdentifier GetRegionIdForRegion(CCharString region);   // 0x00E38290
    void WatchForAssassinDead(CScriptThing assassin);
};
}

void NScript::CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThing assassin)
{
    CScriptThing killer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        killer = assassin.MsgWhoKilledMe();
        if (!killer.IsNull()) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            if (!GivenQuestCard) {
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ASSASSIN_ATTACKS", Game->GetActiveQuestName(), true);
                GivenQuestCard = true;
            }
            RegionOfMostRecentKill = Game->GetRegionName();
            AA_LocationIdentifier region = GetRegionIdForRegion(RegionOfMostRecentKill);
            RegionInfo[region].assassinActive = false;
            if (killer.GetName() == "SCRIPT_NAME_HERO") {
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                RegionKillCounts[region] = ++RegionInfo[region].count;
                Game->GiveHeroExperience(pGlobalScriptDef->AssassinExperience);
            }
            break;
        }
        if (!assassin.IsAlive()) {
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            break;
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
}
#pragma inline_depth()