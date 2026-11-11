// CTCHeroStats::UpdateOutstandingCrimes(const CCharString& region) -- forget every outstanding crime in the region whose deed knowledge has expired since the hero was last seen by a guard
// Leaf: UpdateOutstandingCrimes
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
//
// Retail 0x0057D9FD, 248 bytes (`ret 4`; frame pointer, `leave`). Ego_r.pdb
// `?UpdateOutstandingCrimes@CTCHeroStats@@QAEXABVCCharString@@@Z`. Size TU with a frame pointer: `#pragma optimize("y", off)`
// + `("s", on)` (`and [ebp-4],0`, `push 51h ; pop edi`).
// Callees: std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the deed log
// lookup, type 0x51, zero preload), map<CCharString,MirroredCriminalStatus>::operator[] @ 0x005812DF (four times),
// CTCHeroOpinionDeedLog::GetDeedEffectsDef(long) @ 0x006C5630 (CDefPointer by hidden pointer, twice; label
// "CThingBuilding::GetFloorMaterial" is propagated), the def's virtual slot 1 (release to zero), CTCBase::GetWorld
// @ 0x00686A70, CWorld::GetWorldFrame @ 0x0049D870 (static).
// Layout: retail CTCHeroStats OutstandingCrimes +0x0c (12-byte STLport map, as the landed SetHeroKickPositionForCurrentRegion
// 0x0057D990); MirroredCriminalStatus Crimes[10] +0, LastTimeSeenByGuard +0x28; retail COpinionDeedEffectsDef
// KnowledgeLifeInSeconds +0x3c (PDB +0x40).
// Shape: the effects def is fetched twice per crime -- once as a discarded temporary (released at once and its word cleared),
// once for the knowledge life through the inline `operator->` (left operand: the def pointer is fetched before the
// second map lookup, the field after it); the def pointer temporary's
// destructor releases through the pointee's DecRef/GetRefCount and clears the word; the lookup key and the second temporary
// share a frame word.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
};

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> present;                                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    FABLE_FORCEINLINE bool HasTC(long type) { return present.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};

struct CRefCounted {
    mutable long m_count;   // agentNF16: mutable so the CDefPointer<const T> release compiles (model only; the count is the def's +4 ref count)
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                                 // slot 1
};
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0x3c - 0x08];
    long KnowledgeLifeInSeconds;                            // +0x3c
    FABLE_FORCEINLINE void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#include "engine/EOpinionDeedType.h"
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class CTCHeroOpinionDeedLog { public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType deedType) const;   // 0x006C5630 Ego_r/FableWin ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z (agentNF16: was GetDeedEffectsDef)
};

struct MirroredCriminalStatus {
    long  Crimes[10];                                       // +0x00
    long  LastTimeSeenByGuard;                              // +0x28
    long  LastTimeExited;                                   // +0x2C
    bool  ExitedUsingTeleporter;                            // +0x30
    float EntrancePosX;                                     // +0x34
    float EntrancePosY;                                     // +0x38
    float EntrancePosZ;                                     // +0x3C
    float EntranceAngle;                                    // +0x40
};
struct CCrimeStatusMap {                                    // std::map<CCharString, MirroredCriminalStatus> (retail 0x0C bytes)
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    MirroredCriminalStatus& operator[](const CCharString& key);  // 0x005812DF
};

class CWorld { public:
    static long __fastcall GetFrame();                            // 0x0049D870
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCHeroStats : public CTCBase { public:
    void*           __vftable;                              // +0x00
    CThing*         ParentThing;                            // +0x04 CTCBase
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;                      // +0x0C
    void UpdateOutstandingCrimes(const CCharString& region);
};

void CTCHeroStats::UpdateOutstandingCrimes(const CCharString& region)
{
    CTCHeroOpinionDeedLog* deedLog = 0;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)parent->GetTC(0x51);
    for (long crime = 0; crime < 10; ++crime) {
        if (OutstandingCrimes[region].Crimes[crime] != 0) {
            deedLog->PeekPDeedEffectsDef((EOpinionDeedType)(crime));
            long expiry = deedLog->PeekPDeedEffectsDef((EOpinionDeedType)(crime))->KnowledgeLifeInSeconds * 15
                        + OutstandingCrimes[region].LastTimeSeenByGuard;
            if (expiry < GetWorld().GetFrame())
                OutstandingCrimes[region].Crimes[crime] = 0;
        }
    }
}