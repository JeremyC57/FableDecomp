// Behaviour test for 0x00531B40 NUISystem::CComponent::PositionChildDying: finds the dying child in the position-children set by a linear in-order walk and erases that node (rebalance, free, count - 1); an absent child changes nothing
#include <cstdio>
#include <cstdlib>
class CComponent;
struct SSetNode {
    long        _M_color;
    SSetNode*   _M_parent;
    SSetNode*   _M_left;
    SSetNode*   _M_right;
    CComponent* _M_value_field;
};
struct SSetIterator {
    SSetNode* _M_node;
    SSetIterator() { }
    SSetIterator(SSetNode* node) : _M_node(node) { }
};
struct SInputIteratorTag { SInputIteratorTag() { } };
static SSetNode* g_rebalanced = 0;
SSetNode* Rebalance_for_erase(SSetNode* z, SSetNode*& root, SSetNode*& leftmost, SSetNode*& rightmost)
{
    g_rebalanced = z;                       // stand-in: the real rebalance unlinks z and returns it
    (void)root; (void)leftmost; (void)rightmost;
    return z;
}
SSetIterator Find(SSetIterator first, SSetIterator last, CComponent* const& value, const SInputIteratorTag&)
{
    SSetNode* node = first._M_node;
    while (node != last._M_node && node->_M_value_field != value) {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SSetNode* parent = node->_M_parent;
            while (node == parent->_M_right) {
                node = parent;
                parent = parent->_M_parent;
            }
            if (node->_M_right != parent)
                node = parent;
        }
    }
    return SSetIterator(node);
}
struct CComponentSet {
    SSetNode*     _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    SSetIterator begin() const { return SSetIterator(_M_header->_M_left); }
    SSetIterator end() const { return SSetIterator(_M_header); }
    void erase(SSetIterator position)
    {
        SSetNode* erased = Rebalance_for_erase(position._M_node, _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        if (erased)
            free(erased);
        --_M_node_count;
    }
};
class CComponent {
public:
    virtual void PositionChildDying(CComponent* child);
    virtual ~CComponent() { }
    CComponentSet PositionChildren;
};
void CComponent::PositionChildDying(CComponent* child)
{
    SSetIterator it = Find(PositionChildren.begin(), PositionChildren.end(), child, SInputIteratorTag());
    if (it._M_node != PositionChildren._M_header)
        PositionChildren.erase(it);
}
static SSetNode* NewNode(CComponent* value)
{
    SSetNode* n = (SSetNode*)malloc(sizeof(SSetNode));
    n->_M_color = 0; n->_M_parent = n->_M_left = n->_M_right = 0; n->_M_value_field = value;
    return n;
}
int main()
{
    CComponent owner, a, b, c, stranger;
    SSetNode header;
    SSetNode* na = NewNode(&a);
    SSetNode* nb = NewNode(&b);
    SSetNode* nc = NewNode(&c);
    nb->_M_left = na; nb->_M_right = nc; na->_M_parent = nb; nc->_M_parent = nb; nb->_M_parent = &header;
    header._M_parent = nb; header._M_left = na; header._M_right = nc; header._M_value_field = 0;
    owner.PositionChildren._M_header = &header;
    owner.PositionChildren._M_node_count = 3;
    owner.PositionChildDying(&stranger);
    if (g_rebalanced != 0 || owner.PositionChildren._M_node_count != 3) { std::printf("00531b40_TEST FAIL\n"); return 1; }
    owner.PositionChildDying(&c);           // the last in order: reached through the successor walk
    if (g_rebalanced != nc || owner.PositionChildren._M_node_count != 2) { std::printf("00531b40_TEST FAIL\n"); return 1; }
    nb->_M_right = 0; header._M_right = nb;
    owner.PositionChildDying(&b);
    if (g_rebalanced != nb || owner.PositionChildren._M_node_count != 1) { std::printf("00531b40_TEST FAIL\n"); return 1; }
    free(na);
    std::printf("00531b40_TEST PASS\n");
    return 0;
}