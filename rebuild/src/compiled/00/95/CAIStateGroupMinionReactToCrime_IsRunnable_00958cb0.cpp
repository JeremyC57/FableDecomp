// CAIStateGroup_MinionReactToCrime::IsRunnable() -- for a villager who knows of a crime to run and report (the most recent such deed): validate a hero-crime deed against the hero's deed log, then runnable when a guard is on the level, the village does not already handle the deed and (except for crime types 7/8) nobody else is getting help
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958CB0, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0x23 bit 3 of +0x24 and 0x4f bit 15 of +0x28,
//          ternary picks into ecx) / non-const @ 0x0040F020 (0x51 = CTCHeroOpinionDeedLog on the hero, bit 17 of +0x28),
//          CTCVillageMember::GetPVillage @ 0x007EE540, CTCCreatureOpinionOfHero::FindMostRecentDeed<
//          COpinionDeedFilter_IsCrimeToRunAndFindAGuard> @ 0x006610C7, GetPPlayerManager @ 0x008FCF90 / GetMainPlayer
//          @ 0x00449970 / GetPControlledCreature @ 0x00487DC0, CTCHeroOpinionDeedLog::GetDeedFromHandle @ 0x006C58F0 and the deed
//          removal @ 0x006C59E0, IsGuardOnLevel @ 0x00958C00, the village's deed test @ 0x0065914B (manifest
//          "CBankFileAsyncData::StartRead" is wrong), GetSomeoneElseGettingHelpMessage @ 0x00958770.
// Shape: nested success path; both villager lookups are const one-ternary picks; the village pointer is a named local
//          spilled across the calls (its home doubles as the hero log lookup miss slot).
// Layout: engine/CAIStateGroup_MinionReactToCrime.h (OpinionDeedHandle +0x14, AllowDuplicateReactToCrime +0x19);
//          PBrain is the first dword at +0x04; the deed's +0x00 type / +0x04 / +0x24 handle are descriptive.
// Name fix (agentNF5 2026-11-10): CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 is const and returns a const pointer (?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z, Ego_r.pdb / FableWin.pdb).
#include "fable_compiler.h"
#include "engine/CAIStateGroup_MinionReactToCrime.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterfaceX(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class COpinionDeed { public:
    long Type;                                  // +0x00 (descriptive)
    long Data;                                  // +0x04 (descriptive)
    unsigned char _pad08[0x24 - 0x08];
    long Handle;                                // +0x24 (descriptive)
};
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:
    COpinionDeedFilter_IsCrimeToRunAndFindAGuard() { }
};
class CTCCreatureOpinionOfHero { public:               // interface 0x4f
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* handle,
                                     bool flag);   // 0x006610C7
};
class CTCHeroOpinionDeedLog { public:                  // interface 0x51
    const COpinionDeed* GetDeedFromHandle(long handle) const;   // 0x006C58F0
    void RemoveDeed(long data);                 // 0x006C59E0 (descriptive)
};
class CTCVillage { public:
    bool IsDeedHandled(long handle);            // 0x0065914B (descriptive)
};
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;              // 0x007EE540
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                    // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMessageEvent;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupMinionReactToCrimeMethods : CAIStateGroup_MinionReactToCrime {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    bool IsGuardOnLevel();                      // 0x00958C00
    CMessageEvent* GetSomeoneElseGettingHelpMessage(long handle);   // 0x00958770
    bool IsRunnable();
};

bool CAIStateGroupMinionReactToCrimeMethods::IsRunnable()
{
    CTCVillageMember* member;
    if (((const CThing*)GetCreature())->GetInterfaceX(&member, 0x23)) {
        CTCVillage* village = member->GetPVillage();
        CTCCreatureOpinionOfHero* opinion;
        if (village && ((const CThing*)GetCreature())->GetInterfaceX(&opinion, 0x4f)) {
            COpinionDeed* deed = opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(),
                                                            &OpinionDeedHandle, true);
            if (deed) {
                if (deed->Type == 0x35) {
                    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
                    if (hero) {
                        CTCHeroOpinionDeedLog* log;
                        hero->GetInterface(&log, 0x51);
                        if (!log->GetDeedFromHandle(deed->Handle)) {
                            log->RemoveDeed(deed->Data);
                            return false;
                        }
                    }
                    OpinionDeedHandle = deed->Handle;
                }
                if (IsGuardOnLevel() && !village->IsDeedHandled(OpinionDeedHandle)) {
                    long type = deed->Type;
                    if (type == 7 || type == 8) {
                        AllowDuplicateReactToCrime = true;
                        return true;
                    }
                    if (!GetSomeoneElseGettingHelpMessage(OpinionDeedHandle)) {
                        AllowDuplicateReactToCrime = false;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}