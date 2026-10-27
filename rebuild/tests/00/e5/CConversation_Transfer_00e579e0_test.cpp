#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Vec { int items[4]; int n; };
class CPersistContext { public:
    void Transfer(const char* name, int& v, const int& d) { char b[64]; std::sprintf(b, "%s=%d(def %d)", name, v, d); Log(b); }
    void TransferVector(const char* name, Vec& v) { char b[64]; std::sprintf(b, "%s[%d]", name, v.n); Log(b); }
};
struct Base { int GlobalIndex; };
class CConversation : public Base { public: int Lines; Vec Speaker, Dialogue, Animation, AnimLoop;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("Lines", Lines, 0);
        context.TransferVector("Speaker", Speaker);
        context.TransferVector("Dialogue", Dialogue);
        context.TransferVector("Animation", Animation);
        context.TransferVector("AnimLoop", AnimLoop);
    }
    void Copy(const Base* other) { *this = *(const CConversation*)other; }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    CConversation c; std::memset(&c, 0, sizeof(c)); c.Lines = 3; c.Speaker.n = 3; c.Dialogue.n = 3; c.Animation.n = 1; c.AnimLoop.n = 2;
    CPersistContext p; c.Transfer(p);
    ok = ok && std::strcmp(g_log, "Lines=3(def 0);Speaker[3];Dialogue[3];Animation[1];AnimLoop[2];") == 0 && (true);
    if (!ok) std::printf("log: %s\n", g_log);
    if (!ok) { std::printf("00e579e0_TEST FAIL\n"); return 1; }
    std::printf("00e579e0_TEST PASS\n");
    return 0;
}