// CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity() -- when the creature perceives its enemy (line of sight or smell) and no "combat alert" message (type 0xC) is live, post one from the creature about the enemy (hearing radius 36, five seconds); always true
// Leaf: ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby2@@
// Grade: RELOCATION_MATCH (was DIFFER 208v203 nd=4) -- lever: every condition written as its own early `return true`
//   (inverted tests, flat body) so the no-perception exit shares the epilogue's mov al,1 tail as retail; the nested-if
//   form gives that exit its own copy.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093DFC0, 203 bytes (208 with padding), __fastcall, returns bool (`mov al,1`).
// Identity: Ego_r.pdb ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby2@@AAE_NXZ (private); the only caller is this class's
//   Update 0x0093E675 (landed, where it is declared descriptively as `void AddCombatMessage()`). The catalog's
//   "CAIStateGroup_MeleeCombatNearby2_AddCombatMessage" is descriptive.
// Callees: CVectorMap::LowerBound const 0x004365B0 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PEnemy +0x14, twice), CTCPerceiveThing::GetPerceivedThing 0x007446A0,
//   PeekWorld 0x008FCFA0 / GetWorld 0x008FCFC0, CMessageEventManager::FindMostRecentMessage<IsOfType> 0x004AB8B0,
//   CThingPhysical::PeekPos 0x006A4D00, CMessageEventManager::AddMessage 0x00687540; global 0x013BB714 (this TU's
//   `frames per second * 5`, landed as g_scaled_013bb714).
// Layout: Ego_r.pdb CTCPerceiveThing LineOfSight +0x2d, Smelt +0x31; PEnemy +0x14 as the landed Update 0x0093E5E0.
// Shape: the GetInterface `if` (the key's `push ecx` slot later reused by the IsOfType filter), the perceived-thing
//   compare, `LineOfSight || Smelt`, the !Find guard, then AddMessage(0xC, lifespan, static_cast<CThingPhysical*>(creature)->PeekPos(), creature,
//   PEnemy.GetPItem(), 0.0f, 36.0f, 0) with the creature in a named local. Tried: nested ifs (best), one && chain,
//   #pragma optimize("s"), a `bool` result local -- all keep the duplicated early-exit tail.
// Name fix (agentNF5 2026-11-10): CMessageEventManager::AddMessage 0x00687540 is ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z (Ego_r.pdb / FableWin.pdb): EMessageEventType, a const C3DVector reference, CMessageEvent* result.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Name fix (agentNF7 2026-11-10): the local CInterfaceMap stand-in replaced by the shared CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> > (engine/CVectorMap.h), so LowerBound binds the PDB spelling (0x0040F020 / const 0x004365B0).
enum EMessageEventType;                            // MS forward enum (CMessageEventManager::AddMessage 0x00687540 takes it)
#include "fable_compiler.h"
class CTCBase;
class C3DVector;

#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;   // LowerBound 0x0040F020 / const 0x004365B0
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;           // 0x006A4D00 (Ego_r.pdb ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ)
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
class CTCPerceiveThing { public:                       // interface 0x34 (Ego_r.pdb names)
    unsigned char _pad00[0x2d];
    bool LineOfSight;                           // +0x2d
    bool ExtendedLineOfSight;                   // +0x2e
    bool IgnoreFOVLineOfSight;                  // +0x2f
    bool DirectAwareness;                       // +0x30
    bool Smelt;                                 // +0x31
    const CThing* GetPerceivedThing() const;                // 0x007446A0 Ego_r/FableWin ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (agentNF16)
};

class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData);   // 0x00687540
};
class CWorld {
public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() const { return m_pMessageManager; }
};

extern long g_scaled_013bb714;                  // 0x013BB714 (frames per second * 5, this TU's dynamic initializer)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    const CWorld& PeekWorld() const;            // 0x008FCFA0 (Ego_r.pdb ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ)
    CWorld& GetWorld() const;                   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
public:
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroup_MeleeCombatNearbyBase {
    bool AlertAlliesInVicinity();
};

bool CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity()
{
    CTCPerceiveThing* perceive;
    if (!PBrain->PCreature->GetInterface(&perceive, 0x34))
        return true;
    if (perceive->GetPerceivedThing() != reinterpret_cast<CThing*>(PEnemy.GetPItem()) || (!perceive->LineOfSight && !perceive->Smelt))
        return true;
    if (PeekWorld().GetPMessageManager()->FindMostRecentMessage(CMessageEventFilter_IsOfType(0xc)))
        return true;
    CThing* creature = PBrain->PCreature;
    GetWorld().GetPMessageManager()->AddMessage((EMessageEventType)(0xc), g_scaled_013bb714, static_cast<CThingPhysical*>(creature)->PeekPos(), creature,
                                                  reinterpret_cast<CThing*>(PEnemy.GetPItem()), 0.0f, 36.0f, 0);
    return true;
}