#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRetailAllocProxy { char* m_data; SRetailAllocProxy(char* data) : m_data(data) { } };
struct CRetailStringBase {
    char* m_start; char* m_finish; SRetailAllocProxy m_endOfStorage;
    CRetailStringBase(size_t n) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        m_start = static_cast<char*>(std::malloc(n));
        m_finish = m_start;
        m_endOfStorage.m_data = m_start + n;
    }
};
struct CRetailString : CRetailStringBase {
    CRetailString() : CRetailStringBase(8) { *m_finish = 0; }
    CRetailString& Assign(const char* first, const char* last)
    {
        size_t n = last - first;
        std::memcpy(m_start, first, n);
        m_finish = m_start + n;
        *m_finish = 0;
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + std::strlen(text)); }
};
struct CNamedRecord_00841e90 {
    CRetailString m_name; long m_0c; short m_10; long m_14; long m_18;
    CNamedRecord_00841e90();
};
CNamedRecord_00841e90::CNamedRecord_00841e90()
    : m_name()
{
    m_name = "";
    m_10 = 0;
    m_0c = 0;
    m_14 = 0;
    m_18 = 0;
}
int main()
{
    CNamedRecord_00841e90 r;
    if (!r.m_name.m_start || r.m_name.m_finish != r.m_name.m_start || *r.m_name.m_finish != 0 || r.m_name.m_endOfStorage.m_data != r.m_name.m_start + 8
        || r.m_0c || r.m_10 || r.m_14 || r.m_18 || sizeof(r) != 0x1c) { std::printf("00841e90_TEST FAIL\n"); return 1; }
    std::printf("00841e90_TEST PASS\n");
    return 0;
}