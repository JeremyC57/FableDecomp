// Behaviour test for list<CScriptThing>::push_back (0x00DA01E0): a new node holding a copy of the thing is linked in
// before the header (at the end), keeping the ring consistent in both directions.
#include <cstdio>
#include <cstdlib>
#include <new>

class CScriptThing {
public:
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { }
};
struct CListNodeBase { CListNodeBase* next; CListNodeBase* prev; };
struct CListNode : CListNodeBase { CScriptThing data; };
struct CThingList {
    CListNodeBase* node;                        // the header node
    CListNode* _M_create_node(const CScriptThing& x)
    {
        CListNode* tmp = (CListNode*)std::malloc(sizeof(CListNode));
        new (&tmp->data) CScriptThing(x);
        return tmp;
    }
    CListNodeBase* end() { return node; }
    CListNodeBase* insert(CListNodeBase* position, const CScriptThing& x)
    {
        CListNode* tmp = _M_create_node(x);
        CListNodeBase* n = position;
        CListNodeBase* p = n->prev;
        tmp->next = n;
        tmp->prev = p;
        p->next = tmp;
        n->prev = tmp;
        return tmp;
    }
    void push_back(const CScriptThing& x) { insert(end(), x); }
};

int main()
{
    CListNodeBase header;
    header.next = &header; header.prev = &header;
    CThingList list; list.node = &header;
    list.push_back(CScriptThing(1));
    list.push_back(CScriptThing(2));
    list.push_back(CScriptThing(3));
    int forward[3], backward[3], i = 0;
    for (CListNodeBase* it = header.next; it != &header && i < 3; it = it->next) forward[i++] = ((CListNode*)it)->data.id;
    int j = 0;
    for (CListNodeBase* rit = header.prev; rit != &header && j < 3; rit = rit->prev) backward[j++] = ((CListNode*)rit)->data.id;
    bool ok = i == 3 && j == 3 && forward[0] == 1 && forward[1] == 2 && forward[2] == 3 && backward[0] == 3 && backward[2] == 1;
    while (header.next != &header) { CListNodeBase* n = header.next; header.next = n->next; std::free(n); }
    if (!ok) {
        std::printf("00da01e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00da01e0_TEST PASS\n");
    return 0;
}