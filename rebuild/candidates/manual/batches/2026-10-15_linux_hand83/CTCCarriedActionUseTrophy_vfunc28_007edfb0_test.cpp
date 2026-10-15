// Behaviour test for 007edfb0 (CTCCarriedActionUseTrophy::vfunc_28): the base hook runs first, then the shared use handler, both with the user.
#include <cstdio>

struct CThing { long m_id; };

static int     g_order[2];
static int     g_count = 0;
static CThing* g_users[2];

struct CTCCarriedActionUseBase {
    void OnUseBase(CThing* user) { g_users[g_count] = user; g_order[g_count++] = 1; }
};

struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    void OnActionUseCommon(CThing* user) { g_users[g_count] = user; g_order[g_count++] = 2; }
    void vfunc_28(CThing* user)
    {
        CTCCarriedActionUseBase::OnUseBase(user);
        OnActionUseCommon(user);
    }
};

int main()
{
    CThing user = { 3 };
    CTCCarriedActionUseTrophy t;
    t.vfunc_28(&user);
    if (g_count != 2 || g_order[0] != 1 || g_order[1] != 2 || g_users[0] != &user || g_users[1] != &user) { std::printf("007edfb0_TEST FAIL\n"); return 1; }
    std::printf("007edfb0_TEST PASS\n");
    return 0;
}
