// CAIStateGroup_MinionWatchFires::IsRunnable() -- runnable when the idle scheduler (if any) prefers this group and a burning building lies within the creature's sight range.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00936210, length 448 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xd3 idle scheduler = bit 19 of +0x38, 0x34
// perceive-thing = bit 20 of +0x24), CTCIdleScheduler::IsPreferredIdle @ 0x00801BE0, CTCPerceiveThing::GetSightRadius
// @ 0x007448F0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
// (CIntelligentPointer<CThingBuilding> vtable 0x01244344), CThing::GetPos @ 0x004C73D0 (twice),
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestLeafThing<CFilter_Bind2<filter,
// CThingFilter_In2DRange>, CThingCompare_Nearest, CThingBuilding> @ 0x00936070 (same TU; the manifest's In2DRange /
// IsValidAttackTargetForCreature label is propagated -- the first filter is the empty building-on-fire test 0x006A8A80).
// Layout: engine/CAIStateGroup_MinionWatchFires.h (PBuildingOnFire +0x14, CIntelligentPointer<CThingBuilding>) over the
// landed local CAIStateGroupBase model (the generated header flattens the vptr).
// Shape: the search template gets a same-TU noinline stand-in that reads its arguments without letting their addresses
// escape (VC then schedules the comparator's position copy around the next PBrain load, as retail); the sight radius
// is a named float (spilled; its slot is reused by the Bind2 temporary); the empty filter has a user ctor (no byte store).
// IS_THING_ALIVE names the item before the dead-bit test: the plain `if (GetPItem() && !(GetPItem()->flags & 1))` form
// turns the final `return true` into VC's `mov al,1 ; test al,cl` select instead of retail's test/jne + two epilogues.
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
class C3DVector { public: float x, y, z; };
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
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
};
class CThingBuilding;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {   // CIntelligentPointer<CThingBuilding> (vtable 0x01244344)
    virtual void v00();
    CIntelligentPointerBuilding(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        CThing* item = p.GetPItem();
        if (!(item->m_flags91 & 1))
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase;
class CTCIdleScheduler { public:
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};
class CTCPerceiveThing { public:
    float GetSightRadius();                     // 0x007448F0
};
struct CThingFilter_OnFire { CThingFilter_OnFire() { } };
class CThingFilter_In2DRange { public:
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingSearchTools { public:
    bool GetPBestLeafThing(const CFilter_Bind2<CThingFilter_OnFire, CThingFilter_In2DRange>& filter,
        const CThingCompare_Nearest& compare, CThingBuilding** out);   // 0x00936070
};
long NearSink_936070(const void* self, float a, float b);
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CFilter_Bind2<CThingFilter_OnFire, CThingFilter_In2DRange>& filter,
    const CThingCompare_Nearest& compare, CThingBuilding** out)
{
    long best = NearSink_936070(this, filter.m_pSecond->Range, compare.m_pos.x);
    *out = (CThingBuilding*)best;
    return best != 0;
}
#pragma auto_inline(on)
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();
    virtual void OnReplace();
    virtual bool IsRunnable();
    virtual void Update();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_MinionWatchFires : CAIStateGroupBase {
    CIntelligentPointerBuilding PBuildingOnFire;   // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionWatchFires::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    CTCPerceiveThing* perceive;
    if (PBrain->PCreature->GetInterface(&perceive, 0x34)) {
        CThingBuilding* building = 0;
        PBuildingOnFire = CIntelligentPointerBuilding(0);
        float radius = perceive->GetSightRadius();
        CThingCompare_Nearest nearest(PBrain->PCreature->GetPos());
        CThingFilter_In2DRange inRange(PBrain->PCreature->GetPos(), radius);
        if (PeekThingSearchTools()->GetPBestLeafThing(CThingFilter_OnFire() && inRange, nearest, &building)) {
            PBuildingOnFire = CIntelligentPointerBuilding(building);
            if (IS_THING_ALIVE(PBuildingOnFire))
                return true;
        }
    }
    return false;
}