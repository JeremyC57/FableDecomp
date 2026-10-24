// Behaviour test for 0x00486C40 GFFloatToWideString (model copy over a stand-in wide string).
#include <cstdio>
#include <cstring>

typedef unsigned short wchar_t16;

struct CWideStringModel {
    wchar_t16 m_buf[64];
    long m_len;
    CWideStringModel() : m_len(0) { m_buf[0] = 0; }
    operator const wchar_t16*() const { return m_buf; }
    long GetLength() const { return m_len; }
    void SetLength(long n) { m_len = n; m_buf[n] = 0; }
    CWideStringModel& operator+=(wchar_t16 c) { m_buf[m_len++] = c; m_buf[m_len] = 0; return *this; }
};

static void FormatF(CWideStringModel* text, double value)
{
    char tmp[64];
    std::sprintf(tmp, "%f", value);
    text->m_len = (long)std::strlen(tmp);
    for (long i = 0; i <= text->m_len; ++i) text->m_buf[i] = (wchar_t16)(unsigned char)tmp[i];
}

static CWideStringModel GFFloatToWideStringModel(double value)
{
    CWideStringModel text;
    FormatF(&text, value);
    for (long i = 0; i < text.GetLength(); ++i) {
        if (((const wchar_t16*)text)[i] == '.') {
            while (text.GetLength() > 0) {
                wchar_t16 last = ((const wchar_t16*)text)[text.GetLength() - 1];
                if (last == '.') {
                    text += '0';
                    break;
                }
                if (last != '0')
                    break;
                text.SetLength(text.GetLength() - 1);
            }
            break;
        }
    }
    return text;
}

static int Same(const CWideStringModel& s, const char* expect)
{
    long n = (long)std::strlen(expect);
    if (s.GetLength() != n) return 0;
    for (long i = 0; i < n; ++i) if (s.m_buf[i] != (wchar_t16)expect[i]) return 0;
    return 1;
}

int main()
{
    int ok = Same(GFFloatToWideStringModel(2.5), "2.5")
          && Same(GFFloatToWideStringModel(3.0), "3.0")
          && Same(GFFloatToWideStringModel(0.125), "0.125")
          && Same(GFFloatToWideStringModel(-10.0), "-10.0")
          && Same(GFFloatToWideStringModel(100.25), "100.25");
    if (!ok) { std::printf("00486c40_TEST FAIL\n"); return 1; }
    std::printf("00486c40_TEST PASS\n");
    return 0;
}