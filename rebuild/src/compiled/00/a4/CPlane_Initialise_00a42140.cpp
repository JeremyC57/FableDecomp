// CPlane::Initialise(const C3DVector& normal, float distance) -- store the plane's normal and distance, report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42140, length 36 bytes.
// Identity: Ego_r.pdb `?Initialise@CPlane@@QAE_NABVC3DVector@@M@Z`; PDB CPlane = Normal (C3DVector)
// +0, Distance +0xC.
// Callees: none.
// Shape: the normal is copied by C3DVector's IMPLICIT (compiler-generated) assignment of a POD: a
// `this` copy in edx and each component through esi as a dword (a user memberwise operator= copies
// through edx straight off ecx, 32 v 36); the float distance is a plain dword copy through edx, and
// the constant true result is `mov al,1` (`ret 8`).
class C3DVector { public:
    float X, Y, Z;
};

class CPlane { public:
    C3DVector Normal;                              // +0x00
    float     Distance;                            // +0x0c
    bool Initialise(const C3DVector& normal, float distance);
};

bool CPlane::Initialise(const C3DVector& normal, float distance)
{
    Normal = normal;
    Distance = distance;
    return true;
}