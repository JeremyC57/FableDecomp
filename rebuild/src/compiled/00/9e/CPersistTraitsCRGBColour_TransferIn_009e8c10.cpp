// CPersistTraits<CRGBColour>::TransferIn(CPersistContext& context, CStringParser& parser, CRGBColour& value) -- text load of a colour: either one channel as ".R <n>" / ".G" / ".B" / ".A", or the whole "CRGBColour(r,g,b,a)"
// Leaf: ?TransferIn@?$CPersistTraits@VCRGBColour@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCRGBColour@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E8C10, 1134 bytes, `ret 0Ch` (the empty traits `this` is unused).
// Identity: Ego_r.pdb ?TransferIn@?$CPersistTraits@VCRGBColour@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCRGBColour@@@Z
//   (donor 0x00B3AC80, between the C3DAngles TransferIn and the by-value CRGBColour overload, the same order as the retail
//   persist-traits TU: 0x009E8110 TransferOut, this row, 0x009E9080 CPersistTraits<unsigned char>::TransferIn); strings
//   "Unknown entry in CRGBColour" 0x0129C360, "CRGBColour" 0x0129C354, "Unknown command for CRGBColour" 0x0129C334. The
//   catalog's `_global` placement is the propagated label; the sole caller is 0x006421E5.
// Callees: CStringParser::PeekNextItemType 0x009BA900 / ReadAsSymbol 0x009BA790 / ReadAsString 0x009BA6F0, CBasicString<char>::
//   operator==(const char*) const 0x004115A0, ~CCharString 0x0099EAE0, CCharString(const char*, long) 0x0099EBF0,
//   CPersistContext::PersistError 0x009BAA90, CPersistTraitsSLONG::TransferIn 0x009E2590 (FableWin static __fastcall),
//   the generic CPersistTraits<T>::TransferIn out of line for unsigned char (0x009E9080) and for the 4-byte channel
//   temporary (0x005D8167, CPersistTraits<long> -- see NOTES.md).
// Shape: engine/CStringParser.h, CPersistContext.h, CPersistTraits.h (generic inline TransferIn: SLONG read into a long, then
//   the cast store) and CRGBColour.h. Three devices reproduce retail:
//   - the CBasicString<T>::Compare / operator==(const T*) bodies in the TU (as landed 0x009BB110): VC7.1 then knows the
//     out-of-line compare leaves `name` alone, keeps its data pointer in esi and threads the null test once over the
//     R/G/B/A chain; and CCharString's text operator== forced inline (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ);
//   - `if (!(symbol == ".")) return;` with `name` in the same block: the named local's slot is reserved for the block,
//     so the "." temporary takes the separate 4-byte local (`push ecx`) retail has;
//   - the closing `if (... == ")") return;` (an empty if lets VC7.1 drop the now-visible pure compare); the errors pass
//     bare literals (implicit conversion: the temporary re-addressed with `lea` after the constructor).
//   VC7.1 itself expands the first two traits calls of each path and calls the last two out of line.
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