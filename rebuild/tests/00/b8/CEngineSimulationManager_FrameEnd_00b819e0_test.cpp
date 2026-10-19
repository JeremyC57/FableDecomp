#include <stdlib.h>
#include <stdio.h>
#include <new>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
void trackedFree(void*);
#define free trackedFree
// CEngineSimulationManager::FrameEnd -- erase uniquely held simulation records from all three update lists.
// Retail00b819e0,226bytes; free BFEA14. Native STLport circular lists are one pointer at0c/10/14; donor lists12bytes at10/1c/28.
#ifdef _MSC_VER
#include "engine/CEngineSimulationManager.h"
#include "engine/CIVCountedPointeeBase.h"
#endif
#include <stdlib.h>
struct SimulationInfoNative {virtual void Placeholder();virtual void Release();unsigned long IVRefCount;};
struct SimPointer {SimulationInfoNative* Item;~SimPointer(){SimulationInfoNative* item=Item;if(item){if(--item->IVRefCount==0)item->Release();Item=0;}}};
struct SimNode {SimNode* Next;SimNode* Prev;SimPointer Value;};
struct SimList {SimNode* Head;
 __forceinline void RemoveUnused(){for(SimNode* p=Head->Next;p!=Head;){SimNode* next=p->Next;if(p->Value.Item && p->Value.Item->IVRefCount==1){SimNode* prev=p->Prev;prev->Next=next;next->Prev=prev;p->Value.~SimPointer();free(p);}p=next;}}
};
struct SimulationListsNative {unsigned char ComponentBase[0xc];SimList FullUpdateOnlySimulationInfoList,AlwaysUpdateSimulationInfoList,PausedSimulationInfoList;void FrameEnd(const void*);};
void SimulationListsNative::FrameEnd(const void*){FullUpdateOnlySimulationInfoList.RemoveUnused();AlwaysUpdateSimulationInfoList.RemoveUnused();PausedSimulationInfoList.RemoveUnused();}

#undef free
int frees=0,badFree=0,released=0;void* freed[6];
void trackedFree(void* p){SimulationInfoNative* item=0;memcpy(&item,(unsigned char*)p+offsetof(SimNode,Value)+offsetof(SimPointer,Item),sizeof item);if(item)badFree=1;if(frees<6)freed[frees]=p;++frees;free(p);}
void SimulationInfoNative::Placeholder(){}void SimulationInfoNative::Release(){++released;}
SimNode* node(SimulationInfoNative* v){SimNode* p=new(malloc(sizeof(SimNode))) SimNode;p->Value.Item=v;return p;}
void setup(SimList& l,SimulationInfoNative* a,SimulationInfoNative* b){SimNode* h=node(0);SimNode* u=node(a);SimNode* n=node(0);SimNode* s=node(b);h->Next=u;u->Next=n;n->Next=s;s->Next=h;h->Prev=s;s->Prev=n;n->Prev=u;u->Prev=h;l.Head=h;}
int main(){SimulationListsNative m;SimulationInfoNative unique[3],shared[3];for(int i=0;i<3;++i){unique[i].IVRefCount=1;shared[i].IVRefCount=2;}
setup(m.FullUpdateOnlySimulationInfoList,unique,shared);setup(m.AlwaysUpdateSimulationInfoList,unique+1,shared+1);setup(m.PausedSimulationInfoList,unique+2,shared+2);
SimNode* lists[3]={m.FullUpdateOnlySimulationInfoList.Head,m.AlwaysUpdateSimulationInfoList.Head,m.PausedSimulationInfoList.Head};void* expected[6];for(int e=0;e<3;++e){expected[e]=lists[e]->Next;expected[e+3]=lists[e]->Prev;}
m.FrameEnd(0);if(frees!=3||released!=3||badFree)return 1;for(int j=0;j<3;++j){SimNode* h=lists[j];SimNode* n=h->Next;SimNode* s=h->Prev;if(freed[j]!=expected[j]||n->Value.Item||n->Prev!=h||n->Next!=s||s->Prev!=n||s->Next!=h||s->Value.Item!=shared+j||unique[j].IVRefCount||shared[j].IVRefCount!=2)return 2;}
m.FrameEnd(0);if(frees!=3||released!=3)return 3;
for(int k=0;k<3;++k)shared[k].IVRefCount=1;m.FrameEnd(0);if(frees!=6||released!=6||badFree)return 4;
for(int z=0;z<3;++z){SimNode* h=lists[z];SimNode* n=h->Next;if(freed[z+3]!=expected[z+3]||h->Prev!=n||n->Next!=h||n->Prev!=h||n->Value.Item||shared[z].IVRefCount)return 5;h->Next=h;h->Prev=h;n->~SimNode();free(n);}
m.FrameEnd(0);if(frees!=6||released!=6)return 6;for(int q=0;q<3;++q){lists[q]->~SimNode();free(lists[q]);}printf("00b819e0_TEST PASS\n");return 0;}