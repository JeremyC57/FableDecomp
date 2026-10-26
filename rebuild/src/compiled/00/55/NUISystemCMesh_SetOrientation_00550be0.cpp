// NUISystem::CMesh::SetOrientation(CRightHandedSet& orientation) -- store the mesh's orientation frame
// Leaf: SetOrientation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00550BE0, 47 bytes (`ret 4`), no callees.
// Label: CMesh vtable 0x01249F9C slot 148 (rtti-vtables.tsv); Ego_r.pdb names
// ?SetOrientation@CMesh@NUISystem@@UAEXAAVCRightHandedSet@@@Z (a non-const reference).
// Layout: Ego_r.pdb CMesh Orientation (CRightHandedSet, 24 bytes) at PDB +0x1a4; NUISystem::CSprite-family own
// members sit 0x30 below their PDB offsets in retail, so Orientation +0x174, RotationAxis +0x18c,
// RotationSpeed +0x198 (the same offsets slot 1, 0x00550070, reads).
// Shape: `add ecx,174h` then a six-dword memberwise copy through edx (the address of the member formed once).
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};

namespace NUISystem {
class CMesh { public:
    unsigned char _pad00[0x174];
    CRightHandedSet Orientation;            // +0x174
    C3DVector RotationAxis;                 // +0x18c
    float RotationSpeed;                    // +0x198
    void SetOrientation(CRightHandedSet& orientation);
};

void CMesh::SetOrientation(CRightHandedSet& orientation)
{
    Orientation = orientation;
}
}