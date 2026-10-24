// Behaviour test for 0x0048DBF9 GetNumberOfCharactersOfTypeInString (model copy over a stand-in CWideString).
#include <cstdio>

typedef unsigned short wchar_t16;

struct CWideStringModel {
    const wchar_t16* m_text;
    long m_length;
    operator const wchar_t16*() const { return m_text; }
    long GetLength() const { return m_length; }
};

static long CountModel(const CWideStringModel& text, wchar_t16 character)
{
    long count = 0;
    const wchar_t16* data = text;
    for (long i = 0; i < text.GetLength(); ++i) {
        if (data[i] == character)
            ++count;
    }
    return count;
}

int main()
{
    static const wchar_t16 s[] = { 'a', '.', 'b', '.', '.', 'c', 0 };
    CWideStringModel t;
    t.m_text = s;
    t.m_length = 6;
    CWideStringModel e;
    e.m_text = s;
    e.m_length = 0;
    if (CountModel(t, '.') != 3 || CountModel(t, 'z') != 0 || CountModel(t, 'c') != 1 || CountModel(e, 'a') != 0) {
        std::printf("0048dbf9_TEST FAIL\n");
        return 1;
    }
    std::printf("0048dbf9_TEST PASS\n");
    return 0;
}