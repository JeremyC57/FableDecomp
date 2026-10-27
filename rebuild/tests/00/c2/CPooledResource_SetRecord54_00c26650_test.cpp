#include <cstdio>

// Behaviour model of CPooledResource::SetRecord54 (0x00C26650): copy the 16-byte record.
struct Record { int a, b, c, d; };
struct Resource { Record m_record; void SetRecord54(int, const Record& r) { m_record = r; } };

int main()
{

    Record r; r.a = 1; r.b = 2; r.c = 3; r.d = 4;
    Resource s; s.m_record.a = s.m_record.b = s.m_record.c = s.m_record.d = 0;
    s.SetRecord54(99, r);
    if (s.m_record.a != 1 || s.m_record.b != 2 || s.m_record.c != 3 || s.m_record.d != 4) { std::printf("00c26650_TEST FAIL\n"); return 1; }
    std::printf("00c26650_TEST PASS\n");
    return 0;
}