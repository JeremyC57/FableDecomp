// Behaviour test for 00965f30: src/00965f30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_CarryCrateAndFollow::FindPCrate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CThingObject;
class CThingSearchTools;

class C3DVector { public:
    float X, Y, Z;
};

class CThing { public:
    const C3DVector* GetPosition() const;           // 0x004C73D0
};

class CThingFilter_ObjectHasProperty { public:
    unsigned long m_property;
    CThingFilter_ObjectHasProperty(unsigned long property) : m_property(property) { }
};

class CThingFilter_HasFreeActionPoint { public:
    CThingFilter_HasFreeActionPoint() { }
};

class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};

class CThingCompare_Nearest { public:
    float m_x, m_y, m_z;                            // the reference position, three scalar members
    CThingCompare_Nearest(const C3DVector& position) : m_x(position.X), m_y(position.Y), m_z(position.Z) { }
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

typedef CFilter_Bind3<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC> CCrateFilter;

class CThingSearchTools { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

class CAIStateGroupBase { public:
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

// ---- stubs and driver ----
static long g_best = 0, g_seen = 0; static int g_search = 0, g_peek = 0; static const void* g_self = 0;
static CThingSearchTools g_tools; static C3DVector g_pos = { 0.0f, 2.0f, 3.0f };
long NearSink_s7_0(const void* self, long seen) { ++g_search; g_self = self; g_seen = seen; return g_best; }
const C3DVector* CThing::GetPosition() const { return &g_pos; }
CAIStateGroupBase::~CAIStateGroupBase() {}
CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools() { ++g_peek; return &g_tools; }
int main()
{
    static CThing creature; static CAIBrain brain; brain.Creature = &creature;
    static unsigned char raw[sizeof(CAIStateGroup_CarryCrateAndFollow)]; CAIStateGroup_CarryCrateAndFollow& g = *(CAIStateGroup_CarryCrateAndFollow*)raw; g.PBrain = &brain;
    g_best = 0x1234; CHECK(g.FindPCrate() == (CThingObject*)0x1234); CHECK(g_search == 1 && g_peek == 1 && g_self == &g_tools);
    // the filter's first word is the address of the property filter (X = 0.0f adds nothing): it holds 0x08000000
    CHECK(((const CThingFilter_ObjectHasProperty*)g_seen)->m_property == 0x08000000);
    g_best = 0; CHECK(g.FindPCrate() == 0 && g_search == 2);
    g_pos.X = 1.0f; g_best = 0x40; CHECK(g.FindPCrate() == (CThingObject*)0x40);
    CHECK(((const CThingFilter_ObjectHasProperty*)(g_seen - 0x3f800000))->m_property == 0x08000000);   // X bits added by the compare
    if (fails == 0) printf("00965f30_TEST PASS\n"); else printf("00965f30_TEST FAIL\n");
    return fails;
}