#include <cstdio>
#include <stdlib.h>

static int g_dtors = 0;
struct CPrimitive { long Type; ~CPrimitive() { ++g_dtors; } };
struct CPrimitiveListNode { CPrimitiveListNode* m_next; CPrimitiveListNode* m_prev; CPrimitive m_value; };
struct CPrimitiveList { CPrimitiveListNode* m_head; };
struct CTCGraphicAppearance {
    CPrimitiveList Primitives;
    void RemovePrimitive(CPrimitive* primitive)
    {
        for (CPrimitiveListNode* node = Primitives.m_head->m_next; node != Primitives.m_head; node = node->m_next) {
            if (&node->m_value == primitive) {
                CPrimitiveListNode* next = node->m_next;
                CPrimitiveListNode* prev = node->m_prev;
                prev->m_next = next;
                next->m_prev = prev;
                node->m_value.~CPrimitive();
                free(node);
                return;
            }
        }
    }
};

static CPrimitiveListNode* NewNode(long type)
{
    CPrimitiveListNode* n = (CPrimitiveListNode*)malloc(sizeof(CPrimitiveListNode));
    n->m_value.Type = type;
    return n;
}

int main()
{
    CPrimitiveListNode head; CPrimitiveListNode* a = NewNode(1); CPrimitiveListNode* b = NewNode(2); CPrimitiveListNode* c = NewNode(3);
    head.m_next = a; a->m_prev = &head; a->m_next = b; b->m_prev = a; b->m_next = c; c->m_prev = b; c->m_next = &head; head.m_prev = c;
    CTCGraphicAppearance app; app.Primitives.m_head = &head;
    CPrimitive stranger; stranger.Type = 9;
    app.RemovePrimitive(&stranger);                     // not in the list: untouched
    if (g_dtors != 0 || head.m_next != a) { std::printf("004bf920_TEST FAIL\n"); return 1; }
    app.RemovePrimitive(&b->m_value);
    if (g_dtors != 1 || a->m_next != c || c->m_prev != a) { std::printf("004bf920_TEST FAIL\n"); return 1; }
    app.RemovePrimitive(&a->m_value); app.RemovePrimitive(&c->m_value);
    if (g_dtors != 3 || head.m_next != &head || head.m_prev != &head) { std::printf("004bf920_TEST FAIL\n"); return 1; }
    std::printf("004bf920_TEST PASS\n");
    return 0;
}