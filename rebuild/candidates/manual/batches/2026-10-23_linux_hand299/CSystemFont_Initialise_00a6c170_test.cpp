#include <cstdio>
// Behaviour model of CSystemFont::Initialise: base init gate, one-time fill, case-shared letters, shared brackets.
struct SGlyph { unsigned char Bits[36]; };
static SGlyph Letters[26], Digits[10], OpenBracket, CloseBracket, Percent, Blank;
static const SGlyph* Glyphs[256];
static bool Initialised = false;
static int initCalls = 0;
static bool BaseInit(bool ok) { ++initCalls; return ok; }
static bool Initialise(bool baseOk)
{
    if (BaseInit(baseOk) == true)
    {
        if (!Initialised)
        {
            for (int i = 0; i < 256; ++i) Glyphs[i] = &Blank;
            Glyphs[' '] = &Blank;
            for (int c = 0; c < 26; ++c) { Glyphs['a' + c] = &Letters[c]; Glyphs['A' + c] = &Letters[c]; }
            for (int d = 0; d < 10; ++d) Glyphs['0' + d] = &Digits[d];
            Glyphs['('] = &OpenBracket; Glyphs[')'] = &CloseBracket;
            Glyphs['{'] = &OpenBracket; Glyphs['}'] = &CloseBracket;
            Glyphs['%'] = &Percent;
            Initialised = true;
        }
        return true;
    }
    return false;
}
int main()
{
    if (Initialise(false) || Initialised) { std::printf("00a6c170_TEST FAIL\n"); return 1; }
    if (!Initialise(true) || !Initialised) { std::printf("00a6c170_TEST FAIL\n"); return 1; }
    if (Glyphs['q'] != Glyphs['Q'] || Glyphs['q'] != &Letters[16] || Glyphs['{'] != &OpenBracket || Glyphs['}'] != Glyphs[')']
        || Glyphs['~'] != &Blank || Glyphs['5'] != &Digits[5] || Glyphs['%'] != &Percent)
    { std::printf("00a6c170_TEST FAIL\n"); return 1; }
    Glyphs['~'] = &Percent;
    if (!Initialise(true) || Glyphs['~'] != &Percent || initCalls != 3) { std::printf("00a6c170_TEST FAIL\n"); return 1; }
    std::printf("00a6c170_TEST PASS\n");
    return 0;
}
