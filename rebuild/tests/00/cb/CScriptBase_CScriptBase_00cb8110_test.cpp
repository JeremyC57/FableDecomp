// Behaviour test for 00cb8110: CScriptBase::CScriptBase -- base first, list sentinel self-linked, maps empty and clean, no active thread, LastWorldUpdateFrame untouched, size 0x40
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_seq[16] = "";
static unsigned int g_size = 0;
static void* Alloc(unsigned int n) { std::strcat(g_seq, "m"); g_size = n; return std::malloc(n); }
struct Base { Base() { std::strcat(g_seq, "B"); } virtual ~Base() { } };
struct Node { Node* next; Node* prev; void* data; void* info; };
struct List { Node* node; List() { node = 0; Node* n = (Node*)Alloc(sizeof(Node)); n->next = n; n->prev = n; node = n; } };
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) { } };
struct Map { Vec v; char cmp; bool dirty; Map() : dirty(false) { } };
struct ScriptBase : Base {
    List threads; Map entityScripts; Map bindings; unsigned long lastFrame; void* active; Map persist;
    ScriptBase() : active(0) { }
};
int main()
{
    unsigned char raw[sizeof(ScriptBase)]; std::memset(raw, 0xEE, sizeof raw);
    ScriptBase* s = new (raw) ScriptBase;
    int ok = sizeof(ScriptBase) == 0x40 && std::strcmp(g_seq, "Bm") == 0 && g_size == 0x10;
    ok = ok && s->threads.node && s->threads.node->next == s->threads.node && s->threads.node->prev == s->threads.node;
    ok = ok && !s->entityScripts.v.s && !s->entityScripts.v.e && !s->entityScripts.dirty && !s->bindings.v.f && !s->bindings.dirty;
    ok = ok && !s->persist.v.s && !s->persist.dirty && s->active == 0 && s->lastFrame == 0xEEEEEEEEul;
    std::free(s->threads.node);
    std::printf(ok ? "00cb8110_TEST PASS\n" : "00cb8110_TEST FAIL\n");
    return ok ? 0 : 1;
}
