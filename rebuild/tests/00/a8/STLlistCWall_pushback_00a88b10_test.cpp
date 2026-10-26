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
class C2DVector { public: float X, Y; };
class CWall { public:                                          // 24 bytes
    C2DVector Point1;
    C2DVector Point2;
    C2DVector ToInternal;
};
struct SWallNode { SWallNode* next; SWallNode* prev; CWall data; };
struct SWallList { SWallNode* head; };                  // STLport list<CWall>: the sentinel pointer

// the row: list<CWall>::push_back = insert(end(), x)
static void PushBackWall(SWallList& l, const CWall& x)
{
    SWallNode* tmp = (SWallNode*)malloc(sizeof(SWallNode));
    ++g_nodes;
    new (&tmp->data) CWall(x);
    SWallNode* n = l.head;
    SWallNode* p = n->prev;
    tmp->next = n;
    tmp->prev = p;
    p->next = tmp;
    n->prev = tmp;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    Check(sizeof(CWall) == (unsigned int)24);
    SWallList l;
    l.head = (SWallNode*)malloc(sizeof(SWallNode));
    l.head->next = l.head;
    l.head->prev = l.head;
    for (int k = 0; k < 4; ++k) {
        CWall w;
        w.Point1.X = (float)k; w.Point1.Y = 0.5f;
        w.Point2.X = (float)k + 1.0f; w.Point2.Y = 1.5f;
        w.ToInternal.X = 0.0f; w.ToInternal.Y = -1.0f;
        PushBackWall(l, w);
    }
    Check(g_nodes == 4);
    int count = 0;
    for (SWallNode* n = l.head->next; n != l.head; n = n->next, ++count) {
        Check(Near(n->data.Point1.X, (float)count) && Near(n->data.Point2.X, (float)count + 1.0f));
        Check(Near(n->data.Point2.Y, 1.5f) && Near(n->data.ToInternal.Y, -1.0f));
        Check(n->next->prev == n);
    }
    Check(count == 4);
    Check(Near(l.head->prev->data.Point1.X, 3.0f));
    SWallNode* c = l.head->next;
    while (c != l.head) { SWallNode* t = c; c = c->next; free(t); --g_nodes; }
    free(l.head);
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a88b10_TEST FAIL\n");
        return 1;
    }
    std::printf("00a88b10_TEST PASS\n");
    return 0;
}