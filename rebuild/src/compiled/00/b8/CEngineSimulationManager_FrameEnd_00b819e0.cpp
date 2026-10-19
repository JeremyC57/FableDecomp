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