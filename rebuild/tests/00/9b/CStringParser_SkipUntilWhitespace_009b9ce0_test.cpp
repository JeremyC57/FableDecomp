#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void check(bool p){if(!p)abort();}
static const unsigned char alphabet[]={' ',9,10,11,12,13,'a','"','\'',0x80,0xff};static char text[8],backup[8];static unsigned cases;
static bool whitespace(unsigned char c){return c==' '||(c>=9&&c<=13);}
static void run(unsigned n){text[n]=0;for(unsigned start=0;start<=n;++start)for(int quoted=0;quoted<2;++quoted){CStringParser parser;memset(&parser,0,sizeof parser);parser.String=text;parser.BaseLineOffset=312;parser.CursorPos[0]=(unsigned char)quoted;*reinterpret_cast<unsigned long*>(parser.CursorPos+4)=start;*reinterpret_cast<unsigned long*>(parser.CursorPos+8)=907;unsigned char expected[sizeof parser];memcpy(expected,&parser,sizeof parser);unsigned end=start;bool inQuotes=quoted!=0;while(text[end]&&!whitespace((unsigned char)text[end])){if(text[end]=='"'||text[end]=='\'')inQuotes=!inQuotes;++end;}expected[0x1c]=(unsigned char)inQuotes;*reinterpret_cast<unsigned long*>(expected+0x20)=end;memcpy(backup,text,sizeof text);check(parser.SkipUntilWhitespace()==long(end-start));check(memcmp(&parser,expected,sizeof parser)==0);check(memcmp(text,backup,sizeof text)==0);++cases;}}
static void enumerate(unsigned p,unsigned n){if(p==n){run(n);return;}for(unsigned i=0;i<sizeof alphabet;++i){text[p]=(char)alphabet[i];enumerate(p+1,n);}}
int main(){check(sizeof(CStringParser)==40);for(unsigned n=0;n<=4;++n)enumerate(0,n);check(cases==157830);puts("009b9ce0_TEST PASS");return 0;}