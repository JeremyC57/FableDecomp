// CTCFireballSpell::FinalCleanup() -- stop the parent's fireball build-up loop sound, kill the build-up effect while it lives, then release the parent's special-abilities lock if ability 16 (fireball) is running
// Leaf: ?FinalCleanup@CTCFireballSpell@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C90A0, 185 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?FinalCleanup@CTCFireballSpell@@AAEXXZ (private void()); its only references are the
//   CTCFireballSpell vtable thunks (vtable 0x0126E964) 0x007C9160 (slot 14, OnKill) and 0x007C9170 (slot 2, OnRemove) that
//   tail-jump here (the ability-TC pattern of the landed CTCWillLightning::FinalCleanup 0x007A0950). Label correction: the
//   landed 0x007C9170 is catalogued as FinalCleanup (it is OnRemove), 0x007C9160 as vfunc_14 (OnKill).
// Model: the landed CTCFireballSpell::CancelMove 0x007C8FD0 (same members, same callees).
// Callees: CVectorMap LowerBound 0x0040F020 (inline GetInterface: 0x10 CTCSoundPlayer, 0xA7 CTCSpecialAbilities),
//   CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (inlined
//   IS_THING_ALIVE, then the Kill target), CThing::Kill(bool) 0x004C9B80.
// Layout: CTCFireballSpell InitLoopingSound +0x44, FireballBuildUpEffect +0x48 (retail, 4 below Ego_r after the timer).
// Shape: ParentThing is re-read from the member for each lookup; the lock release stores from one zero register.
class CThing;
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CTCSpecialAbilities {                         // interface 0xA7
public:
    unsigned char _pad00[0x0c];
    bool AbilityMutexLock;                          // +0x0C
    bool UsingBed;                                  // +0x0D
    long CurrentAbility;                            // +0x10 EHeroAbility
};

class CTCSoundPlayer {                              // interface 0x10
public:
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};

class CTCFireballSpell {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x44 - 0x08];
    unsigned long InitLoopingSound;                 // +0x44
    CIntelligentPointer<CThing> FireballBuildUpEffect;  // +0x48 CIntelligentPointer<CThing>
private:
    void FinalCleanup();
};

void CTCFireballSpell::FinalCleanup()
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &sound))
        sound->StopAttachedSound(InitLoopingSound);
    if (IS_THING_ALIVE(FireballBuildUpEffect))
        FireballBuildUpEffect.GetPItem()->Kill(true);
    CTCSpecialAbilities* abilities;
    if (ParentThing->GetTC(TCI_SPECIAL_ABILITIES, &abilities) && abilities->CurrentAbility == 0x10) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
}