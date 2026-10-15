// CPhysicsFacesSegmentBranch::scalar_deleting_destructor(unsigned int) -- `scalar deleting destructor': runs the inlined destructor (deletes both children, restores the interface vtable) and frees the object when bit 0 of the flags is set
// Leaf: ??_GCPhysicsFacesSegmentBranch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86F60, 79 bytes. Callee: 0x00BFE9BC operator delete.
//
// Vtable slot 3 of CPhysicsFacesSegmentBranch (0x0129DE88; the manifest calls it
// `vector_deleting_destructor`, the body is the scalar form: one `test byte ptr [esp+8],1`, no
// array path). The destructor body of 0x00A85FF0 is inlined verbatim, the flag test is scheduled
// next to the NegativeChild clear, and `this` is returned in eax. A `new` factory in the TU pulls
// the vtable (and with it this `??_G`) into the object; the extractor selects `??_G` by name.
// Model: as 0x00A85FF0 -- the engine header flattens the vptr, so the class is local with the PDB
// member names and offsets.
struct CPlane {
    float m_normal[3];
    float m_distance;
};

struct CIPhysicsFacesSegmentBase {
    virtual unsigned char HasChildren() = 0;   // slot 0
    virtual long Sizeof() = 0;                 // slot 1
    virtual long GetNoFaces() = 0;             // slot 2
    virtual ~CIPhysicsFacesSegmentBase() {}    // slot 3
};

struct CPhysicsFacesSegmentBranch : CIPhysicsFacesSegmentBase {
    CPlane                     ChildDividingPlane;   // +0x04
    CIPhysicsFacesSegmentBase* PositiveChild;        // +0x14
    CIPhysicsFacesSegmentBase* NegativeChild;        // +0x18

    virtual unsigned char HasChildren();       // 0x00A86F10
    virtual long Sizeof();                     // 0x00A85FB0
    virtual long GetNoFaces();                 // 0x00A85FD0
    virtual ~CPhysicsFacesSegmentBranch();
};

CPhysicsFacesSegmentBranch::~CPhysicsFacesSegmentBranch()
{
    delete PositiveChild;
    PositiveChild = 0;
    delete NegativeChild;
    NegativeChild = 0;
}

CPhysicsFacesSegmentBranch* NewPhysicsFacesSegmentBranch()
{
    return new CPhysicsFacesSegmentBranch;
}
