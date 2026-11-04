// CNavigatorFlyer::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const -- true when the manager (+0x1c) finds a node under the destination (GetNodeAt @ 0x00A14F70 with the 2D temporary and layer 0) that is not switched off (+0xf)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A199F0, length 64 bytes.
// Leaf: ?CanNavigateToPos@CNavigatorFlyer@@UBE_NABVC3DVector@@0M_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Shape: an 8-byte frame for the conversion temporary, a bool result, two epilogues, `ret 10h`.
#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
class CANavQuadTreeNode { public:
    unsigned char _pad00[0xf];
    bool          m_switchedOff;                   // +0x0f
};
class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70 (Ego_r.pdb, const)
};

class CNavigatorFlyer { public:
    virtual void v000();
    unsigned char      _pad04[0x18];
    CNavigatorManager* m_pManager;                 // +0x1c
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const;
};

bool CNavigatorFlyer::CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool flag) const
{
    const CANavQuadTreeNode* node = m_pManager->GetNodeAt(to, false);
    if (node && !node->m_switchedOff)
        return true;
    return false;
}