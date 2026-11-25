// Behaviour model of ??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z 0x004DBE39: destroy the pair (the intelligent pointer in
// CGroupMemberData, at +8), then free the storage only when bit 0 of the flags is set; returns this.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtor_calls = 0, g_delete_calls = 0;
static const void* g_last_dtor_self = 0;
static const void* g_last_delete_self = 0;
struct CBaseIntelligentPointer { void* PData; ~CBaseIntelligentPointer() { g_dtor_calls++; g_last_dtor_self = this; } };
struct CGroupMemberData { CBaseIntelligentPointer PMember; bool IsActive; unsigned long JoinedFrame, DeactivatedFrame; };
struct CGroupMemberPair {
    unsigned long first_lo, first_hi; CGroupMemberData second;
    static void operator delete(void* p) { g_delete_calls++; g_last_delete_self = p; std::free(p); }
};
static void* ScalarDeletingDestructor(CGroupMemberPair* self, unsigned int flags)
{
    self->~CGroupMemberPair();
    if (flags & 1)
        CGroupMemberPair::operator delete(self);
    return self;
}
int main()
{
    void* mem = std::malloc(sizeof(CGroupMemberPair));
    CGroupMemberPair* p = new (mem) CGroupMemberPair;
    void* r = ScalarDeletingDestructor(p, 0);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL flag0\n"); return 1; }
    if (g_last_dtor_self != (char*)p + 8) { std::printf("FAIL member\n"); return 1; }
    p = new (mem) CGroupMemberPair;
    r = ScalarDeletingDestructor(p, 2);
    if (r != p || g_dtor_calls != 2 || g_delete_calls != 0) { std::printf("FAIL flag2\n"); return 1; }
    p = new (mem) CGroupMemberPair;
    r = ScalarDeletingDestructor(p, 1);
    if (r != p || g_dtor_calls != 3 || g_delete_calls != 1 || g_last_delete_self != p) { std::printf("FAIL flag1\n"); return 1; }
    std::printf("BEHAVIOUR_OK_004dbe39\n");
    return 0;
}
