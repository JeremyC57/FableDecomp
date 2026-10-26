// CTCHeroStats::~CTCHeroStats() -- destroy the hero stats component's members in reverse order (marriages list, primitive handle, drunkenness def pointer, death-recovery marker name, five long/position vectors, outstanding-crimes map), then the CTCBase base
// Leaf: ??1CTCHeroStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x005814E5, 241 bytes; called only by the vector deleting destructor 0x005814C9 =
//   CTCHeroStats vtable 0x0124F70C slot 0 (rtti-vtables.tsv). Label correction: the catalog says
//   CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile (propagated).
// Callees: list<CHeroMarriageInfo> dtor @ 0x0057FDBD and map<CCharString, MirroredCriminalStatus> dtor @ 0x00580A64 (out of
//   line; manifest "CActiveFile::OnReadFinished" is propagated), the primitive handle's persistent primitive virtual slots 3
//   (last-handle notification) and 1 (destroy), CBaseClass::~CBaseClass @ 0x0099A2E0, the def's virtual slot 1,
//   ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14 (x5), ~CTCBase @ 0x00686830 (tail jump). The size TU: `optimize("s", on)`
//   (`and [esi+4],0`, `pop ecx` after free).
// Layout: Ego_r.pdb CTCHeroStats with retail 12-byte vectors/maps and 8-byte list: OutstandingCrimes +0x0C, ExperienceSpentOn
//   +0x18, HeroStatsExperience +0x118, RegionTime +0x124, NumberOfTimesUsedAbility +0x130, HousePositions +0x160,
//   DeathRecoveryMarkerName +0x170, PDef +0x1BC, PrimitiveHandle +0x1C0 (CEnginePrimitiveHandle, vtable 0x0122F598),
//   Marriages +0x1D8.
// Shape: an empty user destructor of a `__declspec(novtable)` class (the vtable is never re-stored); members destroyed by
//   their inline destructors (the CEnginePrimitiveHandle model of the landed ~CParticleEmitter 0x00AF10E0, the CDefPointer
//   release through a local copy of the pointer -- releasing through the member reloads it and gives the long form -- and
//   STLport vector dtors), all `__forceinline` (the size TU otherwise calls them out of line), and the two out-of-line
//   container destructors; ~CTCBase is a tail jump.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
class CBaseClass { public:                                 // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                   // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                                // +0x04
};
template <class T> class CIVCountedPointer { public:       // intrusive counted pointer (count at +4 of the object)
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    __forceinline ~CIVCountedPointer() { Release(); }
    __forceinline void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    __forceinline T* operator->() const { return m_p; }
    __forceinline long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};
class CEnginePrimitiveHandle : public CBaseClass { public:        // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        T* p = Object;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
};
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                 // 0x0099EAE0
};
template <class T> struct CVectorStorage {          // STLport vector<T> (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CVectorStorage() { if (_M_start) free(_M_start); }
};
class CRegion;
class C3DVector { public: float X, Y, Z; };
struct SHousePosition { const CRegion* Region; C3DVector Position; };   // pair<const CRegion*, C3DVector>
struct CCriminalStatusMap {                         // map<CCharString, CTCHeroStats::MirroredCriminalStatus>
    unsigned char _body[0xc];
    ~CCriminalStatusMap();                          // 0x00580A64
};
struct CMarriageList {                              // list<CHeroMarriageInfo>
    unsigned char _body[0x8];
    ~CMarriageList();                               // 0x0057FDBD
};
class CSpecialAbilitiesDrunkennessDef : public CDefPointee { public:};
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();                             // 0x00686830
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class __declspec(novtable) CTCHeroStats : public CTCBase {
public:
    CCriminalStatusMap      OutstandingCrimes;      // +0x0c
    CVectorStorage<long>    ExperienceSpentOn;      // +0x18
    unsigned char           _pad24[0x118 - 0x24];   // NumberOfDecapitations .. TrainingStatus
    CVectorStorage<long>    HeroStatsExperience;    // +0x118
    CVectorStorage<long>    RegionTime;             // +0x124
    CVectorStorage<long>    NumberOfTimesUsedAbility;   // +0x130
    unsigned char           _pad13c[0x160 - 0x13c]; // TotalSpentExperience .. BiggestFish
    CVectorStorage<SHousePosition> HousePositions;  // +0x160
    bool                    InfiniteStamina;        // +0x16c
    CCharString             DeathRecoveryMarkerName;    // +0x170
    unsigned char           _pad174[0x1bc - 0x174]; // FailQuestOnDeath .. UpdateDrunkenness
    CDefPointer<CSpecialAbilitiesDrunkennessDef> PDef;  // +0x1bc
    CEnginePrimitiveHandle  PrimitiveHandle;        // +0x1c0
    unsigned char           _pad1c8[0x1d8 - 0x1c8]; // UCounter, VCounter, RotCounter, IsInCutSceneFirstTime
    CMarriageList           Marriages;              // +0x1d8
    virtual ~CTCHeroStats();
};

CTCHeroStats::~CTCHeroStats()
{
}