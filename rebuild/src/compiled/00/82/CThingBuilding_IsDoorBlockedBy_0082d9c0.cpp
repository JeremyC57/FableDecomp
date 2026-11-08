// CThingBuilding::IsDoorBlockedBy(CThing* thing) -- true when a live occupant registered with the building's door component is standing, moving or pushing (its state component says so) within 1.5 units of the thing
// Leaf: ?IsDoorBlockedBy@CThingBuilding@@QAE_NPAVCThing@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D9C0, 306 bytes (+14 int3), `ret 4`, bool in al.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined GetInterface lookups: type 0x21 on
//   the building = bit 1 of bitset word +0x24, type 0x26 on the occupant = bit 6 of +0x24), CBaseIntelligentPointer::
//   GetPItem @ 0x00A01B50 (four times per occupant), CThing::GetPos @ 0x004C73D0.
// Globals: 2.25f 0x0125B2D4 (1.5 squared: the inline GFWorld3DDistanceSmallerThan(a, b, 1.5f); its out-of-line copy is
//   0x006614F0).
// Layout: CThing interface bitset +0x20, map +0x44, dead flag bit 0 of +0x91; the type-0x21 component keeps an STLport
//   list<CIntelligentPointer<CThing> > (header-node pointer) at +0xC; the type-0x26 component's three state bools at
//   +0x10, +0x12, +0x13 (component classes not pinned).
// Shape: out-parameter GetInterface lookups (component kept in esi, then spilled to the dead key word); the occupant is
//   re-fetched through GetPItem for every use; `a || b || c` over the state bools in retail's order; the distance test is
//   the inline smaller-than with the radius squared folded.
#include "fable_compiler.h"
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    FABLE_FORCEINLINE float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
};
FABLE_FORCEINLINE C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
FABLE_FORCEINLINE bool GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    return (a - b).GetLengthSquared() < distance * distance;
}
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct SOccupantNode {                      // list<CIntelligentPointer<CThing> > node
    SOccupantNode* _M_next;
    SOccupantNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;        // +0x08
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
    if (HasTC(TCI_THING_OWNER)) {
        CTCBuildingDoor* const& door = (CTCBuildingDoor*)(&GetTC(TCI_THING_OWNER));
        for (SOccupantNode* node = door->Occupants->_M_next; node != door->Occupants; node = node->_M_next) {
            CIntelligentPointer<CThing>& occupant = node->_M_data;
            CTCOccupantState* state;
            if (occupant.Get() && !occupant.Get()->IsDead()
                && occupant.Get()->GetTC(TCI_DOOR, &state)) {
                if (state->StateB || state->StateC || state->StateA) {
                    CThing* occupantThing = occupant.Get();
                    if (GFWorld3DDistanceSmallerThan(occupantThing->GetPos(), thing->GetPos(), 1.5f))
                        return true;
                }
            }
        }
    }
    return false;
}