// CPersistContext::CheckUnknownSymbols() -- after a text load, report every item left unparsed in the source text as `Unknown entry "<item>"`
// Leaf: ?CheckUnknownSymbols@CPersistContext@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BBBF0, 207 bytes, `ret` (Ego_r.pdb ?CheckUnknownSymbols@CPersistContext@@QAEXXZ).
// Callees: CStringParser(const CStringParser&) 0x0046381B, CCharString() 0x0099E4B0 (the CParsedItem's string), CStringParser::
//   SkipWhitespace 0x009B9C60 / NextItemExists 0x009BA4F0 / ReadAsStringUntilWhitespace 0x009BA4A0, operator+(const char*, const
//   CCharString&) 0x0099F690 and operator+(const CCharString&, const char*) 0x0099F600, CPersistContext::PersistError(const
//   CStringParser&, const CCharString&) const 0x009BAA90 (Ego_r.pdb), ~CCharString 0x0099EAE0, list<CStringParserCommentBoundaries>
//   ::~list 0x004046B0, ~CBaseClass 0x0099A2E0.
// Shape: the context's parser (PParser +0x8) is copied so the scan does not move the real cursor; an unused CParsedItem local
//   (12 bytes: Type, the Integer/Float union, String +8) sits after it -- only its string is built and destroyed, and its
//   8 leading bytes are the frame gap retail keeps between the message temporaries and the string. The message is one
//   expression, its three temporaries die after the PersistError call.
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CStringParser.h"
#include "engine/CParsedItem.h"
#include "engine/CPersistContext.h"

void CPersistContext::CheckUnknownSymbols()
{
    CStringParser parser(*PParser);
    CParsedItem item;
    parser.SkipWhitespace();
    while (parser.NextItemExists()) {
        PersistError(parser, "Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\"");
        parser.SkipWhitespace();
    }
}