#include <stdio.h>
class CUnprovenTwoLevelCollectionBase_012B3444{public:virtual unsigned long Kind()=0;virtual int UnknownSlot1(){return -1;}virtual int UnknownSlot2(){return -1;}virtual int Add(unsigned int,void*)=0;virtual unsigned int InnerCount(unsigned int)=0;virtual unsigned int OuterCount()=0;virtual void* At(unsigned int,unsigned int)=0;virtual void* RandomValue(unsigned int)=0;virtual void Remove(unsigned int,void*)=0;virtual void UnknownTraversal9(){}virtual void UnknownTraversal10(){}virtual ~CUnprovenTwoLevelCollectionBase_012B3444(){};};
template<class T>class CUnprovenPointerArray12_00C42A60{public:T** Entries;unsigned int Capacity,Count;
#ifdef _MSC_VER
__declspec(noinline)
#endif
unsigned int GetCount();T* RandomValue();};
class CUnprovenTwoLevelCollection_00C34DD0:public CUnprovenTwoLevelCollectionBase_012B3444{public:CUnprovenPointerArray12_00C42A60<CUnprovenPointerArray12_00C42A60<void> > Arrays;virtual unsigned long Kind(){return 0x20200;}virtual int Add(unsigned int,void*){return 0;}virtual unsigned int InnerCount(unsigned int);virtual unsigned int OuterCount(){return Arrays.Count;}virtual void* At(unsigned int,unsigned int){return 0;}virtual void* RandomValue(unsigned int);virtual void Remove(unsigned int,void*){};virtual ~CUnprovenTwoLevelCollection_00C34DD0(){};};
#pragma auto_inline(off)
template<class T>unsigned int CUnprovenPointerArray12_00C42A60<T>::GetCount(){return Count;}
#pragma auto_inline(on)
unsigned int CUnprovenTwoLevelCollection_00C34DD0::InnerCount(unsigned int index){return Arrays.Entries[index]->GetCount();}
void* CUnprovenTwoLevelCollection_00C34DD0::RandomValue(unsigned int){return 0;}static void* expected;static int calls,bad;template<class T>T* CUnprovenPointerArray12_00C42A60<T>::RandomValue(){if(this!=expected)bad=1;++calls;return Count?Entries[Count-1]:0;}
int main(){CUnprovenTwoLevelCollection_00C34DD0 owner;CUnprovenPointerArray12_00C42A60<void> inner[3];CUnprovenPointerArray12_00C42A60<void>* slots[]={inner+2,inner,inner+1};int values[]={5,7,9};void* entries[]={values,values+1,values+2};owner.Arrays.Entries=slots;owner.Arrays.Count=3;owner.Arrays.Capacity=5;for(unsigned int i=0;i<3;++i){inner[i].Entries=entries;inner[i].Capacity=3;inner[i].Count=i;}CUnprovenTwoLevelCollectionBase_012B3444* base=&owner;for(unsigned int i=0;i<3;++i){expected=slots[i];if(base->InnerCount(i)!=slots[i]->Count)goto fail;}if(bad||calls!=0||owner.Arrays.Capacity!=5||owner.Arrays.Count!=3)goto fail;puts("00c34df0_TEST PASS");return 0;fail:puts("00c34df0_TEST FAIL");return 1;}