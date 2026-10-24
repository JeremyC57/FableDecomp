// CTCIdleScheduler::Clear() -- forget the preferred idle group and the last-checked frame and empty both idle maps (state groups and their weights)
// Leaf: ?Clear@CTCIdleScheduler@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008018B0, 97 bytes (+ int3 padding), __fastcall, plain `ret`.
//   Callers: the CTCIdleScheduler(CThing&) ctor 0x004E1537 (vtable 0x012426C4, after building both maps) and
//   CThingAICreature::SetNewBrain 0x00833010 (on the interface-0xD3 component).
// Name: Ego_r.pdb `?Clear@CTCIdleScheduler@@QAEXXZ` (donor 0x00805C30, first after the class's inline members; the
//   landed ChooseNewIdle 0x00801A50 / IsPreferredIdle 0x00801BE0 follow it in retail); the catalog's
//   "LTextBinTree_RebuildWalkerArray" is descriptive.
// Callees: STLport `_Rb_tree::_M_erase(node)` of the two maps -- 0x004DA468 (map<EIdleStateGroup, CAIStateGroupBase*>)
//   and 0x004DA43B (map<EIdleStateGroup, long>), both in the /Os CTCIdleScheduler ctor TU (manifest "BuildTreeArray" is
//   propagated).
// Layout = Ego_r.pdb CTCIdleScheduler with retail 12-byte STLport maps: IdleStates +0x0c, IdleStatesWeight +0x18,
//   Preferred +0x24, FrameLastChecked +0x28 (PDB +0x2c/+0x30). Local model: no engine header for this class.
// Shape: both scalar zeroes first (one shared zero register), then STLport `clear()` inlined per map:
//   `if (_M_node_count != 0) { _M_erase(_M_root()); _M_leftmost() = header; _M_root() = 0; _M_rightmost() = header;
//   _M_node_count = 0; }`, the header pointer re-read for every store.
struct SIdleNode {                               // STLport _Rb_tree_node_base + value
    long       _M_color;
    SIdleNode* _M_parent;                        // +0x04 (root, in the header)
    SIdleNode* _M_left;                          // +0x08 (leftmost)
    SIdleNode* _M_right;                         // +0x0c (rightmost)
};

struct CIdleStateMap {                           // STLport map<EIdleStateGroup, CAIStateGroupBase*> (12 bytes)
    SIdleNode*    _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    void _M_erase(SIdleNode* x);                 // 0x004DA468
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

struct CIdleWeightMap {                          // STLport map<EIdleStateGroup, long> (12 bytes)
    SIdleNode*    _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    void _M_erase(SIdleNode* x);                 // 0x004DA43B
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

enum EIdleStateGroup { IDLE_STATE_GROUP_NONE };

class CTCIdleScheduler {
public:
    void*           __vftable;                   // +0x00
    void*           ParentThing;                 // +0x04 (CTCBase)
    unsigned char   _pad08[4];                   // +0x08 CTCBase flags
    CIdleStateMap   IdleStates;                  // +0x0c
    CIdleWeightMap  IdleStatesWeight;            // +0x18
    EIdleStateGroup Preferred;                   // +0x24
    long            FrameLastChecked;            // +0x28
    void Clear();
};

void CTCIdleScheduler::Clear()
{
    FrameLastChecked = 0;
    Preferred = IDLE_STATE_GROUP_NONE;
    IdleStates.clear();
    IdleStatesWeight.clear();
}