// CPersistTraits<C2DVector>::TransferIn(CPersistContext& context, CStringParser& parser, C2DVector& value) -- text load of a 2D coordinate: either one member entry ".X <float>" / ".Y <float>", or the whole "C2DCoordF(x,y)"
// Leaf: ?TransferIn@?$CPersistTraits@VC2DVector@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E81E0, 750 bytes, thiscall `ret 0Ch` (the empty traits `this` is never read).
// Identity: Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistTraits@VC2DVector@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVC2DVector@@@Z;
//   the strings name it ("C2DCoordF" 0x0129C288, "Unknown command for C2DCoordF" 0x0129C268, "Unknown entry '" 0x0129C294 +
//   "' in C2DCoordF" 0x0129C2A4, "X" 0x0129C2B8 / "Y" 0x0129C2B4) and it sits in the persist-traits TU between the landed
//   C2DVector TransferOut 0x009E7F00 and the CRGBColour TransferIn 0x009E8C10. The catalog label CBrainBehaviour is propagated.
// Callees: CStringParser::PeekNextItemType 0x009BA900 / ReadAsSymbol 0x009BA790 / ReadAsString 0x009BA6F0, CBasicString<char>::
//   operator==(const char*) const 0x004115A0, ~CCharString 0x0099EAE0, CCharString(const char*, long) 0x0099EBF0,
//   operator+(const char*, const CCharString&) 0x0099F690 / operator+(const CCharString&, const char*) 0x0099F600,
//   CPersistTraits<float>::TransferIn 0x0040F7E0, CPersistContext::PersistError 0x009BAA90.
// Shape (the landed twin CPersistTraits<CRGBColour>::TransferIn 0x009E8C10 recipe):
//   - the CBasicString<T>::Compare / operator==(const T*) bodies in the TU: VC7.1 knows the out-of-line compare leaves `name`
//     alone, keeps its data pointer in esi and tests it for null once over the X / Y chain (0x009E8260..0x009E8296);
//   - the float traits body visible (FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY, noinline): the empty traits object
//     shares the read value's frame word (`lea ecx,[esp+24h]` after three pushes, 0x009E8257);
//   - the closing `if (parser.ReadAsSymbol() == ")") return;`: the early return keeps the now-pure compare and its result
//     homed in the dead context slot (0x009E846E `mov byte ptr [esp+18h],al`); a bare expression statement or a named
//     unused bool lets VC7.1 drop it (679 v 750 bytes).
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/C2DVector.h"
#include "engine/CPersistTraits.h"

// Row-local template bodies (fable_string.h declares CBasicString<T>::Compare 0x00411570 and operator==(const T*) 0x004115A0;
// this TU holds their bodies, as the landed 0x009BB110 / 0x009E8C10 do).
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

template<> void CPersistTraits<C2DVector>::TransferIn(CPersistContext& context, CStringParser& parser, C2DVector& value)
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