// Behaviour test for 0x00800020 CTCHaste::UpdateActionSpeedMultiplier: product of all map values, 1.0 when empty.
#include <cstdio>
struct SMultiplierNode { long _M_color; SMultiplierNode* _M_parent; SMultiplierNode* _M_left; SMultiplierNode* _M_right; void* first; float second; };
struct CMultiplierIterator {
    SMultiplierNode* _M_node;
    CMultiplierIterator(SMultiplierNode* node) : _M_node(node) {}
    bool operator!=(const CMultiplierIterator& other) const { return _M_node != other._M_node; }
    SMultiplierNode* operator->() const { return _M_node; }
    static SMultiplierNode* _M_increment(SMultiplierNode* node)
    {
        if (node->_M_right != 0) { node = node->_M_right; while (node->_M_left != 0) node = node->_M_left; }
        else { SMultiplierNode* y = node->_M_parent; while (node == y->_M_right) { node = y; y = y->_M_parent; } if (node->_M_right != y) node = y; }
        return node;
    }
    CMultiplierIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
};
struct CMultiplierMap {
    SMultiplierNode* _M_header;
    CMultiplierIterator begin() { return CMultiplierIterator(_M_header->_M_left); }
    CMultiplierIterator end() { return CMultiplierIterator(_M_header); }
};
struct CTCHaste {
    float Multiplier;
    CMultiplierMap Multipliers;
    void UpdateActionSpeedMultiplier()
    {
        Multiplier = 1.0f;
        for (CMultiplierIterator it = Multipliers.begin(); it != Multipliers.end(); ++it)
            Multiplier *= it->second;
    }
};
int main()
{
    SMultiplierNode header;            // empty tree: header's left/right point at itself, root null
    header._M_parent = 0; header._M_left = &header; header._M_right = &header;
    CTCHaste haste; haste.Multipliers._M_header = &header; haste.Multiplier = 7.0f;
    haste.UpdateActionSpeedMultiplier();
    if (haste.Multiplier != 1.0f) { std::printf("00800020_TEST FAIL\n"); return 1; }
    // three nodes: root b(2.0) with children a(0.5) and c(3.0) (STLport header: parent=root, left=min, right=max)
    SMultiplierNode a, b, c;
    header._M_parent = &b; header._M_left = &a; header._M_right = &c;
    b._M_parent = &header; b._M_left = &a; b._M_right = &c; b.second = 2.0f;
    a._M_parent = &b; a._M_left = 0; a._M_right = 0; a.second = 0.5f;
    c._M_parent = &b; c._M_left = 0; c._M_right = 0; c.second = 3.0f;
    haste.UpdateActionSpeedMultiplier();
    if (haste.Multiplier != 3.0f) { std::printf("00800020_TEST FAIL\n"); return 1; }
    std::printf("00800020_TEST PASS\n");
    return 0;
}