// CCreatureActionBase::GetWorld() -- return the creature's current world through its intelligent pointer
// Leaf: ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ
// Retail00692940,15bytes. Calls intelligent-pointer GetPItem00A01B50 then CThing::GetWorld004C7AA0.
// The prior propagated std::_Dest_val label discarded the result type. Native tailcall returns CWorld& in EAX.
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CWorld.h"
class CCreatureActionBase;
class CThing : public CThingFields {
    friend class CCreatureActionBase;   // Access: friend (PDB IAE; retail-consistent, not attested)
protected:
    CWorld& GetWorld();                 // 0x004C7AA0 reads global main component 013B8A1C+24 (Ego_r.pdb protected)
};
class CBaseObject;
struct CBaseObjectPointer;
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;CBaseObject* GetPItem()const;}; // native8-byte intelligent pointer, actual PDB fields; GetPItem 00A01B50
class CCreatureActionBase : public CCreatureActionBaseFields {
protected:
    CWorld& GetWorld();                 // 0x00692940 (Ego_r.pdb ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ)
};
CWorld& CCreatureActionBase::GetWorld(){return ((CThing*)reinterpret_cast<const CBaseIntelligentPointer*>(PCreature)->GetPItem())->GetWorld();}