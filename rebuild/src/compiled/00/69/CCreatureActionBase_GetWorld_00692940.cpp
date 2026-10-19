// CCreatureActionBase::GetWorld() -- return the creature's current world through its intelligent pointer
// Retail00692940,15bytes. Calls intelligent-pointer GetPItem00A01B50 then CThing::GetWorld004C7AA0.
// The prior propagated std::_Dest_val label discarded the result type. Native tailcall returns CWorld& in EAX.
#include "engine/CCreatureActionBase.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CWorld.h"
struct CThingWorldCalls:CThing {CWorld& GetWorld();}; //004C7AA0 reads global main component013B8A1C+24
struct CBaseObjectPointer;
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;}; // native8-byte intelligent pointer, actual PDB fields
struct CIntelligentPointerCalls:CBaseIntelligentPointer {CThingWorldCalls* GetPItem()const;}; //00A01B50
struct CCreatureActionWorldCalls:CCreatureActionBase {CWorld& GetWorld();};
CWorld& CCreatureActionWorldCalls::GetWorld(){return reinterpret_cast<const CIntelligentPointerCalls*>(PCreature)->GetPItem()->GetWorld();}