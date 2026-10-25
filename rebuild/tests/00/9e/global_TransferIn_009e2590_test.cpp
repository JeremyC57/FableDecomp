// Behaviour test for 0x009E2590 TransferIn (text read of a long): symbols through the map (unknown ones report an
// error and read 0), integers otherwise, then a right-recursive `|`, `+`, `-`, `<<`, `>>` tail.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

struct Parser {                                  // stands in for CStringParser over a token list
    const char** tokens;
    int pos;
    long PeekNextItemType() const { return std::isalpha((unsigned char)tokens[pos][0]) ? 2 : 1; }
    const char* ReadAsString() { return tokens[pos++]; }
    long ReadAsInteger() { return std::atol(tokens[pos++]); }
    bool SkipPastNextItemIfIs(const char* item)
    {
        if (tokens[pos] && std::strcmp(tokens[pos], item) == 0) { ++pos; return true; }
        return false;
    }
};
struct Context {
    int dirtyUpdates;
    int errors;
    bool FindSymbol(const char* name, long& value) const
    {
        if (std::strcmp(name, "FLAG_A") == 0) { value = 1; return true; }
        if (std::strcmp(name, "FLAG_C") == 0) { value = 4; return true; }
        return false;
    }
};

static void TransferIn(Context* context, Parser* p, long* value)
{
    if (p->PeekNextItemType() == 2) {
        ++context->dirtyUpdates;
        const char* name = p->ReadAsString();
        long symbolValue = 0;
        if (!context->FindSymbol(name, symbolValue))
            ++context->errors;
        *value = symbolValue;
    }
    else
        *value = p->ReadAsInteger();
    if (p->SkipPastNextItemIfIs("|")) { long rhs; TransferIn(context, p, &rhs); *value |= rhs; }
    else if (p->SkipPastNextItemIfIs("+")) { long rhs; TransferIn(context, p, &rhs); *value += rhs; }
    else if (p->SkipPastNextItemIfIs("-")) { long rhs; TransferIn(context, p, &rhs); *value -= rhs; }
    else if (p->SkipPastNextItemIfIs("<<")) { long rhs; TransferIn(context, p, &rhs); *value <<= rhs; }
    else if (p->SkipPastNextItemIfIs(">>")) { long rhs; TransferIn(context, p, &rhs); *value >>= rhs; }
}

static long Eval(const char** tokens, Context& c)
{
    Parser p = { tokens, 0 };
    long v = -99;
    TransferIn(&c, &p, &v);
    return v;
}

int main()
{
    int ok = 1;
    Context c = { 0, 0 };
    const char* t1[] = { "FLAG_A", "|", "FLAG_C", 0 };
    if (Eval(t1, c) != 5 || c.dirtyUpdates != 2 || c.errors != 0) ok = 0;
    const char* t2[] = { "10", "-", "3", "-", "2", 0 };            // right-recursive: 10 - (3 - 2)
    if (Eval(t2, c) != 9) ok = 0;
    const char* t3[] = { "1", "<<", "4", "+", "1", 0 };            // 1 << (4 + 1)
    if (Eval(t3, c) != 32) ok = 0;
    const char* t4[] = { "-64", ">>", "2", 0 };
    if (Eval(t4, c) != -16) ok = 0;
    const char* t5[] = { "MISSING", "+", "7", 0 };                 // unknown symbol: error, reads 0
    if (Eval(t5, c) != 7 || c.errors != 1) ok = 0;
    const char* t6[] = { "42", 0 };
    if (Eval(t6, c) != 42) ok = 0;
    if (!ok) { std::printf("009e2590_TEST FAIL\n"); return 1; }
    std::printf("009e2590_TEST PASS\n");
    return 0;
}