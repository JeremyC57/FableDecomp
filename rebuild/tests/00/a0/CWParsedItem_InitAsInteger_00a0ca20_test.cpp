#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
// 00a0ca20 CWParsedItem::InitAsInteger
// Leaf: ?InitAsInteger@CWParsedItem@@QAEXPB_WK@Z
// Flags: /Zc:wchar_t
#include <stddef.h>
#include "fable_string.h"
enum EParsedItemType{};
// Layout: the PDB leaves the numeric union at+4 unnamed. Use an unnamed storage word and a typed local projection of that original union.
class CWParsedItem{public:EParsedItemType Type;unsigned int:32;CWideString String;
 void InitAsInteger(const wchar_t*,unsigned long);void InitAsFloat(const wchar_t*,unsigned long);
};
void CWParsedItem::InitAsInteger(const wchar_t*text,unsigned long length){
 unsigned long i=0;long&value=*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(this)+4);value=0;bool negative=false;wchar_t c=text[0];
 if(i<length){do{if(c==L'-')negative=true;else value=value*10+c-L'0';c=text[++i];}while(i<length);
  if(negative==true){Type=(EParsedItemType)3;value=-value;}else Type=(EParsedItemType)3;
 }else Type=(EParsedItemType)3;
}

CWideString::CWideString(){}CWideString::~CWideString(){}

int main(){CWParsedItem item;unsigned long stringGuard=0x51a7c3d9UL;memcpy((unsigned char*)&item+8,&stringGuard,4);
 for(long n=0;n<=9999;n+=7)for(int sign=0;sign<3;++sign){char narrow[32];sprintf(narrow,"%ld",n);wchar_t text[32];unsigned long length=0;if(sign==1)text[length++]=L'-';for(unsigned long i=0;narrow[i];++i)text[length++]=(wchar_t)narrow[i];if(sign==2)text[length++]=L'-';text[length]=0;
  item.Type=(EParsedItemType)7;item.InitAsInteger(text,length);long actual;unsigned long guard;memcpy(&actual,(unsigned char*)&item+4,4);memcpy(&guard,(unsigned char*)&item+8,4);
  if((int)item.Type!=3||actual!=(sign?-n:n)||guard!=stringGuard)return 1;
 }
 wchar_t empty[1]={0};item.InitAsInteger(empty,0);long actual;memcpy(&actual,(unsigned char*)&item+4,4);if((int)item.Type!=3||actual!=0)return 2;
 puts("00a0ca20_TEST PASS");return 0;}