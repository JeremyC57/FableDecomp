// Behaviour test for 0x009EA7D0 CConsoleCommandBase::GetHelp: the help text for 0, 1 and 3 parameters (slot 3 is
// GetTypeAsString, "command").
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[128];
    CCharString(const char* s = "") { std::strcpy(text, s); }
    CCharString& operator+=(const CCharString& o) { std::strcat(text, o.text); return *this; }
    CCharString& operator+=(const char* s) { std::strcat(text, s); return *this; }
};
static CCharString operator+(const CCharString& l, const char* r) { CCharString t(l.text); t += r; return t; }
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CCharString(b); }

struct CConsoleCommandParameters {
    long* m_pTypesStart; long* m_pTypesFinish; long* m_pTypesEnd;
    long GetNumParameters() const { return m_pTypesFinish - m_pTypesStart; }
    CCharString GetParameterTypeAsString(long index) const
    {
        switch (m_pTypesStart[index]) {
        case 4: return CCharString("bool");
        case 3: return CCharString("float");
        case 1: return CCharString("slong");
        case 2: return CCharString("ulong");
        case 5: return CCharString("string");
        }
        return CCharString("");
    }
};
struct CConsoleCommandBase {
    CConsoleCommandParameters ParamTypes;
    virtual ~CConsoleCommandBase() { }
    virtual CCharString GetTypeAsString() const { return CCharString("command"); }
    CCharString GetHelp();
};
CCharString CConsoleCommandBase::GetHelp()
{
    CCharString help = GetTypeAsString() + " with ";
    if (ParamTypes.GetNumParameters() == 0) {
        help += "no parameters";
    } else {
        help += GFIntToCharString(ParamTypes.GetNumParameters()) + " parameter";
        if (ParamTypes.GetNumParameters() > 1)
            help += "s";
        help += ": ";
        for (long i = 0; i < ParamTypes.GetNumParameters(); ++i)
            help += ParamTypes.GetParameterTypeAsString(i) + " ";
    }
    return help;
}
int main()
{
    long types[3] = { 4, 3, 5 };
    CConsoleCommandBase c;
    c.ParamTypes.m_pTypesStart = types; c.ParamTypes.m_pTypesFinish = types; c.ParamTypes.m_pTypesEnd = types + 3;
    int ok = std::strcmp(c.GetHelp().text, "command with no parameters") == 0;
    c.ParamTypes.m_pTypesFinish = types + 1;
    ok = ok && std::strcmp(c.GetHelp().text, "command with 1 parameter: bool ") == 0;
    c.ParamTypes.m_pTypesFinish = types + 3;
    ok = ok && std::strcmp(c.GetHelp().text, "command with 3 parameters: bool float string ") == 0;
    if (!ok) { std::printf("009ea7d0_TEST FAIL\n"); return 1; }
    std::printf("009ea7d0_TEST PASS\n");
    return 0;
}