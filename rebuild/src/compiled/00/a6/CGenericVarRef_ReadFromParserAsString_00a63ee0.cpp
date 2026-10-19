// CGenericVarRef::ReadFromParserAsString -- parse a quoted string into a referenced CCharString
// Leaf: ReadFromParserAsString
// Retail00A63EE0,217bytes. Actual string/parser headers retained; parser donor cursor differs
// from native but this function passes the object through without layout access.
#include "fable_string.h"
#include "engine/CStringParser.h"
#include <string.h>
struct StringDataMethods:CCharStringData{bool Equals(const char*)const;}; //004115A0 CBasicString<char>::operator==
extern const char EmptyString[]; //0122D70E empty CBasicString fallback
static __forceinline bool StringEquals(const CCharString&s,const char*t){CCharStringData*d=*reinterpret_cast<CCharStringData*const*>(&s);if(!d)return strcmp(t,EmptyString)==0;return static_cast<StringDataMethods*>(d)->Equals(t);}
struct ParsedItem{long Type;long Value;CCharString Text;const CCharString& GetAsToken()const;const CCharString& GetAsString()const;}; //009B9870/009B9880
struct ParserMethods:CStringParser{bool ReadNextItem(ParsedItem&);bool ReadAsStringUntilString(const CCharString&,ParsedItem&);}; //009BA080/009BA110
struct CGenericVarRef{long Type;CCharString*PData;bool ReadFromParserAsString(ParserMethods&);};
bool CGenericVarRef::ReadFromParserAsString(ParserMethods&parser){ParsedItem item;if(!parser.ReadNextItem(item)||item.Type!=1)return false;CCharStringData*data=*reinterpret_cast<CCharStringData*const*>(&item.GetAsToken());if(!data){if(strcmp("\"",EmptyString)!=0)return false;}else{if(!static_cast<StringDataMethods*>(data)->Equals("\""))return false;}bool read;{CCharString delimiter("\"",-1);read=parser.ReadAsStringUntilString(delimiter,item);}if(!read)return false;*PData=item.GetAsString();parser.ReadNextItem(item);return true;}