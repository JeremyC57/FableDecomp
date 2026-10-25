// Grade: DIFFER(605v605) -- only the x sum's fadd operands are commuted: retail `fld [eax+edi] ; add eax,edi ; fadd [esi]` (point first), ours `fld [esi] ; fadd [eax+edi]` (offset first) in all five unrolled/remainder copies; y, the diffs, the squares, the compare, the __allmul range check, the 4x unroll and the frame match. Tried: scalar dx/dy (567v605: no sum temporary, 0xc frame), C3DVector operator+/- temporaries (605v605), plus the PDB's user copy constructor and const returns (this file).
// FindNearestIndexToPoint(const CArray<C3DVector>& points, const C3DVector& offset, long group, const CArray<bool>& used, const C3DVector& target) -- among the three points of a group not yet used, the one whose offset position is nearest (2D) to the target; 0 when none
// Leaf: ?FindNearestIndexToPoint@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8EE0, 605 bytes, __fastcall (points in ecx, offset in edx), `ret 0Ch`, returns long.
// Identity: no PDB name and no direct callers (catalog "FindNearestIndexToPoint", forge autoname-t5); a /Gr-default free
//   function next to the combat-ring code (0x006E8850, 0x006E94D0). The name and parameter names are descriptive.
// Callees: __allmul 0x00BFEB50 (the 64-bit `(group + 1) * 3` range check).
// Layout: points = a retail 12-byte CArray<C3DVector> (start pointer re-read per element), used = a byte-per-entry
//   CArray<bool> (start held in ebp); only X/Y of the sum and difference are used (the Z lanes are dead).
// Shape: nearest = -1 and 1e33f, then `if (end < 0 || end > 0x7fffffff) return 0` on the int64 product, a 4x-unrolled
//   loop over group*3 .. group*3+3 (remainder loop after it) testing `!used[i]` and `distSq < nearest` (`test ah,5 ;
//   jp`), the C3DVector sum/difference temporaries (dx spilled to the temporary's X), `dx*dx + dy*dy` (top of stack
//   squared first), and -1 mapped to 0.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};

template <class T> class CArray {                   // retail STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};

long __fastcall FindNearestIndexToPoint(const CArray<C3DVector>& points, const C3DVector& offset, long group, const CArray<bool>& used, const C3DVector& target)
{
    long nearest = -1;
    float nearestDistSq = 1e33f;
    __int64 end = ((__int64)group + 1) * 3;
    if (end < 0 || end > 0x7fffffff)
        return 0;
    for (long i = group * 3; i < (group + 1) * 3; ++i) {
        if (!used[i]) {
            C3DVector diff = points[i] + offset - target;
            float distSq = diff.X * diff.X + diff.Y * diff.Y;
            if (distSq < nearestDistSq) {
                nearestDistSq = distSq;
                nearest = i;
            }
        }
    }
    if (nearest == -1)
        nearest = 0;
    return nearest;
}
