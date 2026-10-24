#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_nodes = 0;
struct SConn { int* data; int* info; };                 // CCountedPointer<CNavRegionConnection>: copied word for word
struct PNode { PNode* next; PNode* prev; SConn value; };
static PNode* PAlloc() { ++g_nodes; return (PNode*)malloc(sizeof(PNode)); }
static void PFree(PNode* p) { --g_nodes; free(p); }

class PList {                                           // STLport list<CCountedPointer<CNavRegionConnection> >
public:
    PNode* head;
    PList() { Init(); }
    PList(const PList& x)                               // a fresh sentinel, then insert(begin(), x.begin(), x.end())
    {
        Init();
        InsertRange(head->next, x.head->next, x.head);
    }
    ~PList()
    {
        PNode* n = head->next;
        while (n != head) { PNode* t = n; n = n->next; PFree(t); }
        PFree(head);
    }
    void Init() { head = PAlloc(); head->next = head; head->prev = head; }
    void InsertRange(PNode* pos, PNode* first, PNode* last)
    {
        for (; first != last; first = first->next) {
            PNode* n = PAlloc();
            n->value = first->value;
            n->next = pos;
            n->prev = pos->prev;
            pos->prev->next = n;
            pos->prev = n;
        }
    }
    void PushBack(const SConn& c)
    {
        PNode* n = PAlloc();
        n->value = c;
        n->next = head;
        n->prev = head->prev;
        head->prev->next = n;
        head->prev = n;
    }
    int Count() const { int c = 0; for (PNode* n = head->next; n != head; n = n->next) ++c; return c; }
private:
    PList& operator=(const PList&);
};

class CNavRegion {
public:
    long  RegionID;
    PList Connections;
    long  MostRecentSearchID;
    CNavRegion() : RegionID(0), MostRecentSearchID(0) { }
    // the row: member-wise copy (the list deep-copies its nodes)
    CNavRegion(const CNavRegion& other)
        : RegionID(other.RegionID), Connections(other.Connections), MostRecentSearchID(other.MostRecentSearchID)
    {
    }
};

int main()
{
    static int objs[3];
    static int infos[3];
    {
        CNavRegion a;
        a.RegionID = 42;
        a.MostRecentSearchID = 7;
        for (int k = 0; k < 3; ++k) {
            SConn c;
            c.data = &objs[k];
            c.info = &infos[k];
            a.Connections.PushBack(c);
        }
        int nodes = g_nodes;
        CNavRegion b(a);
        Check(b.RegionID == 42 && b.MostRecentSearchID == 7);
        Check(b.Connections.Count() == 3 && a.Connections.Count() == 3);
        Check(g_nodes == nodes + 4);
        Check(b.Connections.head != a.Connections.head);
        PNode* p = a.Connections.head->next;
        PNode* q = b.Connections.head->next;
        for (int j = 0; j < 3; ++j, p = p->next, q = q->next) {
            Check(p != q);
            Check(q->value.data == &objs[j] && q->value.info == &infos[j]);
        }
        Check(q == b.Connections.head && q->prev->next == q);
        CNavRegion empty;
        CNavRegion copy(empty);
        Check(copy.Connections.Count() == 0 && copy.Connections.head->prev == copy.Connections.head);
    }
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a83110_TEST FAIL\n");
        return 1;
    }
    std::printf("00a83110_TEST PASS\n");
    return 0;
}