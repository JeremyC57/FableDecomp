// Behaviour test for 0x006193b0 resize(n) of a vector of {float, CCharString, 5 floats} records
#include <cstdio>
#include <cstring>
class CCharString { public: char buf[16]; CCharString(const char* t) { std::strcpy(buf, t); } };
struct SRec {
    float m_time; CCharString m_name; float v[5];
    SRec() : m_time(0.0f), m_name("") { for (int i = 0; i < 5; ++i) v[i] = 0.0f; }
};
struct CVec {
    SRec data[8]; unsigned int count;
    void resize(unsigned int n, const SRec& x) { for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    void resize(unsigned int n) { resize(n, SRec()); }
};
int main()
{
    CVec v; v.count = 1; v.data[0].m_time = 5.0f; std::strcpy(v.data[0].m_name.buf, "a");
    v.data[2].m_time = 9.0f;
    v.resize(3);
    bool ok = v.count == 3 && v.data[0].m_time == 5.0f && v.data[2].m_time == 0.0f && v.data[2].m_name.buf[0] == 0;
    v.resize(1); ok = ok && v.count == 1;
    if (!ok) { std::printf("006193b0_TEST FAIL\n"); return 1; }
    std::printf("006193b0_TEST PASS\n");
    return 0;
}