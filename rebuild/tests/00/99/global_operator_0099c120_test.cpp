#include <stdio.h>
// global::operator+(const CWideString& left,wchar_t right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCWideString@@ABV0@G@Z
// Flags: /Gr
// Identity: original PDB free fastcall overload; own append0099b9a0 and UnassignString0099b4d0. Counter binding 013bca20 is descriptive, not a recovered original variable name.
// Layout: CWideStringData has original STLport basic_string12 and NoRefs+12. basic_string inherits protected _String_base with allocator proxy; its original pointer/count/default-allocator constructor is evidenced by retail0099ca40. Shared fable_string.h flattens these original storage types and omits the character append API.
#include <stddef.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace std {
template<class T>class allocator{public:allocator(){}allocator(const allocator&){} };template<class T>class char_traits{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){} };
template<class T,class A>class _String_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_String_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0){}~_String_base(){if(_M_start)free(_M_start);}};
template<class T,class Traits=char_traits<T>,class A=allocator<T> >class basic_string:protected _String_base<T,A>{public:typedef unsigned int size_type;basic_string(const T*,size_type,const A& = A());basic_string& append(size_type,T);const T* c_str()const{return this->_M_start;}};
}
class CWideStringData{public:std::basic_string<wchar_t> Data;long NoRefs;CWideStringData(const wchar_t* text,unsigned long n):Data(text,n),NoRefs(1){}};
extern long g_CWideStringInstanceCount;
class CWideString{public:CWideStringData* PStringData;
CWideString():PStringData(0){++g_CWideStringInstanceCount;}
CWideString(const CWideString& other):PStringData(0){++g_CWideStringInstanceCount;if(&other!=this && other.PStringData!=PStringData){if(PStringData)UnassignString();CWideStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}}
~CWideString(){if(PStringData){if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0;}--g_CWideStringInstanceCount;}
protected:void UnassignString();void MakeOwnCopyOfData();CWideStringData* AllocStringData(const wchar_t*,long);
public:const CWideString& operator+=(wchar_t);
};

#pragma auto_inline(off)
__declspec(noinline) void CWideString::UnassignString(){if(PStringData){if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0;}}
__declspec(noinline) CWideStringData* CWideString::AllocStringData(const wchar_t* text,long length){if(length<0){length=0;while(text[length])++length;}return new CWideStringData(text,length);}
__declspec(noinline) void CWideString::MakeOwnCopyOfData(){CWideStringData* data=PStringData;if(data && data->NoRefs>1){CWideStringData* copy=AllocStringData(data->Data.c_str(),-1);UnassignString();PStringData=copy;}}
__declspec(noinline) const CWideString& CWideString::operator+=(wchar_t c){if(PStringData){MakeOwnCopyOfData();PStringData->Data.append(1,c);}else{wchar_t text[2];text[0]=c;text[1]=0;if(c)PStringData=AllocStringData(text,-1);}return *this;}
#pragma auto_inline(on)
CWideString __fastcall operator+(const CWideString& left,wchar_t right){CWideString result(left);result+=right;return result;}
long g_CWideStringInstanceCount=0;

static int appendCalls,bad;static const CWideString* expectedLeft;static wchar_t emptyText[1]={0};
static unsigned int length(const wchar_t* p){unsigned int n=0;if(p)while(p[n])++n;return n;}
static CWideStringData* makeData(const wchar_t* input){return new CWideStringData(input,length(input));}
template<class T,class Traits,class A>std::basic_string<T,Traits,A>::basic_string(const T* p,typename std::basic_string<T,Traits,A>::size_type n,const A& a):std::_String_base<T,A>(a){this->_M_start=(T*)malloc((n+1)*sizeof(T));for(unsigned long i=0;i<n;++i)this->_M_start[i]=p[i];this->_M_start[n]=0;this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_finish+1;}
template<class T,class Traits,class A>std::basic_string<T,Traits,A>& std::basic_string<T,Traits,A>::append(typename std::basic_string<T,Traits,A>::size_type n,T c){++appendCalls;unsigned long count=(unsigned long)(this->_M_finish-this->_M_start);T* p=(T*)malloc((count+n+1)*sizeof(T));for(unsigned long i=0;i<count;++i)p[i]=this->_M_start[i];for(unsigned long j=0;j<n;++j)p[count+j]=c;p[count+n]=0;free(this->_M_start);this->_M_start=p;this->_M_finish=p+count+n;this->_M_end_of_storage._M_data=this->_M_finish+1;return *this;}
static bool equal(const wchar_t* a,const wchar_t* b){while(*a&&*a==*b){++a;++b;}return *a==*b;}
static bool run(bool populated){wchar_t leftText[3]={'a','b',0},rightText[2]={'x',0},joined[4]={'a','b','x',0};CWideString left;expectedLeft=&left;if(populated)left.PStringData=makeData(leftText); bool ok;
{const CWideString& result=left+'x';ok=result.PStringData&&equal(result.PStringData->Data.c_str(),populated?joined:rightText)&&result.PStringData->NoRefs==1;if(populated)ok=ok&&left.PStringData->NoRefs==1&&equal(left.PStringData->Data.c_str(),leftText)&&result.PStringData!=left.PStringData;else ok=ok&&!left.PStringData;}
return ok;}
int main(){bool ok=run(true)&&run(false)&&appendCalls==1&&!bad&&g_CWideStringInstanceCount==0;puts(ok?"0099c120_TEST PASS":"0099c120_TEST FAIL");return ok?0:1;}