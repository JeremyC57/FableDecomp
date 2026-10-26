// Behaviour test for 0x00802890 GetTargetingRange(const CTargetingDef&, ETargetingType): map lookup, 0 when missing.
#include <cstdio>
enum ETargetingType { TT_0, TT_1, TT_2, TT_3, TT_8 = 8 };
struct SRangeNode { long _M_color; SRangeNode* _M_parent; SRangeNode* _M_left; SRangeNode* _M_right; long first; float second; };
struct CRangeConstIterator {
    SRangeNode* _M_node;
    CRangeConstIterator(SRangeNode* node) : _M_node(node) {}
    bool operator!=(const CRangeConstIterator& other) const { return _M_node != other._M_node; }
    const SRangeNode* operator->() const { return _M_node; }
};
struct CTargetingRangeMap {
    SRangeNode* _M_header;
    CRangeConstIterator find(const ETargetingType& key) const   // STLport lower_bound + check (the 0x007F9830 shape)
    {
        SRangeNode* y = _M_header; SRangeNode* x = _M_header->_M_parent;
        while (x) { if (!(x->first < key)) { y = x; x = x->_M_left; } else x = x->_M_right; }
        if (y == _M_header || key < y->first) return CRangeConstIterator(_M_header);
        return CRangeConstIterator(y);
    }
    CRangeConstIterator end() const { return CRangeConstIterator(_M_header); }
};
class CTargetingDef { public:
    CTargetingRangeMap TargetingRanges;
    float GetTargetingRange(ETargetingType type) const
    {
        CRangeConstIterator it = TargetingRanges.find(type);
        if (it != TargetingRanges.end())
            return it->second;
        return 0.0f;
    }
};
float GetTargetingRange(const CTargetingDef& def, ETargetingType type) { return def.GetTargetingRange(type); }
int main()
{
    SRangeNode header, n1, n3, n8;
    header._M_parent = &n3; header._M_left = &n1; header._M_right = &n8;
    n3._M_parent = &header; n3._M_left = &n1; n3._M_right = &n8; n3.first = 3; n3.second = 30.0f;
    n1._M_parent = &n3; n1._M_left = 0; n1._M_right = 0; n1.first = 1; n1.second = 10.0f;
    n8._M_parent = &n3; n8._M_left = 0; n8._M_right = 0; n8.first = 8; n8.second = 80.0f;
    CTargetingDef def; def.TargetingRanges._M_header = &header;
    if (GetTargetingRange(def, TT_1) != 10.0f || GetTargetingRange(def, TT_3) != 30.0f || GetTargetingRange(def, TT_8) != 80.0f
        || GetTargetingRange(def, TT_2) != 0.0f || GetTargetingRange(def, TT_0) != 0.0f) {
        std::printf("00802890_TEST FAIL\n"); return 1;
    }
    std::printf("00802890_TEST PASS\n");
    return 0;
}