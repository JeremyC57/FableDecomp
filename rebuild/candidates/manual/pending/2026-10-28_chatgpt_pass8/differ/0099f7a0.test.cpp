#include <stdio.h>
// global::operator+(const CCharString& left,char right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCCharString@@ABV0@D@Z
// Flags: /Gr
// Identity: original PDB free fastcall overload; own append0099f180 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
// Layout: Original packed CBasicString<char>13 plus NoRefs+13, PStringData sole pointer.
#include <stddef.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#pragma pack(push,1)
template<class T>class CBasicString{public:T* PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;};
class CCharStringData{public:CBasicString<char> Data;long NoRefs;};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;
class CCharString{public:CCharStringData* PStringData;
CCharString():PStringData(0){++g_CCharStringInstanceCount;}
CCharString(const CCharString& other):PStringData(0){++g_CCharStringInstanceCount;if(&other!=this){if(PStringData)UnassignString();CCharStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}}
__forceinline CCharStringData* GetStringData() const{return PStringData;}
CCharString& operator=(const CCharString& other){if(&other!=this){if(PStringData)UnassignString();if(other.PStringData){long refs=other.PStringData->NoRefs+1;PStringData=other.PStringData;PStringData->NoRefs=refs;}}return *this;}
~CCharString(){UnassignString();--g_CCharStringInstanceCount;}
protected:void UnassignString();
public:const CCharString& operator+=(char);
};
CCharString __fastcall operator+(const CCharString& left,char right){CCharString result;result=left;result+=right;return result;}
long g_CCharStringInstanceCount=0;

static int appendCalls,bad;static const CCharString* expectedLeft;static char emptyText[1]={0};
static unsigned int length(const char* p){unsigned int n=0;if(p)while(p[n])++n;return n;}
static CCharStringData* makeData(const char* input){unsigned int n=length(input);CCharStringData* p=new CCharStringData;char* text=(char*)malloc((n+1)*sizeof(char));for(unsigned int i=0;i<n;++i)text[i]=input[i];text[n]=0;p->Data.PData=text;p->Data.StringLength=n;p->Data.DataLength=n+1;p->Data.UseFastExtend=false; p->NoRefs=1;return p;}
void CCharString::UnassignString(){CCharStringData* p=PStringData;if(p&&--p->NoRefs<=0){free(p->Data.PData);delete p;}PStringData=0;}
const CCharString& CCharString::operator+=(char right){++appendCalls;if(this==expectedLeft)bad=1;char single[2]={right,0}; const char* r=single;const char* l=PStringData?PStringData->Data.PData:emptyText;unsigned int n=length(l),m=length(r);char combined[40];for(unsigned int i=0;i<n;++i)combined[i]=l[i];for(unsigned int j=0;j<m;++j)combined[n+j]=r[j];combined[n+m]=0;CCharStringData* fresh=makeData(combined);UnassignString();PStringData=fresh;return *this;}
static bool equal(const char* a,const char* b){while(*a&&*a==*b){++a;++b;}return *a==*b;}
static bool run(bool populated){char leftText[3]={'a','b',0},rightText[2]={'x',0},joined[4]={'a','b','x',0};CCharString left;expectedLeft=&left;if(populated)left.PStringData=makeData(leftText); bool ok;
{const CCharString& result=left+'x';ok=result.PStringData&&equal(result.PStringData->Data.PData,populated?joined:rightText)&&result.PStringData->NoRefs==1;if(populated)ok=ok&&left.PStringData->NoRefs==1&&equal(left.PStringData->Data.PData,leftText)&&result.PStringData!=left.PStringData;else ok=ok&&!left.PStringData;}
return ok;}
int main(){bool ok=run(true)&&run(false)&&appendCalls==2&&!bad&&g_CCharStringInstanceCount==0;puts(ok?"0099f7a0_TEST PASS":"0099f7a0_TEST FAIL");return ok?0:1;}
