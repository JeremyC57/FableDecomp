// 009b99c0 CStringParser::ReadNextSymbol
// Leaf: ?ReadNextSymbol@CStringParser@@IAE_NAAVCParsedItem@@@Z
// Flags:
#define CStringParser CStringParserFields
#include "engine/CStringParser.h"
#undef CStringParser
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CCharStringData;
class CCharString {public:CCharStringData*PStringData;void SetAsSubString(const char*,long);};
enum EParsedItemType {};
// Layout: Type+0, unaccessed anonymous numeric storage+4, String+8; full size12 from PDB.
class CParsedItem {public:EParsedItemType Type;unsigned int:32;CCharString String;__forceinline void InitAsSymbol(const char*text,unsigned long length){String.SetAsSubString(text,length);Type=(EParsedItemType)1;}};
// Layout: header CursorPos storage contains bool InQuotes+0, ulong StringPos+4, ulong Line+8.
class CStringParser:public CStringParserFields {
protected:
 __declspec(noinline) bool CalcInQuotes(bool state,const char*first,const char*last)const{for(;first!=last;++first)if(*first=='"'||*first=='\'')state=!state;return state;}
 __forceinline void MoveStringPos(unsigned long count){if(count){unsigned long pos=*reinterpret_cast<unsigned long*>(CursorPos+4);bool state=*reinterpret_cast<bool*>(CursorPos);*reinterpret_cast<bool*>(CursorPos)=CalcInQuotes(state,(const char*)String+pos,(const char*)String+pos+count);*reinterpret_cast<unsigned long*>(CursorPos+4)=pos+count;}}
 bool ReadNextSymbol(CParsedItem&);
};
bool CStringParser::ReadNextSymbol(CParsedItem&item){if(((const char*)String)[*reinterpret_cast<unsigned long*>(CursorPos+4)]){const char*text=(const char*)String+*reinterpret_cast<unsigned long*>(CursorPos+4);item.InitAsSymbol(text,1);MoveStringPos(1);return true;}return false;}