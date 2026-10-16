// CThingPlayerCreature::OnRegionChange() -- zero the movement acceleration, the relative movement acceleration and its last value, and clear the relative-acceleration component map
// Leaf: OnRegionChange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AA100, length 103 bytes.
// Label correction: the catalog row says "CTCHeroOnlineScoreboard::Close" (a 0x88-byte class); the body works on a CThing
// subclass at +0x180..+0x1c8 and its only caller (0x004A4DDF, CWorld region-change code) calls it on a thing. The members
// match Ego_r.pdb CThingPlayerCreature (MovementAcceleration +0x1a8, RelativeMovementAccelerationComponents +0x1d0,
// RelativeMovementAcceleration +0x1e0, LastRelativeMovementAcceleration +0x1ec), 0x28 lower in retail (map 12 bytes, so the
// later vectors 0x2c lower). The PDB's argument-less CThingPlayerCreature member called from region handling is
// ?OnRegionChange@CThingPlayerCreature@@QAEXXZ (name inferred from the caller; not byte-proven).
// Callees: STLport _Rb_tree<EGameAction, pair<const EGameAction, pair<C3DVector,long>>, ...>::_M_erase(_Rb_tree_node*) @ 0x00527910
// (manifest "LTextTreeWalkThrough::BuildTreeArray", propagated), the inline map::clear() around it.
// Shape: each vector is zeroed with a chained `X = Y = Z = 0.0f` (stores from the highest address down; the chained float
// zero goes through one shared integer zero register);
// clear() tests the node count, erases from the root, then relinks the header (left, root = 0, right) and zeroes the count.
// Layout: MovementAcceleration +0x180, RelativeMovementAccelerationComponents +0x1a8, RelativeMovementAcceleration +0x1b4,
// LastRelativeMovementAcceleration +0x1c0.
struct C3DVector {
    float X, Y, Z;
    void Zero() { X = Y = Z = 0.0f; }
};
struct SRbNode {
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
};
struct CAccelerationComponentMap {              // STLport map<EGameAction, pair<C3DVector,long>> (12 bytes)
    SRbNode*     _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    void _M_erase(SRbNode* x);                  // 0x00527910
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
class CThingPlayerCreature {
public:
    virtual ~CThingPlayerCreature();
    unsigned char _pad04[0x180 - 0x04];
    C3DVector     MovementAcceleration;         // +0x180
    unsigned char _pad18c[0x1a8 - 0x18c];
    CAccelerationComponentMap RelativeMovementAccelerationComponents;   // +0x1a8
    C3DVector     RelativeMovementAcceleration;         // +0x1b4
    C3DVector     LastRelativeMovementAcceleration;     // +0x1c0
    void OnRegionChange();
};

void CThingPlayerCreature::OnRegionChange()
{
    MovementAcceleration.Zero();
    RelativeMovementAcceleration.Zero();
    LastRelativeMovementAcceleration.Zero();
    RelativeMovementAccelerationComponents.clear();
}