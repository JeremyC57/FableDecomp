// CPSCAttractor::DatabaseCopy(const CPSCAttractor&) -- copy an attractor component's database settings: the two flag bits, the position parameter and its name, radius, force, fall-off and the user point list
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18060, 170 bytes (+ int3 padding; `ret 4`).
// Callees: vector<C3DVector>::reserve(size_type) 0x00920FF0 (manifest "vector_reserve") and resize(size_type, const C3DVector&)
//   0x0075C040 (landed as CVector3D::resize in CTCNymph::OnInitialActivate 0x0080FD90; default, uninitialised C3DVector fill).
// Layout = Ego_r.pdb CPSCAttractor (52 bytes; the user point CArray is a 12-byte STLport vector in retail): AttractorEnabled /
//   AttractorUseParamPosition bits 0/1 of +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14,
//   AttractorPositionParamName +0x18, AttractorInfluenceRadius +0x1c, AttractorInfluenceForce +0x20,
//   AttractorUserPointsArray +0x24.
// Shape: the source is read through a `const CPSCAttractor* src = &other` local (with the reference itself VC7.1 re-reads
//   the destination flag byte instead of merging through bl); each flag is a bitfield assignment (`xor ; and mask ; xor`
//   read-modify-write); the scalars copy in retail order; the
//   point list is sized with reserve + resize to the source count, then copied element by element (implicit C3DVector
//   assignment) with both arrays re-read each pass.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
struct CVector3D {                                 // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned int n) const { return _M_start[n]; }
    void reserve(unsigned int count);              // 0x00920FF0
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
class CPSCAttractor {
public:
    void*         __vftable;                       // +0x00
    unsigned char _pad04[8];
    bool          AttractorEnabled : 1;            // +0x0c bit 0
    bool          AttractorUseParamPosition : 1;   // +0x0c bit 1
    long          AttractorInfluenceFallOff;       // +0x10
    unsigned long AttractorPositionParam;          // +0x14
    unsigned long AttractorPositionParamName;      // +0x18
    float         AttractorInfluenceRadius;        // +0x1c
    float         AttractorInfluenceForce;         // +0x20
    CVector3D     AttractorUserPointsArray;        // +0x24
    void DatabaseCopy(const CPSCAttractor& other);
};

void CPSCAttractor::DatabaseCopy(const CPSCAttractor& other)
{
    const CPSCAttractor* src = &other;
    AttractorEnabled = src->AttractorEnabled;
    AttractorPositionParam = src->AttractorPositionParam;
    AttractorUseParamPosition = src->AttractorUseParamPosition;
    AttractorPositionParamName = src->AttractorPositionParamName;
    AttractorInfluenceRadius = src->AttractorInfluenceRadius;
    AttractorInfluenceForce = src->AttractorInfluenceForce;
    AttractorInfluenceFallOff = src->AttractorInfluenceFallOff;
    unsigned int count = src->AttractorUserPointsArray.size();
    AttractorUserPointsArray.reserve(count);
    AttractorUserPointsArray.resize(count);
    for (unsigned int i = 0; i < count; ++i)
        AttractorUserPointsArray[i] = src->AttractorUserPointsArray[i];
}