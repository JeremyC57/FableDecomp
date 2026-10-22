#include <cstdio>

struct CTCFamilyMember {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCFamilyMember* g_last_dtor = 0;

void __fastcall CTCFamilyMember_dtor(CTCFamilyMember* member)
{
    g_last_dtor = member;
    member->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCFamilyMember_scalar_deleting_destructor(CTCFamilyMember* member, int edx_unused, unsigned int flags)
{
    CTCFamilyMember_dtor(member);
    if (flags & 1)
        operator delete(member);
    return member;
}

int main()
{
    CTCFamilyMember a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCFamilyMember_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCFamilyMember b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCFamilyMember_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}