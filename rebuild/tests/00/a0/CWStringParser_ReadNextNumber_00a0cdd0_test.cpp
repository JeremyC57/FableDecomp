// Selected scanner body copied verbatim; conversion boundary records exact span and chosen kind.
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <string>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
class CWParsedItem {public:int Kind,Calls;const wchar_t* Begin;unsigned long Length;void InitAsInteger(const wchar_t* p,unsigned long n){Kind=3;Begin=p;Length=n;++Calls;}void InitAsFloat(const wchar_t* p,unsigned long n){Kind=4;Begin=p;Length=n;++Calls;}};
class CWStringParser {public:const wchar_t* String;unsigned long StringPos,Line;bool ReadNextNumber(CWParsedItem*);};
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

int main(){
 const wchar_t alphabet[]={L'0',L'5',L'.',L'-',L'x',L'\n'};unsigned cases=0;
 for(unsigned length=0;length<=4;++length){unsigned variants=1;for(unsigned z=0;z<length;++z)variants*=6;
  for(unsigned pattern=0;pattern<variants;++pattern)for(unsigned offset=0;offset<3;++offset){
   std::wstring text(offset,L'X');unsigned digits=pattern;for(unsigned k=0;k<length;++k){text+=alphabet[digits%6];digits/=6;}
   unsigned expectedEnd=offset;bool expectedFloat=false;
   if(expectedEnd<text.size()&&text[expectedEnd]==L'-')++expectedEnd;
   while(expectedEnd<text.size()&&(text[expectedEnd]==L'0'||text[expectedEnd]==L'5'))++expectedEnd;
   if(expectedEnd<text.size()&&text[expectedEnd]==L'.'){expectedFloat=true;++expectedEnd;while(expectedEnd<text.size()&&(text[expectedEnd]==L'0'||text[expectedEnd]==L'5'))++expectedEnd;}
   CWStringParser parser;parser.String=text.c_str();parser.StringPos=offset;parser.Line=77;CWParsedItem item;item.Kind=0;item.Calls=0;item.Begin=0;item.Length=999;
   CHECK(parser.ReadNextNumber(&item));++cases;
   CHECK(parser.StringPos==expectedEnd&&parser.Line==77&&parser.String==text.c_str());
   CHECK(item.Calls==1&&item.Begin==text.c_str()+offset&&item.Length==expectedEnd-offset&&item.Kind==(expectedFloat?4:3));
  }
 }
 CHECK(cases==4665);puts("00a0cdd0_TEST PASS");return 0;
}