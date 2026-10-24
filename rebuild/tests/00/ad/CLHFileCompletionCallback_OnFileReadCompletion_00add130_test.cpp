// Behaviour test for 0x00ADD130 CLHFileCompletionCallback::OnFileReadCompletion (forward to the client, then delete this).
#include <cstdio>

static int g_calls = 0, g_deleted = 0; static bool g_lastSuccess = false; static void* g_lastUser = 0;
struct SClient { virtual void OnReadComplete(bool success, void* user) { ++g_calls; g_lastSuccess = success; g_lastUser = user; } virtual ~SClient() { } };
struct SCallbackBase { virtual ~SCallbackBase() { } virtual void OnFileReadCompletion(bool) = 0; };
struct SCompletion : SCallbackBase {
    SClient* m_pClientCallback; void* m_pUserData;
    SCompletion(SClient* c, void* u) : m_pClientCallback(c), m_pUserData(u) { }
    ~SCompletion() { ++g_deleted; }
    // reconstructed body
    void OnFileReadCompletion(bool success) { m_pClientCallback->OnReadComplete(success != false, m_pUserData); delete this; }
};

int main()
{
    SClient client; int token = 0;
    SCallbackBase* cb = new SCompletion(&client, &token);
    cb->OnFileReadCompletion(true);
    if (g_calls != 1 || !g_lastSuccess || g_lastUser != &token || g_deleted != 1) { std::printf("00add130_TEST FAIL\n"); return 1; }
    cb = new SCompletion(&client, 0);
    cb->OnFileReadCompletion(false);
    if (g_calls != 2 || g_lastSuccess || g_lastUser != 0 || g_deleted != 2) { std::printf("00add130_TEST FAIL\n"); return 1; }
    std::printf("00add130_TEST PASS\n");
    return 0;
}