// Behaviour test for 00c16820: PKCWideString::Assign -- resizes the character array to length+1 only when it differs, copies the text and NUL-terminates; a null text gives the empty string
#include <cstdio>
#include <cstring>
#include <cstdlib>

static unsigned int WideLength(const unsigned short* text) { unsigned int n = 0; while (text[n]) ++n; return n; }
static int g_resizes = 0;

struct PKCAutoDeleteArray {
    unsigned short* m_pData;
    unsigned int m_count;
    void Resize(unsigned int count)
    {
        ++g_resizes;
        std::free(m_pData);
        m_pData = (unsigned short*)std::malloc(count * sizeof(unsigned short));
        m_count = count;
    }
    unsigned short& operator[](unsigned int index) { return m_pData[index]; }
};

struct PKCWideString {
    PKCAutoDeleteArray m_chars;
    void Assign(const unsigned short* text)
    {
        unsigned int length = text ? WideLength(text) : 0;
        if (length + 1 != m_chars.m_count)
            m_chars.Resize(length + 1);
        if (length)
            std::memcpy(&m_chars[0], text, length * sizeof(unsigned short));
        m_chars[length] = 0;
    }
};

int main()
{
    PKCWideString s = { { 0, 0 } };
    unsigned short abc[] = { 'a', 'b', 'c', 0 };
    unsigned short xyz[] = { 'x', 'y', 'z', 0 };
    s.Assign(abc);
    if (g_resizes != 1 || s.m_chars.m_count != 4 || s.m_chars.m_pData[0] != 'a' || s.m_chars.m_pData[2] != 'c' || s.m_chars.m_pData[3] != 0) { std::printf("00c16820_TEST FAIL\n"); return 1; }
    unsigned short* before = s.m_chars.m_pData;
    s.Assign(xyz);                         // same length: no resize, buffer reused
    if (g_resizes != 1 || s.m_chars.m_pData != before || s.m_chars.m_pData[1] != 'y' || s.m_chars.m_pData[3] != 0) { std::printf("00c16820_TEST FAIL\n"); return 1; }
    s.Assign(0);                           // null: one NUL character
    if (g_resizes != 2 || s.m_chars.m_count != 1 || s.m_chars.m_pData[0] != 0) { std::printf("00c16820_TEST FAIL\n"); return 1; }
    std::free(s.m_chars.m_pData);
    std::printf("00c16820_TEST PASS\n");
    return 0;
}