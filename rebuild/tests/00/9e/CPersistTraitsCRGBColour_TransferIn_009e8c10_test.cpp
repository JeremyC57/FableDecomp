// behaviour test for 0x009E8C10 CPersistTraits<CRGBColour>::TransferIn (body copied from the row source)
// CPersistTraits<CRGBColour>::TransferIn(CPersistContext& context, CStringParser& parser, CRGBColour& value) -- text load of a colour
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CRGBColour.h"

// Row-local template bodies (fable_string.h declares CBasicString<T>::Compare 0x00411570 and operator==(const T*) 0x004115A0;
// this TU holds their bodies, as the landed 0x009BB110 does).
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

template<> class CPersistTraits<CRGBColour> {
public:
    CRGBColour DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, CRGBColour& value);
};

void CPersistTraits<CRGBColour>::TransferIn(CPersistContext& context, CStringParser& parser, CRGBColour& value)
{
    if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        if (!(parser.ReadAsSymbol() == "."))
            return;
        CCharString name = parser.ReadAsString();
            if (name == "R") {
                long channel;
                CPersistTraits<long>().TransferIn(context, parser, channel);
                value.R = (unsigned char)channel;
            } else if (name == "G") {
                long channel;
                CPersistTraits<long>().TransferIn(context, parser, channel);
                value.G = (unsigned char)channel;
            } else if (name == "B") {
                long channel;
                CPersistTraits<long>().TransferIn(context, parser, channel);
                value.B = (unsigned char)channel;
            } else if (name == "A") {
                long channel;
                CPersistTraits<long>().TransferIn(context, parser, channel);
                value.A = (unsigned char)channel;
            } else {
                context.PersistError(parser, "Unknown entry in CRGBColour");
            }
    } else if (parser.ReadAsString() == "CRGBColour") {
        if (parser.ReadAsSymbol() == "(") {
            CPersistTraits<unsigned char>().TransferIn(context, parser, value.R);
            if (parser.ReadAsSymbol() == ",") {
                CPersistTraits<unsigned char>().TransferIn(context, parser, value.G);
                if (parser.ReadAsSymbol() == ",") {
                    CPersistTraits<unsigned char>().TransferIn(context, parser, value.B);
                    if (parser.ReadAsSymbol() == ",") {
                        CPersistTraits<unsigned char>().TransferIn(context, parser, value.A);
                        if (parser.ReadAsSymbol() == ")") return;
                    }
                }
            }
        }
    } else {
        context.PersistError(parser, "Unknown command for CRGBColour");
    }
}

// ---- test stubs: a scripted token stream ----
#include <stdio.h>
#include <stdlib.h>
struct FakeStringData { char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; long NoRefs; };
static const char* g_tokens[32]; static int g_pos; static int g_errors; static char g_lastError[64];
static void Script(const char* a, const char* b = 0, const char* c = 0, const char* d = 0, const char* e = 0,
                   const char* f = 0, const char* g = 0, const char* h = 0, const char* i = 0, const char* j = 0)
{
    const char* t[10] = { a, b, c, d, e, f, g, h, i, j };
    for (int k = 0; k < 10; ++k) g_tokens[k] = t[k];
    g_tokens[10] = 0; g_pos = 0;
}
CCharString::CCharString(const char* text, long)
{
    FakeStringData* d = new FakeStringData;
    d->StringLength = (unsigned long)strlen(text); d->PData = new char[d->StringLength + 1]; strcpy(d->PData, text);
    d->DataLength = d->StringLength; d->UseFastExtend = false; d->NoRefs = 1;
    PStringData = (CCharStringData*)d;
}
CCharString::~CCharString() { }
static CCharString Next() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return CCharString(t ? t : ""); }
CCharString CStringParser::ReadAsSymbol() { return Next(); }
CCharString CStringParser::ReadAsString() { return Next(); }
EParsedItemType CStringParser::PeekNextItemType()
{ const char* t = g_tokens[g_pos]; return (t && strcmp(t, ".") == 0) ? PARSED_ITEM_SYMBOL : PARSED_ITEM_STRING; }
void CPersistContext::PersistError(const CStringParser&, const CCharString& message) const
{ ++g_errors; strncpy(g_lastError, (*(FakeStringData* const*)&message)->PData, 63); }
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&, long& value)
{ const char* t = g_tokens[g_pos]; if (t) ++g_pos; value = t ? atol(t) : 0; }
int main()
{
    CPersistTraits<CRGBColour> traits;
    CPersistContext* context = (CPersistContext*)malloc(0x400); CStringParser* parser = (CStringParser*)malloc(0x100);
    CRGBColour c; c.R = c.G = c.B = c.A = 0;
    bool ok = true;
    Script("CRGBColour", "(", "10", ",", "20", ",", "30", ",", "40", ")");
    traits.TransferIn(*context, *parser, c);
    ok = ok && c.R == 10 && c.G == 20 && c.B == 30 && c.A == 40 && g_pos == 10 && g_errors == 0;
    Script(".", "G", "77"); traits.TransferIn(*context, *parser, c); ok = ok && c.G == 77 && c.R == 10;
    Script(".", "A", "300"); traits.TransferIn(*context, *parser, c); ok = ok && c.A == (unsigned char)300;
    Script(".", "B", "5"); traits.TransferIn(*context, *parser, c); ok = ok && c.B == 5;
    Script(".", "Q", "5"); traits.TransferIn(*context, *parser, c);
    ok = ok && g_errors == 1 && strcmp(g_lastError, "Unknown entry in CRGBColour") == 0 && g_pos == 2;
    Script("Vector", "(", "1"); traits.TransferIn(*context, *parser, c);
    ok = ok && g_errors == 2 && strcmp(g_lastError, "Unknown command for CRGBColour") == 0;
    Script("CRGBColour", "(", "1", ";", "9"); traits.TransferIn(*context, *parser, c);
    ok = ok && c.R == 1 && c.G == 77 && g_pos == 4;     // stops at the missing comma
    printf(ok ? "009e8c10_TEST PASS\n" : "009e8c10_TEST FAIL\n");
    return ok ? 0 : 1;
}