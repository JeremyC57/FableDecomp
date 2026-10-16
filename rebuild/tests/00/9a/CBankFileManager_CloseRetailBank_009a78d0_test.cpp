// Behaviour test for 009a78d0: CBankFileManager::CloseRetailBank -- remove the first bank whose path matches; release its count.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct TBank { const char* FilePath; };
struct TNode { TNode* next; TNode* prev; TBank* data; int* refs; };
struct TManager {
    TNode* head;
    TManager() { head = (TNode*)std::malloc(sizeof(TNode)); head->next = head->prev = head; }
    void push(TBank* b, int* r) { TNode* n = (TNode*)std::malloc(sizeof(TNode)); n->data = b; n->refs = r; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    int count() const { int c = 0; for (TNode* p = head->next; p != head; p = p->next) ++c; return c; }
    void CloseRetailBank(const char* name)
    {
        for (TNode* it = head->next; it != head; it = it->next)
            if (std::strcmp(name, it->data->FilePath ? it->data->FilePath : "") == 0) {
                it->prev->next = it->next; it->next->prev = it->prev; if (it->refs) --*it->refs; std::free(it); break;
            }
    }
};
int main()
{
    TBank a = { "levels.big" }, b = { "art.big" }, c = { "art.big" };
    int ra = 1, rb = 1, rc = 1;
    TManager m; m.push(&a, &ra); m.push(&b, &rb); m.push(&c, &rc);
    m.CloseRetailBank("art.big");
    if (m.count() != 2 || rb != 0 || rc != 1 || ra != 1 || m.head->next->data != &a || m.head->next->next->data != &c) { std::printf("009a78d0_TEST FAIL\n"); return 1; }
    m.CloseRetailBank("missing.big");
    if (m.count() != 2) { std::printf("009a78d0_TEST FAIL\n"); return 1; }
    std::printf("009a78d0_TEST PASS\n");
    return 0;
}