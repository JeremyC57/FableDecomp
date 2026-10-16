#include <cstdio>
// Behaviour model of CLipSyncDataBank::CLipSyncDataBank (0x00A42B80): empty entries, no phoneme bank, SkipUpdate
// false, static-map flag cleared, a fresh resource bank owned.
struct Bank { int id; };
struct LipSync { void* first; void* last; void* end; Bank* resources; bool skipUpdate; void* phonemes; bool staticMap;
    LipSync(Bank* fresh) : first(0), last(0), end(0), resources(0), skipUpdate(false), phonemes(0), staticMap(true)
    { staticMap = false; resources = fresh; } };
int main()
{
    Bank b = { 1 };
    LipSync l(&b);
    if (l.first || l.last || l.end || l.skipUpdate || l.phonemes || l.staticMap || l.resources != &b) { std::printf("00a42b80_TEST FAIL\n"); return 1; }
    std::printf("00a42b80_TEST PASS\n");
    return 0;
}
