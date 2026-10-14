// CNavigatorFlyer::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const -- true when the manager (+0x1c) finds a node under the destination (GetNodeAt @ 0x00A14F70 with the 2D temporary and layer 0) that is not switched off (+0xf)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A199F0, length 64 bytes.
// Leaf: ?CanNavigateToPos@CNavigatorFlyer@@UBE_NABVC3DVector@@0M_N@Z
// Shape: an 8-byte frame for the conversion temporary, a bool result, two epilogues, `ret 10h`.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
struct CANavQuadTreeNode_00a199f0 {
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
struct CNavigatorManager {
    CANavQuadTreeNode_00a199f0* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};

struct CNavigatorFlyer {
    virtual void v000();
    unsigned char      _pad04[0x18];
    CNavigatorManager* m_pManager;                 // +0x1c
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const;
};

bool CNavigatorFlyer::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const
{
    CANavQuadTreeNode_00a199f0* node = m_pManager->GetNodeAt(to, false);
    if (node && !node->m_switchedOff)
        return true;
    return false;
}
