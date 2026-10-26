// Behaviour test for 0x009AA9E0 std::_Rb_tree<CCharString, pair<const CCharString, NBankFileManager::CContainedBank>, ..>::_M_create_node: one node allocated, the key copied once through its copy constructor, the CContainedBank copied.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_copies = 0; struct Str { char* p; Str() : p(0) {} Str(const Str& o) : p(o.p) { ++g_copies; } }; struct V { Str first; struct { unsigned long DataVersion, EntryCount, InfoOffset, InfoSize, Alignment; } second; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* _M_create_node(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); static char s[] = "x"; v.first.p = s; v.second.Alignment = 9;
    Tree t; Node* n = t._M_create_node(v); bool ok = n && (g_copies == 1 && n->value.first.p == s && n->value.second.Alignment == 9); std::free(n);
    if (!ok) { std::printf("009aa9e0_TEST FAIL\n"); return 1; }
    std::printf("009aa9e0_TEST PASS\n");
    return 0;
}