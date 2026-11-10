// 00a0cdd0 CWStringParser::ReadNextNumber
// Leaf: ?ReadNextNumber@CWStringParser@@AAE_NPAVCWParsedItem@@@Z
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CWStringParser.h"
#include "engine/CWParsedItem.h"
#include "rebuild_abi.h"
extern "C" int FABLE_CDECL iswdigit(unsigned short);
bool CWStringParser::ReadNextNumber(CWParsedItem* item)
{
 bool is_float=false;
 unsigned long c1;
 wchar_t ch;
 for(c1=StringPos,ch=String[c1]; iswdigit(ch)||(ch==L'.'&&!is_float)||(ch==L'-'&&c1==StringPos);ch=String[++c1]) {
  if(ch==L'\n')++Line;
  if(ch==L'.')is_float=true;
 }
 if(!is_float)item->InitAsInteger(&String[StringPos],c1-StringPos);
 else item->InitAsFloat(&String[StringPos],c1-StringPos);
 StringPos=c1;
 return true;
}