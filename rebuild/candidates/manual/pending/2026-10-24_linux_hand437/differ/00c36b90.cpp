// CGridCursor::GetCell(CVector3* fraction) const -- splits the tracked position into its grid cell origin (returned by value) and the fractional position inside the cell (in cell units)
// Grade: DIFFER(219v219) -- the reciprocal as a repeated `(1.0f / size)` expression (no `inv` local) gives retail's x87 order everywhere (`fst [esp+8] ; fmul y`, then `fld inv ; fmul v`); left is one scheduling swap after the third floor call: retail `mov eax,[esp+34h] ; fmul [esp+0Ch]`, ours `fmul ; mov eax` (tried QFE-4035, double-precision cell products (moves the fstp instead), `size * floor(z)`, a declared CVector3 copy ctor (208 bytes))
// Leaf: GetCell@CGridCursor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36B90, 219 bytes, __thiscall `ret 8` (hidden result pointer + fraction), eax =
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
    cell.m_z = (float)floor((1.0f / size) * position.m_z) * size;
    fraction->m_x = (position.m_x - cell.m_x) * (1.0f / size);
    fraction->m_y = (position.m_y - cell.m_y) * (1.0f / size);
    fraction->m_z = (position.m_z - cell.m_z) * (1.0f / size);
    return cell;
}
