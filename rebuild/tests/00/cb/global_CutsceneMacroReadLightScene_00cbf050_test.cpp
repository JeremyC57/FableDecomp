// Behaviour test for CutsceneMacro_ReadLightScene (0x00CBF050): split "name:..." at the colon into the name, then
// walk the rest from the colon: spaces are skipped, other characters accumulate into the token, and each comma
// converts the token into the current colour byte and advances the red/green/blue state machine.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char buf[128];
    CCharString() { buf[0] = 0; }
    CCharString(const char* t) { std::strcpy(buf, t); }
    long GetLength() const { return (long)std::strlen(buf); }
    long FindCharPos(char c) const { const char* p = std::strchr(buf, c); return p ? (long)(p - buf) : -1; }
    CCharString GetSubString(unsigned long start, unsigned long count) const
    {
        CCharString r;
        std::strncpy(r.buf, buf + start, count);
        r.buf[count] = 0;
        return r;
    }
    operator const char*() const { return buf; }
    void Append(char c, unsigned long pos) { buf[pos] = c; buf[pos + 1] = 0; }
};
long GFCharStringToInt(const CCharString& text)     // digits only (the token keeps its leading ':')
{
    long v = 0;
    for (const char* p = text.buf; *p; ++p)
        if (*p >= '0' && *p <= '9') v = v * 10 + (*p - '0');
    return v;
}

void CutsceneMacro_ReadLightScene(CCharString& line, CCharString& name, unsigned char* red, unsigned char* green, unsigned char* blue)
{
    if (line.GetLength() == 0)
        return;
    long colon = line.FindCharPos(':');
    if (colon == -1)
        return;
    name = line.GetSubString(0, colon);
    CCharString token;
    unsigned char* dest = red;
    unsigned char state = 0;
    for (long pos = colon; pos < line.GetLength(); ++pos) {
        if (((const char*)line)[pos] == ',') {
            *dest = (unsigned char)GFCharStringToInt(token);
            switch (state) {
            case 0: dest = red;   state = 1; break;
            case 1: dest = green; state = 2; break;
            case 2: dest = blue;  state = 0; break;
            default: state = 0; break;
            }
        } else if (((const char*)line)[pos] != ' ') {
            token.Append(((const char*)line)[pos], token.GetLength());
        }
    }
}

int main()
{
    unsigned char r = 0xEE, g = 0xEE, b = 0xEE;
    CCharString name("old");
    CCharString line("lamp:1, 2,3,");
    CutsceneMacro_ReadLightScene(line, name, &r, &g, &b);
    // token ":1" -> red = 1 (dest stays red), ":12" -> red = 12 (dest = green), ":123" -> green = 123 (dest = blue)
    bool parsed = std::strcmp(name.buf, "lamp") == 0 && r == 12 && g == 123 && b == 0xEE;
    CCharString empty("");
    CCharString noColon("lamp 1,2,3");
    CCharString name2("keep");
    unsigned char r2 = 5;
    CutsceneMacro_ReadLightScene(empty, name2, &r2, &g, &b);
    CutsceneMacro_ReadLightScene(noColon, name2, &r2, &g, &b);
    if (!parsed || std::strcmp(name2.buf, "keep") != 0 || r2 != 5) {
        std::printf("00cbf050_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbf050_TEST PASS\n");
    return 0;
}