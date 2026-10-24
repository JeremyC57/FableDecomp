// std::_Rb_tree_long_COpinionDeedPtr::equal_range(const long& k) -- STLport equal_range on the deed-log map: pair(lower_bound(k), upper_bound(k)) through the hidden result
// Leaf: ?equal_range@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8350, length 83 bytes, thiscall `ret 8` (hidden result + key).
// Single caller 0x006C9233 inside 0x006C9220, which is STLport `_Rb_tree::erase(const key_type&)` (equal_range, then
// distance 0x006C51A0 and erase(first, last) 0x006C7DE0, both landed); that erase is reached from
// CTCHeroOpinionDeedLog::PostDeed (0x006C6CF2), so the tree is OpinionDeedMap, Ego_r.pdb
// `map<long, COpinionDeed*>` (`_Rb_tree<long, pair<const long, COpinionDeed*>, _Select1st, less<long>, allocator>`).
// The manifest label "BinaryTree_FindInsertionPoint" is descriptive and wrong.
// Layout: STLport tree {_M_header (alloc proxy) +0, _M_node_count +4}; node {colour, parent +4, left +8, right +0xC,
// key +0x10 (signed long: jge/jl)}.
// Shape: STLport 4.x verbatim: `return pair<iterator,iterator>(lower_bound(k), upper_bound(k));` -- VC evaluates the
// arguments right to left, so the upper_bound walk (k < key -> left) runs first into edi, the lower_bound walk
// (!(key < k) -> left) second into ecx. The iterator must be the STLport shape (an `_Rb_tree_base_iterator` base
// holding `_M_node`, user copy constructor `{ _M_node = it._M_node; }`): a plain one-member iterator with an
// implicit copy stores pair.second before pair.first.
namespace std {

struct _Rb_tree_long_COpinionDeedPtr {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; long key; void* value; };
    struct _Rb_tree_base_iterator { node* _M_node; };
    struct iterator : _Rb_tree_base_iterator {
        iterator(node* n) { _M_node = n; }
        iterator(const iterator& it) { _M_node = it._M_node; }
    };
    struct iterator_pair {                     // pair<iterator, iterator>
        iterator first;
        iterator second;
        iterator_pair(const iterator& a, const iterator& b) : first(a), second(b) { }
    };
    node*         head;                        // +0x00 _M_header._M_data
    unsigned long size;                        // +0x04 _M_node_count
    node* _M_lower_bound(const long& k) const
    {
        node* y = head;                        // last node which is not less than k
        node* x = head->parent;                // root
        while (x != 0)
            if (!(x->key < k))
                y = x, x = x->left;
            else
                x = x->right;
        return y;
    }
    node* _M_upper_bound(const long& k) const
    {
        node* y = head;                        // last node which is greater than k
        node* x = head->parent;
        while (x != 0)
            if (k < x->key)
                y = x, x = x->left;
            else
                x = x->right;
        return y;
    }
    iterator lower_bound(const long& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const long& k) { return iterator(_M_upper_bound(k)); }
    iterator_pair equal_range(const long& k);
};

_Rb_tree_long_COpinionDeedPtr::iterator_pair _Rb_tree_long_COpinionDeedPtr::equal_range(const long& k)
{
    return iterator_pair(lower_bound(k), upper_bound(k));
}

}