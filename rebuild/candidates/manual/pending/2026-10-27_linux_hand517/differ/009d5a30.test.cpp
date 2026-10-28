// Test: CLoadingMemoryFailureHandler::UnregisterBank -- finds the bank and erases it (tail moved down one slot); unknown banks leave the array alone
#include <cstdio>
#include <cstring>
struct CBankFileAsync { int id; };
struct CHandler {
    CBankFileAsync* store[4]; CBankFileAsync** first; CBankFileAsync** last;
    void UnregisterBank(CBankFileAsync* bank)
    {
        CBankFileAsync** it = first;
        while (it != last && *it != bank) ++it;
        if (it != last) {
            CBankFileAsync** next = it + 1;
            if (next != last) std::memmove(it, next, (const char*)last - (const char*)next);
            --last;
        }
    }
};
int main()
{
    CBankFileAsync a = { 1 }, b = { 2 }, c = { 3 }, d = { 4 };
    CHandler h; h.store[0] = &a; h.store[1] = &b; h.store[2] = &c; h.first = h.store; h.last = h.store + 3;
    h.UnregisterBank(&b); h.UnregisterBank(&d); h.UnregisterBank(&c);
    if (h.last - h.first != 1 || h.store[0] != &a) { std::printf("009d5a30_TEST FAIL\n"); return 1; }
    std::printf("009d5a30_TEST PASS\n");
    return 0;
}
