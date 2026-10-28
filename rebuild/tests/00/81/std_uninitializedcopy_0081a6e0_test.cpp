#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
enum EHeroMorphType {}; enum EMeshLocationPoint {}; class CDefString { public: long TablePos; }; class CParticleMorphs { public: class CEntry { public: EHeroMorphType MorphType; bool Blending; EMeshLocationPoint Location; CDefString LocationName; long LocationIndex,ParticleIndex,SecondParticleIndex; float StartAtMorphStrength,EndAtMorphStrength; }; };

namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Input,class Output> Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; } }

typedef CParticleMorphs::CEntry Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);
int main() { Element src[3]; memset(src,0,sizeof(src)); for(int i=0;i<3;++i) { src[i].MorphType=(EHeroMorphType)0; src[i].Blending=i%2!=0; src[i].Location=(EMeshLocationPoint)0; src[i].LocationName.TablePos=i+4; src[i].LocationIndex=i+8; src[i].ParticleIndex=i+12; src[i].SecondParticleIndex=i+16; src[i].StartAtMorphStrength=float(i)+0.25f; src[i].EndAtMorphStrength=float(i)+0.75f; } Element* dst=(Element*)malloc(3*sizeof(Element)); memset(dst,0,3*sizeof(Element)); Element* end;
end=std::__uninitialized_copy(src,src+3,dst,std::__false_type()); if(end!=dst+3 || memcmp(src,dst,sizeof(src))) goto fail; end=std::__uninitialized_copy(src,src,dst,std::__false_type()); if(end!=dst) goto fail;
free(dst); puts("0081a6e0_TEST PASS"); return 0; fail: puts("0081a6e0_TEST FAIL"); return 1; }