// NGraphicAppearance::CTCGraphicAppearance::GetTintColour() const -- the appearance's tint: opaque white with no tints, else the channel-wise average of every registered tint colour
// Leaf: ?GetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QBE?BVCRGBColour@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BD040, 287 bytes, thiscall `ret 4` (hidden CRGBColour result); speed TU (esp frame).
// Identity (proven): (1) Ego_r.pdb / FableWin.pdb name ?GetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QBE?BVCRGBColour@@XZ
//   (const, returns a const CRGBColour by value) beside Set/RemoveTintColour(..., void* owner); (2) the body walks Ego_r
//   TintColours (map<void*, CRGBColour>, retail +0x18 after the 12-byte STLport MeshEffects map at +0x0C) summing the
//   CRGBColour channels (R +2, G +1, B +0, A +3 of the node value at +0x14) and divides by the node count (+0x1C);
//   (3) its caller CTCGraphicAppearanceStaticMesh::OnAppearanceDraw 0x0077BA40 builds the result in its argument slot
//   (hand470 NOTES name it GetTintColour).
// Label: the catalog's "Appearance_FloatToCoord" is wrong.
// Callees: _ftol 0x00BFEA70 (x4, the unsigned char channel conversions). Literals 0.0f / 1.0f / 2^32 (unsigned count fix-up).
// Shape: STLport map iteration with _Rb_global<bool>::_M_increment inline (its _tree.c body is visible in this TU); the
//   four float sums live on the x87 stack, declared A, B, G, R (retail's load order); each channel is `sum / count` with
//   a float count local, which VC7.1 turns into one stored reciprocal (`fld 1.0 ; fdiv st(1) ; fstp`) reloaded per channel
//   -- an explicit `scale = 1.0f / count` keeps the reciprocal in a register (`fdivr`) instead.
// Layout: CTCGraphicAppearance has no generated engine header; local model (Ego_r member names, retail offsets).
class CRGBColour {                                      // Ego_r.pdb, 4 bytes (anonymous union)
public:
    union {
        struct {
            unsigned char B;                            // +0x00
            unsigned char G;                            // +0x01
            unsigned char R;                            // +0x02
            unsigned char A;                            // +0x03
        };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node);
};
// STLport 4.6 _tree.c (out of line elsewhere at 0x004292C0; inlined here).
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                             // STLport 4.6, 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned long       _M_node_count;                  // +0x04
    _Compare            _M_key_compare;                 // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    bool empty() const { return _M_node_count == 0; }
    unsigned long size() const { return _M_node_count; }
};
}

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x18];                          // CTCBase (vptr, ParentThing, flags) + MeshEffects map +0x0C
    std::map<void*, CRGBColour> TintColours;            // +0x18 (Ego_r +0x1C)
    const CRGBColour GetTintColour() const;
};

const CRGBColour CTCGraphicAppearance::GetTintColour() const
{
    if (TintColours.empty())
        return CRGBColour(255, 255, 255, 255);
    float a = 0.0f;
    float b = 0.0f;
    float g = 0.0f;
    float r = 0.0f;
    for (std::map<void*, CRGBColour>::const_iterator it = TintColours.begin(); it != TintColours.end(); ++it) {
        r += it->second.R;
        g += it->second.G;
        b += it->second.B;
        a += it->second.A;
    }
    float count = (float)TintColours.size();
    return CRGBColour((unsigned char)(r / count), (unsigned char)(g / count), (unsigned char)(b / count), (unsigned char)(a / count));
}
}