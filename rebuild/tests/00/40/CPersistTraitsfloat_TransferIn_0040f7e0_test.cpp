#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "0040f7e0"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

class CCharString {
public:
    char m_text[64];
    CCharString() { m_text[0] = 0; }
    CCharString(const char* t) { std::strcpy(m_text, t); }
};
CCharString __fastcall operator+(const char* left, const CCharString& right)
{
    CCharString r(left);
    std::strcat(r.m_text, right.m_text);
    return r;
}
static const char g_msgUnknownSymbol[] = "Unknown symbol: ";
struct CStringParser {
    long nextType;
    const char* symbol;
    float number;
    long PeekNextItemType() { return nextType; }
    CCharString ReadAsString() { return CCharString(symbol); }
    float ReadAsFloat() { return number; }
};
struct CSymbolMap {
    bool Lookup(const CCharString& name, LONG32* value) const
    {
        if (std::strcmp(name.m_text, "HALF") == 0) { *value = 7; return true; }
        return false;
    }
};
static int g_errors = 0; static char g_lastError[64];
struct CPersistContext {
    void* PDefinitionManager;
    CSymbolMap* PSymbolMap;
    void Error(const CStringParser&, const CCharString& message) { ++g_errors; std::strcpy(g_lastError, message.m_text); }
    CSymbolMap* GetSymbolMap() const { return PSymbolMap; }
};
enum { PARSED_ITEM_SYMBOL = 2 };
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& p, T* out) const
    {
        if (p.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
            CCharString name = p.ReadAsString();
            LONG32 value = -1;   // retail leaves it unset on a miss; the test pins it
            if (!context.GetSymbolMap()->Lookup(name, &value))
                context.Error(p, g_msgUnknownSymbol + name);
            *out = (float)value;
        }
        else
            *out = p.ReadAsFloat();
    }
};
int main()
{
    CSymbolMap map; CPersistContext c; c.PSymbolMap = &map;
    CStringParser p; CPersistTraits<float> t; float out = 0;
    p.nextType = 1; p.number = 3.25f;
    t.TransferIn(c, p, &out);
    CHECK(out == 3.25f && g_errors == 0);
    p.nextType = PARSED_ITEM_SYMBOL; p.symbol = "HALF";
    t.TransferIn(c, p, &out);
    CHECK(out == 7.0f && g_errors == 0);
    p.symbol = "NOPE";
    t.TransferIn(c, p, &out);
    CHECK(g_errors == 1 && std::strcmp(g_lastError, "Unknown symbol: NOPE") == 0 && out == -1.0f);
    std::printf("0040f7e0_TEST PASS\n");
    return 0;
}