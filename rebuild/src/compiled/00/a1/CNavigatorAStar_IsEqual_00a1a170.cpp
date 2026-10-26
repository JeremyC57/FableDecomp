// CNavigatorAStar::IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b) -- true for the same node, false when either is null, else the nodes' own equality (CANavQuadTreeNode::operator== @ 0x00A763F0: both coordinates within 1e-4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1A170, length 48 bytes.
// Leaf: IsEqual
// Shape: `this` is not read, no frame, the operator's al returned straight through, `ret 8`.
class CANavQuadTreeNode { public:
    bool operator==(const CANavQuadTreeNode& other) const;   // 0x00A763F0
};

class CNavigatorAStar { public:
    bool IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b);
};

bool CNavigatorAStar::IsEqual(const CANavQuadTreeNode* a, const CANavQuadTreeNode* b)
{
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    return *a == *b;
}