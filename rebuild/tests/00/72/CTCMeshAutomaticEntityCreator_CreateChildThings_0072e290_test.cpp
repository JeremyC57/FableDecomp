// Behaviour test for CTCMeshAutomaticEntityCreator::CreateChildThings (0x0072E290): consecutive duplicate dummy names are
// skipped; the CREATEOBJECT / CREATEBUILDING / CREATEPARTICLE prefix selects the creator, anything else creates nothing.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* kind, const char* dummy, const char* name)
{
    std::strcat(g_log, kind); std::strcat(g_log, ":"); std::strcat(g_log, dummy); std::strcat(g_log, ":");
    std::strcat(g_log, name); std::strcat(g_log, ";");
}
static bool Skip(const char*& p, const char* item)
{
    while (*p == ' ') ++p;
    unsigned int n = (unsigned int)std::strlen(item);
    if (std::strncmp(p, item, n) != 0) return false;
    p += n;
    return true;
}
static void ReadName(const char* p, char* out)
{
    while (*p == ' ') ++p;
    std::strcpy(out, p);
}

static void CreateChildThings(const char* const* names, int count)
{
    const char* last = "";
    for (int i = 0; i < count; ++i) {
        if (std::strcmp(last, names[i]) != 0) {
            const char* p = names[i];
            char name[64];
            if (Skip(p, "CREATEOBJECT")) { ReadName(p, name); Log("obj", names[i], name); }
            else {
                p = names[i];
                if (Skip(p, "CREATEBUILDING")) { ReadName(p, name); Log("bld", names[i], name); }
                else {
                    p = names[i];
                    if (Skip(p, "CREATEPARTICLE")) { ReadName(p, name); Log("ptc", names[i], name); }
                }
            }
        }
        last = names[i];
    }
}

int main()
{
    const char* names[5] = { "CREATEOBJECT A", "CREATEOBJECT A", "CREATEBUILDING B", "OTHER", "CREATEPARTICLE C" };
    g_log[0] = 0;
    CreateChildThings(names, 5);
    if (std::strcmp(g_log, "obj:CREATEOBJECT A:A;bld:CREATEBUILDING B:B;ptc:CREATEPARTICLE C:C;") != 0) {
        std::printf("0072e290_TEST FAIL\n"); return 1;
    }
    std::printf("0072e290_TEST PASS\n");
    return 0;
}