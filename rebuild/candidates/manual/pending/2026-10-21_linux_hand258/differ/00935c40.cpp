// CAIStateGroup_MinionPutOutFires::GoToWell() -- find the nearest well (thing-type flag 0x800); while the well and the burning building both live and the building is still on fire, run to within 4 units of the well, then fill a bucket there; true when the bucket was filled
// Grade: DIFFER(494v502) -- frame slot layout (filter/well/present byte), mov eax,[eax+0x44]/mov esi,eax vs mov esi,[eax+0x44], missing loop-align pad before the inlined lower_bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935C40, length 502 bytes (+ int3 padding), `and esp,-8` frame for the
// 8-aligned action temporary, bool in al.
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestLeafThing<filter, CThingCompare_Nearest, CThingObject> @ 0x00936100 (this TU),
// CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThingObject> vtable 0x0125A858; CIntelligentPointer<const CThing> vtable 0x01237F1C, by value),
// CTCFlammable::IsOnFire @ 0x00780400 (interface 0x6d, bit 13 of +0x2c, inline lower_bound on the component map),
// AIAction_MoveToThingProximity @ 0x0097E380, CCreatureAction_FillBucket ctor @ 0x00843840 (vtable 0x01274E2C; its
// intelligent pointer at +0xa8 is destroyed inline), ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformAction @ 0x0097D8B0.
// Layout: Ego_r.pdb CAIStateGroup_MinionPutOutFires (PBuildingOnFire +0x14); retail CThing interface bitset +0x20 /
// map +0x44.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const { return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        bool present = HasTC(interfaceType);
        if (present) {
            CInterfaceMap::SEntry* first = m_interfaces.first;
            CInterfaceMap::SEntry* last = m_interfaces.last;
            int count = last - first;
            while (count > 0) {
                int half = count / 2;
                CInterfaceMap::SEntry* middle = first + half;
                if (middle->type < interfaceType) {
                    first = middle + 1;
                    count = count - half - 1;
                } else
                    count = half;
            }
            *ppOut = (T*)((first == last || interfaceType < first->type) ? last : first)->component;
        }
        return present;
    }
};
struct CThingObject : CThing { };
struct CThingCreatureBase : CThing { };
struct CThingBuilding : CThingObject { };

struct CTCFlammable {                           // interface 0x6d
    bool IsOnFire() const;                      // 0x00780400
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }

struct CThingFilter_HasThingTypeFlags {         // the well filter (0x800)
    unsigned long Flags;
    CThingFilter_HasThingTypeFlags(unsigned long flags) : Flags(flags) { }
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};
struct CThingSearchTools {
    bool GetPBestLeafThing(const CThingFilter_HasThingTypeFlags& filter, const CThingCompare_Nearest& compare,
                           CThingObject** best);   // 0x00936100
};

struct CCreatureActionBase {
    void* __vftable;
    long _pad04;
    __int64 _align08;                           // 8-aligned (the `and esp,-8` frame)
    unsigned char _pad10[0xa8 - 0x10];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_FillBucket : CCreatureActionBase {
    CIntelligentPointer<CThing> PThing;         // +0xa8
    CCreatureAction_FillBucket(CThingCreatureBase* creature);   // 0x00843840
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionPutOutFires : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;   // +0x14
    bool GoToWell();
};

bool CAIStateGroup_MinionPutOutFires::GoToWell()
{
    CThingObject* well = 0;
    if (!PeekThingSearchTools()->GetPBestLeafThing(CThingFilter_HasThingTypeFlags(0x800),
                                                   CThingCompare_Nearest(GetCreature()->GetPosition()), &well))
        return false;
    CIntelligentPointer<CThingObject> pWell(well);
    bool arrived = false;
    for (;;) {
        if (!IsThingAlive(pWell))
            break;
        if (!IsThingAlive(PBuildingOnFire))
            break;
        CTCFlammable* flammable;
        if (!PBuildingOnFire.GetPItem()->GetInterface(&flammable, 0x6d))
            break;
        if (!flammable->IsOnFire())
            break;
        long result = AIAction_MoveToThingProximity(2, pWell, 4.0f, &arrived, 5, true, false, true, true, 0.0f);
        if (result == 4)
            continue;
        if (result != 2)
            break;
        if (AIAction_PerformAction(&CCreatureAction_FillBucket(GetCreature())) == 2)
            return true;
        break;
    }
    return false;
}
