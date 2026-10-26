// Behaviour test for 0075d9a0: src/0075d9a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCCameraPointBuilding::vfunc_17(CCharString& error) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCameraPointBuilding::vfunc_17(CCharString& error) const -- validity check: the camera point must hang off a live building through its owner link, may not be both a start and an exit point, and its inside/outside position must agree with the start/exit flags; writes the reason into `error` and returns false on the first failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D9A0, 254 bytes (+ int3 padding), `ret 4`. Vtable 0x0123ECBC
//   (CTCCameraPointBuilding) slot 17, the slot the landed CTCCameraPointScripted::vfunc_17 0x0075CDF0 fills.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for type 0x33 =
//   word 1 bit 19), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0,
//   CThingBuilding::IsPointInside(const C3DVector&) @ 0x0082D000, CCharString::operator=(const char*) @ 0x0099EFE0.
// Strings: 0x01269FF0 "CAMERA_POINT_BUILDING is not attached to a building!", 0x0126A028 "CAMERA_POINT_BUILDING is both
//   start and end point!", 0x01269F30 "CAMERA_POINT_BUILDING is attached to something other than a building!",
//   0x01269FB8 "point is inside building but flagged as an exit point!", 0x01269F78 "point is not inside building but
//   is flagged as a start point!".
// Layout = Ego_r.pdb CTCCameraPointBuilding (0x44): InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42; the
//   interface-0x33 component's owner link at the odd +0x0d (landed GetOwnerBuilding 0x0075D940); CThing ThingTypeFlags
//   +0x6c (0x40 = building), Dead bit 0 of +0x91.
// Shape: early-out ifs that each set `error` and `goto fail;` to ONE shared `fail: return false;` after `return true;`
//   (the shared exit keeps esi/edi pushed at entry; VC7.1 still duplicates the small epilogue into every failure); the
//   component is fetched with the inline out-param GetInterface before the start/exit test; IsPointInside's result is a
//   named bool tested in two ifs.
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const char* s);      // 0x0099EFE0
};
class C3DVector { public: float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c (0x40 = building)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CThingBuilding : public CThing { public:
    bool IsPointInside(const C3DVector& point) const;   // 0x0082D000
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // the interface-0x33 component (owner link at +0x0d)
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};
#pragma pack(pop)
struct CTCCameraPointBuilding {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];          // CTCCameraPointDefinitionBase members
    bool InMyBuilding;                          // +0x40
    bool ExitPoint;                             // +0x41
    bool StartPoint;                            // +0x42
    bool vfunc_17(CCharString& error) const;
};

bool CTCCameraPointBuilding::vfunc_17(CCharString& error) const
{
    CTCOwnedEntity* owned;
    if (!ParentThing->GetInterface(&owned, 0x33)) {
        error = "CAMERA_POINT_BUILDING is not attached to a building!";
        goto fail;
    }
    if (StartPoint && ExitPoint) {
        error = "CAMERA_POINT_BUILDING is both start and end point!";
        goto fail;
    }
    CThing* owner = owned->POwner.GetPItem();
    if (!owner || owner->IsDead() || !(owner->ThingTypeFlags & 0x40)) {
        error = "CAMERA_POINT_BUILDING is attached to something other than a building!";
        goto fail;
    }
    bool inside = ((CThingBuilding*)owner)->IsPointInside(ParentThing->GetPosition());
    if (ExitPoint && inside) {
        error = "point is inside building but flagged as an exit point!";
        goto fail;
    }
    if (StartPoint && !inside) {
        error = "point is not inside building but is flagged as a start point!";
        goto fail;
    }
    return true;
fail:
    return false;
}

// ---- stubs and driver ----
static const char* g_lastError = 0; static int g_assigns = 0;
CCharString& CCharString::operator=(const char* s) { g_lastError = s; ++g_assigns; return *this; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
static C3DVector g_pos = { 1, 2, 3 }; static const CThing* g_posThing = 0;
const C3DVector& CThing::GetPosition() const { g_posThing = this; return g_pos; }
static bool g_inside = false; static const C3DVector* g_insidePoint = 0;
bool CThingBuilding::IsPointInside(const C3DVector& p) const { g_insidePoint = &p; return g_inside; }
void CTCCameraPointBuilding::v00() { }
static bool Has(const char* s, const char* sub) { return s && strstr(s, sub) != 0; }
int main()
{
    static CThing parent; memset(&parent, 0, sizeof parent);
    static CThingBuilding building; memset(&building, 0, sizeof building);
    static unsigned char ownedMem[0x20]; memset(ownedMem, 0, sizeof ownedMem);
    CTCOwnedEntity* owned = (CTCOwnedEntity*)ownedMem;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x33, (CTCBase*)owned } };
    parent.m_interfaces.first = entries; parent.m_interfaces.last = entries + 2;
    static CTCCameraPointBuilding cp; cp.ParentThing = &parent; cp.InMyBuilding = false; cp.ExitPoint = false; cp.StartPoint = false;
    CCharString err; err.PStringData = 0;
    // 1: no interface 0x33 bit -> not attached
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "not attached"));
    parent.m_presentBits[1] = 1UL << 19;
    // 2: both start and exit
    cp.StartPoint = true; cp.ExitPoint = true;
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "both start and end"));
    cp.StartPoint = false; cp.ExitPoint = false;
    // 3: owner missing / dead / not a building
    unsigned char* ownerLink = ownedMem + 0x0d;
    void* item = 0; memcpy(ownerLink + 4, &item, 4);
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "something other than a building"));
    item = &building; memcpy(ownerLink + 4, &item, 4);
    building.ThingTypeFlags = 0;
    g_lastError = 0;
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "something other than a building"));
    building.ThingTypeFlags = 0x40; building.Flags91 = 1;
    g_lastError = 0;
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "something other than a building"));
    building.Flags91 = 0;
    // 4: valid, neither flag -> true, no error written; the parent's position is tested
    g_assigns = 0;
    CHECK(cp.vfunc_17(err) && g_assigns == 0 && g_posThing == &parent && g_insidePoint == &g_pos);
    // 5: exit point inside -> error; exit point outside -> ok
    cp.ExitPoint = true; g_inside = true;
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "inside building but flagged as an exit"));
    g_inside = false; g_assigns = 0;
    CHECK(cp.vfunc_17(err) && g_assigns == 0);
    cp.ExitPoint = false;
    // 6: start point outside -> error; inside -> ok
    cp.StartPoint = true; g_inside = false;
    CHECK(!cp.vfunc_17(err) && Has(g_lastError, "not inside building but is flagged as a start"));
    g_inside = true; g_assigns = 0;
    CHECK(cp.vfunc_17(err) && g_assigns == 0);
    // 7: interface bit set but the map has no 0x33 entry -> the map end's component (here the map end is entries+1)
    if (fails == 0) printf("0075d9a0_TEST PASS\n");
    return fails;
}
