// Grade: DIFFER(250v250) retry1 -- retail_asm helpers + two-interface slot-13 model; residue: frame slot packing only
// CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const -- convert the requested box to whole navigation cells and ask the navigation manager to fill the area update for it
// Retail-asm: 0x006B2973 `fld dword ptr [ebp+8]` + 0x006B2976 `fistp dword ptr [ebp-1Ch]` (round) and 0x006B299C `fsub [ebp-8]` + 0x006B299F `fistp dword ptr [ebp-4]` (floor) -- the engine inline-assembly FloatToLong / FloorToLong helpers.
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
struct CNavRegionSource {                               // the manager seen through the interface whose slot 13 takes no arguments
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CNavRegion* GetNavRegion();                 // slot 13 (+0x34), no arguments
};
struct CNavManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CNavMap* Query(void* region, const C2DBoxI& box, const CCharString& name, CNavigationAreaUpdate& update);   // slot 13
};
// the engine's inline-assembly float-to-long helper (round to nearest) and floor helper (landed 0x006A68F0 FloorToLong)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

struct CGameNavigatorManagerExternalInterface {
    void* _vptr;
    CNavManager* Manager;                               // +0x04
    void GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const;
};

void CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
{
    long y1 = FloatToLong(box.Y1);
    long x1 = FloatToLong(box.X1);
    long y0 = FloorToLong(box.Y0);
    long x0 = FloorToLong(box.X0);
    C2DBoxI cells = { x0, y0, x1, y1 };
    CNavRegion* source = ((CNavRegionSource*)Manager)->GetNavRegion();
    C2DCoordI corner = { cells.X0, cells.Y0 };
    void* region = source->GetRegionAt(corner);
    CCharString name("NULL", -1);
    Manager->Query(region, cells, name, update)->InitialiseHeightmap();
}
