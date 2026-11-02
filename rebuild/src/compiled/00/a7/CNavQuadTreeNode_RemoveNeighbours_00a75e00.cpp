// CNavQuadTreeNode::RemoveNeighbours() -- recurse into the four child nodes (top-left, top-right, bottom-left, bottom-right) so each removes its neighbours
// Leaf: ?RemoveNeighbours@CNavQuadTreeNode@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75E00, length 36 bytes (ends in `jmp [edx+4Ch]`; int3 padding 0x00A75E24..0x00A75E2F,
//   SetUpNeighbours starts at 0x00A75E30), thiscall.
// Identity: the only reference is slot 19 of vtable 0x0129DCD4 (RTTI CNavQuadTreeNode : CANavQuadTreeNode : CBaseClassNonCopyable :
//   CBaseClass). Ego_r ?RemoveNeighbours@CNavQuadTreeNode@@MAEXXZ (protected virtual), donor order CalculateChildrenNeighbours /
//   RemoveNeighbours / SetUpNeighbours = retail 0x00A75DA0 / 0x00A75E00 / 0x00A75E30 (slots 18 / 19 / 17); FableWin
//   ?RemoveNeighbours@CANavQuadTreeNode@@MAEXXZ is the base slot (the landed leaf CNavNavigableLeafNode::RemoveNeighbours 0x00A76C10
//   overrides the same slot). The call on each child goes through slot 19 again (+0x4c): the recursion.
// Layout: Ego_r CNavQuadTreeNode (0x20 bytes, no containers): CANavQuadTreeNode base 0x10, TLNode +0x10, TRNode +0x14, BLNode +0x18,
//   BRNode +0x1c (CANavQuadTreeNode*). engine/CNavQuadTreeNode.h / CANavQuadTreeNode.h flatten the vptr as data, so the polymorphic
//   chain is modelled locally (header debt).
// Shape: `this` in esi, each child and its vtable reloaded per call, the last call a sibling `jmp`.
#include "engine/CBaseClassNonCopyable.h"

class CANavQuadTreeNode : public CBaseClassNonCopyable { public:   // retail 0x10 bytes
    friend class CNavQuadTreeNode;
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
protected:
    virtual void RemoveNeighbours();                         // slot 19 (+0x4c)
public:
    float         Position_X;                                // +0x04 C2DVector Position
    float         Position_Y;                                // +0x08
    unsigned char QuadTreeLevel;                             // +0x0c
    unsigned char NavLayer;                                  // +0x0d
    bool          LeafNode;                                  // +0x0e
    bool          Blocked;                                   // +0x0f
};

class CNavQuadTreeNode : public CANavQuadTreeNode { public:          // vtable 0x0129DCD4
protected:
    virtual void RemoveNeighbours();
public:
    CANavQuadTreeNode* TLNode;                               // +0x10
    CANavQuadTreeNode* TRNode;                               // +0x14
    CANavQuadTreeNode* BLNode;                               // +0x18
    CANavQuadTreeNode* BRNode;                               // +0x1c
};

void CNavQuadTreeNode::RemoveNeighbours()
{
    TLNode->RemoveNeighbours();
    TRNode->RemoveNeighbours();
    BLNode->RemoveNeighbours();
    BRNode->RemoveNeighbours();
}