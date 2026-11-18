// CGameScriptInterface::BeginLoadingMesh(const CCharString&) -- begin loading the graphic mesh selected from an object or creature definition
// Retail00894AA0 232bytes. Native object Graphic70 vs donor74; creature Graphic100 vs donor108.
// CGameDefinitionManager quarantined header is sizeof1/vptr4 inconsistent; singleton returns actual CDefinitionManager.
//44C6B0 definition manager singleton,8ABF80/8ABFF0 GetDef (propagated template names wrong),
//A24260 CMeshDataBank entry accessor (shared shape; not inventory ability), A23E40 IsLoaded,A256F0 BeginLoading.
// NF79 (round 7, q10974): the mesh is reached through the shared headers' const accessors: CMeshDataBank::PeekMeshInfo 0x00A24260
//   (?PeekMeshInfo@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z, the landed self row; engine/CMeshDataBank.h PEEKINFO), then C3DMeshInfo::IsLoaded
//   0x00A23E40 / BeginLoading 0x00A256F0 (Ego_r.pdb + FableWin.pdb, both const; engine/C3DMeshInfo.h ISLOADED on the retail
//   CMeshDataBankEntry branch); was the stand-ins C3DMeshInfoMethods / CMeshDataBankMethods::GetMesh. The bank pointer is read before the
//   index (retail `mov edx,[ebx+4]` first). Other stand-ins (CDefinitionManagerMethods, the local CDefPointer, CGameScriptInterfaceMethods)
//   pre-existing.
#include "fable_compiler.h"
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CDefinitionManager.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "fable_string.h"
#include "engine/_quarantine/CThingObjectDef.h"
#include "engine/_quarantine/CThingCreatureDef.h"
class CEngineGraphic { public:unsigned long BankIndex;float AnimStep,RenderSizeX;bool AdditiveAlpha;unsigned char Type;};
struct CDefPointeeNative {void* VTable;long References;};
union DefReleaseSlot {void* Native;void(CDefPointeeNative::*Release)();};
struct CThingObjectDefNative:CDefPointeeNative {unsigned char BeforeGraphic[0x68];CEngineGraphic Graphic;};
struct CThingCreatureDefNative:CDefPointeeNative {unsigned char BeforeGraphic[0xf8];CEngineGraphic Graphic;};
FABLE_FORCEINLINE void ReleaseDef(CDefPointeeNative* p){if(--p->References==0)(p->*reinterpret_cast<DefReleaseSlot*>(p->VTable)[1].Release)();}
template<class T>class CDefPointer { public:T* Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)++Object->References;}~CDefPointer(){if(Object)ReleaseDef(Object);}void Reset(T* p){if(Object!=p){if(Object){ReleaseDef(Object);Object=0;}Object=p;if(p)++p->References;}}};
struct CDefinitionManagerMethods:CDefinitionManager {CDefPointer<CDefPointeeNative> GetPDefByName(const CCharString&);bool GetObjectDef(const CCharString&,CDefPointer<CThingObjectDefNative>&);bool GetCreatureDef(const CCharString&,CDefPointer<CThingCreatureDefNative>&);};
class CGameDefinitionManager : public CDefinitionManagerMethods { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
#pragma auto_inline(off)
bool CDefinitionManagerMethods::GetObjectDef(const CCharString& meshName,CDefPointer<CThingObjectDefNative>& out){CDefPointer<CDefPointeeNative> p=GetPDefByName(meshName);if(p.Object){out.Reset(static_cast<CThingObjectDefNative*>(p.Object));return true;}return false;}
bool CDefinitionManagerMethods::GetCreatureDef(const CCharString& meshName,CDefPointer<CThingCreatureDefNative>& out){CDefPointer<CDefPointeeNative> p=GetPDefByName(meshName);if(p.Object){out.Reset(static_cast<CThingCreatureDefNative*>(p.Object));return true;}return false;}
#pragma auto_inline(on)
struct CGameScriptInterfaceMethods:CGameScriptInterface {void BeginLoadingMesh(const CCharString&)const;};
FABLE_STATIC_ASSERT(sizeof(DefReleaseSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CThingObjectDefNative,Graphic)==0x70);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureDefNative,Graphic)==0x100);
void CGameScriptInterfaceMethods::BeginLoadingMesh(const CCharString& meshName)const{
 CDefPointer<CThingObjectDefNative> object;CDefPointer<CThingCreatureDefNative> creature;
 CEngineGraphic* graphic;
 if(CGameDefinitionManager::Get().GetObjectDef(meshName,object))graphic=&object.Object->Graphic;
 else if(CGameDefinitionManager::Get().GetCreatureDef(meshName,creature))graphic=&creature.Object->Graphic;
 else return;
 if(graphic&&(graphic->Type==4||graphic->Type==5)){
 CMeshDataBank* bank=World->PMainMeshBank_Data;
 const C3DMeshInfo* mesh=bank->PeekMeshInfo(graphic->BankIndex);
 if(!mesh->IsLoaded())mesh->BeginLoading((unsigned long)-1,false,false);
 }
}