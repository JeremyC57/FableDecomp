#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct SScalarTag { SScalarTag() { } }; struct SInputRecord { long m_words[11]; }; static long g_grow = 0, g_count = 0; static bool g_noTail = false; static void* g_where = 0; static const void* g_value = 0;
struct CRetailVector { SInputRecord* first; SInputRecord* last; SInputRecord* end;
    void InsertGrow(SInputRecord* where, const SInputRecord& value, const SScalarTag& tag, unsigned long count, bool noTail) { ++g_grow; g_where = where; g_value = &value; g_count = count; g_noTail = noTail; }
    void push_back(const SInputRecord& value) { if (last != end) { new (last) SInputRecord(value); ++last; } else InsertGrow(last, value, SScalarTag(), 1, true); } };

int main() {
    SInputRecord slots[3]; SInputRecord v; for (int i = 0; i < 11; ++i) v.m_words[i] = i + 1; CRetailVector vec; vec.first = slots; vec.last = slots; vec.end = slots + 1;
    vec.push_back(v); if (vec.last != slots + 1 || slots[0].m_words[0] != 1 || slots[0].m_words[10] != 11 || g_grow) { std::printf("006e5390_TEST FAIL\n"); return 1; } vec.push_back(v); if (g_grow != 1 || g_where != slots + 1 || g_value != &v || g_count != 1 || !g_noTail) { std::printf("006e5390_TEST FAIL\n"); return 1; }
    std::printf("006e5390_TEST PASS\n"); return 0;
}
