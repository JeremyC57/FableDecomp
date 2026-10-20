// Behaviour test for 00b1b4d0: body copied from src/00b1b4d0.cpp (SDecalRequest::SDecalRequest), no callees.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SZeroByte { unsigned char v; SZeroByte() : v(0) {} };
struct SDecalRequest {
    long          m_kind;
    long          m_reserved;
    SZeroByte     m_flag8;
    unsigned char m_flag9;
    unsigned char m_flagA;
    void*         m_pSource;
    short         m_extra;
    long          m_valueA;
    long          m_valueB;
    float         m_scale;
    bool          m_enabled;
    SDecalRequest(void* source, bool enabled, long valueA, long valueB, float scale, short extra);
};
SDecalRequest::SDecalRequest(void* source, bool enabled, long valueA, long valueB, float scale, short extra)
    : m_kind(0x1E), m_reserved(0), m_flag9(0), m_flagA(0), m_pSource(source),
      m_extra(extra), m_valueA(valueA), m_valueB(valueB), m_scale(scale), m_enabled(enabled)
{
}
int main()
{
    unsigned char raw[sizeof(SDecalRequest)]; memset(raw, 0xAA, sizeof(raw));
    int marker = 0;
    SDecalRequest* r = new (raw) SDecalRequest(&marker, true, 11, -22, 1.5f, (short)-7);
    CHECK(r->m_kind == 0x1E); CHECK(r->m_reserved == 0);
    CHECK(raw[8] == 0 && raw[9] == 0 && raw[10] == 0);
    CHECK(r->m_pSource == &marker); CHECK(r->m_extra == -7); CHECK(r->m_valueA == 11); CHECK(r->m_valueB == -22);
    CHECK(r->m_scale == 1.5f); CHECK(r->m_enabled == true);
    SDecalRequest q(0, false, 0, 0, 0.0f, 0);
    CHECK(q.m_kind == 0x1E); CHECK(q.m_enabled == false); CHECK(q.m_pSource == 0);
    if (fails == 0) printf("00b1b4d0_TEST PASS\n"); else printf("00b1b4d0_TEST FAIL\n");
    return fails;
}