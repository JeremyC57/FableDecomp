// CPersistTraits<pair<long, COpinionDeed*>>::TransferIn parses "(handle,deed)".
#include "fable_compiler.h"
#include <stdio.h>
#include <string.h>
template <class T> class CBasicString { public:
    T* Data;
    bool operator==(const T* text) const;
    FABLE_FORCEINLINE bool Equals(const T* text) const { return Compare(Data, text) == 0; }
protected:
    static long __fastcall Compare(const T* a, const T* b);
};
class CCharString { public:
    CBasicString<char>* PStringData;
    ~CCharString();
    FABLE_FORCEINLINE bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0) return true;
            return false;
        }
        return PStringData->Equals(text);
    }
    FABLE_FORCEINLINE bool IsEqualSlow(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0) return true;
            return false;
        }
        return *PStringData == text;
    }
};
class CStringParser { public: CCharString ReadAsSymbol(); };
class CPersistContext;
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair() : first(_T1()), second(_T2()) { }
};
}
class COpinionDeed;
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    CPersistTraits() : DefaultVal(T()) { }
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};
template <> class CPersistTraits<long> {
public:
    long DefaultVal;
    CPersistTraits() : DefaultVal(0) { }
    long TransferIn(CPersistContext& context, CStringParser& parser)
    {
        long value;
        ::TransferIn(&context, &parser, &value);
        return value;
    }
};
template <class A, class B> class CPersistTraits<std::pair<A, B> > {
public:
    std::pair<A, B> DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, std::pair<A, B>& value);
};

static const char* g_symbols[8];
static int g_step;
static int g_long_calls;
static int g_deed_calls;
static long g_handle;
static COpinionDeed* g_deed;
CCharString CStringParser::ReadAsSymbol()
{
    CCharString symbol;
    static CBasicString<char> storage[8];
    storage[g_step].Data = (char*)g_symbols[g_step];
    symbol.PStringData = &storage[g_step];
    ++g_step;
    return symbol;
}
CCharString::~CCharString() {}
long __fastcall CBasicString<char>::Compare(const char* a, const char* b) { return strcmp(a, b); }
template <> bool CBasicString<char>::operator==(const char* text) const { return strcmp(Data, text) == 0; }
void __fastcall TransferIn(CPersistContext*, CStringParser*, long* value)
{
    ++g_long_calls;
    *value = g_handle;
}
template <>
void CPersistTraits<COpinionDeed*>::TransferIn(CPersistContext&, CStringParser&, COpinionDeed*& value)
{
    ++g_deed_calls;
    value = g_deed;
}

int main()
{
    CPersistTraits<std::pair<long, COpinionDeed*> > traits;
    CPersistContext* context = 0;
    CStringParser parser;
    COpinionDeed* deed = (COpinionDeed*)0x28;
    g_handle = 42;
    g_deed = deed;
    g_symbols[0] = "("; g_symbols[1] = ","; g_symbols[2] = ")"; g_symbols[3] = 0;
    g_step = 0; g_long_calls = 0; g_deed_calls = 0;
    std::pair<long, COpinionDeed*> value;
    traits.TransferIn(*context, parser, value);
    int ok = value.first == 42 && value.second == deed && g_long_calls == 1 && g_deed_calls == 1 && g_step == 3;
    g_symbols[0] = "nope"; g_symbols[1] = 0;
    g_step = 0; g_long_calls = 0; g_deed_calls = 0;
    std::pair<long, COpinionDeed*> untouched;
    traits.TransferIn(*context, parser, untouched);
    ok = ok && untouched.first == 0 && untouched.second == 0 && g_long_calls == 0 && g_deed_calls == 0 && g_step == 1;
    if (ok) printf("006c9350_TEST PASS\n");
    else printf("006c9350_TEST FAIL\n");
    return ok ? 0 : 1;
}