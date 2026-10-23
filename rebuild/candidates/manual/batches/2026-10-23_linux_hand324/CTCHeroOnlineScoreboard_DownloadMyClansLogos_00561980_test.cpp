// Behaviour test for CTCHeroOnlineScoreboard::DownloadMyClansLogos (0x00561980): request only when idle (state 100/101),
// busy reply -> 1001; on success "SID:OK" -> 1001, else each (clan, logo, size, data) group is written to
// ClanData\<clan>\logo.<logo>.jpg with <size> bytes, and the connection is dropped.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_paths[4][64];
static long g_sizes[4];
static int g_writes = 0;
static int g_requests = 0;
static int g_resets = 0;

struct CModel {
    long TaskState;
    int openResult;
    int transactionResult;
    const char* const* reply;
    int replyCount;
    long Run()
    {
        if (TaskState == 100 || TaskState == 101) {
            ++g_requests;
            bool busy = openResult == 1001;
            if (busy)
                return 1001;
        }
        long result = transactionResult;
        if (result == 1000) {
            int count = replyCount;
            if (count > 0) {
                if (std::strcmp(reply[0], "SID:OK") != 0)
                    return 1001;
                for (int i = 1; i < count; ) {
                    long clanId = std::atoi(reply[i++]);
                    long logoId = std::atoi(reply[i++]);
                    long size = std::atoi(reply[i++]);
                    if (g_writes < 4) {
                        std::sprintf(g_paths[g_writes], "ClanData\\%ld\\logo.%ld.jpg", clanId, logoId);
                        g_sizes[g_writes] = size;
                    }
                    ++g_writes;
                    i++;
                }
            }
            ++g_resets;
        }
        return result;
    }
};

int main()
{
    static const char* const reply[] = { "SID:OK", "7", "3", "12", "data1", "8", "4", "5", "data2" };
    CModel m;
    m.TaskState = 100; m.openResult = 1001; m.transactionResult = 1000; m.reply = reply; m.replyCount = 9;
    if (m.Run() != 1001 || g_requests != 1 || g_writes != 0) { std::printf("00561980_TEST FAIL\n"); return 1; }
    m.TaskState = 5;
    if (m.Run() != 1000 || g_writes != 2 || g_resets != 1) { std::printf("00561980_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_paths[0], "ClanData\\7\\logo.3.jpg") != 0 || g_sizes[0] != 12) { std::printf("00561980_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_paths[1], "ClanData\\8\\logo.4.jpg") != 0 || g_sizes[1] != 5) { std::printf("00561980_TEST FAIL\n"); return 1; }
    static const char* const bad[] = { "SID:ERR" };
    m.reply = bad; m.replyCount = 1;
    if (m.Run() != 1001) { std::printf("00561980_TEST FAIL\n"); return 1; }
    m.transactionResult = 1002;
    if (m.Run() != 1002 || g_resets != 1) { std::printf("00561980_TEST FAIL\n"); return 1; }
    std::printf("00561980_TEST PASS\n");
    return 0;
}
