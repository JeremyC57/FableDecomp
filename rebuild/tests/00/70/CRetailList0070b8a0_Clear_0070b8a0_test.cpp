#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static long g_freed = 0, g_live = 0;
struct SVal { long m_0; char* a; long pad[2]; char* b; SVal() { a = (char*)std::malloc(4); b = 0; ++g_live; } };
struct SNode { SNode* next; SNode* prev; SVal value; };
struct CList { SNode* head; CList() { head = (SNode*)std::malloc(sizeof(SNode)); head->next = head->prev = head; } void Push() { SNode* n = (SNode*)std::malloc(sizeof(SNode)); new (&n->value) SVal(); n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
  void Clear() { SNode* next = head->next; while (next != head) { SNode* node = next; next = next->next; if (node->value.b) { std::free(node->value.b); } if (node->value.a) { std::free(node->value.a); --g_live; } std::free(node); ++g_freed; } head->next = head; head->prev = head; } };

int main() {
    CList l; l.Push(); l.Push(); l.Push(); if (g_live != 3) { std::printf("0070b8a0_TEST FAIL\n"); return 1; } l.Clear(); if (g_live != 0 || g_freed != 3 || l.head->next != l.head || l.head->prev != l.head) { std::printf("0070b8a0_TEST FAIL\n"); return 1; } l.Clear(); if (g_freed != 3) { std::printf("0070b8a0_TEST FAIL\n"); return 1; }
    std::printf("0070b8a0_TEST PASS\n"); return 0;
}