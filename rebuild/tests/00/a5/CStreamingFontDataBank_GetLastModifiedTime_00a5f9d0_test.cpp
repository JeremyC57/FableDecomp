#include <cstdio>
// Behaviour test for 00a5f9d0: the last-modified time of any streaming font entry is the fixed CDateAndTime(1,1,1,1,1,0,0).
class CCharString { public: int m_id; };
class CDateAndTime { public:
    long m_parts[7];
    CDateAndTime(long year, long month, long day, long hour, long minute, long second, long millisecond)
    {
        m_parts[0] = year; m_parts[1] = month; m_parts[2] = day; m_parts[3] = hour; m_parts[4] = minute; m_parts[5] = second; m_parts[6] = millisecond;
    }
};
class CStreamingFontDataBank { public:
    CDateAndTime GetLastModifiedTime(const CCharString& entryName);
};
CDateAndTime CStreamingFontDataBank::GetLastModifiedTime(const CCharString& entryName)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}
int main()
{
    CStreamingFontDataBank bank; CCharString a; a.m_id = 1; CCharString b; b.m_id = 2;
    CDateAndTime t1 = bank.GetLastModifiedTime(a);
    CDateAndTime t2 = bank.GetLastModifiedTime(b);
    static const long expect[7] = { 1, 1, 1, 1, 1, 0, 0 };
    for (int i = 0; i < 7; ++i)
        if (t1.m_parts[i] != expect[i] || t2.m_parts[i] != expect[i]) { std::printf("00a5f9d0_TEST FAIL\n"); return 1; }
    std::printf("00a5f9d0_TEST PASS\n");
    return 0;
}