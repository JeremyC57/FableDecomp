// CGameMapInterface::GetHeightAt(float x, float y) -- with no world map attached return 0.0f; otherwise truncate the position to whole map coordinates and return the map's height there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3F8B0, length 72 bytes (80 with padding). Callees: __ftol2 @ 0x00BFEA70 (twice),
// the attached map's virtual slot 1 (+0x04) taking the integer coordinate pair by pointer and returning a float.
// Globals: 0.0f @ 0x0122DEDC (the `fld` of the pool zero, never `fldz`).
// The chunk label "CTCGraphicAppearance::GetAnims" is propagated and wrong: this is slot 0 of the CGameMapInterface vtable
// 0x0129D288 (slot 1 @ 0x00A3F900 is the same shape returning an int from the map's slot 0). GetHeightAt is descriptive.
// Leaf: GetHeightAt
// Layout: vptr +0x00, the attached map +0x04.
// Shape: the two coordinates are converted in source order (x first) into an aggregate on the stack (`mov [esp+4],eax`,
// `mov [esp+8],eax`); a two-argument constructor would convert y first (VC evaluates call arguments right to left).
struct C2DCoordsI {
    long X;                                       // +0x00
    long Y;                                       // +0x04
};

struct CGameMap_00a3f8b0 {
    virtual long  GetFlagsAt(const C2DCoordsI* pos);    // slot 0 (+0x00)
    virtual float GetHeightAt(const C2DCoordsI* pos);   // slot 1 (+0x04)
};

struct CGameMapInterface {
    virtual float GetHeightAt(float x, float y);        // slot 0
    CGameMap_00a3f8b0* m_pMap;                          // +0x04
};

float CGameMapInterface::GetHeightAt(float x, float y)
{
    if (!m_pMap)
        return 0.0f;
    C2DCoordsI pos;
    pos.X = (long)x;
    pos.Y = (long)y;
    return m_pMap->GetHeightAt(&pos);
}
