#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
struct CTestQuest { CCharString QuestName, HSPName; long ColourIndex; CCharString DisplayName, IniFile, OutiFile, QuestCardDefName; CTestQuest() : ColourIndex(0) { } };
static const char* g_texts[4] = { "q0", "q1", "q2", "q3" };
static const char* g_other[4] = { "o0", "o1", "o2", "o3" };
typedef CTestQuest TElem;
static void Fill(TElem& e, int i)
{
    e.QuestName.m_pData = g_texts[i]; e.HSPName.m_pData = g_other[i]; e.ColourIndex = 5 * i; e.DisplayName.m_pData = g_texts[i - 1]; e.IniFile.m_pData = g_other[i - 1]; e.OutiFile.m_pData = g_texts[3 - i]; e.QuestCardDefName.m_pData = g_other[3 - i];
}
static bool Check(const TElem& e, int i)
{
    return e.QuestName.m_pData == g_texts[i] && e.HSPName.m_pData == g_other[i] && e.ColourIndex == 5 * i && e.DisplayName.m_pData == g_texts[i - 1] && e.IniFile.m_pData == g_other[i - 1] && e.OutiFile.m_pData == g_texts[3 - i] && e.QuestCardDefName.m_pData == g_other[3 - i];
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    if (g_stringCopies != 18) ok = false;
    if (!ok) { std::printf("00623700_TEST FAIL\n"); return 1; }
    std::printf("00623700_TEST PASS\n");
    return 0;
}