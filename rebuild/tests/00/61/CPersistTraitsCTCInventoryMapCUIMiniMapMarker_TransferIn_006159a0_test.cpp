// behaviour test for 0x006159A0 CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn (body copied from the row source)
// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIMiniMapMarker& value) -- text load of a mini-map marker
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCInventoryMap.h"

template<class T> __declspec(noinline) long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
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

template<> void CPersistTraits<float>::TransferIn(CPersistContext& context, CStringParser& parser, float& value);   // 0x0040F7E0
template<> void CPersistTraits<CCharString>::TransferIn(CPersistContext& context, CStringParser& parser, CCharString& value);   // 0x0040E510
template<> class CPersistTraits<CTCInventoryMap::CUIMiniMapMarker> {
public:
    CTCInventoryMap::CUIMiniMapMarker DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIMiniMapMarker& value);
};

void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser,
                                                                   CTCInventoryMap::CUIMiniMapMarker& value)
{
    CPersistTraits<float> float_traits;
    CPersistTraits<CCharString> string_traits;
    value = CTCInventoryMap::CUIMiniMapMarker();
    if (!(parser.ReadAsSymbol() == "("))
        return;
    CPersistTraits<long>().TransferIn(context, parser, value.Region);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    CPersistTraits<long>().TransferIn(context, parser, value.DefNameLength);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    string_traits.TransferIn(context, parser, value.UIDefName);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    float_traits.TransferIn(context, parser, value.Position.X);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    float_traits.TransferIn(context, parser, value.Position.Y);
    CCharString symbol = parser.ReadAsSymbol();
    if (symbol == ",") {
        value.GraphicIndex = CPersistTraitsSLONG::TransferIn(context, parser);
        symbol = parser.ReadAsSymbol();
        if (symbol == ",") {
            float_traits.TransferIn(context, parser, value.Angle);
            if (parser.ReadAsSymbol() == ")") return;
        } else if (symbol == ")") {
            value.Angle = 0.0f;
        }
    } else if (symbol == ")") {
        value.GraphicIndex = 0;
        value.Angle = 0.0f;
    }
}

// ---- test stubs: a scripted token stream ----
#include <stdio.h>
#include <stdlib.h>
struct FakeStringData { char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; long NoRefs; };
static FakeStringData* Make(const char* text)
{
    FakeStringData* d = new FakeStringData;
    d->StringLength = (unsigned long)strlen(text); d->PData = new char[d->StringLength + 1]; strcpy(d->PData, text);
    d->DataLength = d->StringLength; d->UseFastExtend = false; d->NoRefs = 1;
    return d;
}
static const char* Text(const CCharString& s) { FakeStringData* d = *(FakeStringData* const*)&s; return d ? d->PData : ""; }
static const char* g_tokens[40]; static int g_pos;
static void Script(const char* const* t) { int k = 0; for (; t[k]; ++k) g_tokens[k] = t[k]; g_tokens[k] = 0; g_pos = 0; }
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)Make(text); }
CCharString::~CCharString() { }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
static const char* NextToken() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return t ? t : ""; }
CCharString CStringParser::ReadAsSymbol() { return CCharString(NextToken()); }
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&, long& value) { value = atol(NextToken()); }
template<> void CPersistTraits<float>::TransferIn(CPersistContext&, CStringParser&, float& value) { value = (float)atof(NextToken()); }
template<> void CPersistTraits<CCharString>::TransferIn(CPersistContext&, CStringParser&, CCharString& value)
{ value = CCharString(NextToken()); }
int main()
{
    CPersistTraits<CTCInventoryMap::CUIMiniMapMarker> traits;
    CPersistContext* context = (CPersistContext*)malloc(0x400); CStringParser* parser = (CStringParser*)malloc(0x100);
    CTCInventoryMap::CUIMiniMapMarker m;
    bool ok = true;
    static const char* full[] = { "(", "4", ",", "6", ",", "Marker", ",", "1.5", ",", "-2", ",", "3", ",", "0.25", ")", 0 };
    Script(full); traits.TransferIn(*context, *parser, m);
    ok = ok && m.Region == 4 && m.DefNameLength == 6 && strcmp(Text(m.UIDefName), "Marker") == 0 && m.Position.X == 1.5f &&
         m.Position.Y == -2.0f && m.GraphicIndex == 3 && m.Angle == 0.25f && g_pos == 15;
    static const char* old1[] = { "(", "1", ",", "2", ",", "N", ",", "7", ",", "8", ",", "9", ")", 0 };
    m.Angle = 5.0f;
    Script(old1); traits.TransferIn(*context, *parser, m);
    ok = ok && m.Region == 1 && m.GraphicIndex == 9 && m.Angle == 0.0f && m.Position.Y == 8.0f;
    static const char* old2[] = { "(", "2", ",", "3", ",", "N", ",", "1", ",", "1", ")", 0 };
    Script(old2); traits.TransferIn(*context, *parser, m);
    ok = ok && m.Region == 2 && m.GraphicIndex == 0 && m.Angle == 0.0f;
    static const char* bad[] = { "[", 0 };
    m.Region = 99;
    Script(bad); traits.TransferIn(*context, *parser, m);
    ok = ok && m.Region == 0 && m.DefNameLength == 0 && strcmp(Text(m.UIDefName), "") == 0;   // reset to the default marker
    printf(ok ? "006159a0_TEST PASS\n" : "006159a0_TEST FAIL\n");
    return ok ? 0 : 1;
}