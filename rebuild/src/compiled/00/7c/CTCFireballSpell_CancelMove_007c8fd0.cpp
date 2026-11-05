// CTCFireballSpell::CancelMove() -- abort the fireball: release the owner's special-ability lock if the fireball holds it (else do nothing), stop the build-up loop sound, kill the build-up effect and remove the spell component (0x9D)
// Leaf: ?CancelMove@CTCFireballSpell@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C8FD0, length 196 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?CancelMove@CTCFireballSpell@@QAEXXZ`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0xA7 CTCSpecialAbilities, word 5 bit 7:
//          `mov al,[edi+34h] ; test al,al ; jns`; interface 0x10 the sound player, word 0 bit 16), the sound player's
//          StopSample(unsigned long) @ 0x0066ADD0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE,
//          then the Kill target), CThing::Kill(bool) @ 0x004C9B80, CThing::RemoveTC(long) @ 0x004C9840.
// Shape: two out-parameter GetInterface lookups on the owner, a named local kept in edi (`this` in ebx); when another ability holds the special-ability
//        lock (CurrentAbility != 0x10, the fireball) the function returns at once, otherwise the lock is released with
//        one ecx zero.
// Layout: retail CTCFireballSpell sits 4 bytes below Ego_r.pdb after the timer: InitLoopingSound +0x44 (PDB +0x48),
//         FireballBuildUpEffect +0x48 (PDB +0x4C); CTCSpecialAbilities AbilityMutexLock +0x0C, CurrentAbility +0x10 (PDB).
class CThing;
class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
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
    void CancelMove();
};

void CTCFireballSpell::CancelMove()
{
    CThing* parent = ParentThing;
    CTCSpecialAbilities* abilities;
    if (parent->GetTC(TCI_SPECIAL_ABILITIES, &abilities)) {
        if (abilities->CurrentAbility != 0x10)
            return;
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    CTCSoundPlayer* sound;
    if (parent->GetTC(TCI_SOUND_PLAYER, &sound))
        sound->StopAttachedSound(InitLoopingSound);
    if (IS_THING_ALIVE(FireballBuildUpEffect))
        FireballBuildUpEffect.GetPItem()->Kill(true);
    parent->RemoveTC((ETCInterfaceType)(0x9d));
}