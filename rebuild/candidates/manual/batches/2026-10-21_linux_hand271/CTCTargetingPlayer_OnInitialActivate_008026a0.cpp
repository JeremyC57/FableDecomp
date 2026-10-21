// CTCTargetingPlayer::OnInitialActivate() -- after the base resolves the targeting sub-def, take the largest range of the def's range map (times 1.2) as the max targeting distance and zero the targeted-position vectors, offsets and the Z-lock timer
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008026A0, 481 bytes (+int3), plain `ret`; CTCTargetingPlayer vtable 0x0124280C
//   slot 6 (abs ref @ 0x01242824). LABEL: the catalog/manifest name "CTCInventoryItem::OnCreate" is propagated from the
//   byte-identical-prefix base; CTCTargetingAI::OnInitialActivate @ 0x007F59C0 is slot 6 of 0x012411FC (landed).
// Callees: CTCTargetingAI::OnInitialActivate @ 0x007F59C0. Float 1.2f @ 0x0125A690.
// Shape: STLport map<long, float> walk (header node pointer at def +0x74; begin = header->_M_left; node value +0x14) with the
//   tree increment written as a static `_M_increment(node)` returning the next node (an in-place member increment
//   CSEs the 0 constant into a register shared with the float zero store); `if (value > Max) Max = value` copies the float
//   with integer moves. The vectors are assigned from constructed temporaries (`C3DVector(0,0,0)`), which VC7.1 keeps in the
//   frame (store 0 / reload) with no user copy operations.
// Layout: engine/CTCTargetingPlayer.h is the PDB view with 16-byte vectors at +0x68/+0x78; retail STLport vectors are 12 bytes,
//   so everything after them is 8 lower: TargetedPos +0x84, OldTargetedPos +0x90, TargetedPosOffset3D +0x9C,
//   OldTargetedPosOffset3D +0xA8, the four C2DVectors +0xB4..+0xCC; MaxTargetingDistance +0x48 and ZLockTargetSelectionTimer
//   +0x38 agree. CTCTargetingAI PSubDef +0x30 (as the landed 0x007F59C0).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct SRangeNode {                                // STLport _Rb_tree_node<pair<const long, float> >
    long        _M_color;
    SRangeNode* _M_parent;                         // +0x04
    SRangeNode* _M_left;                           // +0x08
    SRangeNode* _M_right;                          // +0x0c
    long        first;                             // +0x10
    float       second;                            // +0x14
};
struct CRangeIterator {
    SRangeNode* _M_node;
    CRangeIterator(SRangeNode* node) : _M_node(node) {}
    bool operator!=(const CRangeIterator& other) const { return _M_node != other._M_node; }
    SRangeNode* operator->() const { return _M_node; }
    static SRangeNode* _M_increment(SRangeNode* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SRangeNode* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
    CRangeIterator& operator++()
    {
        _M_node = _M_increment(_M_node);
        return *this;
    }
};
struct CRangeMap {                                 // STLport map<long, float>: the header node pointer
    SRangeNode* _M_header;
    CRangeIterator begin() const { return CRangeIterator(_M_header->_M_left); }
    CRangeIterator end() const { return CRangeIterator(_M_header); }
};
struct CTargetingPlayerDef {
    unsigned char _pad00[0x74];
    CRangeMap TargetingRanges;                     // +0x74
};
struct CTCTargetingAI {
    void* __vftable;
    unsigned char _pad04[0x2c];
    CTargetingPlayerDef* PSubDef;                  // +0x30
    unsigned char _pad34[0x4];
    void OnInitialActivate();                      // 0x007F59C0
};
struct CTCTargetingPlayer : CTCTargetingAI {
    long          ZLockTargetSelectionTimer;       // +0x38
    unsigned char _pad3c[0x48 - 0x3c];
    float         MaxTargetingDistance;            // +0x48
    unsigned char _pad4c[0x84 - 0x4c];
    C3DVector     TargetedPos;                     // +0x84
    C3DVector     OldTargetedPos;                  // +0x90
    C3DVector     TargetedPosOffset3D;             // +0x9c
    C3DVector     OldTargetedPosOffset3D;          // +0xa8
    C2DVector     TargetedPosOffset;               // +0xb4
    C2DVector     DeltaTargetedPosOffset;          // +0xbc
    C2DVector     DesiredTargetedPosOffset;        // +0xc4
    C2DVector     TargetedPosOffsetSecondOrder;    // +0xcc
    void OnInitialActivate();
};

void CTCTargetingPlayer::OnInitialActivate()
{
    CTCTargetingAI::OnInitialActivate();
    CTargetingPlayerDef* def = PSubDef;
    MaxTargetingDistance = 0.0f;
    for (CRangeIterator it = def->TargetingRanges.begin(); it != def->TargetingRanges.end(); ++it) {
        if (it->second > MaxTargetingDistance)
            MaxTargetingDistance = it->second;
    }
    MaxTargetingDistance *= 1.2f;
    TargetedPos = C3DVector(0.0f, 0.0f, 0.0f);
    OldTargetedPos = C3DVector(0.0f, 0.0f, 0.0f);
    TargetedPosOffset3D = C3DVector(0.0f, 0.0f, 0.0f);
    OldTargetedPosOffset3D = C3DVector(0.0f, 0.0f, 0.0f);
    TargetedPosOffset = C2DVector(0.0f, 0.0f);
    DeltaTargetedPosOffset = C2DVector(0.0f, 0.0f);
    DesiredTargetedPosOffset = C2DVector(0.0f, 0.0f);
    TargetedPosOffsetSecondOrder = C2DVector(0.0f, 0.0f);
    ZLockTargetSelectionTimer = 0;
}
