// CTCBoastingPodium::OnHeroBoast(long boastId, CThing* hero) -- a live hero made a boast: pay its cost out of the hero's money, mark the boast as made and count one boasting position fewer
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813C50, length 168 bytes (+ int3 padding), `ret 8`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 (the by-value "" script names), CQuestManager::GetBoast(long, CCharString)
// @ 0x004AF7A0 and CQuestManager::SetBoastAsMade(long, CCharString) @ 0x004B1A80 on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC for CTCHeroStats, type 4 = bit 4 of
// CThing+0x20), CTCHeroStats::AddMoney(long, bool) @ 0x0057B327.
// Layout: Ego_r.pdb CTCBoastingPodium LastPositionsWithBoastCount +0x20 sits at retail +0x1C (12-byte vector in the
// BoastingPositions CVectorMap); CBoast Cost +4; retail CThing Dead = bit 0 of +0x91.
// Shape: the quest manager is read into a local before each call's arguments (inline helpers); each by-value CCharString is built straight in its argument slot (`push ecx ; mov ecx,esp`) from the "" literal;
// HasTC narrows to a byte test, the one-ternary GetTC leaves the component in ecx for AddMoney.
struct CTCBaseInterface;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad54[0x3d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((CTCBaseInterface*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second);
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};
struct CBoast {
    long ID;                                    // +0x00
    long Cost;                                  // +0x04
    long Reward;                                // +0x08
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    CBoast* GetBoast(long id, CCharString scriptName);          // 0x004AF7A0
    void SetBoastAsMade(long id, CCharString scriptName);       // 0x004B1A80
};
__forceinline CBoast* GetBoast(long id)
{
    CQuestManager* questManager = (&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get());
    return questManager->GetBoast(id, "");
}
__forceinline void SetBoastAsMade(long id)
{
    CQuestManager* questManager = (&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get());
    questManager->SetBoastAsMade(id, "");
}
class CTCHeroStats {
public:
    void AddMoney(long amount, bool showMessage);   // 0x0057B327
};
class CTCBoastingPodium {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char BoastingPositions[0x10];      // +0x0c CVectorMap<long, CIntelligentPointer<CThing>> (retail 16 bytes)
    long    LastPositionsWithBoastCount;        // +0x1c
    void OnHeroBoast(long boastId, CThing* hero);
};

void CTCBoastingPodium::OnHeroBoast(long boastId, CThing* hero)
{
    if (hero && !hero->IsDead()) {
        CBoast* boast = GetBoast(boastId);
        if (boast && hero->HasTC(4))
            hero->GetTC<CTCHeroStats>(4)->AddMoney(-boast->Cost, true);
    }
    SetBoastAsMade(boastId);
    --LastPositionsWithBoastCount;
}