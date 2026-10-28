#include <cstdio>
#include <cstring>

class CCharString { public: const char* m_text; const CCharString& operator=(const char* t) { m_text = t; return *this; } };
struct CTCActivationReceptorCreatureGenerator {
    bool TriggerOnActivate, ActivateOnActivate;
    bool Validate(CCharString& error) const
    {
        if ((ActivateOnActivate && !TriggerOnActivate) || (!ActivateOnActivate && TriggerOnActivate))
            return true;
        error = "must set one and only one creature generator flag, not both or none!!!";
        return false;
    }
};
int main()
{
    CTCActivationReceptorCreatureGenerator g; CCharString e; e.m_text = "";
    for (int i = 0; i < 4; ++i) {
        g.ActivateOnActivate = (i & 1) != 0; g.TriggerOnActivate = (i & 2) != 0; e.m_text = "";
        bool ok = g.Validate(e);
        bool expect = (i == 1 || i == 2);
        if (ok != expect) { std::printf("007f75c0_TEST FAIL\n"); return 1; }
        if (!ok && std::strncmp(e.m_text, "must set one and only one", 25) != 0) { std::printf("007f75c0_TEST FAIL\n"); return 1; }
        if (ok && e.m_text[0] != 0) { std::printf("007f75c0_TEST FAIL\n"); return 1; }
    }
    std::printf("007f75c0_TEST PASS\n");
    return 0;
}