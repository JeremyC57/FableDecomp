// Behaviour test for CPersistContext::CleanOutLine (0x009BB0D0): blank [start, end) except newlines; null string is a no-op.
#include <cstdio>
#include <cstring>
class CCharStringData { public: char* PData; };
class CCharString { public:
    CCharStringData* PStringData;
    const char* PeekData() const { const char* t = ""; if (PStringData) t = PStringData->PData; return t; }
};
class CPersistContext { public:
    void* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback;
    CCharString* PStringToClean;
    void CleanOutLine(long start, long end);
};
inline void BlankLineRange(char* text, long start, long end)
{
    for (long i = start; i < end; ++i)
        if (text[i] != '\n')
            text[i] = ' ';
}
void CPersistContext::CleanOutLine(long start, long end)
{
    if (PStringToClean)
        BlankLineRange((char*)PStringToClean->PeekData(), start, end);
}
int main()
{
    char buf[32];
    std::strcpy(buf, "ab\ncd;ef\ngh");
    CCharStringData d; d.PData = buf;
    CCharString s; s.PStringData = &d;
    CPersistContext pc; pc.PStringToClean = &s;
    pc.CleanOutLine(1, 8);
    if (std::strcmp(buf, "a \n     \ngh") != 0) { std::printf("009bb0d0_TEST FAIL\n"); return 1; }
    pc.CleanOutLine(5, 5);                       // empty range: nothing
    pc.CleanOutLine(9, 3);                       // reversed: nothing
    if (std::strcmp(buf, "a \n     \ngh") != 0) { std::printf("009bb0d0_TEST FAIL\n"); return 1; }
    pc.PStringToClean = 0;
    pc.CleanOutLine(0, 4);                       // null string: no-op, no crash
    std::printf("009bb0d0_TEST PASS\n");
    return 0;
}