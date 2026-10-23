#include <cstdio>

struct CScriptConversation {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScriptConversation* g_last_dtor = 0;

void __fastcall CScriptConversation_dtor(CScriptConversation* conversation)
{
    g_last_dtor = conversation;
    conversation->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScriptConversation_scalar_deleting_destructor(CScriptConversation* conversation, int edx_unused, unsigned int flags)
{
    CScriptConversation_dtor(conversation);
    if (flags & 1)
        operator delete(conversation);
    return conversation;
}

int main()
{
    CScriptConversation a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScriptConversation_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScriptConversation b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScriptConversation_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00718840_TEST PASS\n");
    return 0;
}