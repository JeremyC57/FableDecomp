// CANavQuadTreeNode::operator==(const CANavQuadTreeNode&) -- same position (within 1e-4), level, layer, flags and virtual key
// Grade: DIFFER(116v116) -- scheduling only: retail loads the other node's pointer (`mov ecx,[esp+8]`) before `fld [esi+4]`, the lane after it; tried a by-reference NearlyEqual helper and a pointer local
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A763F0, length 116 bytes.
// Callees: the nodes' virtual slot 5 (+0x14), called on the other node first, then on this one.
//
// Shape: `fabs(a - b) <= 0.0001f` per coordinate (`fsub ; fabs ; fcomp [eps] ; test ah,41h ; jp` to the false
// tail = the negated `<=`), then the four byte members compared in source order QuadTreeLevel (+0xc),
// LeafNode (+0xe), Blocked (+0xf), NavLayer (+0xd) with alternating al/dl loads, then the virtual key:
// the other node's value is fetched first into edi (`long key = other.GetKey();`) and compared as
// `GetKey() == key` (`cmp eax,edi`). The result is a full-register int (`mov eax,1` / `xor eax,eax`).
// The engine header CANavQuadTreeNode.h is data-only (a void* vptr word); the virtual call needs a
// polymorphic local model, members carry the header's names and offsets.
#include <math.h>
#pragma intrinsic(fabs)

class CANavQuadTreeNode {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetKey() const;                // slot 5 (+0x14)
    float Position_X;                           // +0x04 C2DVector::X
    float Position_Y;                           // +0x08 C2DVector::Y
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
    int operator==(const CANavQuadTreeNode& other) const;
};

int CANavQuadTreeNode::operator==(const CANavQuadTreeNode& other) const
{
    if (fabs(Position_X - other.Position_X) <= 0.0001f && fabs(Position_Y - other.Position_Y) <= 0.0001f
        && QuadTreeLevel == other.QuadTreeLevel && LeafNode == other.LeafNode && Blocked == other.Blocked
        && NavLayer == other.NavLayer) {
        long key = other.GetKey();
        if (GetKey() == key)
            return 1;
    }
    return 0;
}
