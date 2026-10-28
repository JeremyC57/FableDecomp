#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// ScriptPredicates::IsNull(const CCharString& text) -- Compare the token without regard to case.
// Leaf: ?IsNull@@YI_NABVCCharString@@@Z
// Flags: /O1 /Oy-
// Identity: original global fastcall IsNull in both PDB name sets; own literal null and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
// Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
extern "C" int __cdecl _stricmp(const char*,const char*);
class CCharStringData{public:char* PData;};
class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);~CCharString();const char* GetAsCharArray()const{return PStringData==0?"":PStringData->PData;}__forceinline bool operator==(const CCharString& other)const{return _stricmp(GetAsCharArray(),other.GetAsCharArray())==0;}};
bool __fastcall IsNull(const CCharString& text){CCharString expected("null");return text==expected;}

#ifndef _MSC_VER
extern "C" int _stricmp(const char* a,const char* b){while(*a&&tolower((unsigned char)*a)==tolower((unsigned char)*b)){++a;++b;}return tolower((unsigned char)*a)-tolower((unsigned char)*b);}
#endif
static int live;CCharString::CCharString(const char* s,long n){if(n<0)n=(long)strlen(s);PStringData=new CCharStringData;PStringData->PData=(char*)malloc(n+1);memcpy(PStringData->PData,s,n);PStringData->PData[n]=0;++live;}CCharString::~CCharString(){if(PStringData){free(PStringData->PData);delete PStringData;--live;}}int main(){bool ok=true;{CCharString empty;ok=ok&&!IsNull(empty)&&live==0;const char* values[]={"null","NULL","nullx","xnull","","other"};for(int i=0;i<6;++i){CCharString text(values[i]);int prior=live;bool result=IsNull(text);ok=ok&&(result==(i<2))&&live==prior;}}ok=ok&&!live;puts(ok?"00cbee5e_TEST PASS":"00cbee5e_TEST FAIL");return ok?0:1;}