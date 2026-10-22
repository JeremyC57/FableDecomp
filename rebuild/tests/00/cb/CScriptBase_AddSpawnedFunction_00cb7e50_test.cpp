// Behaviour test for 0x00CB7E50 (CGuiVarTransferStruct::Add): the variable is named, appended to the list, and its count block outlives the temporary.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_destroyed = 0;
struct GuiVar { const char* name; };
static void DeleteData(GuiVar* v) { if (v) ++g_destroyed; }
struct Counted {
    struct Block { int count; void (*destroy)(GuiVar*); GuiVar* object; Block(GuiVar* o) : count(1), destroy(DeleteData), object(o) {} };
    GuiVar* object; Block* block;
    Counted(GuiVar* o) : object(o) { block = o ? new Block(o) : 0; }
    Counted(const Counted& c) : object(c.object), block(c.block) { if (block) ++block->count; }
    ~Counted() { if (block && --block->count == 0) { block->destroy(block->object); delete block; } }
};
struct Node { Node* next; Node* prev; Counted* value; };
struct List {
    Node* head; int n;
    List() : n(0) { head = new Node; head->next = head; head->prev = head; head->value = 0; }
    void push_back(const Counted& v)
    {
        Node* node = new Node; node->value = new Counted(v);
        Node* prev = head->prev; node->next = head; node->prev = prev; prev->next = node; head->prev = node; ++n;
    }
};
struct TransferStruct {
    List vars;
    void Add(GuiVar* var, const char* name) { var->name = name; vars.push_back(Counted(var)); }
};
int main()
{
    TransferStruct t; GuiVar a, b;
    t.Add(&a, "Health"); t.Add(&b, "Mana");
    int ok = std::strcmp(a.name, "Health") == 0 && std::strcmp(b.name, "Mana") == 0;
    ok = ok && t.vars.head->next->value->object == &a && t.vars.head->next->next->value->object == &b && t.vars.head->prev->value->object == &b;
    ok = ok && t.vars.head->next->value->block->count == 1 && g_destroyed == 0;
    delete t.vars.head->next->value;                         // dropping the list's reference destroys the variable
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("00cb7e50_TEST FAIL\n"); return 1; }
    std::printf("00cb7e50_TEST PASS\n");
    return 0;
}