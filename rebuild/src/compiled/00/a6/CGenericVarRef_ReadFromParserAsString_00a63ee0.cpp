// CGenericVarRef::ReadFromParserAsString -- parse a quoted string into a referenced CCharString
// Leaf: ?ReadFromParserAsString@CGenericVarRef@@AAE_NAAVCStringParser@@@Z
// Retail00A63EE0,217bytes. Actual string/parser headers retained; parser donor cursor differs
// from native but this function passes the object through without layout access.
// Fix (sweep 3): the item is the PDB's CParsedItem (was the invented ParsedItem) with its accessors under their PDB names,
// GetSymbol() const 0x009B9870 (the opening quote is a type-1 symbol) and GetString() const 0x009B9880 (retail body order =
// the PDB member run GetSymbol, GetString, GetInteger, GetFloat at 0x009B9870..0x009B98A0); the parser is CStringParser
// itself (was the invented ParserMethods wrapper) with ReadNextItem 0x009BA080 / ReadAsStringUntilString 0x009BA110; and
// ReadFromParserAsString is CGenericVarRef's private member (Ego_r/FableWin ?ReadFromParserAsString@CGenericVarRef@@AAE...).
// Same bytes.
// NF52 (round 7): the quote test is fable_string.h's CCharString::operator==(const char*) (FableWin ??8CCharString@@QBE_NPBD@Z), expanded
// here with both arms (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ): a null string strcmp's the literal with "", else
// CBasicString<char>::operator==(const char*) is called out of line (??8?$CBasicString@D@@QBE_NPBD@Z, 0x004115A0). Were the made-up
// StringDataMethods::Equals wrapper over a reinterpret_cast of the string's data pointer.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include "fable_compiler.h"
#include "fable_string.h"
#include <string.h>
class CParsedItem{public:long Type;long Value;CCharString String;const CCharString& GetSymbol()const;const CCharString& GetString()const;}; //009B9870/009B9880
class CStringParser{public:bool ReadNextItem(CParsedItem&);bool ReadAsStringUntilString(const CCharString&,CParsedItem&);}; //009BA080/009BA110
class CGenericVarRef{public:long Type;CCharString*PData;private:bool ReadFromParserAsString(CStringParser&);};
bool CGenericVarRef::ReadFromParserAsString(CStringParser&parser){CParsedItem item;if(!parser.ReadNextItem(item)||item.Type!=1)return false;if(!(item.GetSymbol()=="\""))return false;bool read;{CCharString delimiter("\"",-1);read=parser.ReadAsStringUntilString(delimiter,item);}if(!read)return false;*PData=item.GetString();parser.ReadNextItem(item);return true;}