// CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& node1, const CANavQuadTreeNode& node2, float width) const -- the two points `width` either side of the edge two adjacent quad-tree nodes share, on the line through their centres (straight across when the centres are level)
// Leaf: ?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z
// Grade: DIFFER(871v862) -- straight-edge returns build the first point in a named local (87 diff lines v 118); left:
//   gradient-branch return construction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1A7E0, 862 bytes, thiscall (this unused) with hidden result pointer, `ret 10h`.
//   Caller 0x00A1CD30.
// Identity: Ego_r.pdb and FableWin.pdb `?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z`
//   (FableWin: next to the CAStarState helpers and ConstructFinalRoute in the CNavigatorAStar TU); the row returns a
//   16-byte pair of C2DVector through the hidden pointer, takes two node references and a float (`ret 10h`), and
//   reads node+4/+8 (Position) and CANavQuadTreeNode::GetSize 0x00A75B30. The catalog "CANavQuadTreeNode_GetConnectingPoint" is wrong.
#include <math.h>
#pragma intrinsic(fabs)

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CANavQuadTreeNode { public:
    virtual ~CANavQuadTreeNode();
    C2DVector     Position;        // +0x04
    unsigned char QuadTreeLevel;   // +0x0c
    unsigned char NavLayer;        // +0x0d
    bool          LeafNode;        // +0x0e
    bool          Blocked;         // +0x0f
    float GetSize() const;         // 0x00A75B30
};

class CNavigatorAStar {
    std::pair<C2DVector, C2DVector> GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& node1, const CANavQuadTreeNode& node2,
                                                                      float width) const;
};

std::pair<C2DVector, C2DVector> CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& node1,
                                                                                   const CANavQuadTreeNode& node2, float width) const
{
    float halfSize1 = node1.GetSize() * 0.5f;
    C2DBoxF box1(node1.Position.X - halfSize1, node1.Position.Y - halfSize1, halfSize1 + node1.Position.X, halfSize1 + node1.Position.Y);
    float halfSize2 = node2.GetSize() * 0.5f;
    C2DBoxF box2(node2.Position.X - halfSize2, node2.Position.Y - halfSize2, halfSize2 + node2.Position.X, halfSize2 + node2.Position.Y);
    if (fabs(box1.BRX - box2.TLX) < 0.0001f) {
        if (fabs(node1.Position.Y - node2.Position.Y) < 0.0001f)
            { C2DVector first(box1.BRX - width, node1.Position.Y); return std::pair<C2DVector, C2DVector>(first, C2DVector(box1.BRX + width, node1.Position.Y)); }
        float gradient = (node2.Position.Y - node1.Position.Y) / (node2.Position.X - node1.Position.X);
        C2DVector first(box1.BRX - width, (node1.GetSize() * 0.5f - width) * gradient + node1.Position.Y);
        C2DVector second(box1.BRX + width, (node1.GetSize() * 0.5f + width) * gradient + node1.Position.Y);
        return std::pair<C2DVector, C2DVector>(first, second);
    }
    if (fabs(box1.TLX - box2.BRX) < 0.0001f) {
        if (fabs(node1.Position.Y - node2.Position.Y) < 0.0001f)
            { C2DVector first(box1.TLX + width, node1.Position.Y); return std::pair<C2DVector, C2DVector>(first, C2DVector(box1.TLX - width, node1.Position.Y)); }
        float gradient = (node2.Position.Y - node1.Position.Y) / (node2.Position.X - node1.Position.X);
        C2DVector first(box1.TLX + width, (width - node1.GetSize() * 0.5f) * gradient + node1.Position.Y);
        C2DVector second(box1.TLX - width, (-(node1.GetSize() * 0.5f) - width) * gradient + node1.Position.Y);
        return std::pair<C2DVector, C2DVector>(first, second);
    }
    if (fabs(box1.BRY - box2.TLY) < 0.0001f) {
        if (fabs(node1.Position.X - node2.Position.X) < 0.0001f)
            { C2DVector first(node1.Position.X, box1.BRY - width); return std::pair<C2DVector, C2DVector>(first, C2DVector(node1.Position.X, box1.BRY + width)); }
        float gradient = (node2.Position.X - node1.Position.X) / (node2.Position.Y - node1.Position.Y);
        C2DVector first((node1.GetSize() * 0.5f - width) * gradient + node1.Position.X, box1.BRY - width);
        C2DVector second((node1.GetSize() * 0.5f + width) * gradient + node1.Position.X, box1.BRY + width);
        return std::pair<C2DVector, C2DVector>(first, second);
    }
    if (fabs(node1.Position.X - node2.Position.X) < 0.0001f)
        { C2DVector first(node1.Position.X, box1.TLY + width); return std::pair<C2DVector, C2DVector>(first, C2DVector(node1.Position.X, box1.TLY - width)); }
    float gradient = (node2.Position.X - node1.Position.X) / (node2.Position.Y - node1.Position.Y);
    C2DVector first((width - node1.GetSize() * 0.5f) * gradient + node1.Position.X, box1.TLY + width);
    C2DVector second((-(node1.GetSize() * 0.5f) - width) * gradient + node1.Position.X, box1.TLY - width);
    return std::pair<C2DVector, C2DVector>(first, second);
}
