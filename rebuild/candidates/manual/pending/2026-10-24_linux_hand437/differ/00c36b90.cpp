// CGridCursor::GetCell(CVector3* fraction) const -- splits the tracked position into its grid cell origin (returned by value) and the fractional position inside the cell (in cell units)
// Grade: DIFFER(223v219) -- x87 operand order: retail keeps 1/size in st0 after the divide (`fst [esp+8] ; fmul y`) and multiplies it by each memory coordinate (`fld inv ; fmul v`); VC7.1 here pops it (`fstp`) and loads the coordinate first (`fld v ; fmul inv`) whichever operand order the source writes. The y-first call order (raw floor(y) spilled), the 0x24 frame and every store match
// Leaf: GetCell@CGridCursor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36B90, 219 bytes, __thiscall `ret 8` (hidden result pointer + fraction), eax =
//   the result. Descriptive name (no PDB covers this library): the catalog's `Grid_ComputeLocalCoordinates` is kept as
//   the meaning. Caller 0x00C371FA (zero-initialised fraction local, result local).
// Callee: MSVCR71 floor through the IAT (0x01440100), on the double-widened products.
// Shape: the position {x,y,z} is copied out of the chain this +0 -> +0 -> +4 -> +0x38 -> +8 and the cell size (+4) into
//   a frame float; the cell is a named local copied to the hidden result (VC7.1 has no NRVO).
extern "C" __declspec(dllimport) double __cdecl floor(double x);

struct CVector3 {
    float m_x, m_y, m_z;
};

struct CTracked {
    unsigned char _pad00[8];
    CVector3 m_position;                    // +0x08
};

struct CTrackedLink {
    unsigned char _pad00[0x38];
    CTracked* m_pTracked;                   // +0x38
};

struct CTrackedOwner {
    void* m_field00;                        // +0x00
    CTrackedLink* m_pLink;                  // +0x04
};

struct CTrackedHandle {
    CTrackedOwner* m_pOwner;                // +0x00
};

struct CGridCursor {
    CTrackedHandle* m_pHandle;              // +0x00
    float m_cellSize;                       // +0x04
    CVector3 GetCell(CVector3* fraction) const;
};

CVector3 CGridCursor::GetCell(CVector3* fraction) const
{
    CVector3 position = m_pHandle->m_pOwner->m_pLink->m_pTracked->m_position;
    float size = m_cellSize;
    float inv = 1.0f / size;
    float row = (float)floor(inv * position.m_y);
    CVector3 cell;
    cell.m_x = (float)floor(inv * position.m_x) * size;
    cell.m_y = row * size;
    cell.m_z = (float)floor(inv * position.m_z) * size;
    fraction->m_x = (position.m_x - cell.m_x) * inv;
    fraction->m_y = (position.m_y - cell.m_y) * inv;
    fraction->m_z = (position.m_z - cell.m_z) * inv;
    return cell;
}
