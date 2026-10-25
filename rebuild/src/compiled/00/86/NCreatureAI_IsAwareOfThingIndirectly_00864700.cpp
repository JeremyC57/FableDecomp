// NCreatureAI::IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight) -- true when the creature perceives the thing and is aware of it, or the thing is within its sight radius (and, if asked, in navigation line of sight); no field-of-view test
// Leaf: ?IsAwareOfThingIndirectly@NCreatureAI@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00864700, 320 bytes incl. int3 padding, /Gr `__fastcall` (creature in ecx,
//   thing in edx, the bool on the stack), `ret 4`.
// Identity: the catalog's "IsAwareOfThingImplementation" is propagated from the near-identical 0x00864250 (which takes a
//   fourth, member-pointer argument and `ret 8`). FableWin/Ego_r PDB
//   `?IsAwareOfThingIndirectly@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z`: the Ego_r order (IsAwareOfPosition
//   0x00834B30, IsAwareOfThingIndirectly 0x00834C60 (0x150 bytes), IsBestTargetForAttack 0x00834DB0) is the retail order
//   (0x00864620, this row, 0x00864840), and only this NCreatureAI function has the 3-argument shape.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0, CTCPerceiveThing::GetPerceivedThing()
//   const 0x007446A0, CTCPerceiveThing::IsAwareOfThing() const 0x00643B3B (catalog name; `AwarenessTimer != 0`),
//   CTCPerceiveThing::GetSightRadius() const 0x007448F0, CThing::GetPosition() const 0x004C73D0,
//   CTCCreatureNavigation::IsPosNavLineOfSightToPos(const C3DVector&, const C3DVector&) const 0x006CB610.
// Layout: CThing interface bitset +0x20 (0x34 = CTCPerceiveThing: word 1 bit 20; 7 = CTCCreatureNavigation: word 0
//   bit 7), interface map +0x44, Dead bit 0 of +0x91.
// Shape: the IsAwareOfThingImplementation body (0x00864250) without the FOV test and with IsAwareOfThing called
//   directly: alive tests (thing first), HasTC(0x34), the perception component read before the self test, perceived
//   thing and awareness, the 2D squared distance against the squared sight radius, `if (!check_line_of_sight) return
//   true;`, the navigation component assigned under HasTC(7) into an otherwise unset pointer.
class CBaseObject;
class CTCBase;

class C3DVector {
public:
    float X, Y, Z;
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;

    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CThingCreatureBase : public CThing {
public:
};
class CThingAICreature : public CThingCreatureBase { };

class CTCPerceiveThing {                                    // interface type 0x34
public:
    const CThing* GetPerceivedThing() const;                // 0x007446A0
    float GetSightRadius() const;                           // 0x007448F0
    bool IsAwareOfThing() const;                            // 0x00643B3B (AwarenessTimer != 0)
};

class CTCCreatureNavigation {                               // interface type 7
public:
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;   // 0x006CB610
};

namespace NCreatureAI {
bool __fastcall IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight);
}

bool __fastcall NCreatureAI::IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight)
{
    if (thing == 0 || thing->Dead)
        return false;
    if (creature == 0 || creature->Dead)
        return false;
    if (!creature->HasTC(0x34))
        return false;
    const CTCPerceiveThing* perceive = (const CTCPerceiveThing*)creature->PeekTC(0x34);
    if (creature == thing)
        return true;
    if (thing == perceive->GetPerceivedThing() && perceive->IsAwareOfThing())
        return true;
    float sight_radius = perceive->GetSightRadius();
    const C3DVector& thing_pos = thing->GetPosition();
    const C3DVector& creature_pos = creature->GetPosition();
    float dx = creature_pos.X - thing_pos.X;
    float dy = creature_pos.Y - thing_pos.Y;
    if (dx * dx + dy * dy < sight_radius * sight_radius) {
        if (!check_line_of_sight)
            return true;
        const CTCCreatureNavigation* navigation;
        if (creature->HasTC(7))
            navigation = (const CTCCreatureNavigation*)creature->PeekTC(7);
        if (navigation->IsPosNavLineOfSightToPos(creature->GetPosition(), thing->GetPosition()))
            return true;
    }
    return false;
}