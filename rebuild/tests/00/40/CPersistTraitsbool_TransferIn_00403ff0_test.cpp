// Behaviour test (0x00403ff0): CPersistTraits<bool>::TransferIn accepts TRUE/BTRUE and FALSE/BFALSE case-insensitively; anything else is reported as an unknown boolean entry and leaves the value
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#define STRICMP _stricmp
#else
typedef long long i64;
#include <strings.h>
#define STRICMP strcasecmp
#endif
// Token-stream model of CStringParser: every Read* call consumes the next pre-split token.
struct CCharString {
    char Text[128];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    bool operator==(const char* s) const { return std::strcmp(Text, s) == 0; }
};
struct CStringParser {
    const char* const* Tokens; int Count; int Pos;
    CStringParser(const char* const* tokens, int count) : Tokens(tokens), Count(count), Pos(0) {}
    CCharString Next() { return Pos < Count ? CCharString(Tokens[Pos++]) : CCharString(""); }
    CCharString ReadAsString() { return Next(); }
    CCharString ReadAsSymbol() { return Next(); }
    i64 ReadInt64() { CCharString t = Next(); i64 v = 0; const char* p = t.Text; bool neg = *p == '-'; if (neg) ++p; while (*p) v = v * 10 + (*p++ - '0'); return neg ? -v : v; }
    float ReadAsFloat() { return (float)std::atof(Next().Text); }
    bool NextItemIs(const char* name) { if (Pos < Count && std::strcmp(Tokens[Pos], name) == 0) { ++Pos; return true; } return false; }
};
struct CPersistContext {
    int Errors; char LastError[160];
    CPersistContext() : Errors(0) { LastError[0] = 0; }
    void Error(const CStringParser&, const char* message) { ++Errors; std::strcpy(LastError, message); }
};
static void TransferInLongValue(CPersistContext*, CStringParser* parser, int* value) { *value = std::atoi(parser->Next().Text); }   // 0x009E2590 stand-in
static int TransferInLong(CPersistContext* context, CStringParser* parser) { int v; TransferInLongValue(context, parser, &v); return v; }
static void TransferInBool(CPersistContext* context, CStringParser* parser, bool* value)                  // 0x00403FF0
{
    CCharString token = parser->ReadAsString();
    if (STRICMP(token.Text, "TRUE") == 0 || STRICMP(token.Text, "BTRUE") == 0) *value = true;
    else if (STRICMP(token.Text, "FALSE") == 0 || STRICMP(token.Text, "BFALSE") == 0) *value = false;
    else { char m[160]; std::sprintf(m, "Unknown boolean value entry %s", token.Text); context->Error(*parser, m); }
}
static void TransferInFloat(CPersistContext*, CStringParser* parser, float* value) { *value = parser->ReadAsFloat(); }
#define COUNTOF(a) ((int)(sizeof(a) / sizeof((a)[0])))
int main()
{
    const char* t[] = { "true", "BFALSE", "BTRUE", "maybe" };
    CStringParser p(t, COUNTOF(t));
    CPersistContext c;
    bool v = false;
    TransferInBool(&c, &p, &v); if (!v) { std::printf("00403ff0_TEST FAIL\n"); return 1; }
    TransferInBool(&c, &p, &v); if (v) { std::printf("00403ff0_TEST FAIL\n"); return 1; }
    TransferInBool(&c, &p, &v); if (!v) { std::printf("00403ff0_TEST FAIL\n"); return 1; }
    TransferInBool(&c, &p, &v);
    if (!v || c.Errors != 1 || std::strcmp(c.LastError, "Unknown boolean value entry maybe") != 0) { std::printf("00403ff0_TEST FAIL\n"); return 1; }
    std::printf("00403ff0_TEST PASS\n");
    return 0;
}