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