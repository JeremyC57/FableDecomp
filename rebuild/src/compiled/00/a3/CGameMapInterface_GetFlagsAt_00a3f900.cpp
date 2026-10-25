// CGameMapInterface::GetFlagsAt(float, float) -- with no map attached return 0; otherwise truncate the position to whole map coordinates and return the map's slot-0 value there
// Leaf: ?GetFlagsAt@CGameMapInterface@@UAEJMM@Z
// Retail: Fable.exe @ 0x00A3F900, 67 bytes, `ret 8`. Slot 1 of the CGameMapInterface vtable 0x0129D288 (RTTI name);
// slot 0 is the landed twin CGameMapInterface::GetHeightAt 0x00A3F8B0 (float result from the map's slot 1). No PDB name
// covers the class: GetFlagsAt follows the landed twin's naming of the map's slot 0 (descriptive).
// Label correction: the manifest row "CWorldMap::GetNextRegionOnRouteTo" is a propagation (its only reference is the
// vtable slot 0x0129D28C).
// Callees: __ftol2 0x00BFEA70 (twice), the attached map's virtual slot 0 (+0x00) taking the coordinate pair by pointer.
// Shape: as the landed twin: the coordinates are converted in source order (x first) into a frame aggregate filled
// member by member, the miss path returns 0 with its own epilogue.
struct C2DCoordsI {
    long X;                                       // +0x00
    long Y;                                       // +0x04
};

struct CGameMap_00a3f8b0 {
    virtual long  GetFlagsAt(const C2DCoordsI* pos);    // slot 0 (+0x00)
    virtual float GetHeightAt(const C2DCoordsI* pos);   // slot 1 (+0x04)
};

struct CGameMapInterface {
    virtual float GetHeightAt(float x, float y);        // slot 0 0x00A3F8B0
    virtual long  GetFlagsAt(float x, float y);         // slot 1
    CGameMap_00a3f8b0* m_pMap;                          // +0x04
};

long CGameMapInterface::GetFlagsAt(float x, float y)
{
    if (!m_pMap)
        return 0;
    C2DCoordsI pos;
    pos.X = (long)x;
    pos.Y = (long)y;
    return m_pMap->GetFlagsAt(&pos);
}