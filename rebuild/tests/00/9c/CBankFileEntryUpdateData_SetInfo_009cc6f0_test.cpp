#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CArray_char {
    char* m_pFirst; char* m_pLast; char* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    const char* GetData() const { return m_pLast - m_pFirst != 0 ? m_pFirst : reinterpret_cast<const char*>(this); }
};
struct CBankFileEntryUpdateData {
    unsigned long m_infoSize; char* m_pInfo;
    void SetInfo(const CArray_char& info)
    {
        if (m_pInfo != 0) { delete[] m_pInfo; m_pInfo = 0; }
        m_infoSize = 0;
        m_infoSize = info.GetCount();
        if (m_infoSize != 0) { m_pInfo = new char[m_infoSize]; std::memcpy(m_pInfo, info.GetData(), m_infoSize); }
    }
};
static bool Check()
{
    char text[] = "hello"; CArray_char a; a.m_pFirst = text; a.m_pLast = text + 5; a.m_pEnd = text + 6;
    CBankFileEntryUpdateData u; u.m_infoSize = 0; u.m_pInfo = 0;
    u.SetInfo(a);
    if (u.m_infoSize != 5 || u.m_pInfo == 0 || std::memcmp(u.m_pInfo, "hello", 5) != 0) return false;
    text[0] = 'j'; u.SetInfo(a);
    if (u.m_infoSize != 5 || std::memcmp(u.m_pInfo, "jello", 5) != 0) return false;   // old buffer replaced
    a.m_pLast = text; u.SetInfo(a);
    return u.m_infoSize == 0 && u.m_pInfo == 0;                                        // empty: nothing allocated
}
int main()
{
    if (!Check()) { std::printf("009cc6f0_TEST FAIL\n"); return 1; }
    std::printf("009cc6f0_TEST PASS\n");
    return 0;
}