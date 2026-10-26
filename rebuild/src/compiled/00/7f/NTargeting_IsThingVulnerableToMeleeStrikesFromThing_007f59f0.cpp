// NTargeting::IsThingVulnerableToMeleeStrikesFromThing(const CThing& thing, const CThing& fromThing) -- may the attacker's melee strikes hit the thing: always in the attacker's player's kill-everything mode (for a thing of type bit 3), when the switch is on and the thing is the attacker's AI best attack target, else when the two are enemies
// Leaf: ?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F59F0, 125 bytes (+ int3 padding), `__fastcall` (thing in ecx, fromThing in edx).
//   FableWin.pdb `?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z`; 9 call sites.
// Callees: CThing::PeekPlayer @ 0x004C7A10, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (interface
//   8 = CTCTargetingAI, presence word 0 bit 8), CTCTargetingAI slot 36 (+0x90, GetPBestEnemyCreatureForAttack() const),
//   CTCEnemy::AreThingsEnemies(const CThing&, const CThing&) @ 0x0076D2F0 (__fastcall).
// Data: a bool switch @ 0x0137574E (read only here; name unknown), CThing ThingTypeFlags +0x6c (retail), CPlayer +0x21b
//   (KillEverythingMode: Ego_r +0x223 minus the PDB build's 8 extra list bytes).
// Shape: the PDB's const PeekTC<CTCTargetingAI> with the map in a named local and the one-ternary entry pick (the
//   component then loads straight into ecx for the virtual call; the if-reassign form goes through eax); the enemy test is `if (...) return true;
//   return false;` (`test al,al ; setne al` on the callee's bool).
// Layout (supersedes any local-model note above): engine/CPlayer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayer CPlayerDonor
#include "engine/CPlayer.h"
#undef CPlayer
class CTCBase;

struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
};

class CPlayer : public CPlayerDonor {
public:
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned char  _pad50[0x6c - 0x50];
    unsigned long  ThingTypeFlags;              // +0x6c
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            const CTCMap* map = &m_tcs;
            const STCEntry* it = map->LowerBound((unsigned long)T::INTERFACE_TYPE);
            *out = (const T*)((it == map->m_pLast || T::INTERFACE_TYPE < it->m_kind) ? map->m_pLast : it)->m_pTC;
            return true;
        }
        return false;
    }
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};

class CThingCreatureBase : public CThing {
};

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCTargetingAI {
public:
    enum { INTERFACE_TYPE = 8 };
    VF10(0) VF10(1) VF10(2) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35)
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const;   // slot 36 (+0x90)
};

class CTCEnemy {
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};

extern bool g_bMeleeHitsBestAttackTarget;                       // 0x0137574E

namespace NTargeting {
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing& thing, const CThing& fromThing);
}

bool __fastcall NTargeting::IsThingVulnerableToMeleeStrikesFromThing(const CThing& thing, const CThing& fromThing)
{
    if ((thing.ThingTypeFlags & 8) && fromThing.PeekPlayer().KillEverythingMode)
        return true;
    if (g_bMeleeHitsBestAttackTarget) {
        const CTCTargetingAI* ai;
        if (fromThing.PeekTC(&ai) && ai->GetPBestEnemyCreatureForAttack() == &thing)
            return true;
    }
    if (CTCEnemy::AreThingsEnemies(fromThing, thing))
        return true;
    return false;
}