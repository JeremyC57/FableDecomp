// behaviour test for 0x00700780 COpinionReactionManager::AttachOpinionSource
#include <cstdio>
struct CTCBase { int dummy; };
struct CTCOpinionSource : CTCBase {
    int pushed;
    bool PushNewSourceDef(const char* n) { pushed = n[0]; return n[0] != 0; }
};
static CTCOpinionSource g_added;
static int g_addCalls = 0;
struct CThing {
    bool has; CTCOpinionSource* comp; bool addFails;
    bool GetInterface(CTCOpinionSource** ppOut, long) { if (has) { *ppOut = comp; return true; } return false; }
    CTCBase* AddTC(const char* name, bool, void*) { ++g_addCalls; if (addFails || name[0] != 'C') return 0; return &g_added; }
};
static bool AttachOpinionSource(CThing* thing, const char* defName, CTCOpinionSource** ppSource)
{
    CTCOpinionSource* source;
    if (!ppSource) ppSource = &source;
    if (!thing->GetInterface(ppSource, 0x50))
        *ppSource = (CTCOpinionSource*)thing->AddTC("CTCOpinionSource", false, 0);
    if (*ppSource && (*ppSource)->PushNewSourceDef(defName))
        return true;
    return false;
}
int main()
{
    CTCOpinionSource own; own.pushed = -1; g_added.pushed = -1;
    CThing withTC = { true, &own, false };
    CThing without = { false, 0, false };
    CThing failing = { false, 0, true };
    CTCOpinionSource* out = 0;
    int bad = !AttachOpinionSource(&withTC, "Guard", &out) || out != &own || own.pushed == -1 || g_addCalls != 0;
    bad |= !AttachOpinionSource(&without, "Guard", 0) || g_added.pushed == -1 || g_addCalls != 1;
    bad |= AttachOpinionSource(&failing, "Guard", &out) || out != 0;
    bad |= AttachOpinionSource(&withTC, "", 0);
    if (bad) { std::printf("00700780_TEST FAIL\n"); return 1; }
    std::printf("00700780_TEST PASS\n");
    return 0;
}
