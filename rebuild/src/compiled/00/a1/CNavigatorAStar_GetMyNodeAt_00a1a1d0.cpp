// CNavigatorAStar::GetMyNodeAt(const C3DVector* position) -- the navigation node under the position: the owner (+0x14) picks the layer (its slot 16 with the position), the manager (+0x1c) the node (GetNodeAt @ 0x00A14F70 with the position narrowed to 2D in a frame temporary)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1A1D0, length 64 bytes.
// Leaf: GetMyNodeAt
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Shape: an 8-byte frame for the C2DVector conversion temporary, `this` in esi, the layer call first (its result pushed first), `ret 4`.
#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
class CANavQuadTreeNode;                        // Ego_r.pdb node class (GetNodeAt 0x00A14F70 returns it)
struct CANavQuadTreeNode_00a1a1d0 {
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70 (Ego_r.pdb, const)
};
struct CNavigatorOwner_00a1a1d0 {
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
    CNavigatorOwner_00a1a1d0*  m_pOwner;              // +0x14
    long                    m_18;
    CNavigatorManager*      m_pManager;            // +0x1c
    CANavQuadTreeNode_00a1a1d0* GetMyNodeAt(const C3DVector* position);
};

CANavQuadTreeNode_00a1a1d0* CNavigatorAStar::GetMyNodeAt(const C3DVector* position)
{
    return (CANavQuadTreeNode_00a1a1d0*)m_pManager->GetNodeAt(*position, m_pOwner->GetLayerAt(position));
}