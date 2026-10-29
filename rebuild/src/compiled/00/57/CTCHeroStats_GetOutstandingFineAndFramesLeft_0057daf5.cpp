// CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft) -- after expiring old crimes, the region's total outstanding fine (count times the deed's fine) and the most frames left before any of its crimes is forgotten
// Leaf: GetOutstandingFineAndFramesLeft
//
// Retail 0x0057DAF5, 301 bytes (`ret 0Ch`; frame pointer, `leave`). Ego_r.pdb
// `?GetOutstandingFineAndFramesLeft@CTCHeroStats@@QAEXABVCCharString@@AAJ1@Z`. Size TU with a frame pointer:
// `#pragma optimize("y", off)` + `("s", on)`, as UpdateOutstandingCrimes 0x0057D9FD (same model).
// Callees: CTCHeroStats::UpdateOutstandingCrimes @ 0x0057D9FD, std::bitset<>::test @ 0x00410DE0, CVectorMap<...>::LowerBound
// @ 0x0040F020 (deed log, type 0x51), map<CCharString,MirroredCriminalStatus>::operator[] @ 0x005812DF,
// CTCHeroOpinionDeedLog::GetDeedEffectsDef @ 0x006C5630 (twice per crime), CTCBase::GetWorld @ 0x00686A70, CWorld::GetWorldFrame
// @ 0x0049D870 (static).
// Layout: as 0x0057D9FD; retail COpinionDeedEffectsDef CrimeFineAmount +0x74 (PDB +0x88).
// Shape: framesLeft cleared before fine; the deed log pointer lives in the dead `fine` slot (fine itself in edi); per crime the
// fine adds `def->CrimeFineAmount * Crimes[crime]` (def fetched first through `operator->`), the expiry frame is a statement of
// its own (its def temporary is released before the frame query) and framesLeft takes the Dinkumware `Max` with the
// remaining frames (a frame temporary passed by const reference).
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

class CTCBase;
class CCharStringData;
class CCharString { public:
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
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0x3c - 0x08];
    long KnowledgeLifeInSeconds;                            // +0x3c
    unsigned char _pad40[0x74 - 0x40];
    long CrimeFineAmount;                                   // +0x74 (PDB +0x88)
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
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
class CTCHeroOpinionDeedLog { public:
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
    void UpdateOutstandingCrimes(const CCharString& region);   // 0x0057D9FD
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);
};

void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft)
{
    UpdateOutstandingCrimes(region);
    framesLeft = 0;
    fine = 0;
    CTCHeroOpinionDeedLog* deedLog = 0;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)parent->GetTC(0x51);
    for (long crime = 0; crime < 10; ++crime) {
        if (OutstandingCrimes[region].Crimes[crime] != 0) {
            fine += deedLog->GetDeedEffectsDef(crime)->CrimeFineAmount * OutstandingCrimes[region].Crimes[crime];
            long expiry = deedLog->GetDeedEffectsDef(crime)->KnowledgeLifeInSeconds * 15
                        + OutstandingCrimes[region].LastTimeSeenByGuard;
            framesLeft = Max(framesLeft, expiry - GetWorld().GetFrame());
        }
    }
}