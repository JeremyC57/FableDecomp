// CAIStateGroup_SitRoundCampfire::IsRunnable() -- not for a creature with the 0x4a component; only when its idle scheduler prefers this group and (for a villager) it is the village's sociable time: reset the seat state and latch the nearest campfire, runnable when it is alive
// Grade: DIFFER(358v358) -- this in esi / interface map in edi (retail edi / esi); rewritten on the landed sibling CAIStateGroup_BanditSitRoundCampfire::IsRunnable 0x0093AE80 model with PDB names (IsSociableTime, GetPVillage const, private const GetPNearCampfire); nested-if lookups (MinionSitOutside shape) and a named creature local: inert
// Leaf: ?IsRunnable@CAIStateGroup_SitRoundCampfire@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939670, 358 bytes (+ int3 padding), virtual (slot 11), plain `ret`.
//   Ego_r.pdb ?IsRunnable@CAIStateGroup_SitRoundCampfire@@UAE_NXZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xd3 = CTCIdleScheduler, bit 19 of +0x38, and 0x23 =
//   CTCVillageMember, bit 3 of +0x24), CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) 0x00801BE0 (landed),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (twice), CTCVillage::IsSociableTime() const 0x00658D5D (landed; Ego_r
//   donor order IsNightTime / IsSociableTime / IsCurfewTime = retail 0x00658D07 / 0x00658D5D / 0x00658DB3),
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (temporary
//   CIntelligentPointer<CThingObject>, vtable 0x0125A858), CAIStateGroup_SitRoundCampfire::GetPNearCampfire() const
//   0x009395F0 (Ego_r.pdb private ABE).
// Layout / shape: the landed sibling CAIStateGroup_BanditSitRoundCampfire::IsRunnable 0x0093AE80 model (engine/
//   CAIStateGroup_SitRoundCampfire.h flattens the vptr and PCampfire, so the polymorphic model is local, as there):
//   PBrain +0x04 (brain +0x18 creature, re-read after every call), PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20,
//   Sitting +0x21; IS_THING_ALIVE through CThing::IsDead() const.
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


class CTCVillage { public:
    bool IsSociableTime() const;                // 0x00658D5D
};
class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCIdleScheduler { public:
    bool IsPreferredIdle(CAIStateGroupBase* idle);   // 0x00801BE0
};
class CThingObject;
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    CIntelligentPointerThingObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThingObject& operator=(const CIntelligentPointerThingObject& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem(item);
        return *this;
    }
};
class CAIStateGroup_SitRoundCampfire : public CAIStateGroupBase { public:
    CIntelligentPointerThingObject PCampfire;   // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
private:
    CThingObject* GetPNearCampfire() const;     // 0x009395F0
public:
    virtual bool IsRunnable();
};

bool CAIStateGroup_SitRoundCampfire::IsRunnable()
{
    if (PBrain->PCreature->HasTC(0x4a))
        return false;
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3) && !scheduler->IsPreferredIdle(this))
        return false;
    CTCVillageMember* member;
    if (PBrain->PCreature->GetInterface(&member, 0x23) && member->GetPVillage()
        && !member->GetPVillage()->IsSociableTime())
        return false;
    GotSeatPos = false;
    Sitting = false;
    SeatPosIndex = -1;
    PCampfire = CIntelligentPointerThingObject(0);
    PCampfire = CIntelligentPointerThingObject((CThing*)GetPNearCampfire());
    if (IS_THING_ALIVE(PCampfire))
        return true;
    return false;
}
