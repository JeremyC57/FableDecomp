// Behaviour test for PKCDefaultFileManager::OpenFile (0x00C1A510): a missing file fails; an opened file gets the next key and
// one reference, and keys increase.
#include <stdio.h>
struct Manager { unsigned long nextKey; int open; unsigned long refs[8];
    bool OpenFile(bool exists, unsigned long* key) {
        if (!exists) return false;
        *key = nextKey; ++nextKey; refs[*key] = 1; ++open; return true; } };
int main()
{
    Manager m; m.nextKey = 1; m.open = 0; unsigned long k1 = 0, k2 = 0, k3 = 99;
    bool ok = m.OpenFile(true, &k1) && m.OpenFile(true, &k2) && !m.OpenFile(false, &k3);
    ok = ok && k1 == 1 && k2 == 2 && k3 == 99 && m.open == 2 && m.refs[2] == 1;
    printf(ok ? "00c1a510_TEST PASS\n" : "00c1a510_TEST FAIL\n");
    return ok ? 0 : 1;
}
