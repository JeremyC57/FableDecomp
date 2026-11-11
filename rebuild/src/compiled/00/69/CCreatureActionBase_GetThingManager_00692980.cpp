// CCreatureActionBase::GetThingManager() -- the creature's thing manager: fetch the acting creature (PCreature +0x08) and return the current thing manager
// Leaf: ?GetThingManager@CCreatureActionBase@@IAEAAVCThingManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692980, 14 bytes, thiscall: `add ecx,8 ; call 0x00A01B50 ; mov eax,[0x013B8A1C] ; ret`.
// Name (agentNF16): Ego_r.pdb + FableWin.pdb ?GetThingManager@CCreatureActionBase@@IAEAAVCThingManager@@XZ (protected, returns a
//   reference); the body reads the private static CThing::PCurrentThingManager (0x013B8A1C) through the public inline static
//   accessor called on PCreature's item, so the GetPItem call stays and its result is dropped. Was the free stand-in ?CCreatureActionBase_GetThingManager@@YIPAVCThingManager@@PAVCCreatureActionBase@@@Z.
#include "fable_compiler.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER
#include "engine/CCreatureActionBase.h"
// Partial view of CThing: the private static CThing::PCurrentThingManager (0x013B8A1C, ?PCurrentThingManager@CThing@@0PAVCThingManager@@A)
// and its inline public accessor (Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ).
class CThing : public CBaseObject {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }
};
class CThingCreatureBase : public CThing { };

CThingManager& CCreatureActionBase::GetThingManager()
{
    return *PCreature.Get()->GetPCurrentThingManager();
}
