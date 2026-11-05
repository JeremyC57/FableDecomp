// CTCAssassinRush::FinalCleanup() -- when the parent thing is alive: release its special-abilities lock if ability 13 (assassin rush) is running and, unless the exit modes already ran, resume the player's animation (player-controlled parent) or end this TC's bullet-time slowdown; then kill both trail effects that are still alive
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AA4D0, 231 bytes (+9 int3), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface, type 0xA7),
//   CThingAICreature::SuspendAI 0x00833A10, CTCBase::GetWorld 0x00686A70, CBulletTimeManager::RemoveBulletTimeSlowdown
//   0x004C6140 (world +0x68), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80.
// Naming: Ego_r.pdb ?FinalCleanup@CTCAssassinRush@@AAEXXZ; the only references are the two vtable thunks
//   0x007AA820 (slot 14) and 0x007AA830 (slot 2) of CTCAssassinRush (vtable 0x0126CF2C) that tail-jump here
//   (OnKill / OnRemove forwarding to the private FinalCleanup). Catalog label "CTCInventory_OnDeactivate" is propagated.
// Layout: as the landed FrameUpdate 0x007AB190: ParentThing +4, AssassinTrail +0x18, AssassinDisplacementTrail +0x20,
//   CompletedExitModes +0xa4; CThing state byte +0x10 (1 = player), bits +0x20, interface map +0x44, dead bit +0x91.
// Shape: the parent is read from the member at each use (no local: eax for the alive test and the lookup, ecx for the
//   state test and the call); the constant 1 lives in ebx (dead-bit tests, state compare, Kill(true)) only when the
//   three alive tests go through an inline `bool IsDead() const { return (m_stateFlags & 1) != 0; }` (the raw
//   `!(flags & 1)` test leaves immediates, 230 v 231); the two ability clears are immediate stores.
// Leaf: ?FinalCleanup@CTCAssassinRush
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
class CTCSpecialAbilities { public:                            // interface 0xA7
    unsigned char _pad00[0xc];
    bool          AbilityMutexLock;                     // +0x0c
    long          CurrentAbility;                       // +0x10 (13 = assassin rush)
};
class CBulletTimeManager { public:
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
class CWorld { public:
    unsigned char _pad00[0x68];
    CBulletTimeManager* PBulletTimeManager;             // +0x68
    void RemoveBulletTimeSlowdown(unsigned long key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCAssassinRush : public CTCBase { public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x18 - 0x08];
    CBaseIntelligentPointer AssassinTrail;              // +0x18
    CBaseIntelligentPointer AssassinDisplacementTrail;  // +0x20
    unsigned char           _pad28[0xa4 - 0x28];
    bool                    CompletedExitModes;         // +0xa4
    void FinalCleanup();
};

void CTCAssassinRush::FinalCleanup()
{
    if (IS_THING_ALIVE(ParentThing)) {
        CTCSpecialAbilities* abilities;
        if (ParentThing->GetTC(TCI_SPECIAL_ABILITIES, &abilities)) {
            if (abilities->CurrentAbility == 13) {
                abilities->AbilityMutexLock = false;
                abilities->CurrentAbility = 0;
            }
        }
        if (!CompletedExitModes) {
            if (ParentThing->GetType() == 1)
                ((CThingAICreature*)ParentThing)->SuspendAI(false, 0);
            else
                GetWorld().RemoveBulletTimeSlowdown((unsigned long)this);
        }
    }
    if (IS_THING_ALIVE(AssassinTrail))
        ((CThing*)AssassinTrail.GetPItem())->Kill(true);
    if (IS_THING_ALIVE(AssassinDisplacementTrail))
        ((CThing*)AssassinDisplacementTrail.GetPItem())->Kill(true);
}