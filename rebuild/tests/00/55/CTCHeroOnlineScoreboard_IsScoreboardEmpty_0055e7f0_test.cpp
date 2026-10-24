// Behaviour test for 0055e7f0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)   // STLport _Rb_global<bool>::_M_increment
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Rb_tree_node_base* y = _M_node->_M_parent;
        while (_M_node == y->_M_right) {
            _M_node = y;
            y = y->_M_parent;
        }
        if (_M_node->_M_right != y)
            _M_node = y;
    }
    return _M_node;
}
// build a balanced tree over nodes[0..n) (in key order) under header; returns the root
static _Rb_tree_node_base* BuildRange(_Rb_tree_node_base** nodes, int lo, int hi, _Rb_tree_node_base* parent)
{
    if (lo >= hi) return 0;
    int mid = (lo + hi) / 2;
    _Rb_tree_node_base* n = nodes[mid];
    n->_M_color = false;
    n->_M_parent = parent;
    n->_M_left = BuildRange(nodes, lo, mid, n);
    n->_M_right = BuildRange(nodes, mid + 1, hi, n);
    return n;
}
static void BuildTree(_Rb_tree_node_base* header, _Rb_tree_node_base** nodes, int n)
{
    header->_M_color = true;
    header->_M_parent = BuildRange(nodes, 0, n, header);
    header->_M_left = n ? nodes[0] : header;
    header->_M_right = n ? nodes[n - 1] : header;
}

struct SScoreEntry { float first; void* second; };   // pair<float, CWideString>
struct CScoreVector {
    SScoreEntry* _M_start;
    SScoreEntry* _M_finish;
    SScoreEntry* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
enum EOnlineScoreboardActivity { ACTIVITY_NONE };
struct SScoreboardPair {
    EOnlineScoreboardActivity first;
    CScoreVector second;
};
struct SScoreboardNode : _Rb_tree_node_base {
    SScoreboardPair _M_value_field;
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    SScoreboardPair* operator->() const { return &((SScoreboardNode*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};
struct CScoreboardMap {
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long _M_node_count;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
class CTCHeroOnlineScoreboard {
public:
    CScoreboardMap DownloadedClanScoreboard;
    bool IsScoreboardEmpty()
    {
        for (CScoreboardMap::iterator it = DownloadedClanScoreboard.begin(); it != DownloadedClanScoreboard.end(); ++it)
            if (it->second.size() != 0)
                return false;
        return true;
    }
};
static void RunTest()
{
    SScoreEntry rows[3];
    SScoreboardNode n[5];
    _Rb_tree_node_base* order[5];
    int i;
    for (i = 0; i < 5; ++i) {
        n[i]._M_value_field.first = (EOnlineScoreboardActivity)i;
        n[i]._M_value_field.second._M_start = n[i]._M_value_field.second._M_finish = rows;
        n[i]._M_value_field.second._M_end_of_storage = rows + 3;
        order[i] = &n[i];
    }
    _Rb_tree_node_base header;
    CTCHeroOnlineScoreboard board;
    board.DownloadedClanScoreboard._M_header = &header;
    board.DownloadedClanScoreboard._M_node_count = 5;
    BuildTree(&header, order, 5);
    CHECK(board.IsScoreboardEmpty());                  // five activities, all lists empty
    n[4]._M_value_field.second._M_finish = rows + 2;   // the last (rightmost) activity has two scores
    CHECK(!board.IsScoreboardEmpty());
    n[4]._M_value_field.second._M_finish = rows;
    n[0]._M_value_field.second._M_finish = rows + 1;   // the first one has one
    CHECK(!board.IsScoreboardEmpty());
    BuildTree(&header, order, 0);                      // the empty map
    CHECK(board.IsScoreboardEmpty());
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055e7f0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055e7f0_TEST PASS\n");
    return 0;
}