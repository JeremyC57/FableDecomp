// Behaviour test for PKCWideString::operator=(const unsigned short*) (0x00C16970): Assign the text, return *this.
#include <cstdio>
#include <cstdlib>

struct PKCAutoDeleteArray {
    unsigned short* m_pData;
    unsigned long   m_count;
};

class PKCWideString {
public:
    PKCAutoDeleteArray m_chars;
    PKCWideString() { m_chars.m_pData = 0; m_chars.m_count = 0; }
    ~PKCWideString() { std::free(m_chars.m_pData); }
    void Assign(const unsigned short* text)
    {
        unsigned long length = 0;
        if (text)
            while (text[length]) ++length;
        if (length + 1 != m_chars.m_count) {
            std::free(m_chars.m_pData);
            m_chars.m_pData = (unsigned short*)std::malloc((length + 1) * sizeof(unsigned short));
            m_chars.m_count = length + 1;
        }
        for (unsigned long i = 0; i < length; ++i)
            m_chars.m_pData[i] = text[i];
        m_chars.m_pData[length] = 0;
    }
    PKCWideString& operator=(const unsigned short* text)
    {
        Assign(text);
        return *this;
    }
};

int main()
{
    static const unsigned short hello[] = { 'h', 'i', '!', 0 };
    PKCWideString s;
    PKCWideString& r = (s = hello);
    if (&r != &s || s.m_chars.m_count != 4 || s.m_chars.m_pData[0] != 'h' || s.m_chars.m_pData[2] != '!' || s.m_chars.m_pData[3] != 0) {
        std::printf("00c16970_TEST FAIL\n"); return 1;
    }
    s = (const unsigned short*)0;
    if (s.m_chars.m_count != 1 || s.m_chars.m_pData[0] != 0) { std::printf("00c16970_TEST FAIL\n"); return 1; }
    std::printf("00c16970_TEST PASS\n");
    return 0;
}