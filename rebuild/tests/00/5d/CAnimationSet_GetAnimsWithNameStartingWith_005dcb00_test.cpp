// Behaviour test for CAnimationSet::GetAnimsWithNameStartingWith (0x005DCB00), g++ model of the retail logic.
#include <cstdio>
#include <cstring>
#include <vector>
struct Entry { unsigned long key; const char* name; bool accept; };
static void Collect(const Entry* e, int n, const char* prefix, std::vector<const Entry*>& out)
{
    bool skip = false; unsigned long last = 0;
    for (int i = 0; i < n; ++i) {
        unsigned long key = e[i].key;
        if (key != last || !skip) {
            if (std::strncmp(e[i].name, prefix, std::strlen(prefix)) == 0) {
                if (e[i].accept) out.push_back(&e[i]);
                skip = false;
            } else
                skip = true;
            last = key;
        }
    }
}
int main()
{
    Entry e[] = { { 1, "WALK_A", true }, { 1, "RUN", true }, { 1, "WALK_B", true }, { 2, "WALK_C", false }, { 3, "WALK_D", true } };
    std::vector<const Entry*> out;
    Collect(e, 5, "WALK", out);
    int ok = out.size() == 2 && out[0] == &e[0] && out[1] == &e[4];   // WALK_B skipped: same key after a failed match
    std::printf(ok ? "005dcb00_TEST PASS\n" : "005dcb00_TEST FAIL\n");
    return ok ? 0 : 1;
}