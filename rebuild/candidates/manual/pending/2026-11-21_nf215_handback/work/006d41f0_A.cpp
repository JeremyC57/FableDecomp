// CPersistTraits<std::pair<CCharString,long> >::TransferIn(CPersistContext& context, CStringParser& parser, std::pair<CCharString,long>& value) -- reads "(" name "," number ")"
// Leaf: ?TransferIn@?$CPersistTraits@U?$pair@VCCharString@@J@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@VCCharString@@J@std@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_TEXT_NE_INLINE
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

template<> void CPersistTraits<std::pair<CCharString,long> >::TransferIn(CPersistContext& context,CStringParser& parser,std::pair<CCharString,long>& value)
{
    CPersistTraits<CCharString> firstTraits;
    if (parser.ReadAsSymbol() != "(")
        return;
    firstTraits.TransferIn(context,parser,value.first);
    if (parser.ReadAsSymbol() == ",") {
        value.second = CPersistTraitsSLONG::TransferIn(context,parser);
        bool closed = parser.ReadAsSymbol() == ")";
        (void)closed;
    }
}
