// 00737880 BinarySearch_IntelligentPointer
// Flags: none
// Leaf: BinarySearch_IntelligentPointer
// Layout: retail advances 8-byte FixtureCBaseIntelligentPointer values; GetPItem binds 00a01b50.
#include "fable_compiler.h"
class FixtureCBaseObject;
struct FixtureCBaseIntelligentPointer {
 void* Data; void* Info;
 FixtureCBaseObject* GetPItem() const;
 FABLE_FORCEINLINE bool operator==(const FixtureCBaseIntelligentPointer& b) const {return GetPItem()==b.GetPItem();}
};
struct Fixturerandom_access_iterator_tag {};
FixtureCBaseIntelligentPointer* __fastcall BinarySearch_IntelligentPointer(FixtureCBaseIntelligentPointer* first,FixtureCBaseIntelligentPointer* last,const FixtureCBaseIntelligentPointer& value,Fixturerandom_access_iterator_tag = Fixturerandom_access_iterator_tag()) {
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
class FixtureCBaseObject { public:int Value;};
FixtureCBaseObject* FixtureCBaseIntelligentPointer::GetPItem()const {return (FixtureCBaseObject*)Data;}
int main(){FixtureCBaseObject objects[9]; FixtureCBaseIntelligentPointer a[9];for(int i=0;i<9;++i){a[i].Data=&objects[i];a[i].Info=0;}for(int n=0;n<=9;++n){for(int k=0;k<9;++k){FixtureCBaseIntelligentPointer* p=BinarySearch_IntelligentPointer(a,a+n,a[k]);assert(p==(k<n?a+k:a+n));}}puts("00737880_TEST PASS");return 0;}