// Behaviour test (0x00686240): CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn reads "(long,float,long,long,float,long)" and stops at the first missing separator
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
struct CEntry { int Long00; float Float04; int Long08; int Long0C; float Float10; int Long14; };
static void TransferIn(CPersistContext* context, CStringParser* parser, CEntry* value)
{
    if (parser->ReadAsSymbol() == "(") {
        value->Long00 = TransferInLong(context, parser);
        if (parser->ReadAsSymbol() == ",") {
            TransferInFloat(context, parser, &value->Float04);
            if (parser->ReadAsSymbol() == ",") {
                value->Long08 = TransferInLong(context, parser);
                if (parser->ReadAsSymbol() == ",") {
                    value->Long0C = TransferInLong(context, parser);
                    if (parser->ReadAsSymbol() == ",") {
                        TransferInFloat(context, parser, &value->Float10);
                        if (parser->ReadAsSymbol() == ",") {
                            value->Long14 = TransferInLong(context, parser);
                            parser->ReadAsSymbol();
                        }
                    }
                }
            }
        }
    }
}
int main()
{
    const char* t[] = { "(", "3", ",", "0.5", ",", "7", ",", "8", ",", "1.25", ",", "-2", ")" };
    CStringParser p(t, COUNTOF(t)); CPersistContext c;
    CEntry e; e.Long00 = e.Long08 = e.Long0C = e.Long14 = 0; e.Float04 = e.Float10 = 0.0f;
    TransferIn(&c, &p, &e);
    if (e.Long00 != 3 || std::fabs(e.Float04 - 0.5f) > 1e-6f || e.Long08 != 7 || e.Long0C != 8 || std::fabs(e.Float10 - 1.25f) > 1e-6f || e.Long14 != -2 || p.Pos != 13) { std::printf("00686240_TEST FAIL\n"); return 1; }
    const char* u[] = { "(", "4", ",", "2.0", ")", "9" };
    CStringParser q(u, COUNTOF(u));
    CEntry f; f.Long00 = f.Long08 = f.Long0C = f.Long14 = -1; f.Float04 = f.Float10 = -1.0f;
    TransferIn(&c, &q, &f);
    if (f.Long00 != 4 || std::fabs(f.Float04 - 2.0f) > 1e-6f || f.Long08 != -1 || q.Pos != 5) { std::printf("00686240_TEST FAIL\n"); return 1; }
    std::printf("00686240_TEST PASS\n");
    return 0;
}