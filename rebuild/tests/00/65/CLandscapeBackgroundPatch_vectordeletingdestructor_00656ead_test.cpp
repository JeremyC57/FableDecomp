// Behaviour model of ??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z 0x00656EAD: destroy the element (the base
// CQuestInfoElement's PrimitiveHandles vector, after the vptr / ID / ScriptID / Scale), then free the storage only when bit 0 of the
// flags is set; returns this. Its only retail caller, CCountedPointer<CQuestInfoElementCounterList>::DeleteData, passes 1.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtor_calls = 0, g_delete_calls = 0;
static const void* g_last_dtor_self = 0;
static const void* g_last_delete_self = 0;
struct CPrimitiveHandleVector { void* Start; void* Finish; void* EndOfStorage; ~CPrimitiveHandleVector() { g_dtor_calls++; g_last_dtor_self = this; } };
struct CQuestInfoElementCounterList {
    void* VTable; int ID, ScriptID; float Scale; CPrimitiveHandleVector PrimitiveHandles; int Graphic, Value, MaxValue;
    static void operator delete(void* p) { g_delete_calls++; g_last_delete_self = p; std::free(p); }
};
static void* ScalarDeletingDestructor(CQuestInfoElementCounterList* self, unsigned int flags)
{
    self->~CQuestInfoElementCounterList();
    if (flags & 1)
        CQuestInfoElementCounterList::operator delete(self);
    return self;
}
int main()
{
    void* mem = std::malloc(sizeof(CQuestInfoElementCounterList));
    CQuestInfoElementCounterList* p = new (mem) CQuestInfoElementCounterList;
    void* r = ScalarDeletingDestructor(p, 0);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL flag0\n"); return 1; }
    if (g_last_dtor_self != &p->PrimitiveHandles) { std::printf("FAIL member\n"); return 1; }
    p = new (mem) CQuestInfoElementCounterList;
    r = ScalarDeletingDestructor(p, 2);
    if (r != p || g_dtor_calls != 2 || g_delete_calls != 0) { std::printf("FAIL flag2\n"); return 1; }
    p = new (mem) CQuestInfoElementCounterList;
    r = ScalarDeletingDestructor(p, 1);
    if (r != p || g_dtor_calls != 3 || g_delete_calls != 1 || g_last_delete_self != p) { std::printf("FAIL flag1\n"); return 1; }
    std::printf("VDD_OK\n");
    return 0;
}
