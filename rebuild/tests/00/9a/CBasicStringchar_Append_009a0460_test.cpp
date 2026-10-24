// Behaviour test for 0x009A0460 CBasicString<char>::Append(char, unsigned long): appends `count` copies of the char,
// grows through ExtendData, terminates the text and zeroes the slack bytes up to the dword-aligned end.
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CBasicStringChar {
    char*        PData;
    unsigned int StringLength;
    unsigned int DataLength;

    static unsigned int Align(unsigned int value, unsigned int alignment) { return (value + alignment - 1) & ~(alignment - 1); }
    void ExtendData(unsigned int length)
    {
        unsigned int need = (length + 4) & ~3u;
        if (need <= DataLength) return;
        char* p = (char*)std::malloc(need);
        std::memset(p, 0x5A, need);                    // garbage so the slack clearing is observable
        if (PData) { std::memcpy(p, PData, StringLength + 1); std::free(PData); }
        PData = p;
        DataLength = need;
    }
    static void Fill(char* dest, unsigned int count, char c)
    {
        for (unsigned int i = 0; i < count; ++i)
            dest[i] = c;
    }
    void ClearStringEnd(char* data, unsigned int length)
    {
        unsigned int start = length + 1;
        std::memset(data + start, 0, Align(start, 4) - start);
    }
    void SetStringLength(unsigned int length)
    {
        StringLength = length;
        PData[length] = 0;
        ClearStringEnd(PData, StringLength);
    }
    void Append(char c, unsigned int count)
    {
        ExtendData(StringLength + count);
        Fill(PData + StringLength, count, c);
        SetStringLength(StringLength + count);
    }
};

int main()
{
    CBasicStringChar s;
    s.PData = 0; s.StringLength = 0; s.DataLength = 0;
    s.ExtendData(0);
    s.PData[0] = 0;
    s.Append('a', 3);
    s.Append('-', 0);
    s.Append('b', 2);
    int bad = std::strcmp(s.PData, "aaabb") != 0 || s.StringLength != 5;
    for (unsigned int i = 6; i < CBasicStringChar::Align(6, 4); ++i)
        if (s.PData[i] != 0) bad = 1;
    if (s.DataLength < 8) bad = 1;
    std::free(s.PData);
    if (bad) { std::printf("009a0460_TEST FAIL\n"); return 1; }
    std::printf("009a0460_TEST PASS\n");
    return 0;
}