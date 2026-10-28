#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// ScriptPredicates::IsTrue(const CCharString& text) -- Compare the token without regard to case.
// Leaf: ?IsTrue@@YI_NABVCCharString@@@Z
// Flags: /O1 /Oy-
// Identity: original global fastcall IsTrue in both PDB name sets; own literal true and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
// Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
extern "C" int __cdecl _stricmp(const char*,const char*);
class CCharStringData{public:char* PData;};
class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);~CCharString();const char* GetAsCharArray()const{return PStringData==0?"":PStringData->PData;}__forceinline bool operator==(const CCharString& other)const{return _stricmp(GetAsCharArray(),other.GetAsCharArray())==0;}};
bool __fastcall IsTrue(const CCharString& text){CCharString expected("true");return text==expected;}

#ifndef _MSC_VER
extern "C" int _stricmp(const char* a,const char* b){while(*a&&tolower((unsigned char)*a)==tolower((unsigned char)*b)){++a;++b;}return tolower((unsigned char)*a)-tolower((unsigned char)*b);}
#endif
static int live;CCharString::CCharString(const char* s,long n){if(n<0)n=(long)strlen(s);PStringData=new CCharStringData;PStringData->PData=(char*)malloc(n+1);memcpy(PStringData->PData,s,n);PStringData->PData[n]=0;++live;}CCharString::~CCharString(){if(PStringData){free(PStringData->PData);delete PStringData;--live;}}int main(){bool ok=true;{CCharString empty;ok=ok&&!IsTrue(empty)&&live==0;const char* values[]={"true","TRUE","truex","xtrue","","other"};for(int i=0;i<6;++i){CCharString text(values[i]);int prior=live;bool result=IsTrue(text);ok=ok&&(result==(i<2))&&live==prior;}}ok=ok&&!live;puts(ok?"00cbedba_TEST PASS":"00cbedba_TEST FAIL");return ok?0:1;}