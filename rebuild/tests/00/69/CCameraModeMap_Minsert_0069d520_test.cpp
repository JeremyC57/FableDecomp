// Behaviour test for 0069d520: STLport _Rb_tree::_M_insert over pair<const EMode, CCountedPointer<CBaseInit>> (model copy).
#include <cstdio>
#include <cstdlib>
#include <new>
class CCPPointerInfo { public: unsigned int RefCount; };
struct CCountedPointer {
    void* Data;
    CCPPointerInfo* Info;
    CCountedPointer(void* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct SValue {
    const long first;
    CCountedPointer second;
    SValue(long k, void* d, CCPPointerInfo* i) : first(k), second(d, i) { }
    SValue(const SValue& other) : first(other.first), second(other.second) { }
};
#define VALUE SValue
static CCPPointerInfo g_info;
static int g_obj;
static SValue MAKE(long k) { return SValue(k, &g_obj, k == 3 ? 0 : &g_info); }

struct SRbNodeBase { long _M_color; SRbNodeBase* _M_parent; SRbNodeBase* _M_left; SRbNodeBase* _M_right; };
struct SRbNode : SRbNodeBase { VALUE _M_value_field; };
static int g_rebalance = 0;
static SRbNodeBase* g_lastRoot = 0;
void Rebalance(SRbNodeBase* node, SRbNodeBase*& root) { ++g_rebalance; g_lastRoot = root; (void)node; }
struct CTree {
    SRbNodeBase* _M_header;
    unsigned long _M_node_count;
    SRbNode* _M_create_node(const VALUE& v)
    {
        SRbNode* tmp = (SRbNode*)std::malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) VALUE(v);
        return tmp;
    }
    SRbNodeBase* _M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const VALUE& v, SRbNodeBase* w_)
    {
        SRbNode* w = (SRbNode*)w_; SRbNode* y = (SRbNode*)y_; SRbNode* x = (SRbNode*)x_; SRbNode* z;
        if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
            z = _M_create_node(v);
            y->_M_left = z;
            if (y == _M_header) { _M_header->_M_parent = z; _M_header->_M_right = z; }
            else if (y == _M_header->_M_left) _M_header->_M_left = z;
        }
        else {
            z = _M_create_node(v);
            y->_M_right = z;
            if (y == _M_header->_M_right) _M_header->_M_right = z;
        }
        z->_M_parent = y; z->_M_left = 0; z->_M_right = 0;
        Rebalance(z, _M_header->_M_parent);
        ++_M_node_count;
        return z;
    }
};

int main()
{
    g_info.RefCount = 1;
    {
    SRbNodeBase head; head._M_color = 0; head._M_parent = 0; head._M_left = &head; head._M_right = &head;
    CTree t; t._M_header = &head; t._M_node_count = 0;
    int fail = 0;
    SRbNode* a = (SRbNode*)t._M_insert(0, &head, MAKE(5), 0);              // first node: root, leftmost, rightmost
    if (head._M_parent != a || head._M_left != a || head._M_right != a || a->_M_parent != &head || a->_M_left || a->_M_right) fail = 1;
    SRbNode* b = (SRbNode*)t._M_insert(0, a, MAKE(3), 0);                  // key < y: left child, new leftmost
    if (a->_M_left != b || head._M_left != b || head._M_right != a || b->_M_parent != a) fail = 1;
    SRbNode* c = (SRbNode*)t._M_insert(0, a, MAKE(9), 0);                  // key > y: right child, new rightmost
    if (a->_M_right != c || head._M_right != c || head._M_left != b) fail = 1;
    SRbNode* d = (SRbNode*)t._M_insert(0, b, MAKE(4), b);                  // w != 0 forces the right side
    if (b->_M_right != d || head._M_right != c) fail = 1;
    if (t._M_node_count != 4 || g_rebalance != 4 || g_lastRoot != a) fail = 1;
    if (a->_M_value_field.first != 5 || d->_M_value_field.first != 4) fail = 1;

    if (g_info.RefCount < 4 || b->_M_value_field.second.Info != 0 || a->_M_value_field.second.Info != &g_info || a->_M_value_field.second.Data != &g_obj) fail = 1;
    if (fail) { std::printf("0069d520_TEST FAIL\n"); return 1; }
    }
    std::printf("0069d520_TEST PASS\n");
    return 0;
}