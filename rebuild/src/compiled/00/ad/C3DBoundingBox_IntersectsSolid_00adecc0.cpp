// C3DBoundingBox::IntersectsSolid(const C3DBoundingSphere&) const -- true when the solid box and the sphere overlap (squared distance from the centre to the box <= radius^2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADECC0, length 176 bytes (`ret 4`).
// Ego_r.pdb: ?IntersectsSolid@C3DBoundingBox@@QBE_NABVC3DBoundingSphere@@@Z. Callees: none (0.0f pool @ 0x0122DEDC).
// Shape: Arvo's test: per axis, below the minimum or (else) above the maximum adds the squared excess to a
// running float; `if (d <= r*r) return true; return false;` gives the byte constants.
// Layout: PDB C3DBoundingBox MinCoord +0, MaxCoord +0xc; C3DBoundingSphere Center +0, Radius +0xc.
// Leaf: IntersectsSolid
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public:
    C3DVector Center;              // +0
    float Radius;                  // +0xc
};
class C3DBoundingBox { public:
    C3DVector MinCoord;            // +0
    C3DVector MaxCoord;            // +0xc
    bool IntersectsSolid(const C3DBoundingSphere& sphere) const;
};

bool C3DBoundingBox::IntersectsSolid(const C3DBoundingSphere& sphere) const
{
    float distance = 0.0f;
    if (sphere.Center.X < MinCoord.X) {
        float e = sphere.Center.X - MinCoord.X;
        distance += e * e;
    }
    else if (sphere.Center.X > MaxCoord.X) {
        float e = sphere.Center.X - MaxCoord.X;
        distance += e * e;
    }
    if (sphere.Center.Y < MinCoord.Y) {
        float e = sphere.Center.Y - MinCoord.Y;
        distance += e * e;
    }
    else if (sphere.Center.Y > MaxCoord.Y) {
        float e = sphere.Center.Y - MaxCoord.Y;
        distance += e * e;
    }
    if (sphere.Center.Z < MinCoord.Z) {
        float e = sphere.Center.Z - MinCoord.Z;
        distance += e * e;
    }
    else if (sphere.Center.Z > MaxCoord.Z) {
        float e = sphere.Center.Z - MaxCoord.Z;
        distance += e * e;
    }
    float radius = sphere.Radius;
    if (distance <= radius * radius)
        return true;
    return false;
}