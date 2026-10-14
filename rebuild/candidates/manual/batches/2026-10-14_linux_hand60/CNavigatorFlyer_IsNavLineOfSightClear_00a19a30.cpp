// CNavigatorFlyer::IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const -- true when the manager (+0x1c, read once) finds a node under both positions (GetNodeAt @ 0x00A14F70 with the 2D temporaries and layer 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A19A30, length 95 bytes.
// Leaf: ?IsNavLineOfSightClear@CNavigatorFlyer@@UBE_NABVC3DVector@@0@Z
// Shape: an 8-byte frame reused for both conversion temporaries, the manager in esi, `ret 8`.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
struct CANavQuadTreeNode_00a19a30 {
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
struct CNavigatorManager {
    CANavQuadTreeNode_00a19a30* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};

struct CNavigatorFlyer {
    virtual void v000();
    unsigned char      _pad04[0x18];
    CNavigatorManager* m_pManager;                 // +0x1c
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;
};

bool CNavigatorFlyer::IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const
{
    CNavigatorManager* manager = m_pManager;
    if (manager->GetNodeAt(from, false) && manager->GetNodeAt(to, false))
        return true;
    return false;
}
