#include <cstdio>
#include <cstdlib>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; }; static const char g_empty[] = ""; class CCharString { public: CCharStringData* m_pStringData; unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; } const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; } };
static const char* g_chars = 0; static unsigned long g_key = 99; static void* g_entry = 0; static void* g_record = 0; static long g_a, g_b, g_c;
static unsigned long Calc(unsigned long, const char* d, unsigned long n) { g_chars = d; return n * 7; } class CMap { public: void* LookupValue(unsigned long k) { g_key = k; return (void*)&g_entry; } }; struct SWorld { unsigned char _pad[0x14]; CMap* m_pTextMap; }; static CMap g_map; static SWorld g_world; static SWorld* g_pWorld = &g_world;
struct CStream { unsigned char _pad[0x38]; unsigned char m_record[4]; void Play(void* e, void* r, long a, long b, long c) { g_entry = e; g_record = r; g_a = a; g_b = b; g_c = c; }
    void OpenStringSource(const CCharString& name, long a, long b, long c) { g_world.m_pTextMap = &g_map; CMap* map = g_pWorld->m_pTextMap; void* entry = map->LookupValue(Calc(0, name.GetChars(), name.GetLength())); Play(entry, m_record, a, b, c); } };

int main() {
    CStream s; std::memset(&s, 0, sizeof s); CCharStringData d; d.m_pChars = "abc"; d.m_length = 3; CCharString name; name.m_pStringData = &d; s.OpenStringSource(name, 1, 2, 3);
    if (g_key != 3 * 7 || g_entry != (void*)&g_entry || g_record != s.m_record || g_a != 1 || g_b != 2 || g_c != 3 || std::strcmp(g_chars, "abc")) { std::printf("006e3ab0_TEST FAIL\n"); return 1; } name.m_pStringData = 0; s.OpenStringSource(name, 4, 5, 6); if (g_key != 0 || g_chars != g_empty || g_c != 6) { std::printf("006e3ab0_TEST FAIL\n"); return 1; }
    std::printf("006e3ab0_TEST PASS\n"); return 0;
}