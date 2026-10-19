// CGameScriptInterface::BeginLoadingMesh(const CCharString&) -- begin loading the graphic mesh selected from an object or creature definition
// Retail00894AA0 232bytes. Native object Graphic70 vs donor74; creature Graphic100 vs donor108.
// CGameDefinitionManager quarantined header is sizeof1/vptr4 inconsistent; singleton returns actual CDefinitionManager.
//44C6B0 definition manager singleton,8ABF80/8ABFF0 GetDef (propagated template names wrong),
//A24260 CMeshDataBank entry accessor (shared shape; not inventory ability), A23E40 IsLoaded,A256F0 BeginLoading.
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CDefinitionManager.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "fable_string.h"
#include "engine/_quarantine/CThingObjectDef.h"
#include "engine/_quarantine/CThingCreatureDef.h"
struct CEngineGraphic {unsigned long BankIndex;float AnimStep,RenderSizeX;bool AdditiveAlpha;unsigned char Type;};
struct CDefPointeeNative {void* VTable;long References;};
union DefReleaseSlot {void* Native;void(CDefPointeeNative::*Release)();};
struct CThingObjectDefNative:CDefPointeeNative {unsigned char BeforeGraphic[0x68];CEngineGraphic Graphic;};
struct CThingCreatureDefNative:CDefPointeeNative {unsigned char BeforeGraphic[0xf8];CEngineGraphic Graphic;};
__forceinline void ReleaseDef(CDefPointeeNative* p){if(--p->References==0)(p->*reinterpret_cast<DefReleaseSlot*>(p->VTable)[1].Release)();}
template<class T>struct CDefPointer {T* Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)++Object->References;}~CDefPointer(){if(Object)ReleaseDef(Object);}void Reset(T* p){if(Object!=p){if(Object){ReleaseDef(Object);Object=0;}Object=p;if(p)++p->References;}}};
struct CDefinitionManagerMethods:CDefinitionManager {static CDefinitionManagerMethods& Get();CDefPointer<CDefPointeeNative> GetPDefByName(const CCharString&);bool GetObjectDef(const CCharString&,CDefPointer<CThingObjectDefNative>&);bool GetCreatureDef(const CCharString&,CDefPointer<CThingCreatureDefNative>&);};
#pragma auto_inline(off)
bool CDefinitionManagerMethods::GetObjectDef(const CCharString& name,CDefPointer<CThingObjectDefNative>& out){CDefPointer<CDefPointeeNative> p=GetPDefByName(name);if(p.Object){out.Reset(static_cast<CThingObjectDefNative*>(p.Object));return true;}return false;}
bool CDefinitionManagerMethods::GetCreatureDef(const CCharString& name,CDefPointer<CThingCreatureDefNative>& out){CDefPointer<CDefPointeeNative> p=GetPDefByName(name);if(p.Object){out.Reset(static_cast<CThingCreatureDefNative*>(p.Object));return true;}return false;}
#pragma auto_inline(on)
struct C3DMeshInfoMethods:C3DMeshInfo {bool IsLoaded()const;void BeginLoading(unsigned long,bool,bool);};
struct CMeshDataBankMethods:CMeshDataBank {C3DMeshInfoMethods* GetMesh(unsigned long);};
struct CGameScriptInterfaceMethods:CGameScriptInterface {void BeginLoadingMesh(const CCharString&)const;};
FABLE_STATIC_ASSERT(sizeof(DefReleaseSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CThingObjectDefNative,Graphic)==0x70);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDefNative,Graphic)==0x100);
void CGameScriptInterfaceMethods::BeginLoadingMesh(const CCharString& name)const{
 CDefPointer<CThingObjectDefNative> object;CDefPointer<CThingCreatureDefNative> creature;
 CEngineGraphic* graphic;
 if(CDefinitionManagerMethods::Get().GetObjectDef(name,object))graphic=&object.Object->Graphic;
 else if(CDefinitionManagerMethods::Get().GetCreatureDef(name,creature))graphic=&creature.Object->Graphic;
 else return;
 if(graphic&&(graphic->Type==4||graphic->Type==5)){
 CMeshDataBankMethods* bank=static_cast<CMeshDataBankMethods*>(World->PMainMeshBank_Data);
 C3DMeshInfoMethods* mesh=bank->GetMesh(graphic->BankIndex);
 if(!mesh->IsLoaded())mesh->BeginLoading((unsigned long)-1,false,false);
 }
}