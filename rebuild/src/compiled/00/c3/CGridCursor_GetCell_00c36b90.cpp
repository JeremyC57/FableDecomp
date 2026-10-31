// CGridCursor::GetCell(CVector3* fraction) const -- splits the tracked position into its grid cell origin (returned by value) and the fractional position inside the cell (in cell units)
// Leaf: GetCell@CGridCursor
// Grade: RELOCATION_MATCH (wave-2 sec6): the third floor result held in a named local (float layer = floor(...);
//   cell.m_z = layer * size;), like the parked row local for y, gives retail's mov eax,[fraction] before the fmul
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36B90, 219 bytes, __fastcall `ret 8` (hidden result pointer + fraction), eax =
//   the result. Descriptive name (no PDB covers this library): the catalog's `Grid_ComputeLocalCoordinates` is kept as
//   the meaning. Caller 0x00C371FA (zero-initialised fraction local, result local).
// Callee: MSVCR71 floor through the IAT (0x01440100), on the double-widened products.
// Shape: the position {x,y,z} is copied out of the chain this +0 -> +0 -> +4 -> +0x38 -> +8 and the cell size (+4) into
//   a frame float; the cell is a named local copied to the hidden result (VC7.1 has no NRVO).
// Reciprocal: retail's 1/size is a compiler CSE temporary (spilled across the floor calls), not a named local: the
//   named `inv` local is popped after the divide and multiplied as the right operand.
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
    float row = (float)floor((1.0f / size) * position.m_y);
    CVector3 cell;
    cell.m_x = (float)floor((1.0f / size) * position.m_x) * size;
    cell.m_y = row * size;
    float layer = (float)floor((1.0f / size) * position.m_z);
    cell.m_z = layer * size;
    fraction->m_x = (position.m_x - cell.m_x) * (1.0f / size);
    fraction->m_y = (position.m_y - cell.m_y) * (1.0f / size);
    fraction->m_z = (position.m_z - cell.m_z) * (1.0f / size);
    return cell;
}