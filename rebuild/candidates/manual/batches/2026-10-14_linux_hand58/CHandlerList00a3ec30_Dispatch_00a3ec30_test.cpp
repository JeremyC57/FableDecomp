#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_calls = 0; static void* g_arg = 0;
struct IHandler { virtual ~IHandler() { } virtual bool HandleEvent(void* e) = 0; };
struct CH : IHandler { bool accept; virtual bool HandleEvent(void* e) { ++g_calls; g_arg = e; return accept; } };
struct SNode { SNode* next; SNode* prev; IHandler* value; };
struct CPtrList { SNode* head; };
struct CDispatcher { unsigned char pad[0x4c]; CPtrList handlers;
    bool Dispatch(void* e) { for (SNode* n = handlers.head->next; n != handlers.head; n = n->next) { if (n->value->HandleEvent(e)) return true; } return false; } };
int main()
{
    CH a, b, c; a.accept = false; b.accept = true; c.accept = true; SNode h, n1, n2, n3; h.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &h; n1.value = &a; n2.value = &b; n3.value = &c;
    CDispatcher d; d.handlers.head = &h; int ev;
    if (!d.Dispatch(&ev) || g_calls != 2 || g_arg != &ev) { std::printf("00a3ec30_TEST FAIL\n"); return 1; }
    b.accept = false; c.accept = false; g_calls = 0; if (d.Dispatch(&ev) || g_calls != 3) { std::printf("00a3ec30_TEST FAIL\n"); return 1; }
    h.next = &h; g_calls = 0; if (d.Dispatch(&ev) || g_calls != 0) { std::printf("00a3ec30_TEST FAIL\n"); return 1; }
    std::printf("00a3ec30_TEST PASS\n");
    return 0;
}
