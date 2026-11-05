// NCreatureMode::GetOldTargetFlagsAndReplaceWith(CThing& thing, ETargetingType type) -- swap the thing's targeting flags (CTCTargeted, interface 9) for `type` and return the old flags (0 without the component)
// Leaf: ?GetOldTargetFlagsAndReplaceWith@NCreatureMode@@YIKAAVCThing@@W4ETargetingType@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085F830, length 73 bytes. `__fastcall` free function (ecx = the thing), `type` in edx, old flags in eax.
// Mangled (Ego_r.pdb): ?GetOldTargetFlagsAndReplaceWith@NCreatureMode@@YIKAAVCThing@@W4ETargetingType@@@Z.
// Callees: the non-const CVectorMap LowerBound 0x0040F020, CTCTargeted::SetTargetingFlags 0x007EA270 (manifest
//   "CObservable::AddExclusiveObserver" is propagated). Caller: NCreatureMode::CModeUnderground::OnActivate 0x008D66E0.
// Layout: CTCTargeted TargetingFlags +0x0C from engine/CTCTargeted.h.
// Shape: the old flags are read into a named local (esi) before the setter call; `type` lives in edi.
// Layout (retail, local model; no retail-accurate CThing header exists): CThing type tag +0x10 (2 = creature), interface
//   present-bit words +0x20 (word = type >> 5), interface map {first,last,end} +0x44, ThingTypeFlags +0x6C (bit 1 physical,
//   bit 3 creature), PBaseDef +0x70, scan bits +0x91 (Dead = bit 0), CThingPhysical flag byte +0xBC, CThingCreatureBase
//   PCurrentAction +0x100 (CCountedPointer<CCreatureActionBase>::Object) and movement state +0x200 (landed models 0x0085EE90,
//   0x006E0DF0, 0x004497C0). The interface map is CVectorMap<ETCInterfaceType,CTCBase*>: const LowerBound 0x004365B0,
//   non-const 0x0040F020. Interface 0x31 = CTCCreatureModeManager (GetCurrentMode 0x0073A6E0, IsModeActive 0x0073A8A0,
//   RemoveMode 0x0073ABA0; the manifest labels "CPixelShader::CountReferences" / "CPlayer::HasMode" are propagated twins).
//   A mode's vtable slot 1 (+4) returns its NCreatureMode::EMode number (read from each CMode*::vfunc_1 body).
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> CTCInterfaceEntry;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCInterfaceMap;
class CModeBase {                                       // CTCCreatureModeManager::CModeBase
public:
    virtual void v00();
    virtual long GetType() const;                       // slot 1 (+4): NCreatureMode::EMode
};
class CTCCreatureModeManager {                          // interface 0x31
public:
    CModeBase& GetCurrentMode() const;                  // 0x0073A6E0
    bool IsModeActive(unsigned long mode) const;        // 0x0073A8A0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};
#define VF(n) virtual void v##n();
class CCreatureActionBase {
public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31)
    virtual bool IsPhysicsPushableByHero() const;                 // slot 32 (+0x80)
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;  // slot 33 (+0x84)
    VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44)
    virtual bool IsToEvadeAllHits() const;              // slot 45 (+0xB4)
    virtual bool IsToCauseRecoil() const;               // slot 46 (+0xB8)
    virtual bool IsAbleToBeAttacked() const;            // slot 47 (+0xBC)
    virtual bool IsPlayHitResponse() const;             // slot 48 (+0xC0)
    VF(49)
    virtual bool IsPreventingLookTurning() const;       // slot 50 (+0xC8)
};
class CPlayer;
class CDefClassBase;
enum ETargetingType { TARGETING_TYPE_NONE };

class CThingCreatureBase : public CThing {
public:
    unsigned char        _padc0[0x100 - 0xc0];
    CCreatureActionBase* PCurrentAction;                // +0x100
    unsigned char        _pad104[0x200 - 0x104];
    long                 MovementState;                 // +0x200 (3 tested by the sprint check)
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
#define CTCTargeted CTCTargetedLayout
#include "engine/CTCTargeted.h"
#undef CTCTargeted

class CTCTargeted : public CTCTargetedLayout
{
public:
    void SetTargetingFlags(unsigned long);   // 0x007EA270
};
class CTCTargetedMethods : public CTCTargeted {         // interface 9 (the methods are declared on a derived name: the header is data-only)
public:
};
namespace NCreatureMode {
unsigned long __fastcall GetOldTargetFlagsAndReplaceWith(CThing& thing, ETargetingType type);
}

unsigned long __fastcall NCreatureMode::GetOldTargetFlagsAndReplaceWith(CThing& thing, ETargetingType type)
{
    if (thing.HasTC(TCI_TARGETED)) {
        CTCTargetedMethods* targeted = (CTCTargetedMethods*)((CTCTargetedMethods*)&thing.GetTC(TCI_TARGETED));
        unsigned long old = targeted->TargetingFlags;
        targeted->SetTargetingFlags(type);
        return old;
    }
    return 0;
}