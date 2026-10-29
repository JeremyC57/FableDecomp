// Behaviour test for NUISystem::CMorphingSprite::~CMorphingSprite @ 0x005541E0: every engine primitive handle owned by the
// Handles list is deleted, the list is emptied, then the graphic-index map and the CSprite base are destroyed.
#include <stdio.h>
static int g_handleDtor = 0, g_spriteDtor = 0, g_mapDtor = 0, g_listNodesFreed = 0;
class CEnginePrimitiveHandle { public: int Id; ~CEnginePrimitiveHandle() { g_handleDtor++; } };
template <class T> class list { public:                    // minimal doubly linked list with a header node
    struct node { node* next; node* prev; T data; };
    node* head;
    list() { head = new node; head->next = head->prev = head; }
    struct iterator { node* n; T& operator*() { return n->data; } iterator& operator++() { n = n->next; return *this; }
        bool operator!=(const iterator& o) const { return n != o.n; } };
    iterator begin() { iterator i; i.n = head->next; return i; }
    iterator end() { iterator i; i.n = head; return i; }
    void push_back(const T& v) { node* x = new node; x->data = v; x->next = head; x->prev = head->prev; head->prev->next = x; head->prev = x; }
    unsigned int size() const { unsigned int c = 0; for (node* x = head->next; x != head; x = x->next) ++c; return c; }
    void clear() { node* x = head->next; while (x != head) { node* n = x->next; delete x; g_listNodesFreed++; x = n; } head->next = head->prev = head; }
    ~list() { clear(); delete head; }
};
class GraphicIndexMap { public: ~GraphicIndexMap() { g_mapDtor++; } };
namespace NUISystem {
class CSprite { public: virtual ~CSprite() { g_spriteDtor++; } };
class CMorphingSprite : public CSprite { public:
    int                             RenderCount;
    list<CEnginePrimitiveHandle*>   Handles;
    GraphicIndexMap                 GraphicIndices;
    virtual ~CMorphingSprite();
};
CMorphingSprite::~CMorphingSprite()
{
    for (list<CEnginePrimitiveHandle*>::iterator it = Handles.begin(); it != Handles.end(); ++it)
        delete *it;
    Handles.clear();
    if (Handles.size() != 0) g_handleDtor = -1000;          // the list must be empty before the members go
}
}
static int fail(int line) { printf("005541e0_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    NUISystem::CMorphingSprite* sprite = new NUISystem::CMorphingSprite;
    for (int i = 0; i < 3; ++i) { CEnginePrimitiveHandle* h = new CEnginePrimitiveHandle; h->Id = i; sprite->Handles.push_back(h); }
    NUISystem::CSprite* base = sprite;
    delete base;
    if (g_handleDtor != 3 || g_listNodesFreed != 3 || g_mapDtor != 1 || g_spriteDtor != 1) return fail(__LINE__);
    NUISystem::CMorphingSprite* empty = new NUISystem::CMorphingSprite;
    delete empty;
    if (g_handleDtor != 3 || g_listNodesFreed != 3 || g_mapDtor != 2 || g_spriteDtor != 2) return fail(__LINE__);
    printf("005541e0_TEST PASS\n");
    return 0;
}