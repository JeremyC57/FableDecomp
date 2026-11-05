// CTCBulletTime::FinalCleanup() -- end of the bullet-time power: no longer pausable, drop this power's haste multiplier from the caster (recomputing its speed) and clear the caster's interface-0x46 flag
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses; FinalCleanup is private as in Ego_r.pdb
// Tried: out-parameter GetInterface (this file), HasTC + one-ternary GetTC for the closing lookup (149v144, `shr ; test dl,1`),
//   both component locals declared first. Next: the same gap as CTCBulletTime::Pause 0x0079CF30 (landing queue) -- try
//   an inline CTCBase::GetThing() accessor or a callee-side form that keeps `this` live.
// Leaf: ?FinalCleanup@CTCBulletTime@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079D2D0, 144 bytes, __fastcall, plain `ret`.
// Identity: CTCBulletTime's vtable 0x0126C49C slots 2 and 14 (OnRemove / OnKill) are `jmp` stubs 0x0079D7D0 / 0x0079D7C0 to
//   it, the same arrangement as CTCDivineWrath (slots -> 0x0078FAF0) and CTCUnholyPower (-> 0x00795090 FinalCleanup); Ego_r.pdb
//   `?FinalCleanup@CTCBulletTime@@AAEXXZ` (private). The catalog's "ETCInterface_OnUpdate" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (two inline GetInterface lookups: 0x8C = CTCHaste,
//   bitset word +0x30 bit 12; 0x46, byte +0x28 bit 6), CTCHaste's map<void*, float>::erase(const key&) 0x0058119B and
//   CTCHaste::RecalculateMultiplier 0x00800020 (the inline RemoveMultiplier; its out-of-line /Os copy is 0x0058148D).
// Layout: CTCBulletTime (Ego_r.pdb = retail here): Pausable +0x54; CTCHaste Multipliers +0x18; the 0x46 component's bool
//   +0x1c (the landed Pause 0x0079CF30 / ImmediateCancel 0x0079D020 clear it too; 0x46 is called CTCCarrying elsewhere).
// Shape: `Pausable = false` first; each component through the out-parameter GetInterface (the single frame word holds the
//   0x8C key, then the multiplier key `this` passed by reference, then the 0x46 key); the caster re-read for the second test.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CMultiplierMap {                                  // retail map<void*, float>
    void* m_head;
    unsigned long erase(void* const& key);               // 0x0058119B
};

class CTCHaste {                                         // interface 0x8C
public:
    unsigned char  _pad00[0x18];
    CMultiplierMap Multipliers;                          // +0x18
    void RecalculateMultiplier();                        // 0x00800020
    void RemoveMultiplier(void* source)
    {
        Multipliers.erase(source);
        RecalculateMultiplier();
    }
};

class CTCInterface46 {                                   // interface 0x46 component
public:
    unsigned char _pad00[0x1c];
    bool          m_flag1c;                              // +0x1c
};

class CTCBulletTime {
public:
    virtual ~CTCBulletTime();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    bool          Pausable;                              // +0x54
    bool          Paused;                                // +0x55
    bool          CutSceneCancel;                        // +0x56

private:
    void FinalCleanup();
};

void CTCBulletTime::FinalCleanup()
{
    Pausable = false;
    CTCHaste* haste;
    if (ParentThing->GetTC(TCI_HASTE, &haste))
        haste->RemoveMultiplier(this);
    CTCInterface46* component;
    if (ParentThing->GetTC(TCI_CARRYING, &component))
        component->m_flag1c = false;
}