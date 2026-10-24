// CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity() -- when the creature perceives its enemy (line of sight or smell) and no "combat alert" message (type 0xC) is live, post one from the creature about the enemy (hearing radius 36, five seconds); always true
// Grade: DIFFER(208v203) -- block layout only: retail's no-perception early exit jumps into the shared `mov al,1 ; pop edi ; pop ecx ; ret` tail of the main epilogue, ours gives it its own 5-byte copy; every instruction before it matches
// Leaf: ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby2@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093DFC0, 203 bytes (208 with padding), __thiscall, returns bool (`mov al,1`).
// Identity: Ego_r.pdb ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby2@@AAE_NXZ (private); the only caller is this class's
//   Update 0x0093E675 (landed, where it is declared descriptively as `void AddCombatMessage()`). The catalog's
//   "CAIStateGroup_MeleeCombatNearby2_AddCombatMessage" is descriptive.
// Callees: CVectorMap::LowerBound const 0x004365B0 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PEnemy +0x14, twice), CTCPerceiveThing::GetPerceivedThing 0x007446A0,
//   GetPThingWorld 0x008FCFA0 / GetPWorld 0x008FCFC0, CMessageEventManager::FindMostRecentMessage<IsOfType> 0x004AB8B0,
//   CThing::GetPosition 0x006A4D00, CMessageEventManager::AddMessage 0x00687540; global 0x013BB714 (this TU's
//   `frames per second * 5`, landed as g_scaled_013bb714).
// Layout: Ego_r.pdb CTCPerceiveThing LineOfSight +0x2d, Smelt +0x31; PEnemy +0x14 as the landed Update 0x0093E5E0.
// Shape: the GetInterface `if` (the key's `push ecx` slot later reused by the IsOfType filter), the perceived-thing
//   compare, `LineOfSight || Smelt`, the !Find guard, then AddMessage(0xC, lifespan, creature->GetPosition(), creature,
//   PEnemy.GetPItem(), 0.0f, 36.0f, 0) with the creature in a named local. Tried: nested ifs (best), one && chain,
//   #pragma optimize("s"), a `bool` result local -- all keep the duplicated early-exit tail.
struct CTCBase;
class C3DVector;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;       // 0x006A4D00
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CTCPerceiveThing {                       // interface 0x34 (Ego_r.pdb names)
    unsigned char _pad00[0x2d];
    bool LineOfSight;                           // +0x2d
    bool ExtendedLineOfSight;                   // +0x2e
    bool IgnoreFOVLineOfSight;                  // +0x2f
    bool DirectAwareness;                       // +0x30
    bool Smelt;                                 // +0x31
    CThing* GetPerceivedThing();                // 0x007446A0
};

struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    CMessageEvent* AddMessage(long type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target,
                              float sightRadius, float hearingRadius, long extraData);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

extern long g_scaled_013bb714;                  // 0x013BB714 (frames per second * 5, this TU's dynamic initializer)

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
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
    if (PBrain->PCreature->GetInterface(&perceive, 0x34)) {
        if (perceive->GetPerceivedThing() == PEnemy.GetPItem() && (perceive->LineOfSight || perceive->Smelt)) {
            if (!GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(CMessageEventFilter_IsOfType(0xc))) {
                CThing* creature = PBrain->PCreature;
                GetPWorld()->GetPMessageManager()->AddMessage(0xc, g_scaled_013bb714, creature->GetPosition(), creature,
                                                              PEnemy.GetPItem(), 0.0f, 36.0f, 0);
            }
        }
    }
    return true;
}
