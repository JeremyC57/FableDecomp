// CTCCreatureOpinionOfHero::ReportAllCrimes(CTCCreatureOpinionOfHero* informed) -- for a villager with a village that has a
// guard: every crime this creature knows of (the 92 known-deed lists, then the pending list) that the village has not yet
// handled is passed on -- told to the given creature, or reported to the village when none is given
// Leaf: ReportAllCrimes
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x00684210, 519 bytes (`ret 4`).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (CTCVillager 0x23) / const @ 0x004365B0 (the hero's CTCHeroOpinionDeedLog
// 0x51), GFGetPVillageInterface(CTCVillager*) @ 0x007EE540 (__fastcall, called twice), CTCBase::GetThingSearchTools
// @ 0x00686D90, CThingSearchTools::GetPFirstLeafThing<CThingFilter_AICreatureHasProperty, CThingAICreature> @ 0x00685EE0
// (property 3), CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::GetHeroCreature
// @ 0x00487DD0, CTCHeroOpinionDeedLog::GetDeedFromHandle @ 0x006C58F0, CTCVillage::IsCrimeHandled @ 0x0065914B (descriptive)
// / ReportCrime @ 0x0065CB6B, CTCCreatureOpinionOfHero::InformOfDeed(long, bool) @ 0x00683590.
// Layout: as the landed DecriminaliseAllKnownCrimes (0x00684050): PendingDeeds +0x54, KnownDeeds list<long>[92] +0x1c8
// (retail 4-byte STLport lists); COpinionDeed type +0, Decriminalised +0x22, IsCrime +0x23, linked handle +0x24.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceConst(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };               // 0x00487DD0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
   // 0x00449960
struct CTCVillager;
class CTCVillage { public:
    bool IsCrimeHandled(long handle);                        // 0x0065914B
    void ReportCrime(long handle);                           // 0x0065CB6B
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)
class CThingAICreature;
class CThingFilter_AICreatureHasProperty { public:
    long Property;
    CThingFilter_AICreatureHasProperty(long property) : Property(property) { }
};
class CThingSearchTools { public:
    bool GetPFirstLeafThing(const CThingFilter_AICreatureHasProperty& filter, CThingAICreature** out) const;   // 0x00685EE0
};
class COpinionDeed { public:
    long          Type;                                      // +0x00
    unsigned char _pad04[0x22 - 0x04];
    bool          Decriminalised;                            // +0x22
    bool          IsCrime;                                   // +0x23
    long          LinkedHandle;                              // +0x24
};
class CTCHeroOpinionDeedLog { public: COpinionDeed const* GetDeedFromHandle(long a0) const; private:            // 0x006C58F0
};
struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    long       _M_data;
};
struct CLongList { SListNode* _M_node; };
__forceinline bool IsCrime(const COpinionDeed* deed)
{
    if (deed->Type < 2)
        return false;
    if (deed->Type > 9)
        return deed->Type == 0x35 && deed->LinkedHandle != -1;
    return deed->IsCrime && !deed->Decriminalised;
}
class CTCCreatureOpinionOfHero;
__forceinline void ReportList(CTCHeroOpinionDeedLog* log, CTCVillage* village, CTCCreatureOpinionOfHero* informed,
                              CLongList& deeds);
class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90
};

class CTCCreatureOpinionOfHero : public CTCBase { public:
    void*         __vftable;                                 // +0x00
    CThing*       Thing;                                     // +0x04
    unsigned char _pad08[0x54 - 0x08];
    CLongList     PendingDeeds;                              // +0x54
    long          LastFrameAppliedDeedEffect[92];            // +0x58
    CLongList     KnownDeeds[92];                            // +0x1c8
    void InformOfDeed(long handle, bool flag);               // 0x00683590
    void ReportAllCrimes(CTCCreatureOpinionOfHero* informed);
};
__forceinline void ReportList(CTCHeroOpinionDeedLog* log, CTCVillage* village, CTCCreatureOpinionOfHero* informed,
                              CLongList& deeds)
{
    SListNode* end = deeds._M_node;
    SListNode* it = end->_M_next;
    while (it != end) {
        long handle = it->_M_data;
        it = it->_M_next;
        COpinionDeed* deed = ((COpinionDeed*)log->GetDeedFromHandle(handle));
        if (deed && IsCrime(deed)) {
            if (deed->Type == 0x35 && deed->LinkedHandle != -1)
                handle = deed->LinkedHandle;
            if (!village->IsCrimeHandled(handle)) {
                if (informed)
                    informed->InformOfDeed(handle, false);
                else
                    village->ReportCrime(handle);
            }
        }
    }
}

void CTCCreatureOpinionOfHero::ReportAllCrimes(CTCCreatureOpinionOfHero* informed)
{
    CTCVillager* villager;
    if (!Thing->GetInterface(&villager, 0x23))
        return;
    if (!((CTCVillageMember*)villager)->GetPVillage())
        return;
    CTCVillage* village = ((CTCVillageMember*)villager)->GetPVillage();
    {
        CThingAICreature* guard;
        if (!PeekThingSearchTools().GetPFirstLeafThing(CThingFilter_AICreatureHasProperty(3), &guard))
            return;
    }
    const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (!hero)
        return;
    CTCHeroOpinionDeedLog* log;
    hero->GetInterfaceConst(&log, 0x51);
    for (int i = 0; i < 92; ++i)
        ReportList(log, village, informed, KnownDeeds[i]);
    ReportList(log, village, informed, PendingDeeds);
}