// CTCHaste::UpdateActionSpeedMultiplier() -- recompute the combined action-speed multiplier: 1.0 times every source's multiplier in the map
// Leaf: UpdateActionSpeedMultiplier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800020, 90 bytes (+ int3 padding), __fastcall, plain `ret`. Callers:
//   the landed CTCHaste::RemoveMultiplier 0x0058148D (which names this body "RecalculateMultiplier", descriptive),
//   CCreatureActionBase::FrameUpdate / FinishAction and several creature actions (18 references in all).
// Name: Ego_r.pdb `?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ` follows FrameUpdate in the donor CTCHaste TU, as
//   0x00800020 follows the landed CTCHaste::FrameUpdate 0x00800000 in retail; the catalog's
//   "CAnimation_RecalculateScale" is descriptive.
// Callees: none (STLport map<void*, float> walk inlined). Layout = Ego_r.pdb CTCHaste (the landed 0x00800000 /
//   0x0058148D model): Multiplier +0x0c, Multipliers map<void*, float> +0x18 (retail STLport header node pointer first;
//   node value +0x10 key / +0x14 float). No engine header exists for CTCHaste.
// Shape: `Multiplier = 1.0f` stored as an immediate, then a `for` over begin()..end() with end() re-read from the member
//   every pass (the float store may alias it); `Multiplier *= it->second` is `fld [node+14h] ; fmul [this+0Ch]`; the
//   iterator increment is STLport's static `_M_increment(node)` inlined in place (right subtree's leftmost, else climb).
struct SMultiplierNode {                         // STLport _Rb_tree_node<pair<void* const, float> >
    long             _M_color;
    SMultiplierNode* _M_parent;                  // +0x04
    SMultiplierNode* _M_left;                    // +0x08
    SMultiplierNode* _M_right;                   // +0x0c
    void*            first;                      // +0x10
    float            second;                     // +0x14
};

struct CMultiplierIterator {
    SMultiplierNode* _M_node;
    CMultiplierIterator(SMultiplierNode* node) : _M_node(node) {}
    bool operator!=(const CMultiplierIterator& other) const { return _M_node != other._M_node; }
    SMultiplierNode* operator->() const { return _M_node; }
    static SMultiplierNode* _M_increment(SMultiplierNode* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SMultiplierNode* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
    CMultiplierIterator& operator++()
    {
        _M_node = _M_increment(_M_node);
        return *this;
    }
};

struct CMultiplierMap {                          // STLport map<void*, float>
    SMultiplierNode* _M_header;
    CMultiplierIterator begin() { return CMultiplierIterator(_M_header->_M_left); }
    CMultiplierIterator end() { return CMultiplierIterator(_M_header); }
};

class CTCHaste {
public:
    void*          __vftable;                    // +0x00
    void*          ParentThing;                  // +0x04 (CTCBase)
    unsigned char  _pad08[4];                    // +0x08 CTCBase flags
    float          Multiplier;                   // +0x0c
    bool           ToTerminate;                  // +0x10
    long           PauseFrames;                  // +0x14
    CMultiplierMap Multipliers;                  // +0x18
private:
    void UpdateActionSpeedMultiplier();
};

void CTCHaste::UpdateActionSpeedMultiplier()
{
    Multiplier = 1.0f;
    for (CMultiplierIterator it = Multipliers.begin(); it != Multipliers.end(); ++it)
        Multiplier *= it->second;
}