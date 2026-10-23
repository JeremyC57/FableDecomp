// 006c4060 _STL::find<CBaseIntelligentPointer*,CBaseIntelligentPointer>
// Flags: /Gr
// Leaf: ??$find@UCBaseIntelligentPointer@FableFind_006c4060@@U12@@FableFind_006c4060@@YIPAUCBaseIntelligentPointer@0@PAU10@0ABU10@ABUrandom_access_iterator_tag@0@@Z
// Layout: CBaseIntelligentPointer stride8, GetPItem00a01b50 reads pointer+4; SGI random-access find unroll4.
namespace FableFind_006c4060 {
struct CBaseIntelligentPointer {void* __vftable;mutable void* PData;void* GetPItem()const;__forceinline bool operator==(const CBaseIntelligentPointer& p)const{return GetPItem()==p.GetPItem();}};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class T,class V> T* find(T* first,T* last,const V& value,const random_access_iterator_tag&){
 int count=(last-first)>>2;
 for(;count>0;--count){
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
 }
 switch(last-first){
 case 3:if(*first==value)return first;++first;
 case 2:if(*first==value)return first;++first;
 case 1:if(*first==value)return first;
 default:return last;
 }
}
template CBaseIntelligentPointer* find<CBaseIntelligentPointer,CBaseIntelligentPointer>(CBaseIntelligentPointer*,CBaseIntelligentPointer*,const CBaseIntelligentPointer&,const random_access_iterator_tag&);
}