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
    CConversation a, b; std::memset(&a, 0, sizeof(a)); std::memset(&b, 0, sizeof(b));
    a.GlobalIndex = 9; a.Lines = 2; a.Speaker.n = 2; a.Speaker.items[1] = 42; a.AnimLoop.n = 1;
    b.Copy(&a);
    ok = b.GlobalIndex == 9 && b.Lines == 2 && b.Speaker.n == 2 && b.Speaker.items[1] == 42 && b.AnimLoop.n == 1;
    if (!ok) { std::printf("00e54d10_TEST FAIL\n"); return 1; }
    std::printf("00e54d10_TEST PASS\n");
    return 0;
}