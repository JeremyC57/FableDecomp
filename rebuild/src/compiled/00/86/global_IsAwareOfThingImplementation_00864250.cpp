// IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight, bool (CTCPerceiveThing::*is_aware)() const) -- true when the creature perceives the thing through the given perception predicate, or the thing is in its field of view and sight radius (and, if asked, in navigation line of sight)
// Leaf: ?IsAwareOfThingImplementation@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00864250, 336 bytes incl. int3 padding, /Gr `__fastcall` (creature in ecx,
//   thing in edx, the bool and the 4-byte member-function pointer on the stack), `ret 8`.
// Identity: FableWin/Ego_r PDB, anonymous namespace:
//   `?IsAwareOfThingImplementation@?A0x4b0ddf32@@YI_NPBVCThingAICreature@@PBVCThing@@_NP8CTCPerceiveThing@@BE_NXZ@Z`.
//   Callers: NCreatureAI::IsAwareOfThing 0x008645E0 (predicate 0x00661600), IsAwareOfThingMoreGenerous 0x00864600
//   (predicate 0x00643B3B) and IsToAttackTarget 0x00864D00 (IsAwareOfThing inlined).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0, CTCPerceiveThing::GetPerceivedThing()
//   const 0x007446A0, the predicate through the member pointer, CThingCreatureBase::IsThingInFOV(const CThing&, bool)
//   const 0x00665190, CTCPerceiveThing::GetSightRadius() const 0x007448F0, CThing::GetPos() const 0x004C73D0,
//   CTCCreatureNavigation::IsPosNavLineOfSightToPos(const C3DVector&, const C3DVector&) const 0x006CB610 (PDB name by
//   signature: TC 7's only const CTC method of that shape; catalog "CScriptThing::operator==" is propagated).
// Layout: CThing interface bitset +0x20 (0x34 = CTCPerceiveThing: word 1 bit 20; 7 = CTCCreatureNavigation: word 0
//   bit 7), interface map +0x44, Dead bit 0 of +0x91.
// Shape: alive tests (thing first), HasTC(0x34) then the perception component read before the self test;
//   `thing == GetPerceivedThing()` (that operand order gives `cmp ebx,eax`) and the predicate; FOV, then the 2D
//   squared distance against the squared sight radius (the radius spilled to the dead predicate slot); `if (!check_line_of_sight) return true;` and the navigation component assigned under
//   HasTC(7) into an otherwise unset pointer (absent: the dead bool slot is read). HasTC must be __forceinline (VC7.1
//   leaves its second use out of line otherwise).
class CBaseObject;
class CTCBase;

class C3DVector {
public:
    float X, Y, Z;
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;

    const C3DVector& GetPos() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsThingInFOV(const CThing& thing, bool flag) const;    // 0x00665190
};
class CThingAICreature : public CThingCreatureBase { };

class CTCPerceiveThing {                                    // interface type 0x34
public:
    const CThing* GetPerceivedThing() const;                // 0x007446A0
    float GetSightRadius() const;                           // 0x007448F0
    bool CanSeeOrSmellPerceivedThing() const;               // 0x00661600
};

class CTCCreatureNavigation {                               // interface type 7
public:
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;   // 0x006CB610
};

namespace {
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight,
                                             bool (CTCPerceiveThing::*is_aware)() const);
}

namespace NCreatureAI {
bool __fastcall IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight);
}

bool __fastcall NCreatureAI::IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight)
{
    return IsAwareOfThingImplementation(creature, thing, check_line_of_sight, &CTCPerceiveThing::CanSeeOrSmellPerceivedThing);
}

namespace {
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight,
                                             bool (CTCPerceiveThing::*is_aware)() const)
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
    if (thing == perceive->GetPerceivedThing() && (perceive->*is_aware)())
        return true;
    if (creature->IsThingInFOV(*thing, true)) {
        float sight_radius = perceive->GetSightRadius();
        const C3DVector& thing_pos = thing->GetPos();
        const C3DVector& creature_pos = creature->GetPos();
        float dx = creature_pos.X - thing_pos.X;
        float dy = creature_pos.Y - thing_pos.Y;
        if (dx * dx + dy * dy < sight_radius * sight_radius) {
            if (!check_line_of_sight)
                return true;
            const CTCCreatureNavigation* navigation;
            if (creature->HasTC(7))
                navigation = (const CTCCreatureNavigation*)creature->PeekTC(7);
            if (navigation->IsPosNavLineOfSightToPos(creature->GetPos(), thing->GetPos()))
                return true;
        }
    }
    return false;
}
}