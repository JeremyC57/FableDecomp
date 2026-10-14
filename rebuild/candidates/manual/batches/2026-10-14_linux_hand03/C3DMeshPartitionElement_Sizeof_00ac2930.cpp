// C3DMeshPartitionElement::Sizeof() const -- recursive footprint of the two sub-element pointers (4 bytes each plus the child's Sizeof when present)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2930, length 64 bytes.
// Callees: 0x00AC2930 (self, through both SubElements)
// The manifest label (C3DMeshLODInfo::Sizeof) is propagated; the body sits between the landed
// C3DMeshFilePartitionChunk::WriteToFile (0x00AC2920) and ::ReadFromFile (0x00AC2970) and recurses through the
// PDB's `C3DMeshPartitionElement* SubElements[2]` at +0x20/+0x24. The element's own size is not counted here.
//
#include "rebuild_abi.h"

struct C3DVectorF { float X, Y, Z; };
struct C3DBoundingBox    { C3DVectorF Min, Max; };
struct C3DBoundingSphere { C3DVectorF Center; float Radius; };
struct C3DMeshPartitionElement {
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1c
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28   sizeof == 0x38
    unsigned long Sizeof() const;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshPartitionElement) == 0x38);
unsigned long C3DMeshPartitionElement::Sizeof() const
{
    unsigned long firstSize  = SubElements[0] ? SubElements[0]->Sizeof() + sizeof(C3DMeshPartitionElement*) : sizeof(C3DMeshPartitionElement*);
    unsigned long secondSize = SubElements[1] ? SubElements[1]->Sizeof() + sizeof(C3DMeshPartitionElement*) : sizeof(C3DMeshPartitionElement*);
    return secondSize + firstSize;
}
