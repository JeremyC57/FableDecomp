// C3DHelperPoint::C3DHelperPoint(const C3DVector&, long) -- copies the position (memberwise) and stores the bone index at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E470, length 32 bytes. No callees.
// PDB layout: C3DHelperPoint = {C3DVector Position +0, long BoneIndex +0xc} (16 bytes). The
// position is copied one component at a time through a single register (a memberwise C3DVector
// copy constructor), the index as a dword; ret 8.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct C3DHelperPoint {
    C3DVector Position;                    // +0
    long BoneIndex;                        // +0xc
    C3DHelperPoint(const C3DVector& position, long boneIndex);
};

C3DHelperPoint::C3DHelperPoint(const C3DVector& position, long boneIndex)
    : Position(position), BoneIndex(boneIndex)
{
}