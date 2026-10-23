// 007379d0 _STL::find<CBaseIntelligentPointer*>
// Flags: /O2 /Oy /W3 
// Leaf: find
// Layout: CBaseIntelligentPointer is two dwords; GetPItem at00a01b50 is a landed symbol.
struct CBaseObjectPointer{void*object;unsigned refcount;};
struct CBaseIntelligentPointer{virtual ~CBaseIntelligentPointer();CBaseObjectPointer*PData;CBaseIntelligentPointer();void*GetPItem();void SetPItem(void*);};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
namespace _STL {
CBaseIntelligentPointer* __fastcall find(CBaseIntelligentPointer*first,CBaseIntelligentPointer*last,CBaseIntelligentPointer&value,const random_access_iterator_tag&){
 int count=(last-first)>>2;
 for(;count>0;--count){
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
 }
 switch(last-first){
  case 3:if(first->GetPItem()==value.GetPItem())return first;++first;
  case 2:if(first->GetPItem()==value.GetPItem())return first;++first;
  case 1:if(first->GetPItem()==value.GetPItem())return first;
  default:return last;
 }
}
}