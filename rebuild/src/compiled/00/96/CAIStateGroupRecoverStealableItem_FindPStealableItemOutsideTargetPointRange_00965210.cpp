// CAIStateGroup_RecoverStealableItem::FindPStealableItemOutsideTargetPointRange(const C3DVector&, float) -- the nearest
// Leaf: FindPStealableItemOutsideTargetPointRange
// stealable object (property 0x8000000, a free action point, interface 0x47) that lies OUTSIDE `range` of the target point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965210, length 154 bytes (+ int3 padding), `ret 8`.
// Callees: the creature position getter @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThing<CFilter_Bind4<ObjectHasProperty, HasFreeActionPoint, HasTC, unary_negate<In3DRange>>,
// CThingCompare_Nearest, CThingObject> @ 0x009650C0 (filter, compare, out).
// Shape: four filter objects + the Bind4 pointer block in the 0x38 frame, the nearest-compare holding a copy of the
// creature position; the out object lands in the dead first-argument slot. Engine header names (base modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
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

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CThingCreaturePos : public CThing {
};
class CThingObject;

class CThingFilter_ObjectHasProperty { public:
    unsigned long Property;
    CThingFilter_ObjectHasProperty(unsigned long property) : Property(property) { }
};
class CThingFilter_HasFreeActionPoint { public: CThingFilter_HasFreeActionPoint() { } };
class CThingFilter_HasTC { public:
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
class CThingFilter_In3DRange { public:
    C3DVector Point;
    float Range;
    CThingFilter_In3DRange(const C3DVector& point, float range) : Point(point), Range(range) { }
};
struct CThingFilter_NotIn3DRange {              // std::unary_negate<CThingFilter_In3DRange>
    CThingFilter_In3DRange Pred;
    CThingFilter_NotIn3DRange(const CThingFilter_In3DRange& pred) : Pred(pred) { }
};
struct CFilter_Bind4 {
    const CThingFilter_ObjectHasProperty* P1;
    const CThingFilter_HasFreeActionPoint* P2;
    const CThingFilter_HasTC* P3;
    const CThingFilter_NotIn3DRange* P4;
    CFilter_Bind4(const CThingFilter_ObjectHasProperty& a, const CThingFilter_HasFreeActionPoint& b,
                  const CThingFilter_HasTC& c, const CThingFilter_NotIn3DRange& d) : P1(&a), P2(&b), P3(&c), P4(&d) { }
};
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools { public:
    bool GetPBestThing(const CFilter_Bind4& filter, const CThingCompare_Nearest& compare, CThingObject** out);   // 0x009650C0
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the search GetPBestThing runs (returns the best thing or 0)
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThing @ 0x009650C0 (compiled in retail's TU): clears the out pointer, stores the best
// thing found, reads the filter and comparator through their const references; no argument escapes.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CFilter_Bind4& filter, const CThingCompare_Nearest& compare, CThingObject** out)
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)

struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000 (CAIStateGroupBase)
    CThingObject* FindPStealableItemOutsideTargetPointRange(const C3DVector& targetPoint, float range);
};

CThingObject* CAIStateGroup_RecoverStealableItem::FindPStealableItemOutsideTargetPointRange(const C3DVector& targetPoint,
                                                                                         float range)
{
    CThingObject* item;
    PeekThingSearchTools()->GetPBestThing(
        CFilter_Bind4(CThingFilter_ObjectHasProperty(0x8000000), CThingFilter_HasFreeActionPoint(), CThingFilter_HasTC(0x47),
                      CThingFilter_NotIn3DRange(CThingFilter_In3DRange(targetPoint, range))),
        CThingCompare_Nearest(((CThingCreaturePos*)PBrain->PCreature)->GetPos()), &item);
    return item;
}