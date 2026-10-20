// CAIStateGroup_CarryCrateAndFollow::FindPCrate() -- the nearest crate object (object property 0x08000000, a free action point, interface 0x47) to the creature, through the thing search tools
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965F30, length 115 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (const C3DVector*; the manifest's DoSizeof label is propagated),
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000 (landed), CThingSearchTools::GetPBestThing<CFilter_Bind3<
// CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC>, CThingCompare_Nearest,
// CThingObject> @ 0x00965E80 (manifest name; thiscall, three pointer arguments, bool result unused).
// Shape: the filter is the landed message-filter expression chain (0x00939D70 / 0x00919A70) over thing filters:
// `ObjectHasProperty(0x08000000) && HasFreeActionPoint() && HasTC(0x47)` -> CFilter_Bind3 of three pointers at
// frame +0x1c over the temporaries at +4, +3 (the empty filter, a user constructor so no store: the high byte of
// the first slot) and +8; the comparator holds a copy of the creature position at +0x10 (three dword moves);
// the result pointer at +0xc is returned. The search tools are fetched last (the object expression after the
// argument pushes).
// Model: local (engine/CAIStateGroup_CarryCrateAndFollow.h flattens the base).
struct CThingObject;
struct CThingSearchTools;

struct C3DVector {
    float X, Y, Z;
};

struct CThing {
    const C3DVector* GetPosition() const;           // 0x004C73D0
};

struct CThingFilter_ObjectHasProperty {
    unsigned long m_property;
    CThingFilter_ObjectHasProperty(unsigned long property) : m_property(property) { }
};

struct CThingFilter_HasFreeActionPoint {
    CThingFilter_HasFreeActionPoint() { }
};

struct CThingFilter_HasTC {
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};

struct CThingCompare_Nearest {
    float m_x, m_y, m_z;                            // the reference position, three scalar members
    CThingCompare_Nearest(const C3DVector& position) : m_x(position.X), m_y(position.Y), m_z(position.Z) { }
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

typedef CFilter_Bind3<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC> CCrateFilter;

struct CThingSearchTools {
    bool GetPBestThing(const CCrateFilter& filter, const CThingCompare_Nearest& compare, CThingObject** ppBest);   // 0x00965E80
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the search GetPBestThing runs (returns the best thing or 0)
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThing @ 0x00965E80 (compiled in retail's TU): clears the out pointer, stores the best
// thing found, reads the filter and comparator through their const references; no argument escapes.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CCrateFilter& filter, const CThingCompare_Nearest& compare, CThingObject** ppBest)
{
    *ppBest = 0;
    long best = NearSink_s7_0(this, *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)ppBest = best;
    return *ppBest != 0;
}
#pragma auto_inline(on)

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
    CThingSearchTools* PeekThingSearchTools();      // 0x008FD000
};

struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    unsigned char PCrate[8];                        // +0x14 CIntelligentPointer<CThing>
    unsigned char PPreviouslyCarriedItem[8];        // +0x1c CIntelligentPointer<CThing>
    unsigned char PThingToFollow[8];                // +0x24 CIntelligentPointer<CThing>
    bool GotActionPoint;                            // +0x2c
    long ActionPointIndex;                          // +0x30
    long PickUpActionID;                            // +0x34
    CThingObject* FindPCrate();
};

CThingObject* CAIStateGroup_CarryCrateAndFollow::FindPCrate()
{
    CThingObject* crate;
    PeekThingSearchTools()->GetPBestThing(
        CThingFilter_ObjectHasProperty(0x08000000) && CThingFilter_HasFreeActionPoint() && CThingFilter_HasTC(0x47),
        CThingCompare_Nearest(*PBrain->Creature->GetPosition()), &crate);
    return crate;
}