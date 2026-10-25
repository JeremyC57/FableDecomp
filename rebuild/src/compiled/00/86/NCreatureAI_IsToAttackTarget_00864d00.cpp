// NCreatureAI::IsToAttackTarget(CThingAICreature* creature, const CThingCreatureBase* target) -- true when both are alive, the target is the creature's best enemy for attack (re-picked once when the two are no longer enemies and no faction override holds), and the creature has a script target override or is aware of the target
// Leaf: ?IsToAttackTarget@NCreatureAI@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00864D00, 256 bytes incl. int3 padding, /Gr `__fastcall` (creature in
//   ecx, target in edx), plain `ret`.
// Identity: FableWin/Ego_r PDB `?IsToAttackTarget@NCreatureAI@@YI_NPAVCThingAICreature@@PBVCThingCreatureBase@@@Z` (the
//   catalog label agrees). The FableWin debug image places CTCTargetingAI::HasScriptTargetOverride() const right after it.
// Callees: NCreatureAI::IsBestTargetForAttack 0x00864840 (landed, same TU), CTCEnemy::AreThingsEnemies(const CThing&,
//   const CThing&) 0x0076D2F0 (static __fastcall), NCreatureAI::IsOverridingFactionEnemyOnTarget 0x00864B30,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (non-const) / 0x004365B0 (const),
//   CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool) 0x007F6200, CBaseIntelligentPointer::GetPItem() const
//   0x00A01B50, the anonymous-namespace IsAwareOfThingImplementation 0x00864250 (fastcall, 4 args) with the predicate
//   0x00661600, the landed CTCPerceiveThing::CanSeeOrSmellPerceivedThing (`LineOfSight || Smelt`; descriptive -- the
//   catalog's CTCPerceiveThing::IsAwareOfThing is 0x00643B3B, the predicate IsAwareOfThingMoreGenerous 0x00864600 passes).
// Layout: CThing interface bitset +0x20 (type 8 = CTCTargetingAI: word 0 bit 8), interface map +0x44, Dead bit 0 of
//   +0x91; CTCTargetingAI (PDB) PBestEnemyCreatureForAttack +0x38, PScriptTargetOverride +0x40 (CIntelligentPointer).
// Shape: two alive tests (creature first), then everything under `if (IsBestTargetForAttack)` with one shared `return
//   false`: when the two are neither enemies nor faction-overridden, re-pick the best enemy through the non-const
//   out-pointer component lookup (descriptive GetTC: the component lands in eax, `mov ecx,eax`) and require the best
//   target again; then the const out-pointer lookup (descriptive PeekTC, 0x004365B0) and
//   `HasScriptTargetOverride() || IsAwareOfThing(creature, target, false)` (both inlined; the out-of-line
//   NCreatureAI::IsAwareOfThing is 0x008645E0, its `false` pushed from the zero eax). The lookups must be __forceinline.
class CBaseObject;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* m_pData;                                          // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CTCBase;
class CThingCreatureBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    SEntry* LowerBound(const long* type);                   // 0x0040F020
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

    template <class T> __forceinline bool GetTC(long type, T** out)      // component through an out-pointer (non-const lookup)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool PeekTC(long type, const T** out) const    // the const lookup
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

class CTCTargetingAI {                                      // interface type 8
public:
    unsigned char _pad0[0x38];
    CIntelligentPointer<CThingCreatureBase> PBestEnemyCreatureForAttack;   // +0x38
    CIntelligentPointer<CThingCreatureBase> PScriptTargetOverride;         // +0x40
    void UpdatePBestEnemyCreatureForAttack(bool force);     // 0x007F6200
    bool HasScriptTargetOverride() const { return PScriptTargetOverride.GetPItem() != 0; }
};

class CTCPerceiveThing {
public:
    bool CanSeeOrSmellPerceivedThing() const;               // 0x00661600
};

class CTCEnemy {
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);     // 0x0076D2F0
};

namespace {
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool b,
                                             bool (CTCPerceiveThing::*predicate)() const);  // 0x00864250
}

namespace NCreatureAI {
bool __fastcall IsBestTargetForAttack(const CThingAICreature& creature, const CThingCreatureBase* target);   // 0x00864840
bool __fastcall IsOverridingFactionEnemyOnTarget(const CThingAICreature* creature, const CThing* target); // 0x00864B30
inline bool __fastcall IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool b)   // 0x008645E0
{
    return IsAwareOfThingImplementation(creature, thing, b, &CTCPerceiveThing::CanSeeOrSmellPerceivedThing);
}
bool __fastcall IsToAttackTarget(CThingAICreature* creature, const CThingCreatureBase* target);
}

bool __fastcall NCreatureAI::IsToAttackTarget(CThingAICreature* creature, const CThingCreatureBase* target)
{
    if (creature == 0 || creature->Dead)
        return false;
    if (target == 0 || target->Dead)
        return false;
    if (IsBestTargetForAttack(*creature, target)) {
        if (!CTCEnemy::AreThingsEnemies(*creature, *target) && !IsOverridingFactionEnemyOnTarget(creature, target)) {
            CTCTargetingAI* targeting;
            if (creature->GetTC(8, &targeting))
                targeting->UpdatePBestEnemyCreatureForAttack(true);
            if (!IsBestTargetForAttack(*creature, target))
                return false;
        }
        const CTCTargetingAI* targeting;
        if (creature->PeekTC(8, &targeting)) {
            if (targeting->HasScriptTargetOverride() || IsAwareOfThing(creature, target, false))
                return true;
        }
    }
    return false;
}