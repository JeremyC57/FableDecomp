// behaviour test for 00b3a180 CEnginePrimitiveAttachmentManager::SetAttachedPrimitive: a handle already holding an
// attached primitive (type 0x2e) is re-attached in place; otherwise a new attached primitive is created, the handle is
// pointed at it (old reference released) and it is appended to the manager's list; a failed attach releases the handle
#include <cstdio>
struct Obj { int refs, type; bool attachOk; int attachCalls; };
struct Handle { Obj* obj; };
static Obj g_pool[8];
static int g_used = 0, g_listCount = 0, g_destroyed = 0;
static Obj* g_list[8];
static void Release(Obj* o) { if (--o->refs == 0) ++g_destroyed; }
static void SetAttached(Handle& h, bool ok)
{
    if (!h.obj || h.obj->type != 0x2e) {
        Obj* created = &g_pool[g_used++];
        created->refs = 1; created->type = 0x2e; created->attachOk = ok; created->attachCalls = 0;
        if (h.obj) Release(h.obj);
        h.obj = created; ++created->refs;           // handle copy
        g_list[g_listCount++] = created; ++created->refs; // list copy
        --created->refs;                            // local info destroyed
    }
    ++h.obj->attachCalls;
    if (!h.obj->attachOk) { Release(h.obj); h.obj = 0; }
}
int main()
{
    Obj other = { 1, 3, true, 0 };
    Handle h = { &other };
    SetAttached(h, true);
    if (g_destroyed != 1 || g_listCount != 1 || h.obj != g_list[0] || h.obj->refs != 2 || h.obj->attachCalls != 1) {
        std::printf("00b3a180_TEST FAIL\n"); return 1; }
    SetAttached(h, true);
    if (g_listCount != 1 || h.obj->attachCalls != 2) { std::printf("00b3a180_TEST FAIL\n"); return 1; }
    Handle empty = { 0 };
    SetAttached(empty, false);
    if (g_listCount != 2 || empty.obj != 0 || g_list[1]->refs != 1 || g_list[1]->attachCalls != 1) {
        std::printf("00b3a180_TEST FAIL\n"); return 1; }
    std::printf("00b3a180_TEST PASS\n");
    return 0;
}
