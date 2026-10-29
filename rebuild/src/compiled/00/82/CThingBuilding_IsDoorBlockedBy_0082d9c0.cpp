// CThingBuilding::IsDoorBlockedBy(CThing* thing) -- true when a live occupant registered with the building's door component is standing, moving or pushing (its state component says so) within 1.5 units of the thing
// Leaf: ?IsDoorBlockedBy@CThingBuilding
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
    const C3DVector& GetPos() const;   // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
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
    if (HasTC(0x21)) {
        CTCBuildingDoor* const& door = (CTCBuildingDoor*)GetTC(0x21);
        for (SOccupantNode* node = door->Occupants->_M_next; node != door->Occupants; node = node->_M_next) {
            CIntelligentPointer<CThing>& occupant = node->_M_data;
            CTCOccupantState* state;
            if (occupant.GetPItem() && !(occupant.GetPItem()->m_statusFlags & 1)
                && occupant.GetPItem()->GetInterface(&state, 0x26)) {
                if (state->StateB || state->StateC || state->StateA) {
                    CThing* occupantThing = occupant.GetPItem();
                    if (GFWorld3DDistanceSmallerThan(occupantThing->GetPos(), thing->GetPos(), 1.5f))
                        return true;
                }
            }
        }
    }
    return false;
}