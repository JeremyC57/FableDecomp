// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser, CTCInventoryMap::CUIMiniMapMarker& value) -- text load of a mini-map marker "(region,nameLength,name,x,y[,graphic[,angle]])" over a default marker
// Leaf: ?TransferIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCUIMiniMapMarker@CTCInventoryMap@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006159A0, 1127 bytes, `ret 0Ch` (traits `this` unused).
// Identity: Ego_r.pdb ?TransferIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@
//   AAVCUIMiniMapMarker@CTCInventoryMap@@@Z; sole caller CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::
//   TransferIn 0x00615920 (landed; 0x1C stride). Field order = the landed text writer 0x00614080 (Region, DefNameLength,
//   UIDefName, Position.X, Position.Y, GraphicIndex, Angle). The catalog's `_global::TransferOut_HeroTraits` is an autoname.
// Callees: CCharString() 0x0099E4B0 (string traits DefaultVal), CCharString(const char*, long) 0x0099EBF0, operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CStringParser::ReadAsSymbol 0x009BA790, CBasicString<char>::Compare 0x00411570 (static fastcall),
//   CPersistTraitsSLONG::TransferIn 0x009E2590, CPersistTraits<CCharString>::TransferIn 0x0040E510, CPersistTraits<float>::
//   TransferIn 0x0040F7E0.
// Shape: `value = CUIMiniMapMarker()` first (the default marker through the implicit operator=); engine/CTCInventoryMap.h for the
//   nested marker. In this TU CBasicString<char>::operator==(const char*) is expanded (`Compare(PData, text) == 0`,
//   neg/sbb/inc) and Compare is called: its body is in the TU (noinline, as retail's out-of-line 0x00411570), so VC7.1 knows the
//   compare leaves `symbol` alone, keeps its data pointer in esi and threads the null test. The format is versioned: the
//   trailing ",graphic" / ",angle" are optional (")" zeroes the missing fields). The prefix is a chain of
//   `if (!(sym == ",")) return;` so the named `symbol` local keeps its own frame slot. GraphicIndex is read with the
//   value-returning CPersistTraitsSLONG::TransferIn: the generic CPersistTraits<long> reader there is called out of line by
//   VC7.1 in every chain shape tried, where retail expands it (same bytes as the expansion; original spelling unproven).
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