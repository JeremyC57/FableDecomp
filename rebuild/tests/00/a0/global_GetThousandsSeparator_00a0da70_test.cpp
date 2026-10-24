// Behaviour test for 0x00A0DA70 GetThousandsSeparator: space for French, '.' for German/Spanish/Italian/Portuguese, ',' otherwise (primary language only).
#include <cstdio>

static unsigned short g_langId = 0;
static unsigned short Stub_GetSystemDefaultLangID() { return g_langId; }

enum {
    LANG_GERMAN_ID     = 0x07,
    LANG_SPANISH_ID    = 0x0A,
    LANG_FRENCH_ID     = 0x0C,
    LANG_ITALIAN_ID    = 0x10,
    LANG_PORTUGUESE_ID = 0x16
};

unsigned short GetThousandsSeparator()
{
    switch (Stub_GetSystemDefaultLangID() & 0x3FF) {
    case LANG_FRENCH_ID:
        return 0x0020;
    case LANG_GERMAN_ID:
    case LANG_SPANISH_ID:
    case LANG_ITALIAN_ID:
    case LANG_PORTUGUESE_ID:
        return 0x002E;
    default:
        return 0x002C;
    }
}

int main()
{
    static const unsigned short ids[]  = { 0x040C, 0x080C, 0x0407, 0x0C0A, 0x0410, 0x0816, 0x0409, 0x0809, 0x0411, 0x0008, 0x0017 };
    static const unsigned short want[] = { ' ',    ' ',    '.',    '.',    '.',    '.',    ',',    ',',    ',',    ',',    ','    };
    for (int i = 0; i < (int)(sizeof(ids) / sizeof(ids[0])); ++i) {
        g_langId = ids[i];
        if (GetThousandsSeparator() != want[i]) { std::printf("00a0da70_TEST FAIL\n"); return 1; }
    }
    std::printf("00a0da70_TEST PASS\n");
    return 0;
}