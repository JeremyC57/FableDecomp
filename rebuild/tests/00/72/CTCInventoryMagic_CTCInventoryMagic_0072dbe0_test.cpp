// Full original destructor body; instrumented member/base destruction at engine boundaries.
#include <vector>
#include <assert.h>
#include <stdio.h>
struct Resource {int count,deleted;};
static bool Observe;
static int Stage,Destroyed,Expected;
class CEnginePrimitiveHandle {public:Resource* Data;CEnginePrimitiveHandle():Data(0){}
 virtual ~CEnginePrimitiveHandle(){if(Observe){assert(Stage==0);++Destroyed;}if(Data){--Data->count;if(!Data->count)++Data->deleted;}}
};
namespace NInventory {class CTCInventoryBase {public:virtual ~CTCInventoryBase(){if(Observe){assert(Stage==1 && Destroyed==Expected);Stage=2;}}};}
namespace NHeroInformationScreens {class CBase {public:virtual int GetScreenType()const{return 0;}~CBase(){if(Observe){assert(Stage==0 && Destroyed==Expected);Stage=1;}}};}
class CTCInventoryMagic:public NInventory::CTCInventoryBase,public NHeroInformationScreens::CBase {
public:virtual ~CTCInventoryMagic();std::vector<CEnginePrimitiveHandle> InventoryMagicPrimitiveHandles;
};
CTCInventoryMagic::~CTCInventoryMagic() {}
int main(){for(int n=0;n<8;++n)for(int pattern=0;pattern<3;++pattern){
 Resource state[2];for(int i=0;i<2;++i){state[i].count=0;state[i].deleted=0;}
 CTCInventoryMagic* p=new CTCInventoryMagic;p->InventoryMagicPrimitiveHandles.resize(n);
 for(int i=0;i<n;++i)if(pattern && (pattern==1 || i%2==0)){
  p->InventoryMagicPrimitiveHandles[i].Data=&state[i%2];++state[i%2].count;
 }
 int used[2]={state[0].count!=0,state[1].count!=0};Stage=Destroyed=0;Expected=n;Observe=true;
 NInventory::CTCInventoryBase* base=p;delete base;Observe=false;
 assert(Stage==2 && Destroyed==n);for(int i=0;i<2;++i){assert(state[i].count==0);assert(state[i].deleted==used[i]);}
 }puts("0072dbe0_TEST PASS");return 0;}