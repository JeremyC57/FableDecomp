// C3DBoundingBox::BuildFromPoints(const C3DVector* points, float num_points) -- an empty point set (count 0.0) zeroes the box; otherwise MinCoord and MaxCoord start at the first point and every point (the first included) widens them per component; the count is a float, so the loop index is compared as an unsigned-to-float conversion
// Leaf: ?BuildFromPoints@C3DBoundingBox@@QAEXPBVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADED70, 294 bytes (+ int3 padding), thiscall `ret 8`.
// Identity (proven): Ego_r.pdb and FableWin.pdb name `?BuildFromPoints@C3DBoundingBox@@QAEXPBVC3DVector@@M@Z`, and the
//   landed vector overload 0x00ADEEA0 binds exactly this spelling for its call to 0x00ADED70 (forwarding &points[0] and
//   the size converted to float). The catalog's gapscan name is a placeholder.
// Layout: Ego_r.pdb C3DVector (12: X, Y, Z), C3DBoundingBox (24: MinCoord, MaxCoord), as the landed 0x00ADEEA0 model.
// Shape: `count == 0.0f` is the `fucompp ; test ah,44h ; jp` test against 0.0f (0x0122DEDC); the min / max arms are
//   `a < b ? a : b` (`test ah,5 ; jp`) and `a > b ? a : b` (`test ah,41h ; jne`); the loop index is an unsigned long
//   converted with the 2^32 fix-up (0x0122DCB4) in the dead `points` slot.
class C3DVector {
public:
    float X;
    float Y;
    float Z;
};

class C3DBoundingBox {
public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
    void BuildFromPoints(const C3DVector* points, float num_points);
};

void C3DBoundingBox::BuildFromPoints(const C3DVector* points, float num_points)
{
    if (num_points == 0.0f) {
        MinCoord.X = 0.0f;
        MinCoord.Y = 0.0f;
        MinCoord.Z = 0.0f;
        MaxCoord.X = 0.0f;
        MaxCoord.Y = 0.0f;
        MaxCoord.Z = 0.0f;
        return;
    }
    MinCoord = points[0];
    MaxCoord = points[0];
    for (unsigned long i = 0; i < num_points; ++i) {
        MinCoord.X = MinCoord.X < points[i].X ? MinCoord.X : points[i].X;
        MinCoord.Y = MinCoord.Y < points[i].Y ? MinCoord.Y : points[i].Y;
        MinCoord.Z = MinCoord.Z < points[i].Z ? MinCoord.Z : points[i].Z;
        MaxCoord.X = MaxCoord.X > points[i].X ? MaxCoord.X : points[i].X;
        MaxCoord.Y = MaxCoord.Y > points[i].Y ? MaxCoord.Y : points[i].Y;
        MaxCoord.Z = MaxCoord.Z > points[i].Z ? MaxCoord.Z : points[i].Z;
    }
}