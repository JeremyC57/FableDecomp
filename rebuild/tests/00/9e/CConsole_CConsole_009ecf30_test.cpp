// Behaviour test for 0x009ECF30 CConsole::~CConsole (clear maps, release input pointers, clear history twice, free storage).
#include <cstdio>
#include <cstdlib>

static int g_lineDtors = 0;
static int g_released = 0;
static int g_frees = 0;
static int g_erases = 0;

struct Info { unsigned long refs; };

struct Line { virtual ~Line() { ++g_lineDtors; } };
struct LinkNode { LinkNode* next; LinkNode* prev; };
struct HistNode { LinkNode link; Line* line; };

struct InputEntry { void* data; Info* info; };

static void Release(InputEntry& e)
{
    if (e.info) {
        --e.info->refs;
        if (e.info->refs == 0) { ++g_released; delete e.info; }
    }
    e.info = 0; e.data = 0;
}

struct Map { unsigned int count; Map() : count(0) {} void Clear() { if (count != 0) { ++g_erases; count = 0; } } };

struct History {
    LinkNode* head;
    History() { head = (LinkNode*)std::malloc(sizeof(LinkNode)); head->next = head->prev = head; }
    void Push()
    {
        HistNode* n = (HistNode*)std::malloc(sizeof(HistNode)); n->line = new Line;
        n->link.next = head; n->link.prev = head->prev; head->prev->next = &n->link; head->prev = &n->link;
    }
    void Clear()
    {
        LinkNode* cur = head->next;
        while (cur != head) { LinkNode* tmp = cur; cur = cur->next; delete ((HistNode*)tmp)->line; ++g_frees; std::free(tmp); }
        head->next = head->prev = head;
    }
    ~History() { Clear(); if (head) { ++g_frees; std::free(head); } }
};

struct Console {
    Map keys, strings, inputsMap; InputEntry inputs[3]; History history;
    ~Console()
    {
        keys.Clear(); strings.Clear();
        for (int i = 0; i < 3; ++i) Release(inputs[i]);
        inputsMap.Clear();
        history.Clear();
    }
};

int main()
{
    int ok = 1;
    Info* a = new Info; a->refs = 1;
    Info* b = new Info; b->refs = 2;
    Console* c = new Console;
    c->inputs[0].data = c; c->inputs[0].info = a;
    c->inputs[1].data = c; c->inputs[1].info = b;
    c->inputs[2].data = 0; c->inputs[2].info = 0;
    c->keys.count = 2; c->strings.count = 0; c->inputsMap.count = 3;
    c->history.Push(); c->history.Push();
    delete c;
    if (g_released != 1 || b->refs != 1) ok = 0;
    if (g_erases != 2 || g_lineDtors != 2 || g_frees != 3) ok = 0;   // two history nodes and the sentinel
    delete b;
    if (!ok) { std::printf("009ecf30_TEST FAIL\n"); return 1; }
    std::printf("009ecf30_TEST PASS\n");
    return 0;
}