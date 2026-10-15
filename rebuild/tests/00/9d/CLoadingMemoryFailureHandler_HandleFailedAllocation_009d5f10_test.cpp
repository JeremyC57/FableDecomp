// Behaviour test for 0x009D5F10 (CLoadingMemoryFailureHandler::HandleFailedAllocation): first bank that frees memory wins; lock released on both exits.
#include <cstdio>
static int g_locks;
struct Lock { Lock() { ++g_locks; } ~Lock() { --g_locks; } };
struct Bank { bool frees; int asked; bool Handle() { ++asked; return frees; } };
struct Handler {
    Bank** first; Bank** last;
    unsigned int Size() const { return (unsigned int)(last - first); }
    bool HandleFailedAllocation(unsigned int)
    {
        Lock lock;
        for (unsigned int i = 0; i < Size(); ++i) if (first[i]->Handle()) return true;
        return false;
    }
};
int main()
{
    Bank a = { false, 0 }, b = { true, 0 }, c = { true, 0 };
    Bank* banks[3] = { &a, &b, &c };
    Handler h; h.first = banks; h.last = banks + 3;
    int ok = h.HandleFailedAllocation(100) && a.asked == 1 && b.asked == 1 && c.asked == 0 && g_locks == 0;
    h.last = banks + 1; ok &= !h.HandleFailedAllocation(100) && a.asked == 2 && g_locks == 0;
    h.last = banks; ok &= !h.HandleFailedAllocation(1);
    if (!ok) { std::printf("009d5f10_TEST FAIL\n"); return 1; }
    std::printf("009d5f10_TEST PASS\n");
    return 0;
}