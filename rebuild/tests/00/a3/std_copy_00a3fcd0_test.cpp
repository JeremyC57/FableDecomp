#include <stdio.h>
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();CCharString& operator=(const CCharString&);};namespace std{template<class A,class B>struct pair{A first;B second;};}
class CUnprovenSoundNameEntry_00A3FCD0{public:CCharString Name;unsigned long UnprovenValue;};
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}}
typedef CUnprovenSoundNameEntry_00A3FCD0 Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);
class CCharStringData{public:int ID;};static CCharStringData data[3];static int calls;CCharString::~CCharString(){}CCharString& CCharString::operator=(const CCharString& s){++calls;PStringData=s.PStringData;return *this;}
int main(){Element src[3],dst[3];for(int i=0;i<3;++i){data[i].ID=i+3;src[i].Name.PStringData=data+i;src[i].UnprovenValue=i+9;}if(std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0)!=dst+3)goto fail;for(int i=0;i<3;++i)if(dst[i].Name.PStringData!=src[i].Name.PStringData || dst[i].UnprovenValue!=src[i].UnprovenValue)goto fail;if(calls!=3)goto fail;if(std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0)!=dst || calls!=3)goto fail;puts("00a3fcd0_TEST PASS");return 0;fail:puts("00a3fcd0_TEST FAIL");return 1;}