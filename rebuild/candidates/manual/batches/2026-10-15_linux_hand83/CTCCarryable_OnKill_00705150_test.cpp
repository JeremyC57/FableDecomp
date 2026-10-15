// Behaviour test for 00705150 (CTCCarryable::OnKill): a carried thing asks its carrier to drop it; an uncarried one does nothing.
#include <cstdio>

struct CThing { long m_id; };

static CThing* g_removed = 0;
static int     g_calls = 0;

struct CTCCarrying {
    void RemoveThing(CThing* thing) { g_removed = thing; ++g_calls; }
};

struct CTCBase {
    CThing* ParentThing;
};

struct CTCCarryable : CTCBase {
    bool          ForceFragility;
    CTCCarrying*  PCarryingTC;
    void OnKill()
    {
        if (PCarryingTC)
            PCarryingTC->RemoveThing(ParentThing);
    }
};

int main()
{
    CThing thing = { 7 };
    CTCCarrying carrier;
    CTCCarryable c;
    c.ParentThing = &thing; c.ForceFragility = false; c.PCarryingTC = 0;
    c.OnKill();
    if (g_calls != 0) { std::printf("00705150_TEST FAIL\n"); return 1; }
    c.PCarryingTC = &carrier;
    c.OnKill();
    if (g_calls != 1 || g_removed != &thing) { std::printf("00705150_TEST FAIL\n"); return 1; }
    std::printf("00705150_TEST PASS\n");
    return 0;
}
