// CAIStateGroup_WorkHammerAnvil::OnReplace() -- give back the anvil's action point (when it was taken and the anvil
// is alive), stop the creature's current action, drop whatever is in both carry slots and make it pushable again.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944DB0, length 201 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PAnvil +0x15, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, user action point 0x2D = bit 13 of +0x24),
// CTCUserActionPointBase::ReleaseSlot(long) @ 0x007FEF40 (manifest "EditCyclePreviousKeyCamera" propagated; landed
// name), CThingCreatureBase::FinishCurrentAction @ 0x00662650, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0, CAIStateGroupBase::DestroyItem(const CCharString&) @ 0x008FD1B0 (twice),
// SetPushableByHero(bool) @ 0x0097E330. Literals 0x01265440 "CARRY_SLOT_RIGHT_HAND", 0x01265428 "CARRY_SLOT_LEFT_HAND".
// Shape: pack(1) layout from the PDB (TookActionPoint +0x14 then the unaligned PAnvil +0x15, ActionPointIndex
// +0x44); inline IS_THING_ALIVE; the slot names are implicit conversion temporaries (re-addressed with `lea`).
// Local model (the engine header's layout is padding apart from these names).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;

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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};


class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCharString { public:                            // 4 bytes: the shared string data pointer
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CTCUserActionPointBase {                 // interface 0x2D
    void ReleaseSlot(long slot);                // 0x007FEF40
};

class CThingCreatureBase { public:
    void FinishCurrentAction();                 // 0x00662650
};

struct CAIStateGroupBaseI : CAIStateGroupBase {
    void DestroyItem(const CCharString& slot);  // 0x008FD1B0
    void SetPushableByHero(bool pushable);      // 0x0097E330
};

#pragma pack(push, 1)
struct CAIStateGroup_WorkHammerAnvil : CAIStateGroupBaseI {
    bool TookActionPoint;                       // +0x14
    CIntelligentPointer<CThing> PAnvil;             // +0x15 CIntelligentPointer<CThingObject>
    float ActionPointPos_X;                     // +0x1d
    float ActionPointPos_Y;                     // +0x21
    float ActionPointPos_Z;                     // +0x25
    unsigned char ActionPointRHSet[0x1b];       // +0x29 CRightHandedSet
    long ActionPointIndex;                      // +0x44
    long NextFrameHammer;                       // +0x48
    virtual void OnReplace();
};
#pragma pack(pop)

void CAIStateGroup_WorkHammerAnvil::OnReplace()
{
    if (IS_THING_ALIVE(PAnvil) && TookActionPoint) {
        CTCUserActionPointBase* point;
        if (PAnvil.GetPItem()->GetInterface(&point, 0x2D))
            point->ReleaseSlot(ActionPointIndex);
    }
    ((CThingCreatureBase*)PBrain->PCreature)->FinishCurrentAction();
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    DestroyItem("CARRY_SLOT_LEFT_HAND");
    SetPushableByHero(true);
}