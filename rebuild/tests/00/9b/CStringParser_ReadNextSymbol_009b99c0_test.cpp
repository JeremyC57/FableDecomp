#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void check(bool p){if(!p)abort();}
class TestParser:public CStringParser {public:using CStringParser::ReadNextSymbol;};
// Contract mock records the string operation; allocation/storage are outside this row.
static unsigned calls;static CCharString*receiver;static const char*requested;static unsigned char captured;
void CCharString::SetAsSubString(const char*text,long length){check(length==1);receiver=this;requested=text;captured=(unsigned char)*text;++calls;}
static unsigned checked;
int main(){check(sizeof(TestParser)==40&&sizeof(CParsedItem)==12);for(unsigned code=0;code<256;++code)for(unsigned prefix=0;prefix<4;++prefix)for(unsigned quoted=0;quoted<2;++quoted){char text[8];for(unsigned i=0;i<8;++i)text[i]='z';text[prefix]=(char)code;text[prefix+1]=0;char original[8];memcpy(original,text,8);TestParser parser;memset(&parser,0,sizeof parser);parser.String=text;parser.BaseLineOffset=13;parser.CursorPos[0]=(unsigned char)quoted;*reinterpret_cast<unsigned long*>(parser.CursorPos+4)=prefix;*reinterpret_cast<unsigned long*>(parser.CursorPos+8)=37;unsigned char expected[40];memcpy(expected,&parser,40);CParsedItem item;memset(&item,0,sizeof item);unsigned char beforeItem[12];memcpy(beforeItem,&item,12);calls=0;receiver=0;requested=0;captured=0;bool result=parser.ReadNextSymbol(item);if(code){check(result&&calls==1&&receiver==&item.String&&requested==text+prefix&&captured==code);*reinterpret_cast<unsigned long*>(expected+0x20)=prefix+1;expected[0x1c]=(unsigned char)(quoted^((code=='"'||code=='\'')?1:0));unsigned int itemType=0;memcpy(&itemType,&item,4);check(itemType==1);check(memcmp((unsigned char*)&item+4,beforeItem+4,8)==0);}else{check(!result&&calls==0&&memcmp(&item,beforeItem,12)==0);}check(memcmp(&parser,expected,40)==0&&memcmp(text,original,8)==0);++checked;}check(checked==2048);puts("009b99c0_TEST PASS");return 0;}