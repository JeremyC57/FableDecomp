// CPhysicsFacesSegmentBranch::~CPhysicsFacesSegmentBranch() -- deletes both child segments (virtual deleting destructor, slot 3), clears the pointers, then the inline base destructor restores the interface vtable
// Leaf: ??1CPhysicsFacesSegmentBranch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85FF0, 59 bytes (+5 int3 padding).
// No direct callees: the children are destroyed through their vtable slot 3 (`push 1 ; call [eax+0Ch]`).
//
// Vtables (no RTTI in this module): CPhysicsFacesSegmentBranch 0x0129DE88 = { 0x00A86F10
// HasChildren, 0x00A85FB0 Sizeof, 0x00A85FD0 GetNoFaces, 0x00A86F60 scalar deleting destructor };
// the interface CIPhysicsFacesSegmentBase 0x0129DE70 has three `_purecall` slots and its own
// deleting destructor in slot 3. The body is `delete PositiveChild; PositiveChild = 0;` twice: the
// null test belongs to `delete` (the clear lands after the join, on both paths), the derived vtable
// store is scheduled into the first test, and the base's inline (empty) destructor stores 0x0129DE70
// last.
// Model: rebuild/include/engine/CPhysicsFacesSegmentBranch.h flattens the vptr into a `void*`
// member, which cannot produce the vtable stores, so the class is modelled locally with the same
// PDB member names and offsets (ChildDividingPlane +0x04, PositiveChild +0x14, NegativeChild +0x18).
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