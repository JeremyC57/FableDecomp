// behaviour test for 0x005664b0 (std::fill over 16-byte {three strings, two flags} records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
struct CCharString {
    void* m_pStringData;                        // +0x00
    CCharString& operator=(const CCharString& other) { m_pStringData = other.m_pStringData; ++g_stringAssigns; return *this; }
};
struct SQuestStringRecord {                     // 16 bytes
    CCharString Name;                           // +0x00
    CCharString QuestName;                      // +0x04
    CCharString PersistScript;                  // +0x08
    bool        m_flag0;                        // +0x0C
    bool        m_flag1;                        // +0x0D
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(SQuestStringRecord*, SQuestStringRecord*, const SQuestStringRecord&);
}

int main()
{
    static char a, b, c, d;
    SQuestStringRecord v[3];
    for (int i = 0; i < 3; ++i) { v[i].Name.m_pStringData = &a; v[i].QuestName.m_pStringData = &a; v[i].PersistScript.m_pStringData = &a; v[i].m_flag0 = false; v[i].m_flag1 = false; }
    SQuestStringRecord val; val.Name.m_pStringData = &b; val.QuestName.m_pStringData = &c; val.PersistScript.m_pStringData = &d;
    val.m_flag0 = true; val.m_flag1 = true;
    std::fill(v + 1, v + 3, val);
    int ok = g_stringAssigns == 6 && v[0].Name.m_pStringData == &a && v[1].Name.m_pStringData == &b && v[2].QuestName.m_pStringData == &c
          && v[2].PersistScript.m_pStringData == &d && v[1].m_flag0 && v[2].m_flag1 && !v[0].m_flag1;
    if (!ok) { printf("005664b0_TEST FAIL\n"); return 1; }
    printf("005664b0_TEST PASS\n");
    return 0;
}