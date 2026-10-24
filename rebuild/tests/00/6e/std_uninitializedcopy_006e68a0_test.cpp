#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
struct CConversationLine { long PersonSpeaking, PersonSpokenTo; CCharString LineToSay; bool ShowSubtitle; CConversationLine() : PersonSpeaking(0), PersonSpokenTo(0), ShowSubtitle(false) { } };
static const char* g_texts[4] = { "hello", "there", "hero", "bye" };
typedef CConversationLine TElem;
static void Fill(TElem& e, int i)
{
    e.PersonSpeaking = i; e.PersonSpokenTo = 10 + i; e.LineToSay.m_pData = g_texts[i]; e.ShowSubtitle = i != 2;
}
static bool Check(const TElem& e, int i)
{
    return e.PersonSpeaking == i && e.PersonSpokenTo == 10 + i && e.LineToSay.m_pData == g_texts[i] && e.ShowSubtitle == (i != 2);
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
    if (g_stringCopies != 3) ok = false;
    if (!ok) { std::printf("006e68a0_TEST FAIL\n"); return 1; }
    std::printf("006e68a0_TEST PASS\n");
    return 0;
}