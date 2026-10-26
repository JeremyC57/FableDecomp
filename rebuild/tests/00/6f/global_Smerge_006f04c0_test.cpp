#include <cstdio>
class CThingMarker { public: float dist; };
struct SListNode { SListNode* Next; SListNode* Prev; CThingMarker* Value; };
struct SMarkerList { SListNode* Node; };
class CThingCompare_Nearest { public: float X, Y, Z; long operator()(CThingMarker* a, CThingMarker* b) { return a->dist < b->dist ? 1 : 0; } };
static void _M_transfer(SListNode* position, SListNode* first, SListNode* last)
{
    if (position != last) {
        last->Prev->Next = position; first->Prev->Next = last; position->Prev->Next = first;
        SListNode* tmp = position->Prev; position->Prev = last->Prev; last->Prev = first->Prev; first->Prev = tmp;
    }
}
static void _S_merge(SMarkerList& list, SMarkerList& other, CThingCompare_Nearest comp)
{
    SListNode* last1 = list.Node; SListNode* last2 = other.Node;
    SListNode* first2 = last2->Next; SListNode* first1 = last1->Next;
    while (first1 != last1 && first2 != last2) {
        if (comp(first2->Value, first1->Value)) { SListNode* next = first2->Next; _M_transfer(first1, first2, next); first2 = next; }
        else first1 = first1->Next;
    }
    if (first2 != last2) _M_transfer(last1, first2, last2);
}
static void Build(SListNode* head, SListNode* nodes, CThingMarker* m, int n)
{
    SListNode* prev = head;
    for (int i = 0; i < n; ++i) { nodes[i].Value = &m[i]; prev->Next = &nodes[i]; nodes[i].Prev = prev; prev = &nodes[i]; }
    prev->Next = head; head->Prev = prev;
}
int main()
{
    CThingMarker a[3] = { {1}, {4}, {6} }, b[4] = { {2}, {3}, {7}, {8} };
    SListNode h1, h2, n1[3], n2[4];
    Build(&h1, n1, a, 3); Build(&h2, n2, b, 4);
    SMarkerList l1, l2; l1.Node = &h1; l2.Node = &h2;
    CThingCompare_Nearest c; c.X = c.Y = c.Z = 0;
    _S_merge(l1, l2, c);
    float expect[7] = { 1, 2, 3, 4, 6, 7, 8 }; int i = 0;
    for (SListNode* p = h1.Next; p != &h1; p = p->Next, ++i) {
        if (i >= 7 || p->Value->dist != expect[i] || p->Next->Prev != p) { std::printf("006f04c0_TEST FAIL\n"); return 1; }
    }
    if (i != 7 || h2.Next != &h2 || h2.Prev != &h2) { std::printf("006f04c0_TEST FAIL\n"); return 1; }
    std::printf("006f04c0_TEST PASS\n");
    return 0;
}