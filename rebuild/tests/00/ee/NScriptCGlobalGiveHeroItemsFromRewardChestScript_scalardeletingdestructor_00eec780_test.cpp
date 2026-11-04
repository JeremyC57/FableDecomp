#include <cstdio>

class CGlobal_GiveHeroItemsFromRewardChestScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGlobal_GiveHeroItemsFromRewardChestScript* g_last_dtor = 0;

void __fastcall CGlobal_GiveHeroItemsFromRewardChestScript_dtor_00cbd510(CGlobal_GiveHeroItemsFromRewardChestScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGlobal_GiveHeroItemsFromRewardChestScript_vector_deleting_destructor(CGlobal_GiveHeroItemsFromRewardChestScript* self, int edx_unused, unsigned int flags)
{
    CGlobal_GiveHeroItemsFromRewardChestScript_dtor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CGlobal_GiveHeroItemsFromRewardChestScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGlobal_GiveHeroItemsFromRewardChestScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGlobal_GiveHeroItemsFromRewardChestScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGlobal_GiveHeroItemsFromRewardChestScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00eec780_TEST PASS\n");
    return 0;
}