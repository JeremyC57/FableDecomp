// Leaf: ?TransferIn@?$CPersistTraits@VC3DVector@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/C3DVector.h"
#include "engine/CPersistTraits.h"

void CPersistTraits<C3DVector>::TransferIn(CPersistContext& persist_context, CStringParser& parser, C3DVector& val)
{
    if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        // Retail returns at the first failed parser assertion.
        if (!(parser.ReadAsSymbol() == "."))
            return;
        CCharString name = parser.ReadAsString();
        CPersistTraits<float> float_traits;
        float fval;
        float_traits.TransferIn(persist_context, parser, fval);
        if (name == "X")
            val.X = fval;
        else if (name == "Y")
            val.Y = fval;
        else if (name == "Z")
            val.Z = fval;
        else
            persist_context.PersistError(parser, "Unknown entry '" + name + " in C3DCoordF");
    } else if (parser.ReadAsString() == "C3DCoordF") {
        CPersistTraits<float> float_traits;
        if (!(parser.ReadAsSymbol() == "("))
            return;
        float_traits.TransferIn(persist_context, parser, val.X);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        float_traits.TransferIn(persist_context, parser, val.Y);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        float_traits.TransferIn(persist_context, parser, val.Z);
        if (!(parser.ReadAsSymbol() == ")"))
            return;
    } else {
        persist_context.PersistError(parser, "Unknown command for C3DCoordF");
    }
}