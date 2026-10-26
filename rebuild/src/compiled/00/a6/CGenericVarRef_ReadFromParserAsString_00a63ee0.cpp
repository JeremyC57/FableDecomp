// CGenericVarRef::ReadFromParserAsString -- parse a quoted string into a referenced CCharString
// Leaf: ?ReadFromParserAsString@CGenericVarRef@@AAE_NAAVCStringParser@@@Z
// Retail00A63EE0,217bytes. Actual string/parser headers retained; parser donor cursor differs
// from native but this function passes the object through without layout access.
// Fix (sweep 3): the item is the PDB's CParsedItem (was the invented ParsedItem) with its accessors under their PDB names,
// GetSymbol() const 0x009B9870 (the opening quote is a type-1 symbol) and GetString() const 0x009B9880 (retail body order =
// the PDB member run GetSymbol, GetString, GetInteger, GetFloat at 0x009B9870..0x009B98A0); the parser is CStringParser
// itself (was the invented ParserMethods wrapper) with ReadNextItem 0x009BA080 / ReadAsStringUntilString 0x009BA110; and
// ReadFromParserAsString is CGenericVarRef's private member (Ego_r/FableWin ?ReadFromParserAsString@CGenericVarRef@@AAE...).
// Same bytes. Left: the StringDataMethods::Equals wrapper for CBasicString<char>::operator==(const char*) 0x004115A0.
#include "fable_string.h"
#include <string.h>
struct StringDataMethods:CCharStringData{bool Equals(const char*)const;}; //004115A0 CBasicString<char>::operator==
extern const char EmptyString[]; //0122D70E empty CBasicString fallback
static __forceinline bool StringEquals(const CCharString&s,const char*t){CCharStringData*d=*reinterpret_cast<CCharStringData*const*>(&s);if(!d)return strcmp(t,EmptyString)==0;return static_cast<StringDataMethods*>(d)->Equals(t);}
class CParsedItem{public:long Type;long Value;CCharString String;const CCharString& GetSymbol()const;const CCharString& GetString()const;}; //009B9870/009B9880
class CStringParser{public:bool ReadNextItem(CParsedItem&);bool ReadAsStringUntilString(const CCharString&,CParsedItem&);}; //009BA080/009BA110
class CGenericVarRef{public:long Type;CCharString*PData;private:bool ReadFromParserAsString(CStringParser&);};
bool CGenericVarRef::ReadFromParserAsString(CStringParser&parser){CParsedItem item;if(!parser.ReadNextItem(item)||item.Type!=1)return false;CCharStringData*data=*reinterpret_cast<CCharStringData*const*>(&item.GetSymbol());if(!data){if(strcmp("\"",EmptyString)!=0)return false;}else{if(!static_cast<StringDataMethods*>(data)->Equals("\""))return false;}bool read;{CCharString delimiter("\"",-1);read=parser.ReadAsStringUntilString(delimiter,item);}if(!read)return false;*PData=item.GetString();parser.ReadNextItem(item);return true;}