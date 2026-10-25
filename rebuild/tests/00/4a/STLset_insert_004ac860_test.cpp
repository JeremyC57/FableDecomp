// Behaviour test for 0x004AC860: set<CCharString>::insert converts insert_unique's (tree iterator, bool) result into the set's (const iterator, bool)
#include <cstdio>

struct CCharString { const char* Text; };               // stand-in key (compared by text pointer here)
struct TestNode { CCharString Value; };

struct TestTreeIterator { TestNode* _M_node; };                      // _Rep_type::iterator
struct TestConstIterator {                                           // the set's iterator (const traits)
    TestNode* _M_node;
    TestConstIterator() : _M_node(0) { }
    TestConstIterator(const TestTreeIterator& it) : _M_node(it._M_node) { }
};

template <class _T1, class _T2> struct TestPair {
    _T1 first;
    _T2 second;
    TestPair(const _T1& a, const _T2& b) : first(a), second(b) { }
    template <class _U1, class _U2> TestPair(const TestPair<_U1, _U2>& p) : first(p.first), second(p.second) { }
};

class TestTree {                                     // insert_unique stand-in over a tiny array
public:
    TestNode nodes[8];
    int count;
    TestTree() : count(0) { }
    TestPair<TestTreeIterator, bool> insert_unique(const CCharString& x)
    {
        TestTreeIterator it;
        for (int i = 0; i < count; ++i)
            if (nodes[i].Value.Text == x.Text) { it._M_node = &nodes[i]; return TestPair<TestTreeIterator, bool>(it, false); }
        nodes[count].Value = x;
        it._M_node = &nodes[count++];
        return TestPair<TestTreeIterator, bool>(it, true);
    }
};

class TestSet {
public:
    TestTree _M_t;
    TestPair<TestConstIterator, bool> insert(const CCharString& x)          // the reconstructed body
    {
        TestPair<TestTreeIterator, bool> p = _M_t.insert_unique(x);
        return TestPair<TestConstIterator, bool>(p.first, p.second);
    }
};

int main()
{
    static const char hello[] = "hello";
    static const char world[] = "world";
    CCharString k5 = { hello };
    CCharString k9 = { world };
    TestSet s;
    TestPair<TestConstIterator, bool> a = s.insert(k5);
    TestPair<TestConstIterator, bool> b = s.insert(k9);
    TestPair<TestConstIterator, bool> c = s.insert(k5);
    if (!a.second || !b.second || c.second || c.first._M_node != a.first._M_node || a.first._M_node->Value.Text != hello
        || b.first._M_node->Value.Text != world || s._M_t.count != 2) {
        std::printf("004ac860_TEST FAIL\n");
        return 1;
    }
    std::printf("004ac860_TEST PASS\n");
    return 0;
}