// Behaviour test for the IsSameNavQuadTreeNode __find_if (0x00A16550): over every range length 0..9 and every
// position, the first element equal (by CANavQuadTreeNode::operator==) to the target is found, else last.
#include <cstdio>

class CANavQuadTreeNode {
public:
    int Key;
    bool operator==(const CANavQuadTreeNode& other) const { return Key == other.Key; }
};

class IsSameNavQuadTreeNode {
public:
    const CANavQuadTreeNode* Node;
    IsSameNavQuadTreeNode(const CANavQuadTreeNode* node) : Node(node) {}
    bool operator()(const CANavQuadTreeNode* node) const { return *Node == *node; }
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred, const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
}

int main()
{
    int ok = 1;
    CANavQuadTreeNode nodes[10];
    const CANavQuadTreeNode* v[10];
    for (int i = 0; i < 10; ++i) { nodes[i].Key = 100 + (i % 7); v[i] = &nodes[i]; }   // keys repeat after 7
    _STL::random_access_iterator_tag tag;
    for (int n = 0; n <= 9; ++n) {
        for (int key = 99; key <= 107; ++key) {
            CANavQuadTreeNode target; target.Key = key;
            const CANavQuadTreeNode** r = _STL::__find_if(v, v + n, IsSameNavQuadTreeNode(&target), tag);
            int expect = n;
            for (int i = 0; i < n; ++i) if (nodes[i].Key == key) { expect = i; break; }
            if (r != v + expect) ok = 0;
        }
    }

    if (!ok) { std::printf("00a16550_TEST FAIL\n"); return 1; }
    std::printf("00a16550_TEST PASS\n");
    return 0;
}