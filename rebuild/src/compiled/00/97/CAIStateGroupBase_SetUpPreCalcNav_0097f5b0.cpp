// CAIStateGroupBase::SetUpPreCalcNav(const CThing* routeThing) -- when the creature navigates (interface 7) and the route thing's precalculated route (0xA6) has a route, set the navigation up from the creature's position along it and return the route's destination; else the zero vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F5B0, length 219 bytes (`ret 8`, C3DVector by hidden pointer).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 7) and the const copy @ 0x004365B0
// (type 0xA6 on the const route thing), CTCPreCalculatedNavigationRoute::HasRoute @ 0x00800500 and ::GetRouteDestination
// @ 0x008004D0 (twice), CThing::GetPosition @ 0x004C73D0, the navigation component's route setup @ 0x006CB9C0 (three
// arguments; manifest "CScriptThing::operator==" is propagated).
// Shape: the route component is deliberately unset when the interface is absent (retail reads a dead slot), the
// navigation component is kept in ebx; the creature is re-read into a named local before the setup call (retail loads
// it ahead of the argument calls, reusing the `this` register).
class C3DVector
{ public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CTCBase;
struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    const C3DVector& GetPosition() const;           // 0x004C73D0

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};

class CTCPreCalculatedNavigationRoute              // interface type 0xA6
{ public:
    unsigned char _pad00[0x20];
    C3DVector RouteStart;                           // +0x20
    bool HasRoute();                                // 0x00800500
    C3DVector GetRouteDestination();                // 0x008004D0
};

struct CTCNavigation                                // interface type 7
{
    void SetUpRoute(const C3DVector& from, const C3DVector& start, const C3DVector& destination);   // 0x006CB9C0
};

class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

class CAIStateGroupBase
{ public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    C3DVector SetUpPreCalcNav(const CThing* routeThing);
};

C3DVector CAIStateGroupBase::SetUpPreCalcNav(const CThing* routeThing)
{
    CThing* creature = PBrain->Creature;
    if (creature->HasTC(7))
    {
        CTCNavigation* nav = (CTCNavigation*)creature->GetTC(7);
        CTCPreCalculatedNavigationRoute* route;
        if (routeThing->HasTC(0xa6))
            route = (CTCPreCalculatedNavigationRoute*)routeThing->GetTC(0xa6);
        if (route->HasRoute())
        {
            CThing* mover = PBrain->Creature;
            nav->SetUpRoute(mover->GetPosition(), route->RouteStart, route->GetRouteDestination());
            return route->GetRouteDestination();
        }
    }
    return C3DVector(0.0f, 0.0f, 0.0f);
}