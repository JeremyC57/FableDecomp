// CNavigatorAStar::GetCostFromNodeToNode(const CANavQuadTreeNode*, const CANavQuadTreeNode*, bool) const -- A* edge cost: the 2D distance between the nodes, scaled (unless told to ignore it) by the cost the external interface gives the destination's preferability class
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1A0A0, 151 bytes (`ret 0Ch`).
// Callees: the 2D distance helper @ 0x009E2460 (`__fastcall`, two const C2DVector&; label "Calc_Distance2D"), the destination's
//   virtual slot 8 GetPreferabilityCost (+0x20, an unsigned char: 0 / 0x40 / 0x80), the external interface's (+0x14) virtual
//   slots 18 (+0x48) and 19 (+0x4c). Constants 1.0f @ 0x0122DED8, 0.5f @ 0x0122F59C.
// Shape: the distance is a named float (VC homes it in the dead `to` slot); the factor starts at 1.0f and a dense `switch`
//   on the byte class picks it (case 0x40 is the mean of the two interface costs, slot 19 called first; 0x80 and default
//   both use slot 19); the result is `factor * distance`.
class C2DVector { public:
    float X, Y;
};
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b);   // 0x009E2460
class CANavQuadTreeNode { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual unsigned char GetPreferabilityCost() const;     // slot 8 (+0x20)
    C2DVector     Position;                                 // +0x04
    unsigned char QuadTreeLevel;                            // +0x0c
    unsigned char NavLayer;                                 // +0x0d
    bool          LeafNode;                                 // +0x0e
    bool          Blocked;                                  // +0x0f
};
class CNavigatorExternalInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual float GetPreferredNodeCost() const;             // slot 18 (+0x48)
    virtual float GetUnpreferredNodeCost() const;           // slot 19 (+0x4c)
};
enum { PREFERABILITY_PREFERRED = 0x00, PREFERABILITY_NEUTRAL = 0x40, PREFERABILITY_UNPREFERRED = 0x80 };
class CNavigatorAStar { public:
    void*                        __vftable;                 // +0x00
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;                // +0x14 (CNavigatorBase)
private:
    float GetCostFromNodeToNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to, bool ignorePreferability) const;
};

float CNavigatorAStar::GetCostFromNodeToNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* to, bool ignorePreferability) const
{
    float distance = Distance2D(from->Position, to->Position);
    float factor = 1.0f;
    if (!ignorePreferability) {
        switch (to->GetPreferabilityCost()) {
        case PREFERABILITY_PREFERRED:
            factor = PInterface->GetPreferredNodeCost();
            break;
        case PREFERABILITY_NEUTRAL:
            factor = (PInterface->GetUnpreferredNodeCost() + PInterface->GetPreferredNodeCost()) * 0.5f;
            break;
        case PREFERABILITY_UNPREFERRED:
            factor = PInterface->GetUnpreferredNodeCost();
            break;
        default:
            factor = PInterface->GetUnpreferredNodeCost();
            break;
        }
    }
    return factor * distance;
}