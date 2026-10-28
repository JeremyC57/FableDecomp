#include <stdio.h>
// global::operator+(const CWideString& left,const wchar_t* right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCWideString@@ABV0@PBG@Z
// Flags: /Gr
// Identity: original PDB free fastcall overload; own append0099b940 and UnassignString0099b4d0. Counter binding 013bca20 is descriptive, not a recovered original variable name.
// Layout: CWideStringData real STLport basic_string12 plus NoRefs+12; donor string28 differs.
#include <stddef.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace std {
template<class T>class allocator{};template<class T>class char_traits{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _String_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;~_String_base(){if(_M_start)free(_M_start);}};
template<class T,class Traits=char_traits<T>,class A=allocator<T> >class basic_string:public _String_base<T,A>{};
}
class CWideStringData{public:std::basic_string<wchar_t> Data;long NoRefs;};
extern long g_CWideStringInstanceCount;
class CWideString{public:CWideStringData* PStringData;
CWideString():PStringData(0){++g_CWideStringInstanceCount;}
CWideString(const CWideString& other):PStringData(0){++g_CWideStringInstanceCount;if(&other!=this && other.PStringData!=PStringData){if(PStringData)UnassignString();CWideStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}}
~CWideString(){if(PStringData){if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0;}--g_CWideStringInstanceCount;}
protected:void UnassignString();
public:const CWideString& operator+=(const wchar_t*);
};
CWideString __fastcall operator+(const CWideString& left,const wchar_t* right){CWideString result(left);result+=right;return result;}

long g_CWideStringInstanceCount=0;

static int appendCalls,bad;static const CWideString* expectedLeft;static wchar_t emptyText[1]={0};
static unsigned int length(const wchar_t* p){unsigned int n=0;if(p)while(p[n])++n;return n;}
static CWideStringData* makeData(const wchar_t* input){unsigned int n=length(input);CWideStringData* p=new CWideStringData;wchar_t* text=(wchar_t*)malloc((n+1)*sizeof(wchar_t));for(unsigned int i=0;i<n;++i)text[i]=input[i];text[n]=0;p->Data._M_start=text;p->Data._M_finish=text+n;p->Data._M_end_of_storage._M_data=text+n+1; p->NoRefs=1;return p;}
void CWideString::UnassignString(){CWideStringData* p=PStringData;if(p&&--p->NoRefs<=0){delete p;}PStringData=0;}
const CWideString& CWideString::operator+=(const wchar_t* right){++appendCalls;if(this==expectedLeft)bad=1; const wchar_t* r=right;const wchar_t* l=PStringData?PStringData->Data._M_start:emptyText;unsigned int n=length(l),m=length(r);wchar_t combined[40];for(unsigned int i=0;i<n;++i)combined[i]=l[i];for(unsigned int j=0;j<m;++j)combined[n+j]=r[j];combined[n+m]=0;CWideStringData* fresh=makeData(combined);UnassignString();PStringData=fresh;return *this;}
static bool equal(const wchar_t* a,const wchar_t* b){while(*a&&*a==*b){++a;++b;}return *a==*b;}
static bool run(bool populated){wchar_t leftText[3]={'a','b',0},rightText[2]={'x',0},joined[4]={'a','b','x',0};CWideString left;expectedLeft=&left;if(populated)left.PStringData=makeData(leftText); bool ok;
{const CWideString& result=left+rightText;ok=result.PStringData&&equal(result.PStringData->Data._M_start,populated?joined:rightText)&&result.PStringData->NoRefs==1;if(populated)ok=ok&&left.PStringData->NoRefs==1&&equal(left.PStringData->Data._M_start,leftText)&&result.PStringData!=left.PStringData;else ok=ok&&!left.PStringData;}
return ok;}
int main(){bool ok=run(true)&&run(false)&&appendCalls==2&&!bad&&g_CWideStringInstanceCount==0;puts(ok?"0099bf30_TEST PASS":"0099bf30_TEST FAIL");return ok?0:1;}
