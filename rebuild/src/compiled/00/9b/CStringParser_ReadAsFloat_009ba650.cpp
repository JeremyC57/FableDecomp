// CStringParser::ReadAsFloat() -- read the next item as a float (an integer item converts), else report "Error parsing float" and return 0
// Leaf: ?ReadAsFloat@CStringParser@@QAEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BA650, 154 bytes; Ego_r.pdb ?ReadAsFloat@CStringParser@@QAEMXZ.
// Callees: CCharString() 0x0099E4B0 (the item's string), CStringParser::ReadNextItem(CParsedItem&) 0x009BA080,
//   ~CCharString 0x0099EAE0, CCharString(const char*, long = -1) 0x0099EBF0, CStringParser::Error 0x009B95F0 (protected).
// Shape: the PDB CParsedItem accessors (IsFloat / IsInteger / GetFloat / GetInteger, header overlay FABLE_RETAIL_CPARSEDITEM):
//   GetInteger's long result is kept in a frame word across the item's destructor and converted after it (`fild` last);
//   `return item.Integer` direct converts before the destructor. The message is the bare literal (implicit conversion
//   through the retail default-length CCharString ctor), its temporary's address pushed with `lea eax`. The error test comes
//   first (`!ReadNextItem || (!IsFloat && !IsInteger)`), then `IsFloat ? GetFloat : GetInteger` as two returns: VC7.1 merges the
//   repeated IsFloat test and colours the integer arm's copy edx, the float arm's ecx like retail (the nested if/else form
//   swaps them).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#include "engine/CStringParser.h"

float CStringParser::ReadAsFloat()
{
    CParsedItem item;
    if (!ReadNextItem(item) || (!item.IsFloat() && !item.IsInteger())) {
        Error("Error parsing float");
        return 0.0f;
    }
    if (item.IsFloat())
        return item.GetFloat();
    return item.GetInteger();
}