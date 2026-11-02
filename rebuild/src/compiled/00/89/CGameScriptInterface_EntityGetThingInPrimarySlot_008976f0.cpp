// CGameScriptInterface::EntityGetThingInPrimarySlot(const CScriptThing&) -- return live primary carried thing or empty script handle
// Leaf: ?EntityGetThingInPrimarySlot@CGameScriptInterface@@UBE?AVCScriptThing@@ABV2@@Z
// Naming wave 31 (family d3): DECISIONS_W30 D3: the carrying component is CTCCarrying and its call the PDB const
//   ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ 0x006A1440 (was the transitional ?GetPrimarySlotThing@CarryingMethods@@QAEPAVCThing@@XZ)
// Naming wave 30 (family csm): the ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h's retail class
//   (virtual ~CScriptThing, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ; the projection's inline bodies defined after the include),
//   CThingNative is the CThing layout (?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ, both PDBs: const), and the own row is the retail
//   virtual CGameScriptInterface member (was the invented CGameScriptInterfaceMethods)
// Retail008976F0,254 bytes;0040F020 lower_bound,0099A2E0 base destructor,00BFE9BC delete; CTCAIScratchpad donor layout.
#include "fable_string.h"
class C3DVector{ public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //004365B0 (const lower_bound specialization)
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

class CTCCarrying{ public:CThing* GetThingInPrimarySlot() const;}; //006A1440 ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ (Ego_r.pdb / FableWin.pdb)

CScriptThing CGameScriptInterface::EntityGetThingInPrimarySlot(const CScriptThing& script)const{
 CScriptThing result;
 if(script.IsAlive()){CThing* thing=script.GetPThing();if(thing->UsingComponents[2]&0x40){ComponentMap* map=&thing->Components;const ComponentEntry* it=map->LowerBound((ETCInterfaceType)70);const ComponentEntry* end=map->Finish;CTCCarrying* carrying=static_cast<CTCCarrying*>(((void*)(it==end||it->first>70?end:it)->second));CThing* carried=carrying->GetThingInPrimarySlot();if(carried&&!(carried->DeletedFlags&1))result=carried->GetScriptThing();}}
 return result;
}