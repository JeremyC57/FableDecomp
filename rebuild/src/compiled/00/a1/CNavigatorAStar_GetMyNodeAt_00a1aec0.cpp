// CNavigatorAStar::GetMyNodeAt(const C3DVector* position) -- the navigation node under the position: the owner (+0x14) picks the layer (its slot 16 with the position), the manager (+0x1c) the node (GetNodeAt @ 0x00A14F70 with the position narrowed to 2D in a frame temporary)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1AEC0, length 64 bytes.
// Leaf: GetMyNodeAt
// Shape: an 8-byte frame for the C2DVector conversion temporary, `this` in esi, the layer call first (its result pushed first), `ret 4`.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
struct CANavQuadTreeNode_00a1aec0 {
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
class CNavigatorManager { public:
    CANavQuadTreeNode_00a1aec0* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};
struct CNavigatorOwner_00a1aec0 {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual bool GetLayerAt(const C3DVector* position);   // slot 16 (+0x40)
};

class CNavigatorAStar { public:
    unsigned char           _pad00[0x14];
    CNavigatorOwner_00a1aec0*  m_pOwner;              // +0x14
    long                    m_18;
    CNavigatorManager*      m_pManager;            // +0x1c
    CANavQuadTreeNode_00a1aec0* GetMyNodeAt(const C3DVector* position);
};

CANavQuadTreeNode_00a1aec0* CNavigatorAStar::GetMyNodeAt(const C3DVector* position)
{
    return m_pManager->GetNodeAt(*position, m_pOwner->GetLayerAt(position));
}