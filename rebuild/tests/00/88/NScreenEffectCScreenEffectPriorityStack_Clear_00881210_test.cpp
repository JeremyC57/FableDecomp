// Behaviour test for NScreenEffect::CScreenEffectPriorityStack::Clear 0x00881210 (standalone model: circular STLport-style
// lists with virtual entries, trees cleared through an erase walk, Reset at the end).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0;
static int g_freedNodes = 0;
static int g_reset = 0;
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Entry { virtual ~Entry() { ++g_destroyed; } int v; };
struct EntryNode : NodeBase { Entry data; };
struct List {
    NodeBase* head;
    void init() { head = (NodeBase*)std::malloc(sizeof(NodeBase)); head->next = head->prev = head; }
    void push(int v) {
        EntryNode* n = (EntryNode*)std::malloc(sizeof(EntryNode));
        new (&n->data) Entry; n->data.v = v;
        n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
    }
    void clear() {
        EntryNode* cur = (EntryNode*)head->next;
        while (cur != head) {
            EntryNode* tmp = cur;
            cur = (EntryNode*)cur->next;
            tmp->data.~Entry();
            std::free(tmp); ++g_freedNodes;
        }
        head->next = head;
        head->prev = head;
    }
    int size() const { int n = 0; for (NodeBase* p = head->next; p != head; p = p->next) ++n; return n; }
};
struct TNode { TNode* parent; TNode* left; TNode* right; };
struct Tree {
    TNode header; TNode* hdr; unsigned int count;
    void init() { hdr = &header; header.parent = 0; header.left = header.right = hdr; count = 0; }
    void erase(TNode* x) { while (x) { erase(x->right); TNode* y = x->left; std::free(x); ++g_freedNodes; x = y; } }
    void insertOne() { TNode* n = (TNode*)std::malloc(sizeof(TNode)); n->left = n->right = 0; n->parent = header.parent; if (header.parent) n->left = header.parent; header.parent = n; ++count; }
    void clear() { if (count != 0) { erase(header.parent); header.left = hdr; header.parent = 0; header.right = hdr; count = 0; } }
};
struct CScreenEffectPriorityStack {
    List RadialParamStack[3]; Tree RadialBlurKeys;
    List DisplacementParamStack[3]; Tree DisplacementKeys;
    List ScreenFilterParamStack[3]; Tree ScreenFilterKeys;
    void Reset() { ++g_reset; }
    void Clear()
    {
        RadialBlurKeys.clear();
        DisplacementKeys.clear();
        ScreenFilterKeys.clear();
        for (long i = 0; i < 3; ++i) {
            RadialParamStack[i].clear();
            DisplacementParamStack[i].clear();
            ScreenFilterParamStack[i].clear();
        }
        Reset();
    }
};
int main()
{
    CScreenEffectPriorityStack s;
    int entries = 0;
    for (int i = 0; i < 3; ++i) {
        s.RadialParamStack[i].init(); s.DisplacementParamStack[i].init(); s.ScreenFilterParamStack[i].init();
        for (int k = 0; k <= i; ++k) { s.RadialParamStack[i].push(k); s.DisplacementParamStack[i].push(k); s.ScreenFilterParamStack[i].push(k); entries += 3; }
    }
    s.RadialBlurKeys.init(); s.DisplacementKeys.init(); s.ScreenFilterKeys.init();
    s.RadialBlurKeys.insertOne(); s.RadialBlurKeys.insertOne(); s.ScreenFilterKeys.insertOne();
    s.Clear();
    bool ok = g_destroyed == entries && g_freedNodes == entries + 3 && g_reset == 1
        && s.RadialBlurKeys.count == 0 && s.ScreenFilterKeys.count == 0 && s.RadialBlurKeys.header.left == s.RadialBlurKeys.hdr;
    for (int i = 0; i < 3; ++i)
        if (s.RadialParamStack[i].size() || s.DisplacementParamStack[i].size() || s.ScreenFilterParamStack[i].size()) ok = false;
    s.Clear();
    if (g_reset != 2 || g_destroyed != entries) ok = false;
    if (!ok) { std::printf("00881210_TEST FAIL\n"); return 1; }
    std::printf("00881210_TEST PASS\n");
    return 0;
}