// behaviour test for 0x00615E70 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn (body copied from the row source)
// CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIRegionInfo& value) -- text load of a map region record
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CPARSEDITEM_SYMBOL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_CHAR_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR
#include <string.h>
#include "engine/CParsedItem.h"
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

template<> class CPersistTraits<CTCInventoryMap::CUIRegionInfo> {
public:
    CTCInventoryMap::CUIRegionInfo DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIRegionInfo& value);
};

void CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn(CPersistContext& context, CStringParser& parser,
                                                                CTCInventoryMap::CUIRegionInfo& value)
{
    value = CTCInventoryMap::CUIRegionInfo();
    if (!(parser.ReadAsSymbol() == "("))
        return;
    value.Region = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.Shops = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.BarberShops = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.TatooShops = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.TitleShops = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.ShopsForSale = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.HousesForSale = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.ShopsOwned = CPersistTraitsSLONG::TransferIn(context, parser);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    value.HousesOwned = CPersistTraitsSLONG::TransferIn(context, parser);
    CParsedItem item;
    if (parser.PeekNextItem(item) && item.IsSymbol()) {
        if (item.GetSymbol() == ',') {
            parser.ReadAsSymbol();
            value.Taverns = CPersistTraitsSLONG::TransferIn(context, parser);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        }
        else if (parser.ReadAsSymbol() == ")") {
            value.Taverns = 0;
        }
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
static const char* g_tokens[40]; static int g_pos;
static void Script(const char* const* t) { int k = 0; for (; t[k]; ++k) g_tokens[k] = t[k]; g_tokens[k] = 0; g_pos = 0; }
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)Make(text); }
CCharString::~CCharString() { }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
static const char* NextToken() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return t ? t : ""; }
CCharString CStringParser::ReadAsSymbol() { return CCharString(NextToken()); }
bool CStringParser::PeekNextItem(CParsedItem& item)
{
    const char* t = g_tokens[g_pos];
    if (!t) return false;
    *(long*)&item = (t[0] >= '0' && t[0] <= '9') ? 3 : 1;           // PARSED_ITEM_INTEGER / PARSED_ITEM_SYMBOL
    *(CCharStringData**)((char*)&item + 8) = (CCharStringData*)Make(t);
    return true;
}
const CCharString& CParsedItem::GetSymbol() const { return *(const CCharString*)((const char*)this + 8); }
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&, long& value) { value = atol(NextToken()); }
int main()
{
    CPersistTraits<CTCInventoryMap::CUIRegionInfo> traits;
    CPersistContext* context = (CPersistContext*)malloc(0x400); CStringParser* parser = (CStringParser*)malloc(0x100);
    CTCInventoryMap::CUIRegionInfo r;
    bool ok = true;
    static const char* full[] = { "(", "4", ",", "1", ",", "2", ",", "3", ",", "5", ",", "6", ",", "7", ",", "8", ",", "9", ",", "10", ")", 0 };
    Script(full); traits.TransferIn(*context, *parser, r);
    ok = ok && r.Region == 4 && r.Shops == 1 && r.BarberShops == 2 && r.TatooShops == 3 && r.TitleShops == 5 &&
         r.ShopsForSale == 6 && r.HousesForSale == 7 && r.ShopsOwned == 8 && r.HousesOwned == 9 && r.Taverns == 10 && g_pos == 21;
    static const char* old[] = { "(", "1", ",", "1", ",", "1", ",", "1", ",", "1", ",", "1", ",", "1", ",", "1", ",", "2", ")", 0 };
    r.Taverns = 77;
    Script(old); traits.TransferIn(*context, *parser, r);
    ok = ok && r.Region == 1 && r.HousesOwned == 2 && r.Taverns == 0 && g_pos == 19;
    static const char* bad[] = { "(", "3", ";", 0 };
    r.Shops = 99;
    Script(bad); traits.TransferIn(*context, *parser, r);
    ok = ok && r.Region == 3 && r.Shops == 0 && r.Taverns == 0;          // stops at the bad separator, record reset first
    printf(ok ? "00615e70_TEST PASS\n" : "00615e70_TEST FAIL\n");
    return ok ? 0 : 1;
}