// map<void*, CRGBColour>::operator[](void* const& key) -- find the tint colour registered under the key, inserting a default colour when absent
// Grade: DIFFER(ours v retail) -- 130v132: retail sinks the B/G byte stores (bl/bh) past the hidden-return push and re-reads the returned iterator from its slot ([esp+0x18]) instead of [eax]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5890, 132 bytes (manifest "CombatAnimationList::FindOrInsert", propagated).
// Only caller NGraphicAppearance::CTCGraphicAppearance::SetTintColour @ 0x004C00B0 on TintColours (+0x18, Ego_r.pdb
// map<void*,CRGBColour>). STLport operator[]: inline lower_bound over the header-pointer tree (root header+4, left +8,
// right +0xC, key node+0x10, value node+0x14), then insert(hint, value_type) @ 0x004C4D50 returning through a hidden pointer.
// CRGBColour (4 bytes B,G,R,A) has a no-op default ctor, so the T() temporary is the dead key argument slot.
// No engine header models map<void*, CRGBColour>: local model.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct STintColourNode {                    // STLport _Rb_tree_node<pair<void* const, CRGBColour> >
    bool             _M_color;              // +0x00
    STintColourNode* _M_parent;             // +0x04
    STintColourNode* _M_left;               // +0x08
    STintColourNode* _M_right;              // +0x0C
    void*            first;                 // +0x10
    CRGBColour       second;                // +0x14
};

struct STintColourPair {                    // pair<void* const, CRGBColour>
    void*      first;
    CRGBColour second;
    STintColourPair(void* const& a, const CRGBColour& b) : first(a), second(b) { }
};

struct CRbTreeBaseIterator {                // STLport _Rb_tree_base_iterator
    STintColourNode* _M_node;
};

struct CTintColourMapIterator : CRbTreeBaseIterator {   // STLport _Rb_tree_iterator
    CTintColourMapIterator() { }
    CTintColourMapIterator(STintColourNode* n) { _M_node = n; }
    CTintColourMapIterator(const CTintColourMapIterator& o) { _M_node = o._M_node; }
    STintColourPair& operator*() const { return *(STintColourPair*)&_M_node->first; }
};

struct CTintColourMap {                     // STLport map<void*, CRGBColour>, 12 bytes retail
    STintColourNode* _M_header;             // +0x00
    unsigned long    _M_node_count;         // +0x04
    unsigned char    _M_key_compare;        // +0x08

    CTintColourMapIterator insert(CTintColourMapIterator hint, const STintColourPair& v);   // 0x004C4D50
    CRGBColour& operator[](void* const& key);
};

CRGBColour& CTintColourMap::operator[](void* const& key)
{
    STintColourNode* y = _M_header;
    STintColourNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->first < key)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    }
    CTintColourMapIterator i(y);
    if (i._M_node == _M_header || key < (*i).first)
        i = insert(i, STintColourPair(key, CRGBColour()));
    return (*i).second;
}
