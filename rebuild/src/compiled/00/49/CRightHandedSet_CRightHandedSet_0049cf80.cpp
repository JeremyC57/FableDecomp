// CRightHandedSet::CRightHandedSet(const CRightHandedSet& other) -- copy constructor: Up and Forward memberwise (six dwords)
// Leaf: ??0CRightHandedSet@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049CF80, 24-byte memberwise copy.
// Identity (proven, chunk c675): the by-value CRightHandedSet copy in CTCInventoryItem::GetRHSetFromOrientation 0x005D8EF0 (and the
//   landed callers that bind ??0CRightHandedSet@@QAE@ABV0@@Z) reaches this address; Ego_r.pdb CRightHandedSet {C3DVector Up +0,
//   Forward +0xC}, 24 bytes.
// Naming wave 17: landed as CNearestBuildingWithProperty's copy constructor (six int members m0..m5); respelled on the PDB class
//   (name-only: the bytes are unchanged).
class C3DVector {                                       // Ego_r.pdb {X, Y, Z}, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CRightHandedSet {
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const CRightHandedSet& other);
};
CRightHandedSet::CRightHandedSet(const CRightHandedSet& other)
    : Up(other.Up), Forward(other.Forward)
{
}
