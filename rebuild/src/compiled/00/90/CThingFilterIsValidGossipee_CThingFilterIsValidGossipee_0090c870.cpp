// CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(CThingAICreature* thing) -- records the gossiping creature, its faction (from the kind-0x49 component) and its village (from the village-member component, kind 0x23)
// Leaf: ??0CThingFilter_IsValidGossipee
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C870, length 135 bytes (`ret 4`, + int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (two inlined GetTC lookups: kind 0x49 = bit 9 of
// CThing+0x28, kind 0x23 = bit 3 of CThing+0x24), the CTCVillageMember village getter @ 0x007EE540 (thiscall on the
// component; manifest "IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>>" is a propagated label -- the body reads the
// member's CIntelligentPointer at +0x10).
// Layout: PDB CThingFilter_IsValidGossipee { CThingAICreature* Thing (+0); CFaction* PFaction (+4); CTCVillage* PTCVillage (+8); };
// the kind-0x49 component's faction pointer at +0x18 (local model); CThing component bits +0x20, interface map +0x44.
// Shape: init list Thing(thing), PFaction(0), PTCVillage(0) (one `xor eax,eax`), then two out-parameter __forceinline GetInterface
// lookups (HasTC + GetTC picks the wrong registers); both keys are homed in the dead thing argument slot.
class CTCBase;
class CFaction;
class CTCVillage;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing {};
struct CTCFactionMember {                       // component kind 0x49
    unsigned char _pad00[0x18];
    CFaction*     PFaction;                     // +0x18
};
class CTCVillageMember { public:                       // component kind 0x23
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
class CThingFilter_IsValidGossipee { public:
    CThingAICreature* Thing;                    // +0x00
    CFaction*         PFaction;                 // +0x04
    CTCVillage*       PTCVillage;               // +0x08
    CThingFilter_IsValidGossipee(CThingAICreature* thing);
};

CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(CThingAICreature* thing)
    : Thing(thing), PFaction(0), PTCVillage(0)
{
    const CTCFactionMember* faction;
    if (thing->PeekTC(TCI_ENEMY, &faction))
        PFaction = faction->PFaction;
    const CTCVillageMember* member;
    if (thing->PeekTC(TCI_VILLAGE_MEMBER, &member))
        PTCVillage = member->GetPVillage();
}