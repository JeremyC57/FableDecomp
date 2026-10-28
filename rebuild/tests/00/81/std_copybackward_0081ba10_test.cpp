#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
enum EHeroMorphType {}; enum EMeshLocationPoint {}; class CDefString { public: long TablePos; }; class CParticleMorphs { public: class CEntry { public: EHeroMorphType MorphType; bool Blending; EMeshLocationPoint Location; CDefString LocationName; long LocationIndex,ParticleIndex,SecondParticleIndex; float StartAtMorphStrength,EndAtMorphStrength; }; };
namespace std{template<class A,class B>struct pair{A first;B second;};}
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>__forceinline Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n)*--result=*--last;return result;}template<class T>T* __fastcall copy_backward(T* first,T* last,T* result){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
typedef std::pair<EHeroMorphType,CParticleMorphs::CEntry> Element;
template Element* __fastcall std::copy_backward<Element>(Element*,Element*,Element*);
int main(){Element data[4],saved[3];memset(data,0,sizeof(data));for(int i=0;i<4;++i){Element& x=data[i];x.first=(EHeroMorphType)0;x.second.Blending=i%2!=0;x.second.Location=(EMeshLocationPoint)0;x.second.LocationName.TablePos=i+2;x.second.LocationIndex=i+3;x.second.ParticleIndex=i+4;x.second.SecondParticleIndex=i+5;x.second.StartAtMorphStrength=float(i)+0.5f;x.second.EndAtMorphStrength=float(i)+0.75f;}memcpy(saved,data,sizeof(saved));if(std::copy_backward(data,data+3,data+4)!=data+1 || memcmp(data+1,saved,sizeof(saved)))goto fail;if(std::copy_backward(data,data,data+4)!=data+4)goto fail;puts("0081ba10_TEST PASS");return 0;fail:puts("0081ba10_TEST FAIL");return 1;}