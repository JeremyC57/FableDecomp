// Behaviour test for the pair<uint64, CPersistInfo> scalar deleting destructor (0x00CB9200): the persist-script string is
// destroyed and the block freed.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_strings;
struct Str { Str() { ++g_strings; } ~Str() { --g_strings; } };
struct Info { unsigned long long UID; Str PersistScript; };
struct Entry { unsigned long long first; Info second; };
int main()
{
    Entry* e = new Entry;
    bool before = g_strings == 1;
    delete e;
    bool ok = before && g_strings == 0;
    printf(ok ? "00cb9200_TEST PASS\n" : "00cb9200_TEST FAIL\n");
    return ok ? 0 : 1;
}