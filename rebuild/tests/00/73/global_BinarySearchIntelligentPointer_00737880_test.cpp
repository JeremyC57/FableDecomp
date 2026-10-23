// 00737880 BinarySearch_IntelligentPointer
// Flags: none
// Leaf: BinarySearch_IntelligentPointer
// Layout: retail advances 8-byte CBaseIntelligentPointer values; GetPItem binds 00a01b50.
struct CBaseObject;
struct CBaseIntelligentPointer {
 void* Data; void* Info;
 CBaseObject* GetPItem() const;
 __forceinline bool operator==(const CBaseIntelligentPointer& b) const {return GetPItem()==b.GetPItem();}
};
struct random_access_iterator_tag {};
CBaseIntelligentPointer* __fastcall BinarySearch_IntelligentPointer(CBaseIntelligentPointer* first,CBaseIntelligentPointer* last,const CBaseIntelligentPointer& value,random_access_iterator_tag = random_access_iterator_tag()) {
 int trip=(last-first)>>2;
 for(;trip>0;--trip) {
  if(*first==value)return first;
  ++first;
  if(*first==value)return first;
  ++first;
  if(*first==value)return first;
  ++first;
  if(*first==value)return first;
  ++first;
 }
 switch(last-first) {
 case 3: if(*first==value)return first; ++first;
 case 2: if(*first==value)return first; ++first;
 case 1: if(*first==value)return first; ++first;
 case 0: default: return last;
 }
}

#include <stdio.h>
#include <assert.h>
struct CBaseObject {int Value;};
CBaseObject* CBaseIntelligentPointer::GetPItem()const {return (CBaseObject*)Data;}
int main(){CBaseObject objects[9]; CBaseIntelligentPointer a[9];for(int i=0;i<9;++i){a[i].Data=&objects[i];a[i].Info=0;}for(int n=0;n<=9;++n){for(int k=0;k<9;++k){CBaseIntelligentPointer* p=BinarySearch_IntelligentPointer(a,a+n,a[k]);assert(p==(k<n?a+k:a+n));}}puts("00737880_TEST PASS");return 0;}