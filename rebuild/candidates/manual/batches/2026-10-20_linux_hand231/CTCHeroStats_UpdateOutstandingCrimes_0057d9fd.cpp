// CTCHeroStats::UpdateOutstandingCrimes(const CCharString& region) -- forget every outstanding crime in the region whose deed knowledge has expired since the hero was last seen by a guard
// Leaf: UpdateOutstandingCrimes
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
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
};

struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool HasTC(long type) { return present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x3c - 0x08];
    long KnowledgeLifeInSeconds;                            // +0x3c
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
struct CTCHeroOpinionDeedLog {
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
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

struct CWorld {
    static long GetWorldFrame();                            // 0x0049D870
};

struct CTCHeroStats {
    void*           __vftable;                              // +0x00
    CThing*         ParentThing;                            // +0x04 CTCBase
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;                      // +0x0C
    CWorld* GetWorld() const;                               // 0x00686A70
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
            deedLog->GetDeedEffectsDef(crime);
            long expiry = deedLog->GetDeedEffectsDef(crime)->KnowledgeLifeInSeconds * 15
                        + OutstandingCrimes[region].LastTimeSeenByGuard;
            if (expiry < GetWorld()->GetWorldFrame())
                OutstandingCrimes[region].Crimes[crime] = 0;
        }
    }
}
