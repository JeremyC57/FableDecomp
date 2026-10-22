// Behaviour test for 00a64360 CGenericVar::ReadFromParser: dispatch on the variable's type (1 SLONG, 2 ULONG, 3 float,
// 4 bool, 5 quoted string) to the matching reader and return its result; any other type reads nothing and fails.
#include <cstdio>
#include <cstring>
struct CCharString { const char* text; };
struct CStringParser {
    char calls[16];
    bool ok;
    bool ReadNextItemAsQuotedString(CCharString& text) { std::strcat(calls, "S"); text.text = "quoted"; return ok; }
};
struct CGenericVar {
    long Type;
    CCharString ValString;
    bool ReadFromParserAsSLONG(CStringParser& p) { std::strcat(p.calls, "L"); return p.ok; }
    bool ReadFromParserAsULONG(CStringParser& p) { std::strcat(p.calls, "U"); return p.ok; }
    bool ReadFromParserAsFloat(CStringParser& p) { std::strcat(p.calls, "F"); return p.ok; }
    bool ReadFromParserAsBool(CStringParser& p) { std::strcat(p.calls, "B"); return p.ok; }
    bool ReadFromParserAsString(CStringParser& p) { return p.ReadNextItemAsQuotedString(ValString); }
    bool ReadFromParser(CStringParser& parser)
    {
        switch (Type) {
        case 3: return ReadFromParserAsFloat(parser);
        case 5: return ReadFromParserAsString(parser);
        case 4: return ReadFromParserAsBool(parser);
        case 1: return ReadFromParserAsSLONG(parser);
        case 2: return ReadFromParserAsULONG(parser);
        default: return false;
        }
    }
};
int main()
{
    static const char* want[7] = { "", "L", "U", "F", "B", "S", "" };
    for (long t = 0; t < 7; ++t) {
        for (int k = 0; k < 2; ++k) {
            CStringParser p; p.calls[0] = 0; p.ok = (k == 1);
            CGenericVar v; v.Type = t; v.ValString.text = "";
            bool r = v.ReadFromParser(p);
            bool expectResult = (t >= 1 && t <= 5) && p.ok;
            if (std::strcmp(p.calls, want[t]) != 0 || r != expectResult) { std::printf("00a64360_TEST FAIL\n"); return 1; }
            if (t == 5 && std::strcmp(v.ValString.text, "quoted") != 0) { std::printf("00a64360_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00a64360_TEST PASS\n");
    return 0;
}