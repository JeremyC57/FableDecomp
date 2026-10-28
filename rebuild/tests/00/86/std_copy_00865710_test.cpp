#include <stdio.h>
class CEnginePrimitiveWeaponTrail{public:class CWeaponTrailEntry;};namespace std{template<class T>class allocator{};template<class T,class Al=allocator<T> >class list{public:void* _M_node;list& operator=(const list&);};}namespace NCreatureHitNotification{class CFlash{public:std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;long CreationFrame;};}
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}}
typedef NCreatureHitNotification::CFlash Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);
static int calls;namespace std{template<class T,class Al>list<T,Al>& list<T,Al>::operator=(const list& v){++calls;_M_node=v._M_node;return *this;}}
int main(){Element src[3],dst[3];for(int i=0;i<3;++i){src[i].FlashGeometry._M_node=(void*)(i+1);src[i].CreationFrame=i+9;}if(std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0)!=dst+3)goto fail;for(int i=0;i<3;++i)if(dst[i].FlashGeometry._M_node!=src[i].FlashGeometry._M_node || dst[i].CreationFrame!=src[i].CreationFrame)goto fail;if(calls!=3)goto fail;if(std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0)!=dst || calls!=3)goto fail;puts("00865710_TEST PASS");return 0;fail:puts("00865710_TEST FAIL");return 1;}