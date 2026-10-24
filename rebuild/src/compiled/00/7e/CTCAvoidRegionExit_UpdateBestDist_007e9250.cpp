// CTCAvoidRegionExit::UpdateBestDist() -- recompute the static best distance as the largest distance in the static per-thing distance map (0 when the map is empty)
// Leaf: ?UpdateBestDist@CTCAvoidRegionExit@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E9250, 124 bytes (+ int3 padding). FableWin.pdb / Ego_r.pdb
//   `?UpdateBestDist@CTCAvoidRegionExit@@QAEXXZ` (public thiscall, `this` unused); called after the map erase by the
//   landed OnSetInLimbo 0x007E9360, vfunc_2 0x007E93C0 and OnKill 0x007E9410 (which call it UpdateMaxExitWeight).
//   Catalog label `TreeWalk_FindMaxValue` was an autoname guess.
// Data: CTCAvoidRegionExit::DistMap (static STLport map<CThing*, float> @ 0x013BAF00: header-node pointer + count;
//   node colour +0, parent +4, left +8, right +0xc, key +0x10, distance +0x14) and the static best distance float
//   @ 0x013BAE28 (name not in the PDB lists on disk).
// Shape: begin()..end() with STLport's iterator increment inlined (right subtree's leftmost, else climb while we are the
//   parent's right child, then the header guard `if (node->right != y) node = y`) as a by-value static helper whose
//   result is stored back (`mov eax,edx` .. `mov edx,eax`: the member form works on the node in place); `if (d > best) best = d` is
//   `fcomp ; test ah,41h ; jne` and the float copy goes through eax.
class CThing;

struct SRbNodeBase {
    char         _M_color;
    SRbNodeBase* _M_parent;                                     // +0x04
    SRbNodeBase* _M_left;                                       // +0x08
    SRbNodeBase* _M_right;                                      // +0x0c
};
struct SDistNode : SRbNodeBase {
    CThing* first;                                              // +0x10
    float   second;                                             // +0x14
};

struct CDistMapIterator {
    SRbNodeBase* _M_node;
    CDistMapIterator(SRbNodeBase* n) : _M_node(n) { }
    SDistNode* operator->() const { return (SDistNode*)_M_node; }
    bool operator!=(const CDistMapIterator& other) const { return _M_node != other._M_node; }
    static SRbNodeBase* _M_increment(SRbNodeBase* node)       // STLport _Rb_global<bool>::_M_increment
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SRbNodeBase* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
    CDistMapIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
};

struct CDistMap {                                               // STLport map<CThing*, float>
    SRbNodeBase*  _M_header;
    unsigned long _M_node_count;
    CDistMapIterator begin() { return CDistMapIterator(_M_header->_M_left); }
    CDistMapIterator end() { return CDistMapIterator(_M_header); }
};

class CTCAvoidRegionExit {
public:
    static CDistMap DistMap;                                    // 0x013BAF00
    static float    BestDist;                                   // 0x013BAE28
    void UpdateBestDist();
};

void CTCAvoidRegionExit::UpdateBestDist()
{
    BestDist = 0.0f;
    for (CDistMapIterator it = DistMap.begin(); it != DistMap.end(); ++it) {
        if (it->second > BestDist)
            BestDist = it->second;
    }
}