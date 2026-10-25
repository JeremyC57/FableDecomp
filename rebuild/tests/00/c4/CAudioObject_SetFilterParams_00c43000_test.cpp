// Behaviour test for 00c43000: CAudioObject::SetFilterParams -- set/clear the optional 6-float record (+0x2C -> +0x7C storage), dirty bit 8; the equality ignores the fourth field (retail compares the first twice)
#include <cstdio>

struct SFilterParams {
    float m_a, m_b, m_c, m_d, m_e, m_f;
    bool operator==(const SFilterParams& o) const
    {
        return m_a == o.m_a && m_b == o.m_b && m_c == o.m_c && m_a == o.m_a && m_e == o.m_e && m_f == o.m_f;
    }
};
class CAudioObject {
public:
    unsigned int   m_dirtyFlags;
    SFilterParams* m_pFilterParams;
    SFilterParams  m_filterParamsStorage;
    void SetFilterParams(const SFilterParams* value)
    {
        if (!value) {
            if (m_pFilterParams) { m_pFilterParams = 0; m_dirtyFlags |= 8; }
            return;
        }
        if (!m_pFilterParams) {
            m_pFilterParams = &m_filterParamsStorage;
            m_filterParamsStorage = *value;
            m_dirtyFlags |= 8;
            return;
        }
        if (!(*m_pFilterParams == *value)) { *m_pFilterParams = *value; m_dirtyFlags |= 8; }
    }
};

int main()
{
    CAudioObject o; o.m_dirtyFlags = 0; o.m_pFilterParams = 0;
    o.SetFilterParams(0);
    if (o.m_dirtyFlags != 0) { std::printf("00c43000_TEST FAIL\n"); return 1; }
    SFilterParams v = { 1, 2, 3, 4, 5, 6 };
    o.SetFilterParams(&v);
    if (o.m_pFilterParams != &o.m_filterParamsStorage || o.m_filterParamsStorage.m_f != 6 || o.m_dirtyFlags != 8) { std::printf("00c43000_TEST FAIL\n"); return 1; }
    o.m_dirtyFlags = 0;
    v.m_d = 40;                                // the fourth field is not compared: no update
    o.SetFilterParams(&v);
    if (o.m_dirtyFlags != 0 || o.m_filterParamsStorage.m_d != 4) { std::printf("00c43000_TEST FAIL\n"); return 1; }
    v.m_e = 50;
    o.SetFilterParams(&v);
    if (o.m_dirtyFlags != 8 || o.m_filterParamsStorage.m_d != 40 || o.m_filterParamsStorage.m_e != 50) { std::printf("00c43000_TEST FAIL\n"); return 1; }
    o.m_dirtyFlags = 0;
    o.SetFilterParams(0);
    if (o.m_pFilterParams != 0 || o.m_dirtyFlags != 8) { std::printf("00c43000_TEST FAIL\n"); return 1; }
    std::printf("00c43000_TEST PASS\n");
    return 0;
}