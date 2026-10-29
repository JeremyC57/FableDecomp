// 009b9ce0 CStringParser::SkipUntilWhitespace
// Leaf: ?SkipUntilWhitespace@CStringParser@@QAEJXZ
// Flags:
// Implementation-only layout alias: the repository header owns the physical fields.
#define CStringParser CStringParserFields
#include "engine/CStringParser.h"
#undef CStringParser
#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#endif
extern "C" int __cdecl isspace(int);
// Layout: CursorPos+0 is InQuotes, +4 StringPos, +8 Line (PDB CCursorPos).
// Its class key is not needed: use the header's documented byte storage directly.
class CStringParser:public CStringParserFields {
protected:
 __forceinline bool CalcInQuotes(bool state,const char*first,const char*last)const {for(;first!=last;++first)if(*first=='"'||*first=='\'')state=!state;return state;}
 __forceinline void MoveStringPos(unsigned long count){if(count){*reinterpret_cast<bool*>(CursorPos)=CalcInQuotes(*reinterpret_cast<bool*>(CursorPos),(const char*)String+*reinterpret_cast<unsigned long*>(CursorPos+4),(const char*)String+*reinterpret_cast<unsigned long*>(CursorPos+4)+count);*reinterpret_cast<unsigned long*>(CursorPos+4)+=count;}}
public:long SkipUntilWhitespace();
};
long CStringParser::SkipUntilWhitespace(){char*p=(char*)String+*reinterpret_cast<unsigned long*>(CursorPos+4);while(!isspace((unsigned char)*p)&&*p)++p;unsigned long oldPos=*reinterpret_cast<unsigned long*>(CursorPos+4);unsigned long endPos=p-(char*)String;MoveStringPos(endPos-oldPos);return endPos-oldPos;}