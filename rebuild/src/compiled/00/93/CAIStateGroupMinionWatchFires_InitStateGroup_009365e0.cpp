// CAIStateGroup_MinionWatchFires::InitStateGroup(CAIBrain&, CThingAICreature&) -- register with the idle scheduler as idle type 6.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009365E0, length 75 bytes. Vtable slot 13 of
// CAIStateGroup_MinionWatchFires (0x0128B44C).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup on the creature argument, type 0xD3 =
// bit 19 of the bitset dword at +0x38), CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase*, EIdleStateGroup)
// @ 0x00801920. Same shape as the landed CAIStateGroup_CleanHouse::InitStateGroup @ 0x00917AF0 (idle type 2).
// The lookup's bool is tested, so the miss path falls to the epilogue; the key temporary is the only local.
class CTCBase;
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the shape of the landed
// GetInterface_0x.. bodies, inlined into the AI bodies through GetTC<T>().
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0 (`[this+60h]+0Ch`)
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0 (PBrain->+0x1c->+0x24)
public:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 (world random seed)
    void Terminate();                           // 0x008FCF40 (Terminated = true)
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200 (EAIActionReturn)
};

class CTCIdleScheduler { public:                       // interface type 0xD3 component
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long idleType);   // 0x00801920
};

struct CAIStateGroup_MinionWatchFires : CAIStateGroupBase {
    unsigned char PBuildingOnFire[8];           // +0x14 CIntelligentPointer<CThingBuilding>
    virtual void InitStateGroup(CAIBrain& brain, CThing& creature);
};

void CAIStateGroup_MinionWatchFires::InitStateGroup(CAIBrain& brain, CThing& creature)
{
    CTCIdleScheduler* pScheduler;
    if (creature.GetInterface(&pScheduler, 0xD3))
        pScheduler->RegisterIdleStateGroup(this, 6);
}