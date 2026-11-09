// Behaviour test for ReadBankData (0x00C30F60): the first failing check logs its message and stops; a good bank loads all
// three tables.
#include <stdio.h>
#include <string.h>
struct Bank { int errors; long version; bool segments; bool wav; int loaded; };
static const char* lastError;
static void Read(Bank& b)
{
    if (b.errors) { lastError = "There were problems decoding the file segment structure"; return; }
    if (b.version < 0) { lastError = "Could not extract the version number"; return; }
    if (!b.segments) { lastError = "This bank is not a normal LUG!"; return; }
    if (b.version != 0) { lastError = "Expecting normal version number "; return; }
    if (!b.wav) { lastError = "Could not open the wav segment"; return; }
    b.loaded = 3;
}
int main()
{
    Bank good = { 0, 0, true, true, 0 }; Read(good);
    Bank v = { 0, 2, true, true, 0 }; lastError = 0; Read(v);
    bool ok = good.loaded == 3 && v.loaded == 0 && lastError && strncmp(lastError, "Expecting", 9) == 0;
    Bank broken = { 1, 0, true, true, 0 }; Read(broken);
    ok = ok && broken.loaded == 0 && strncmp(lastError, "There were", 10) == 0;
    printf(ok ? "01214077_TEST PASS\n" : "00c30f60_TEST FAIL\n");
    return ok ? 0 : 1;
}