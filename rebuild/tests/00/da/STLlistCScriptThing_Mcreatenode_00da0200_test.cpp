// Behaviour test for list<CScriptThing>::_M_create_node (0x00DA0200): allocate one node and copy-construct the
// element into its data slot (the links are left for the caller).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
class CScriptThing {
public:
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { ++g_copies; }
};
struct CListNodeBase { CListNodeBase* next; CListNodeBase* prev; };
struct CListNode : CListNodeBase { CScriptThing data; };
template <class T1, class T2> void Construct(T1* p, const T2& v) { new (p) T1(v); }
struct CThingList {
    CListNodeBase* node;
    CListNode* Allocate() { return (CListNode*)std::malloc(sizeof(CListNode)); }
    CListNode* _M_create_node(const CScriptThing& x)
    {
        CListNode* tmp = Allocate();
        Construct(&tmp->data, x);
        return tmp;
    }
};

int main()
{
    CThingList list;
    list.node = 0;
    CScriptThing spawnPoint(42);
    CListNode* n = list._M_create_node(spawnPoint);
    if (n == 0 || n->data.id != 42 || g_copies != 1) {
        std::printf("00da0200_TEST FAIL\n");
        return 1;
    }
    std::free(n);
    std::printf("00da0200_TEST PASS\n");
    return 0;
}