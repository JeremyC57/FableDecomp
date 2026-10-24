// Behaviour test for IsThingWithHelperPoints (0x00856570): true only when every named helper point exists.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    const char* PString;
};
class CTCGraphicAppearance {
public:
    const char* Helpers;                     // one-letter helper names
    bool HasHelperPoint(const CCharString& name) const { return std::strchr(Helpers, name.PString[0]) != 0; }
};
class CThing {
public:
    CTCGraphicAppearance* PTCGraphicAppearance;
};
struct Names {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
};

static bool IsThingWithHelperPoints(const CThing& thing, Names& helperNames)
{
    const CTCGraphicAppearance* appearance = thing.PTCGraphicAppearance;
    CCharString* it = helperNames.begin();
    do {
        if (!appearance->HasHelperPoint(*it))
            break;
        ++it;
    } while (it != helperNames.end());
    return it == helperNames.end() ? true : false;
}

int main()
{
    CTCGraphicAppearance app;
    app.Helpers = "abc";
    CThing thing;
    thing.PTCGraphicAppearance = &app;
    CCharString n[3];
    n[0].PString = "a"; n[1].PString = "c"; n[2].PString = "z";
    Names all = { n, n + 2 };                // a, c: both present
    Names some = { n, n + 3 };               // z missing
    Names last = { n + 2, n + 3 };           // only z
    if (!IsThingWithHelperPoints(thing, all)) { std::printf("00856570_TEST FAIL\n"); return 1; }
    if (IsThingWithHelperPoints(thing, some)) { std::printf("00856570_TEST FAIL\n"); return 1; }
    if (IsThingWithHelperPoints(thing, last)) { std::printf("00856570_TEST FAIL\n"); return 1; }
    std::printf("00856570_TEST PASS\n");
    return 0;
}