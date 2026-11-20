// CGenericVar::ReadFromParserAsFloat(CStringParser& parser) -- reads the parser's next item into the float at +0x18: an integer item (type 3) converted, a float item (type 4) as is; false for anything else
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A641A0, length 131 bytes (padded to 144).
// Callees: CCharString::CCharString @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0 (the item's text), CStringParser::ReadNextItem
//          @ 0x009BA080, CParsedItem::GetInteger @ 0x009B9890 and CParsedItem::GetFloat @ 0x009B98A0 (out of line in this TU).
// Shape: the landed ReadFromParserAsSLONG (0x00A640D0) item model -- a 12-byte frame item that owns its string; each
// successful arm runs its own item destructor before `mov al,1`; the long is converted through the dead parser argument
// slot (`mov [esp+14h],eax ; fild`).
// Naming (NF129, 2026-11-20, q4657): the local SParsedItem / CCharString stand-ins replaced by engine/CParsedItem.h (Ego_r.pdb CParsedItem:
//   Type +0, Integer/Float +4, String +8; compiler-generated ctor / dtor) and fable_string.h, so ReadNextItem binds
//   ?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z (Ego_r + FableWin) and the accessors their Ego_r names (IsInteger / IsFloat inline,
//   GetInteger / GetFloat out of line: overlay opt-in FABLE_RETAIL_CPARSEDITEM_GETVALUE_OUTOFLINE). CStringParser stays a local declaration
//   (the row only names ReadNextItem; it already modelled the class locally).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CPARSEDITEM_GETVALUE_OUTOFLINE
#include "engine/CParsedItem.h"
class CStringParser { public:
    bool ReadNextItem(CParsedItem& item);          // 0x009BA080
};
class CGenericVar { public:
    unsigned char _pad[0x18];
    float         m_floatValue;                    // +0x18
    bool ReadFromParserAsFloat(CStringParser& parser);
};

bool CGenericVar::ReadFromParserAsFloat(CStringParser& parser)
{
    CParsedItem item;
    if (parser.ReadNextItem(item)) {
        if (item.IsInteger()) {
            m_floatValue = (float)item.GetInteger();
            return true;
        }
        if (item.IsFloat()) {
            m_floatValue = item.GetFloat();
            return true;
        }
    }
    return false;
}
