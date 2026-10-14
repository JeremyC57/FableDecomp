#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static long g_freed = 0, g_live = 0;
struct CStr { long m_tag; CStr() { ++g_live; } ~CStr() { --g_live; } }; struct SVal { CStr a; long b; CStr c, d; };
struct SNode { SNode* next; SNode* prev; SVal value; };
struct CList { SNode* head; CList() { head = (SNode*)std::malloc(sizeof(SNode)); head->next = head->prev = head; } void Push() { SNode* n = (SNode*)std::malloc(sizeof(SNode)); new (&n->value) SVal(); n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
  void Clear() { SNode* next = head->next; while (next != head) { SNode* node = next; next = next->next; node->value.~SVal(); std::free(node); ++g_freed; } head->next = head; head->prev = head; } };

int main() {
    CList l; l.Push(); l.Push(); l.Push(); if (g_live != 9) { std::printf("00566dc0_TEST FAIL\n"); return 1; } l.Clear(); if (g_live != 0 || g_freed != 3 || l.head->next != l.head || l.head->prev != l.head) { std::printf("00566dc0_TEST FAIL\n"); return 1; } l.Clear(); if (g_freed != 3) { std::printf("00566dc0_TEST FAIL\n"); return 1; }
    std::printf("00566dc0_TEST PASS\n"); return 0;
}
