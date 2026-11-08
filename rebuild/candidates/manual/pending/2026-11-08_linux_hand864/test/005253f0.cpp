// behaviour test: CThingManager::LoadGameState -- the inline CCharString::Replace (every "\r\n" removed, repeated matches), the
// UNATTACHED -> "" definition-name rule and the NPC name table filled per UID (SetNameForThing + NPCNameMap[uid].first)
#include <stdio.h>
#include <string.h>
static void Replace(char* text, const char* search, const char* replacement)
{
    char result[256]; result[0] = 0;
    char* cur = text;
    char* found = strstr(cur, search);
    while (found) {
        *found = 0;
        found += strlen(search);
        strcat(result, cur);
        strcat(result, replacement);
        cur = found;
        found = strstr(found, search);
    }
    strcat(result, cur);
    strcpy(text, result);
}
static long atoi_like(const char* s) { long v = 0; while (*s >= '0' && *s <= '9') v = v * 10 + (*s++ - '0'); return v; }
struct Entry { unsigned long uid_lo; char def[32]; char name[32]; };
static Entry g_table[8]; static int g_count;
static void Store(unsigned long uid, const char* def, const char* name)
{
    for (int i = 0; i < g_count; ++i) if (g_table[i].uid_lo == uid) { strcpy(g_table[i].def, def); strcpy(g_table[i].name, name); return; }
    g_table[g_count].uid_lo = uid; strcpy(g_table[g_count].def, def); strcpy(g_table[g_count].name, name); ++g_count;
}
int main()
{
    char a[64] = "12\r\n"; Replace(a, "\r\n", "");
    char b[64] = "a\r\nb\r\nc"; Replace(b, "\r\n", "");
    char c[64] = "none"; Replace(c, "\r\n", "");
    if (strcmp(a, "12") || strcmp(b, "abc") || strcmp(c, "none")) { printf("005253f0_TEST FAIL\n"); return 1; }
    const char* lines[] = { "2\r\n", "7\r\n", "UNATTACHED\r\n", "Bob\r\n", "9\r\n", "CREATURE_GUARD\r\n", "Ann\r\n" };
    int li = 0; char line[64];
    strcpy(line, lines[li++]); Replace(line, "\r\n", "");
    unsigned long count = (unsigned long)atoi_like(line);
    for (unsigned long i = 0; i < count; ++i) {
        strcpy(line, lines[li++]); Replace(line, "\r\n", ""); unsigned long uid = (unsigned long)atoi_like(line);
        strcpy(line, lines[li++]); Replace(line, "\r\n", ""); if (strcmp(line, "UNATTACHED") == 0) line[0] = 0;
        char def[32]; strcpy(def, line);
        strcpy(line, lines[li++]); Replace(line, "\r\n", "");
        Store(uid, def, line);
    }
    if (g_count != 2 || g_table[0].uid_lo != 7 || g_table[0].def[0] != 0 || strcmp(g_table[0].name, "Bob") || g_table[1].uid_lo != 9
        || strcmp(g_table[1].def, "CREATURE_GUARD") || strcmp(g_table[1].name, "Ann")) { printf("005253f0_TEST FAIL\n"); return 1; }
    printf("005253f0_TEST PASS\n");
    return 0;
}
