// CGameScriptInterface::CreateLight -- create MARKER_LIGHT and configure its light component
// Leaf: ?CreateLight@CGameScriptInterface@@UBE?AVCScriptThing@@ABVC3DVector@@ABVCRGBColour@@ABVCCharString@@MM_N@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 30 (family csm): the ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h's retail class
//   (virtual ~CScriptThing, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ; the projection's inline bodies defined after the include),
//   CThingNative is the CThing layout (?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ, both PDBs: const), and the own row is the retail
//   virtual CGameScriptInterface member (was the invented CGameScriptInterfaceMethods)
// Retail0089FAA0,267 bytes;0040F020 lower_bound,0099A2E0 base destructor,00BFE9BC delete; CTCAIScratchpad donor layout.
#include "fable_string.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //0040F020
class CScriptThing;
class CThing{ public:void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;CScriptThing GetScriptThing() const;}; //004C7E50 //004C73D0,004C7E50
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840
// Retail expands these at this row's sites (the CScriptThing projection had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { if (!PImp.Data) return 0; return PImp.Data->GetPThing(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#include "engine/CGameScriptInterface.h"

class CRGBColour{ public:unsigned char B,G,R,A;};
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager: public CDefinitionManager { public:}; //009AD410
CThing* __fastcall CreateMarker(long,const C3DVector&,const CCharString&); //0083D460
class CTCLight{ public:void SetOverridden(bool);void SetColour(const CRGBColour&);void SetInnerRadius(float);void SetOuterRadius(float);void SetFlicker(float);public:
    void SetActive(bool);   // 0x006412B0
}; //00640DF0,00640BB0,00640C40,00640C70,00640CA0,006412B0; CLightInfo offsets0/4/8/C

CScriptThing CGameScriptInterface::CreateLight(const C3DVector& position,const CRGBColour& colour,const CCharString& name,float inner,float outer,bool flicker)const{
 long id;{CCharString markerName("MARKER_LIGHT",-1);id=DefinitionManager->GetDefGlobalIndexFromName(markerName);}
 if(id){CThing* marker=CreateMarker(id,position,name);if(marker){
 if(marker->UsingComponents[1]&0x80){ComponentMap* map=&marker->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)39);ComponentEntry* end=map->Finish;CTCLight* light=static_cast<CTCLight*>(((void*)(it==end||it->first>39?end:it)->second));if(light){light->SetOverridden(true);light->SetColour(colour);light->SetInnerRadius(inner);light->SetOuterRadius(outer);light->SetFlicker((float)(unsigned char)flicker);light->SetActive(true);}}
 return marker->GetScriptThing();}}
 return CScriptThing();
}