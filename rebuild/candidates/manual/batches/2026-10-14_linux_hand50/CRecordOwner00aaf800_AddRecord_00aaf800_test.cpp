#include <cstdio>
#include <cstring>
#include <new>
static long g_grow = 0; static const void* g_growValue = 0; static unsigned long g_growCount = 0; static bool g_growTail = false;
struct SScalarTag { SScalarTag() { } };
struct SRecord_00aaf800 { long m_words[12]; };
struct CRetailVector_00aaf800 {
    SRecord_00aaf800* first; SRecord_00aaf800* last; SRecord_00aaf800* end;
    void InsertGrow(SRecord_00aaf800* where, const SRecord_00aaf800& value, const SScalarTag&, unsigned long count, bool noTail) { ++g_grow; g_growValue = &value; g_growCount = count; g_growTail = noTail; (void)where; }
    void push_back(const SRecord_00aaf800& value) { if (last != end) { new (last) SRecord_00aaf800(value); ++last; } else InsertGrow(last, value, SScalarTag(), 1, true); }
};
struct CRecordOwner_00aaf800 { unsigned char _pad[0x40]; CRetailVector_00aaf800 m_records; void AddRecord(const SRecord_00aaf800& value) { m_records.push_back(value); } };
int main()
{
    CRecordOwner_00aaf800 o; SRecord_00aaf800 buf[2]; std::memset(buf, 0, sizeof buf); o.m_records.first = buf; o.m_records.last = buf; o.m_records.end = buf + 2;
    SRecord_00aaf800 r; for (int i = 0; i < 12; ++i) r.m_words[i] = i * 3;
    o.AddRecord(r); if (o.m_records.last != buf + 1 || std::memcmp(&buf[0], &r, sizeof r) || g_grow) { std::printf("00aaf800_TEST FAIL\n"); return 1; }
    o.AddRecord(r); o.AddRecord(r); if (o.m_records.last != buf + 2 || g_grow != 1 || g_growValue != &r || g_growCount != 1 || !g_growTail) { std::printf("00aaf800_TEST FAIL\n"); return 1; }
    std::printf("00aaf800_TEST PASS\n");
    return 0;
}
