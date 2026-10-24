// GetTargetingRange(const CTargetingDef& def, ETargetingType type) -- the def's targeting range for one targeting type (its TargetingRanges map entry), 0 when the type has no entry
// Leaf: ?GetTargetingRange@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802890, 56 bytes (+ int3 padding), __fastcall (def in ecx, type in edx,
//   `ret`), float result on the x87 stack. Callers: GetTargetingRangeForTargetType 0x008028D0 (three calls, ecx = def,
//   edx = the ETargetingType immediate).
// Name: Ego_r.pdb `?GetTargetingRange@?A0x58ed6966@@YIMABVCTargetingDef@@W4ETargetingType@@@Z` -- a file-static
//   (anonymous-namespace) `__fastcall` wrapper over the inline `CTargetingDef::GetTargetingRange(ETargetingType) const`
//   (FableWin.pdb names both). Declared at global scope here: VC7.1 does not emit an unreferenced anonymous-namespace
//   function (GOTCHAS: file-static helpers).
// Callees: STLport `_Rb_tree<ETargetingType, pair<const ETargetingType, float>, ...>::find(const key&) const` 0x007F9830
//   (__fastcall on the map, iterator returned through a hidden pointer, `ret 8`). Constant: 0.0f @ 0x0122DEDC.
// Layout (Ego_r.pdb CTargetingDef): TargetingRanges map<ETargetingType, float> at +0x74 (retail STLport map: the header
//   node pointer first, node value at +0x10 key / +0x14 float).
// Shape: the key parameter is homed in a frame word (its address goes to find), the iterator result in the other one;
//   `it != end()` with end() read after the call, the found arm first, the 0.0f literal from the pool.
enum ETargetingType { TARGETING_TYPE_ANY };

struct SRangeNode {                                  // STLport _Rb_tree_node<pair<const ETargetingType, float> >
    long        _M_color;
    SRangeNode* _M_parent;                           // +0x04
    SRangeNode* _M_left;                             // +0x08
    SRangeNode* _M_right;                            // +0x0c
    long        first;                               // +0x10
    float       second;                              // +0x14
};

struct CRangeConstIterator {
    SRangeNode* _M_node;
    CRangeConstIterator(SRangeNode* node) : _M_node(node) {}
    bool operator!=(const CRangeConstIterator& other) const { return _M_node != other._M_node; }
    const SRangeNode* operator->() const { return _M_node; }
};

struct CTargetingRangeMap {                          // STLport map<ETargetingType, float> (12 bytes in retail)
    SRangeNode*   _M_header;                         // +0x00
    unsigned long _M_node_count;                     // +0x04
    unsigned long _M_key_compare;                    // +0x08 (empty less<>, padded)
    CRangeConstIterator find(const ETargetingType& key) const;   // 0x007F9830
    CRangeConstIterator end() const { return CRangeConstIterator(_M_header); }
};

struct CTargetingDef {
    unsigned char      _pad00[0x74];
    CTargetingRangeMap TargetingRanges;              // +0x74
    float GetTargetingRange(ETargetingType type) const
    {
        CRangeConstIterator it = TargetingRanges.find(type);
        if (it != TargetingRanges.end())
            return it->second;
        return 0.0f;
    }
};

float __fastcall GetTargetingRange(const CTargetingDef& def, ETargetingType type)
{
    return def.GetTargetingRange(type);
}