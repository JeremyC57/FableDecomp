// CNavigatorAStar::IsNavigableAt(const C3DVector* position, bool layer) -- 1 when the manager (+0x1c) finds a node under the position (GetNodeAt @ 0x00A14F70 with the 2D temporary) that is not switched off (+0xf); 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1A210, length 80 bytes.
// Leaf: IsNavigableAt
// Shape: an 8-byte frame for the conversion temporary, a long result (`mov eax,1`), two epilogues, `ret 8`.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
struct CANavQuadTreeNode_00a1a210 {
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
class CNavigatorManager { public:
    CANavQuadTreeNode_00a1a210* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};

class CNavigatorAStar { public:
    unsigned char      _pad00[0x1c];
    CNavigatorManager* m_pManager;                 // +0x1c
    long IsNavigableAt(const C3DVector* position, bool layer);
};

long CNavigatorAStar::IsNavigableAt(const C3DVector* position, bool layer)
{
    CANavQuadTreeNode_00a1a210* node = m_pManager->GetNodeAt(*position, layer);
    if (node && !node->m_switchedOff)
        return 1;
    return 0;
}