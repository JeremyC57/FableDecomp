// Behaviour test for 0x00406350 GetValidProfileName: every file-name-illegal character becomes '_', the input is untouched.
// (Narrow characters stand in for the retail 16-bit CWideString; the replace logic is the same.)
#include <cstdio>
#include <cstring>

static int g_live = 0;

class CWideString {
public:
    CWideString(const char* text) { std::strcpy(m_text, text); ++g_live; }
    CWideString(const CWideString& other) { std::strcpy(m_text, other.m_text); ++g_live; }
    ~CWideString() { --g_live; }
    void Replace(const CWideString& search, const CWideString& replacement)
    {
        char out[256]; int o = 0;
        size_t n = std::strlen(search.m_text);
        for (const char* p = m_text; *p; ) {
            if (std::strncmp(p, search.m_text, n) == 0) { std::strcpy(out + o, replacement.m_text); o += (int)std::strlen(replacement.m_text); p += n; }
            else out[o++] = *p++;
        }
        out[o] = 0;
        std::strcpy(m_text, out);
    }
    char m_text[256];
};

static void ReplaceWithUnderscore(CWideString& text, const char* character)
{
    CWideString replacement("_");
    CWideString search(character);
    text.Replace(search, replacement);
}

CWideString GetValidProfileName(const CWideString& name)
{
    CWideString result(name);
    ReplaceWithUnderscore(result, "\\");
    ReplaceWithUnderscore(result, "/");
    ReplaceWithUnderscore(result, ":");
    ReplaceWithUnderscore(result, "*");
    ReplaceWithUnderscore(result, "?");
    ReplaceWithUnderscore(result, "\"");
    ReplaceWithUnderscore(result, "<");
    ReplaceWithUnderscore(result, ">");
    ReplaceWithUnderscore(result, "|");
    return result;
}

int main()
{
    int ok = 1;
    {
        CWideString in("a\\b/c:d*e?f\"g<h>i|j");
        CWideString out = GetValidProfileName(in);
        if (std::strcmp(out.m_text, "a_b_c_d_e_f_g_h_i_j") != 0) ok = 0;
        if (std::strcmp(in.m_text, "a\\b/c:d*e?f\"g<h>i|j") != 0) ok = 0;
        CWideString plain("Hero Of Oakvale");
        if (std::strcmp(GetValidProfileName(plain).m_text, "Hero Of Oakvale") != 0) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00406350_TEST FAIL\n"); return 1; }
    std::printf("00406350_TEST PASS\n");
    return 0;
}