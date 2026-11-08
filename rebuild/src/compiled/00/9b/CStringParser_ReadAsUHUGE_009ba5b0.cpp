// 009ba5b0 CStringParser::ReadAsUHUGE
// Leaf: ?ReadAsUHUGE@CStringParser@@QAE_KXZ
// Flags:
#include "fable_compiler.h"
#define CStringParser CStringParserFields
#include "engine/CStringParser.h"
#undef CStringParser
extern "C" int __cdecl isdigit(int);
// Layout: supplied native parser fields; original nested cursor12 bytes at1c.
class CStringParser:public CStringParserFields{public:
 class CCursorPos{public:bool InQuotes;unsigned long StringPos,Line;};
 long SkipWhitespace();unsigned __int64 ReadAsUHUGE();
 unsigned long GetStringPos()const{return reinterpret_cast<const CCursorPos&>(CursorPos).StringPos;}
protected:bool CalcInQuotes(bool,const char*,const char*)const;
 FABLE_FORCEINLINE void MoveStringPos(unsigned long n){if(n){
 reinterpret_cast<CCursorPos*>(CursorPos)->InQuotes=CalcInQuotes(reinterpret_cast<CCursorPos*>(CursorPos)->InQuotes,static_cast<const char*>(String)+reinterpret_cast<CCursorPos*>(CursorPos)->StringPos,static_cast<const char*>(String)+reinterpret_cast<CCursorPos*>(CursorPos)->StringPos+n);
 reinterpret_cast<CCursorPos*>(CursorPos)->StringPos+=n;
 }}
};
#pragma auto_inline(off)
bool CStringParser::CalcInQuotes(bool quotes,const char*first,const char*last)const{for(;first!=last;++first)if(*first=='"'||*first=='\'')quotes=!quotes;return quotes;}
#pragma auto_inline(on)
unsigned __int64 CStringParser::ReadAsUHUGE(){
 SkipWhitespace();const char*p=static_cast<const char*>(String)+GetStringPos();unsigned __int64 value=0;
 while(*p&&isdigit((unsigned char)*p)){value=value*10+(*p-'0');++p;}
 MoveStringPos((p-static_cast<const char*>(String))-GetStringPos());return value;
}