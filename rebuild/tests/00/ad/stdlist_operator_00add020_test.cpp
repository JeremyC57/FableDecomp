// Behaviour test for 0x00ADD020 list<CVertexGroupBlock>::operator= (assign pairwise, then erase the surplus or append the rest).
#include <cstdio>
#include <cstdlib>

static int g_assigns = 0;
struct SBlock { int Id; SBlock& operator=(const SBlock& o) { Id = o.Id; ++g_assigns; return *this; } };
struct SNode { SNode* Next; SNode* Prev; SBlock Data; };
struct SList {
    SNode* Node;
    SList() { Node = (SNode*)std::malloc(sizeof(SNode)); Node->Next = Node->Prev = Node; }
    void Link(SNode* pos, int id) { SNode* t = (SNode*)std::malloc(sizeof(SNode)); t->Data.Id = id; SNode* p = pos->Prev; t->Next = pos; t->Prev = p; p->Next = t; pos->Prev = t; }
    SNode* Erase(SNode* first, SNode* last)                   // 0x00ADC4E0
    { while (first != last) { SNode* pos = first; first = first->Next; pos->Prev->Next = pos->Next; pos->Next->Prev = pos->Prev; std::free(pos); } return last; }
    void Insert(SNode* pos, const SNode* first, const SNode* last)   // 0x00ADCC80
    { for (; first != last; first = first->Next) Link(pos, first->Data.Id); }
    // reconstructed body
    SList& operator=(const SList& x)
    {
        if (this != &x) {
            SNode* first1 = Node->Next; SNode* last1 = Node;
            const SNode* first2 = x.Node->Next; const SNode* last2 = x.Node;
            while (first1 != last1 && first2 != last2) { first1->Data = first2->Data; first1 = first1->Next; first2 = first2->Next; }
            if (first2 == last2) Erase(first1, last1);
            else Insert(last1, first2, last2);
        }
        return *this;
    }
    int Size() const { int n = 0; for (SNode* p = Node->Next; p != Node; p = p->Next) ++n; return n; }
    int At(int i) const { SNode* p = Node->Next; while (i--) p = p->Next; return p->Data.Id; }
};

int main()
{
    SList a, b, c;
    for (int i = 0; i < 5; ++i) a.Link(a.Node, 100 + i);
    b.Link(b.Node, 1); b.Link(b.Node, 2);
    b = a;                                                    // grow: 2 assigned, 3 appended
    if (b.Size() != 5 || g_assigns != 2 || b.At(0) != 100 || b.At(4) != 104) { std::printf("00add020_TEST FAIL\n"); return 1; }
    c.Link(c.Node, 7);
    b = c;                                                    // shrink: 1 assigned, 4 erased
    if (b.Size() != 1 || g_assigns != 3 || b.At(0) != 7) { std::printf("00add020_TEST FAIL\n"); return 1; }
    SList& r = (b = b);                                       // self-assignment is a no-op
    if (&r != &b || g_assigns != 3 || b.Size() != 1) { std::printf("00add020_TEST FAIL\n"); return 1; }
    std::printf("00add020_TEST PASS\n");
    return 0;
}