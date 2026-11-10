// behaviour test for 0x009E81E0 CPersistTraits<C2DVector>::TransferIn (body copied from the row source; self-contained stubs)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
class CCharString {
public:
    std::string S;
    CCharString(const char* t = "") : S(t) { }
    bool operator==(const char* t) const { return S == t; }
};
static CCharString operator+(const char* a, const CCharString& b) { CCharString r(a); r.S += b.S; return r; }
static CCharString operator+(const CCharString& a, const char* b) { CCharString r(a); r.S += b; return r; }
enum EParsedItemType { PARSED_ITEM_STRING, PARSED_ITEM_SYMBOL };
static const char* g_tokens[16]; static int g_pos; static int g_errors; static std::string g_lastError;
static void Script(const char* a, const char* b = 0, const char* c = 0, const char* d = 0, const char* e = 0, const char* f = 0)
{ const char* t[6] = { a, b, c, d, e, f }; for (int k = 0; k < 6; ++k) g_tokens[k] = t[k]; g_tokens[6] = 0; g_pos = 0; }
static const char* Next() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return t ? t : ""; }
class CStringParser {
public:
    EParsedItemType PeekNextItemType() { const char* t = g_tokens[g_pos]; return (t && strcmp(t, ".") == 0) ? PARSED_ITEM_SYMBOL : PARSED_ITEM_STRING; }
    CCharString ReadAsSymbol() { return CCharString(Next()); }
    CCharString ReadAsString() { return CCharString(Next()); }
};
class CPersistContext {
public:
    void PersistError(const CStringParser&, const CCharString& message) const { ++g_errors; g_lastError = message.S; }
};
class C2DVector { public: float X, Y; };
template <class T> class CPersistTraits;
template <> class CPersistTraits<float> {
public:
    void TransferIn(CPersistContext&, CStringParser&, float& value) { value = (float)atof(Next()); }
};
template <> class CPersistTraits<C2DVector> {
public:
    void TransferIn(CPersistContext& context, CStringParser& parser, C2DVector& value);
};
void CPersistTraits<C2DVector>::TransferIn(CPersistContext& context, CStringParser& parser, C2DVector& value)
{
    CPersistTraits<float> floatTraits;
    if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        if (!(parser.ReadAsSymbol() == "."))
            return;
        CCharString name = parser.ReadAsString();
        float component;
        floatTraits.TransferIn(context, parser, component);
        if (name == "X")
            value.X = component;
        else if (name == "Y")
            value.Y = component;
        else
            context.PersistError(parser, "Unknown entry '" + name + "' in C2DCoordF");
    } else if (parser.ReadAsString() == "C2DCoordF") {
        if (parser.ReadAsSymbol() == "(") {
            floatTraits.TransferIn(context, parser, value.X);
            if (parser.ReadAsSymbol() == ",") {
                floatTraits.TransferIn(context, parser, value.Y);
                if (parser.ReadAsSymbol() == ")") return;
            }
        }
    } else {
        context.PersistError(parser, "Unknown command for C2DCoordF");
    }
}
int main()
{
    CPersistTraits<C2DVector> traits; CPersistContext context; CStringParser parser;
    C2DVector v; v.X = 0.0f; v.Y = 0.0f;
    bool ok = true;
    Script("C2DCoordF", "(", "1.5", ",", "-2.25", ")"); traits.TransferIn(context, parser, v);
    ok = ok && v.X == 1.5f && v.Y == -2.25f && g_pos == 6 && g_errors == 0;
    Script(".", "Y", "4"); traits.TransferIn(context, parser, v); ok = ok && v.Y == 4.0f && v.X == 1.5f;
    Script(".", "X", "8"); traits.TransferIn(context, parser, v); ok = ok && v.X == 8.0f;
    Script(".", "Z", "3"); traits.TransferIn(context, parser, v);
    ok = ok && g_errors == 1 && g_lastError == "Unknown entry 'Z' in C2DCoordF" && g_pos == 3;
    Script("C3DCoordF", "(", "1"); traits.TransferIn(context, parser, v);
    ok = ok && g_errors == 2 && g_lastError == "Unknown command for C2DCoordF" && g_pos == 1;
    Script("C2DCoordF", "(", "6", ";", "7"); traits.TransferIn(context, parser, v);
    ok = ok && v.X == 6.0f && v.Y == 4.0f && g_pos == 4 && g_errors == 2;   // stops at the missing comma
    printf(ok ? "009e81e0_TEST PASS\n" : "009e81e0_TEST FAIL\n");
    return ok ? 0 : 1;
}