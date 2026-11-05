// CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIRegionInfo& value) -- text load of a map region record "(region,shops,barbers,tattoos,titles,forSale,housesForSale,owned,housesOwned[,taverns])" over a zeroed record
// Leaf: ?TransferIn@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCUIRegionInfo@CTCInventoryMap@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00615E70, 1423 bytes, `ret 0Ch` (traits `this` unused), 0x38-byte frame, no EH.
// Identity: Ego_r.pdb ?TransferIn@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@
//   AAVCUIRegionInfo@CTCInventoryMap@@@Z; sole caller CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>::TransferIn
//   0x00615E10 (landed, names this address). Field order = the landed text writer 0x00614290 (TatooShops +0x10 before
//   TitleShops +0xC). The catalog name is right.
// Shape: the sibling CUIMiniMapMarker reader 0x006159A0 (landed): `value = CUIRegionInfo()` (Ego_r ??0CUIRegionInfo, ten zero
//   stores into a temporary, then `rep movsd`), then a chain of `if (!(parser.ReadAsSymbol() == ",")) return;` with each long
//   read through the value-returning CPersistTraitsSLONG::TransferIn 0x009E2590 (the generic CPersistTraits<long> reader goes
//   out of line at the later, colder sites). The trailing Taverns field is optional (versioned format): a CParsedItem is
//   peeked (CStringParser::PeekNextItem 0x009BA8C0); only a symbol item is looked at -- "," (CCharString::operator==(char),
//   inline: {',', 0} built in the dead context slot, strcmp intrinsic) reads the field and the closing ")", anything else
//   must be ")" and zeroes Taverns. Any other peeked item leaves the record as read.
// Callees: CStringParser::ReadAsSymbol 0x009BA790, CBasicString<char>::Compare 0x00411570 (body in the TU, as the sibling),
//   ~CCharString 0x0099EAE0, CPersistTraitsSLONG::TransferIn 0x009E2590, CCharString() 0x0099E4B0 (CParsedItem's implicit
//   ctor), CStringParser::PeekNextItem 0x009BA8C0, CParsedItem::GetSymbol 0x009B9870. Strings "(" 0x0122E028, "," 0x0122E024,
//   ")" 0x0122E020, "" 0x0122D70E.
// Headers: FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR (engine/CTCInventoryMap.h), FABLE_RETAIL_CPARSEDITEM_SYMBOL
//   (engine/CParsedItem.h), FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM (engine/CStringParser.h), FABLE_RETAIL_CCHARSTRING_CHAR_EQ
//   (fable_string.h).
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