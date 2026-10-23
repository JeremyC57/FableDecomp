// Behaviour test for CTCHeroOnlineScoreboard::DownloadListOfMyClans (005615c0): a reply not led by "SID:OK" is busy; otherwise (name, id, description) triples are appended, "NO_DESCRIPTION" leaving the description empty
#include <cstdio>
#include <cstring>
#include <cstdlib>

enum { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001 };
struct ClanInfo { const char* Name; long ClanID; const char* Description; };

static int Parse(const char* const* lines, int count, ClanInfo* out, int* outCount)
{
    *outCount = 0;
    if (count > 0) {
        if (std::strcmp(lines[0], "SID:OK") != 0)
            return NETWORK_BUSY;
        for (int i = 1; i < count; ) {
            const char* name = lines[i++];
            long id = std::atoi(lines[i++]);
            const char* description = lines[i++];
            ClanInfo info;
            info.Name = name;
            info.ClanID = id;
            info.Description = "";
            if (std::strcmp(description, "NO_DESCRIPTION") != 0)
                info.Description = description;
            out[(*outCount)++] = info;
        }
    }
    return NETWORK_SUCCESS;
}

int main()
{
    const char* good[7] = { "SID:OK", "Wolves", "42", "We hunt", "Bears", "7", "NO_DESCRIPTION" };
    const char* bad[1] = { "SID:FAIL" };
    ClanInfo out[4];
    int n = 0;
    if (Parse(bad, 1, out, &n) != NETWORK_BUSY || n != 0) { std::printf("005615c0_TEST FAIL\n"); return 1; }
    if (Parse(good, 7, out, &n) != NETWORK_SUCCESS || n != 2) { std::printf("005615c0_TEST FAIL\n"); return 1; }
    if (out[0].ClanID != 42 || std::strcmp(out[0].Description, "We hunt") != 0 || out[1].ClanID != 7 ||
        std::strcmp(out[1].Description, "") != 0 || std::strcmp(out[1].Name, "Bears") != 0) {
        std::printf("005615c0_TEST FAIL\n");
        return 1;
    }
    std::printf("005615c0_TEST PASS\n");
    return 0;
}
