// Grade: DIFFER(231v250) -- the four box edges are rounded with bare dword `fistp` (inline-asm FastFloor/RoundToInt helpers in the original, the asm_bake family) and the body calls interface slot 13 once with no arguments and once with four (the frame pointer absorbs the imbalance); lane gap
// CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const -- convert the requested box to whole navigation cells and ask the navigation manager to fill the area update for it
// Leaf: GetNavigationInformation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B2960, length 250 bytes, `ret 8`, frame pointer.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("NULL" @ 0x012393E4) / ~CCharString @ 0x0099EAE0, the manager's
//   (+4) virtual slot 13 (+0x34, twice) and the slot-13 result's slot 17 (+0x44); 0x00509A20 on the final result.
// Shape (retail): maxY/maxX rounded straight with `fld ; fistp`; minY/minX with `fistp(v - 0.5)` then `if (v == i + 1.0f) ++i`
//   (a floor), the four ints stored as a C2DBoxI at [ebp-30h]; the cell's top-left copied into a C2DCoordI for slot 17; the
//   "NULL" name built in the dead `box` slot.
struct C2DBoxF { float X0, Y0, X1, Y1; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct C2DCoordI { long X, Y; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CNavigationAreaUpdate;
struct CNavMap {
    void InitialiseHeightmap();                         // 0x00509A20
};
struct CNavRegion {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void* GetRegionAt(const C2DCoordI& pos);    // slot 17 (+0x44)
};
struct CNavManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CNavMap* Query(void* region, const C2DBoxI& box, const CCharString& name, CNavigationAreaUpdate& update);   // slot 13
};
static inline long RoundToInt(float v) { return (long)v; }
static inline long FloorToInt(float v) { long i = (long)(v - 0.5f); if (v == (float)i + 1.0f) ++i; return i; }

struct CGameNavigatorManagerExternalInterface {
    void* _vptr;
    CNavManager* Manager;                               // +0x04
    void GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const;
};

void CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
{
    long y1 = RoundToInt(box.Y1);
    long x1 = RoundToInt(box.X1);
    long y0 = FloorToInt(box.Y0);
    long x0 = FloorToInt(box.X0);
    C2DBoxI cells = { x0, y0, x1, y1 };
    C2DCoordI corner = { cells.X0, cells.Y0 };
    void* region = ((CNavRegion*)Manager)->GetRegionAt(corner);
    CCharString name("NULL", -1);
    Manager->Query(region, cells, name, update)->InitialiseHeightmap();
}
