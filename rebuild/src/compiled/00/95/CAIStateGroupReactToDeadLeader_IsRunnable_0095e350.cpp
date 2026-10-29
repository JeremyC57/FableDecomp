// Retry 10b (getsw sweep, FIXES B-15): the message event manager is read through an inline getter `GetMessageEventManager()`; the plain member read gives the parked register/scheduling difference.
// CAIStateGroup_ReactToDeadLeader::IsRunnable() -- runnable when a recent leader-death message lies within 20 units
// (2D) of the creature and its killer is a live-flagged creature (all four low state bits set); remember the killer in
// PEnemy and restart FramesRunning.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E350, length 213 bytes (+ int3 padding).
// Callees: the world getter @ 0x008FCFA0 (+0x60 = CMessageEventManager), CMessageEventManager::FindMostRecentMessage<
// leader-death filter> @ 0x0095E6D0, the creature position getter @ 0x004C73D0, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on the message's +0x48 pointer (three times), the converting CIntelligentPointer temporary (vtable
// 0x0123E7D8): ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem, dtor @ 0x00A01C10; 400.0f @ 0x0125D824.
// Shape: the one-word filter is a block-scoped named local built before the world call (its slot is reused by the
// pointer temporary); `C2DVector(msg.X - pos.X, msg.Y - pos.Y).GetMagnitudeSquared() < 400`
// keeps both components on the x87 stack (`fstp st(0)` twice after the compare); `(~flags & 0xF) == 0` gives
// `not ecx ; test cl,0Fh`. Engine header names (PEnemy +0x14, FramesRunning +0x1c); base modelled locally.
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

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
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
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
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
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetMagnitudeSquared() const { return X * X + Y * Y; }
};

struct CThingCreaturePos : public CThing {
};
struct CThingState {
    unsigned char _pad[0x6c];
    unsigned long StateFlags;                   // +0x6c
};

struct CMessageEventFilter_IsLeaderDeath {
    CThing* PCreature;
    CMessageEventFilter_IsLeaderDeath(CThing* creature) : PCreature(creature) { }
};
class CMessageEvent { public:
    unsigned char _pad[0xc];
    C3DVector Pos;                              // +0x0c
    unsigned char _pad_0x18[0x30];
    CBaseIntelligentPointerCtor PKiller;        // +0x48
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsLeaderDeath& filter);   // 0x0095E6D0
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    CMessageEventManager* GetMessageEventManager() const { return PMessageEventManager; }
};

struct CAIStateGroup_ReactToDeadLeader : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14 CIntelligentPointer<CThingCreatureBase>
    long FramesRunning;                         // +0x1c
    virtual bool IsRunnable();
};

bool CAIStateGroup_ReactToDeadLeader::IsRunnable()
{
    CMessageEvent* message;
    {
        CMessageEventFilter_IsLeaderDeath filter(PBrain->PCreature);
        message = PeekWorld().GetMessageEventManager()->FindMostRecentMessage(filter);
    }
    if (message != 0) {
        const C3DVector& pos = ((CThingCreaturePos*)PBrain->PCreature)->GetPos();
        if (C2DVector(message->Pos.X - pos.X, message->Pos.Y - pos.Y).GetMagnitudeSquared() < 400.0f) {
            CBaseIntelligentPointerCtor& killer = message->PKiller;
            if (killer.GetPItem() != 0 && (~((CThingState*)killer.GetPItem())->StateFlags & 0xf) == 0) {
                CThing* enemy = killer.GetPItem();
                if (enemy != 0) {
                    PEnemy = enemy;
                    FramesRunning = 0;
                    return true;
                }
            }
        }
    }
    return false;
}