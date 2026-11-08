// CTCPhysicalShieldSpell::FinalCleanup() -- end of the physical-shield power: clear this spell's mesh effect from the caster's graphic appearance, mark the spell unpausable and stop its looping sound
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses; FinalCleanup is protected as in Ego_r.pdb
// Leaf: ?FinalCleanup@CTCPhysicalShieldSpell@@IAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079EAA0, 138 bytes (+ int3 padding to 144), __fastcall, plain `ret`.
// Identity: CTCPhysicalShieldSpell's OnRemove / OnKill vtable slots are `jmp` stubs 0x0079EC80 / 0x0079EC70 to it (landed as
//   forwarders), as for CTCDivineWrath / CTCUnholyPower / CTCBulletTime; Ego_r.pdb `?FinalCleanup@CTCPhysicalShieldSpell@@IAEXXZ`.
//   The catalog's "CThing_OnDestroy" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetInterface: 0x5B = CTCGraphicAppearance,
//   bitset word +0x28 bit 27; 0x10 = the sound player, word +0x20 bit 16), CTCGraphicAppearance::ClearMeshEffect(void* key)
//   0x004BFFC0 (key = this, as CTCMultiArrow::RemoveEffectOnWeapon), CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0.
// Layout: Ego_r.pdb = retail: LoopingSound +0x40, Pausable +0x44.
// Shape: the appearance through the out-parameter GetInterface; `Pausable = false` then the inline StopSound(LoopingSound)
//   (the sound id is read into ebx before the flag store, the lookup re-reads the caster); the out-of-line StopSound at
//   0x0079DDB0 (called by the landed Pause) is inlined here.
#include "fable_compiler.h"
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* found = map->LowerBound((const ETCInterfaceType&)type);
            CInterfaceMap::value_type* it;
            if (found != map->end() && !(interfaceType < found->first))
                it = found;
            else
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCGraphicAppearance {                             // interface 0x5B
public:
    void ClearMeshEffect(void* key);                     // 0x004BFFC0
};

class CTCSoundPlayer {                                   // interface 0x10
public:
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};

class CTCPhysicalShieldSpell {
public:
    virtual ~CTCPhysicalShieldSpell();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];
    unsigned long LoopingSound;                          // +0x40
    bool          Pausable;                              // +0x44
    bool          Paused;                                // +0x45
    bool          CutSceneCancelled;                     // +0x46

    FABLE_FORCEINLINE void StopSound(unsigned long sound)    // out of line at 0x0079DDB0
    {
        CTCSoundPlayer* player;
        if (ParentThing->GetInterface(&player, 0x10))
            player->StopAttachedSound(sound);
    }
protected:
    void FinalCleanup();
};

void CTCPhysicalShieldSpell::FinalCleanup()
{
    CTCGraphicAppearance* appearance;
    if (ParentThing->GetInterface(&appearance, 0x5b))
        appearance->ClearMeshEffect(this);
    Pausable = false;
    StopSound(LoopingSound);
}