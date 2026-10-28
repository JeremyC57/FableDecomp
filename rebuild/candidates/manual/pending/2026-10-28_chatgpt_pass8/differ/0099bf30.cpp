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