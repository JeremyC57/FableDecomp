// std::copy_backward(iterator first, iterator last, iterator result) -- copy the range backward and return its new beginning.
// Leaf: ??$copy_backward@U?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@@std@@YIPAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@0@PAU10@00@Z
// Identity: probable helper spelling; proven mutable element: particle morph pair40 __linear_insert0081BE30 and map pair chain/GetAllMorphsOfType0081B160; PDB CParticleMorphs::CEntry36. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
// Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.
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