// CAIStateGroup_MinionReactToBurglary::IsRunnable() -- during the creature's sleep hours, runnable when the live
// player is visible to it and standing inside its home building; remember the player in PBurglar.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957F20, length 210 bytes (+ int3 padding).
// Callees: CThingAICreature::GetSleepTimes @ 0x00833390 (two out longs), CAIStateGroupBase::GetPWorld @ 0x008FCFC0
// (twice), the world clock's IsTimeOfDayInRange @ 0x006BB390 (world +0x1c), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on the creature's home-building pointer (+0x168), the world's player-manager getter @ 0x0049D9D0
// and its player getter @ 0x00487DD0 (manifest "_Cons_val<...>" labels are propagated and wrong),
// CanCreatureSeeCreature @ 0x008645E0, the thing position getter @ 0x004C73D0, CThingBuilding::IsPointInside
// @ 0x0082D000, CIntelligentPointer(CThing*) @ 0x0056C860, CIntelligentPointer::operator= @ 0x005ED300,
// ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the home pointer read through an inline getter (`lea ecx,[eax+168h]`); the time test result and the home pointer are named locals computed before the combined test (bl / ebp);
// `PBurglar = CIntelligentPointer(player)` through a named out-of-line-constructed pointer + operator=; the clock is a named local (GetPWorld before the argument loads).
// No engine header for this class; PBurglar is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public: float X, Y, Z; };

class CThingAICreature { public:
    void GetSleepTimes(long* start, long* end); // 0x00833390
};
struct CThingPos {
    const C3DVector& GetPosition();             // 0x004C73D0
};
class CThingBuilding { public:
    bool IsPointInside(const C3DVector& point); // 0x0082D000
};
struct CHomeOwner {
    unsigned char _pad[0x168];
    CBaseIntelligentPointerCtor PHome;          // +0x168
    CThing* GetPHome() { return PHome.GetPItem(); }
};

struct CWorldClock {
    bool IsTimeOfDayInRange(long start, long end);  // 0x006BB390
};
class CPlayerManager { public:
    CThing* GetPlayerCharacter();               // 0x00487DD0
};
class CWorld { public:
    unsigned char _pad[0x1c];
    CWorldClock* PClock;                        // +0x1c
    CPlayerManager* GetPlayerManager();         // 0x0049D9D0
};

bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0

struct CIntelligentPointerOut : CBaseIntelligentPointerCtor {
    CIntelligentPointerOut(CThing* item);       // 0x0056C860
    CIntelligentPointerOut& operator=(const CIntelligentPointerOut& other);   // 0x005ED300
};

struct CAIStateGroup_MinionReactToBurglary : CAIStateGroupBase {
    CIntelligentPointerOut PBurglar;            // +0x14
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToBurglary::IsRunnable()
{
    long start;
    long end;
    ((CThingAICreature*)PBrain->PCreature)->GetSleepTimes(&start, &end);
    CWorldClock* clock = GetPWorld()->PClock;
    bool sleepTime = clock->IsTimeOfDayInRange(start, end);
    CThingBuilding* home = (CThingBuilding*)((CHomeOwner*)PBrain->PCreature)->GetPHome();
    if (sleepTime && home != 0) {
        CThing* player = GetPWorld()->GetPlayerManager()->GetPlayerCharacter();
        if (player != 0 && !(player->m_flags91 & 1) && CanCreatureSeeCreature(PBrain->PCreature, player, false) &&
            home->IsPointInside(((CThingPos*)player)->GetPosition())) {
            CIntelligentPointerOut burglar(player);
            PBurglar = burglar;
            return true;
        }
    }
    return false;
}