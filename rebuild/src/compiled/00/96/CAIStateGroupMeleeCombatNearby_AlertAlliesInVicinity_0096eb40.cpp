// CAIStateGroup_MeleeCombatNearby::AlertAlliesInVicinity() -- when the creature still perceives its enemy (line of sight or smell) and no type-0xC message is pending, post message 0xC from the creature to the enemy at the creature's position (the global alert delay, value2 36.0) so nearby allies join in; always true
// Leaf: ?AlertAlliesInVicinity@
// Grade: RELOCATION_MATCH (was DIFFER 208v203 nd=4) -- levers: every condition as its own early `return true` (the no-
//   perception exit shares the epilogue tail, as 0x0093DFC0) and the IsOfType filter + manager lookup in their own block
//   (the filter reuses the perception out-pointer's frame word: push ecx frame).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096EB40, 203 bytes (+ int3 padding to 208), plain `ret`. Byte-identical twin:
//   0x0093DFC0 (CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity, catalog "CAIStateGroup_MeleeCombatNearby2_
//   AddCombatMessage"; only its delay global differs, 0x013BB714).
// Callees: CVectorMap LowerBound const @ 0x004365B0 (the inlined const GetInterface of type 0x34 = CTCPerceiveThing:
//   bitset word +0x24 bit 20), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy at +0x14, twice),
//   CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CAIStateGroupBase::PeekWorld @ 0x008FCFA0 / GetWorld
//   @ 0x008FCFC0 (byte-identical accessors), CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType>
//   @ 0x004AB8B0, the creature's CThingPhysical::PeekPos copy @ 0x006A4D00, CMessageEventManager::AddMessage @ 0x00687540; the
//   delay is the global @ 0x013BC3A4 (initialised to g_01375550 * 5 by 0x01223C80).
// Shape: GetInterface(&perceive, 0x34) && perceive->GetPerceivedThing() == PEnemy.GetPItem() && (LineOfSight || Smelt);
//   the filter is a named local built before the world call; both managers are named locals fetched before their
//   call's arguments; the creature is re-read for the AddMessage arguments.
// Layout: Ego_r.pdb CTCPerceiveThing LineOfSight +0x2d, Smelt +0x31; CAIStateGroup_MeleeCombatNearbyBase::PEnemy +0x14,
//   CAIStateGroup_MeleeCombatNearby::Archer +0x1c; the world's message-event manager at +0x60.
// Label: catalog "Combat_ProcessMeleeAttackMessage" -> Ego_r.pdb ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby@@AAE_NXZ
//   (between AttackEnemy 0x0096EA50 and MoveToEnemyCreatureCombatPosition 0x0096EC10; its one caller is Update 0x0096EE90).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;           // 0x006A4D00 (Ego_r.pdb ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ)
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CTCPerceiveThing { public:                       // interface 0x34
    unsigned char _pad00[0x2d];
    bool LineOfSight;                           // +0x2d
    bool ExtendedLineOfSight;                   // +0x2e
    bool IgnoreFOVLineOfSight;                  // +0x2f
    bool DirectAwareness;                       // +0x30
    bool Smelt;                                 // +0x31
    CThing* GetPerceivedThing();                // 0x007446A0
};
class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(CMessageEventFilter_IsOfType* filter);   // 0x004AB8B0
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                              float value1, float value2, long data);   // 0x00687540
};
class CWorld {
public:
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;  // +0x60
};
extern unsigned long g_AlertAlliesMessageDelay; // 0x013BC3A4
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CWorld& PeekWorld() const;            // 0x008FCFA0 (Ego_r.pdb ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ)
    CWorld& GetWorld() const;                   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
public:
};
struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
};
class CAIStateGroup_MeleeCombatNearby : public CAIStateGroup_MeleeCombatNearbyBase {
public:
    bool Archer;                                // +0x1c
private:
    bool AlertAlliesInVicinity();
};

bool CAIStateGroup_MeleeCombatNearby::AlertAlliesInVicinity()
{
    CTCPerceiveThing* perceive;
    if (!((const CThing*)GetCreature())->GetInterface(&perceive, 0x34))
        return true;
    if (perceive->GetPerceivedThing() != PEnemy.GetPItem() || (!perceive->LineOfSight && !perceive->Smelt))
        return true;
    {
        CMessageEventFilter_IsOfType filter(0xc);
        CMessageEventManager* recent = PeekWorld().MessageEventManager;
        if (recent->FindMostRecentMessage(&filter) != 0)
            return true;
    }
    CThingCreatureBase* creature = GetCreature();
    CMessageEventManager* messages = GetWorld().MessageEventManager;
    messages->AddMessage(0xc, g_AlertAlliesMessageDelay, creature->PeekPos(), creature, PEnemy.GetPItem(),
                         0.0f, 36.0f, 0);
    return true;
}