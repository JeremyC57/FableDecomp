// Behaviour test for CTCStealableItemLocation::OnSerialise (0x007B2800): both radii transferred in order, zero defaults.
#include <cstdio>
#include <cstring>

static const char* g_names[2];
static float* g_values[2];
static float g_defaults[2] = { -1.0f, -1.0f };
static int g_count = 0;

struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue)
    {
        g_names[g_count] = name; g_values[g_count] = &value; g_defaults[g_count] = defaultValue; ++g_count;
    }
};

struct CTCStealableItemLocation {
    float RadiusToBeWithin;
    float RadiusToTakeItemsBackTo;
    void OnSerialise(CPersistContext* context, long unused);
};

void CTCStealableItemLocation::OnSerialise(CPersistContext* context, long)
{
    float defaultWithin = 0.0f;
    context->Transfer("RadiusToBeWithin", RadiusToBeWithin, defaultWithin);
    float defaultBack = 0.0f;
    context->Transfer("RadiusToTakeItemsBackTo", RadiusToTakeItemsBackTo, defaultBack);
}

int main()
{
    CPersistContext context;
    CTCStealableItemLocation l; l.RadiusToBeWithin = 1.0f; l.RadiusToTakeItemsBackTo = 2.0f;
    l.OnSerialise(&context, 0);
    if (g_count != 2 || std::strcmp(g_names[0], "RadiusToBeWithin") != 0 || std::strcmp(g_names[1], "RadiusToTakeItemsBackTo") != 0
        || g_values[0] != &l.RadiusToBeWithin || g_values[1] != &l.RadiusToTakeItemsBackTo || g_defaults[0] != 0.0f || g_defaults[1] != 0.0f) {
        std::printf("007b2800_TEST FAIL\n"); return 1;
    }
    std::printf("007b2800_TEST PASS\n");
    return 0;
}
