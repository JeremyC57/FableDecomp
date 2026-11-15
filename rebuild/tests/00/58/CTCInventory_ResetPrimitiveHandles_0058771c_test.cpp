// Standalone full method body; traceable external primitive/handle behavior.
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <stdio.h>
struct PrimitiveState { int references,notifications,deletions; };
static bool Observing;
static int Clears,Destructions,ExpectedClears;
class CEnginePrimitiveHandle {
public:
 PrimitiveState* Data;
 CEnginePrimitiveHandle():Data(0){}
 ~CEnginePrimitiveHandle(){if(Observing){assert(Clears==ExpectedClears);assert(Data==0);++Destructions;}}
 void Clear(){
  ++Clears;
  if(Data){if(Data->references==1)++Data->notifications;
   --Data->references;if(Data->references==0)++Data->deletions;Data=0;}
 }
};
class CTCInventory {
public:
 std::vector<CEnginePrimitiveHandle> InventoryMainPrimitiveHandles;
 long Unrelated;
 void ResetPrimitiveHandles();
};
void CTCInventory::ResetPrimitiveHandles()
{
    std::for_each(InventoryMainPrimitiveHandles.begin(), InventoryMainPrimitiveHandles.end(),
                  std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    InventoryMainPrimitiveHandles.clear();
}
int main(){
 for(int count=0;count<=6;++count)for(int mode=0;mode<3;++mode){
  PrimitiveState states[2];for(int k=0;k<2;++k){states[k].references=0;states[k].notifications=0;states[k].deletions=0;}
  CTCInventory inv;inv.Unrelated=981;
  inv.InventoryMainPrimitiveHandles.resize(count);
  for(int k=0;k<count;++k)if(mode!=0 && (mode==1 || k%2==0)){
   inv.InventoryMainPrimitiveHandles[k].Data=&states[k%2];++states[k%2].references;
  }
  int used[2]={states[0].references!=0,states[1].references!=0};
  unsigned long capacity=(unsigned long)inv.InventoryMainPrimitiveHandles.capacity();
  Clears=Destructions=0;ExpectedClears=count;Observing=true;
  inv.ResetPrimitiveHandles();
  assert(Clears==count && Destructions==count);assert(inv.InventoryMainPrimitiveHandles.empty());
  assert(inv.InventoryMainPrimitiveHandles.capacity()==capacity && inv.Unrelated==981);
  for(int k=0;k<2;++k){assert(states[k].references==0);assert(states[k].deletions==used[k]);assert(states[k].notifications==used[k]);}
  inv.ResetPrimitiveHandles();assert(Clears==count && Destructions==count);Observing=false;
 }
 puts("0058771c_TEST PASS");return 0;
}