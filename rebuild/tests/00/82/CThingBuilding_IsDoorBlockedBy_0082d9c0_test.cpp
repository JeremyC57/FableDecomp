// Behaviour test for 0082d9c0: src/0082d9c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingBuilding::IsDoorBlockedBy(CThing* thing)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingBuilding::IsDoorBlockedBy(CThing* thing) -- true when a live occupant registered with the building's door component is standing, moving or pushing (its state component says so) within 1.5 units of the thing
// Leaf: ?IsDoorBlockedBy@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D9C0, 306 bytes (+14 int3), `ret 4`, bool in al.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined GetInterface lookups: type 0x21 on
//   the building = bit 1 of bitset word +0x24, type 0x26 on the occupant = bit 6 of +0x24), CBaseIntelligentPointer::
//   GetPItem @ 0x00A01B50 (four times per occupant), CThing::GetPosition @ 0x004C73D0.
// Globals: 2.25f 0x0125B2D4 (1.5 squared: the inline GFWorld3DDistanceSmallerThan(a, b, 1.5f); its out-of-line copy is
//   0x006614F0).
// Layout: CThing interface bitset +0x20, map +0x44, dead flag bit 0 of +0x91; the type-0x21 component keeps an STLport
//   list<CIntelligentPointer<CThing> > (header-node pointer) at +0xC; the type-0x26 component's three state bools at
//   +0x10, +0x12, +0x13 (component classes not pinned).
// Shape: out-parameter GetInterface lookups (component kept in esi, then spilled to the dead key word); the occupant is
//   re-fetched through GetPItem for every use; `a || b || c` over the state bools in retail's order; the distance test is
//   the inline smaller-than with the radius squared folded.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline bool GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    return (a - b).GetLengthSquared() < distance * distance;
}
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;   // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};
struct SOccupantNode {                      // list<CIntelligentPointer<CThing> > node
    SOccupantNode* _M_next;
    SOccupantNode* _M_prev;
    CBaseIntelligentPointer _M_data;        // +0x08
};
struct CTCBuildingDoor {                    // interface type 0x21 (class not pinned)
    unsigned char  _pad00[0x0c];
    SOccupantNode* Occupants;               // +0x0c STLport list header node
};
struct CTCOccupantState {                   // interface type 0x26 (class not pinned)
    unsigned char _pad00[0x10];
    bool StateA;                            // +0x10
    bool _pad11;
    bool StateB;                            // +0x12
    bool StateC;                            // +0x13
};
class CThingBuilding : public CThing { public:
    bool IsDoorBlockedBy(CThing* thing);
};

bool CThingBuilding::IsDoorBlockedBy(CThing* thing)
{
    if (HasTC(0x21)) {
        CTCBuildingDoor* const& door = (CTCBuildingDoor*)GetTC(0x21);
        for (SOccupantNode* node = door->Occupants->_M_next; node != door->Occupants; node = node->_M_next) {
            CBaseIntelligentPointer& occupant = node->_M_data;
            CTCOccupantState* state;
            if (occupant.GetPItem() && !(occupant.GetPItem()->m_statusFlags & 1)
                && occupant.GetPItem()->GetInterface(&state, 0x26)) {
                if (state->StateB || state->StateC || state->StateA) {
                    CThing* occupantThing = occupant.GetPItem();
                    if (GFWorld3DDistanceSmallerThan(occupantThing->GetPosition(), thing->GetPosition(), 1.5f))
                        return true;
                }
            }
        }
    }
    return false;
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
struct TThing { CThing t; C3DVector pos; };
const C3DVector& CThing::GetPosition() const { return ((const TThing*)this)->pos; }
static TThing g_th[4];
static void AddTC(CThing* t, CInterfaceMap::SEntry* e, int n) { t->m_interfaces.first = e; t->m_interfaces.last = e + n; for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31); }
int main()
{
    memset(g_th, 0, sizeof g_th);
    static SOccupantNode head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; head._M_prev = &n2; n2._M_prev = &n1; n1._M_prev = &head;
    static CTCBuildingDoor door; memset(&door, 0, sizeof door); door.Occupants = &head;
    static CTCOccupantState st1, st2; memset(&st1, 0, sizeof st1); memset(&st2, 0, sizeof st2);
    CInterfaceMap::SEntry be[1] = { { 0x21, (CTCBase*)&door } };
    CThingBuilding* b = (CThingBuilding*)&g_th[0].t; AddTC(b, be, 1);
    CInterfaceMap::SEntry e1[1] = { { 0x26, (CTCBase*)&st1 } }, e2[1] = { { 0x26, (CTCBase*)&st2 } };
    AddTC(&g_th[1].t, e1, 1); AddTC(&g_th[2].t, e2, 1);
    n1._M_data.PItem = &g_th[1].t; n2._M_data.PItem = &g_th[2].t;
    g_th[1].pos = C3DVector(10, 0, 0); g_th[2].pos = C3DVector(0, 1, 0); g_th[3].pos = C3DVector(0, 0, 1);
    CThing* probe = &g_th[3].t;
    CHECK(!b->IsDoorBlockedBy(probe));                  // nobody in a blocking state
    st2.StateC = true; CHECK(b->IsDoorBlockedBy(probe)); // occupant 2 at distance sqrt(2) < 1.5
    g_th[2].pos = C3DVector(0, 2, 0); CHECK(!b->IsDoorBlockedBy(probe));   // sqrt(5) > 1.5
    st1.StateA = true; g_th[1].pos = C3DVector(0, 0, 2); CHECK(b->IsDoorBlockedBy(probe));   // occupant 1 at distance 1
    g_th[1].t.m_statusFlags = 1; CHECK(!b->IsDoorBlockedBy(probe));          // dead occupant ignored
    CThingBuilding* nb = (CThingBuilding*)&g_th[3].t; CHECK(!nb->IsDoorBlockedBy(probe));   // no door component
    if (fails == 0) printf("0082d9c0_TEST PASS\n"); else printf("0082d9c0_TEST FAIL\n");
    return fails;
}