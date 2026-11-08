// CTCHeroStats::ModifyStaminaBy(long) -- adds to the stamina (clamped to [0,max]), restarts the refill timer and cancels the shield spell when it runs out
// Leaf: ModifyStaminaBy
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Clamp: the landed AddMoney 0x0057B327 form, `GFLimit(Stamina, 0L, MaxStamina)` through the __forceinline
// GFLimit(T&, const T&, const T&) template (bounds by reference): retail's stamina-in-eax / MaxStamina-in-ecx colouring.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B1DB, 148 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC the CHeroStatsDef pointer; def +0x40 the stamina refill
//          delay in seconds), __ftol2 @ 0x00BFEA70, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
//          @ 0x0040F020, and the interface-0x93 component's routine @ 0x0079E300 (manifest
//          "CTCPhysicalShieldSpell::CancelMove", propagated; called without a null test).
// Globals: the game's frames-per-second long @ 0x01375550 (`fild`).
// Shape: InfiniteStamina blocks only negative changes (`cmp byte ; je ; test ; jl`); `Stamina += delta` is a
// read-modify-write add whose flags feed the `jns` of the `< 0` test, the upper clamp reads MaxStamina into ecx;
// the refill frames are `(long)(def->delay * fps)` (`fild fps ; fmul [def+40h]`). When the stamina is exhausted the
// parent thing's inline GetInterface(&c, 0x93) keeps the type in edi (`mov edi,93h`) and the lookup key in the dead
// parameter slot. The def is read through an inline GetHeroStatsDef() accessor.
// Layout: retail Stamina +0x58, MaxStamina +0x5C, FramesUntilStaminaRefills +0x60, InfiniteStamina +0x16C
// (PDB +0x60/+0x64/+0x68/+0x184).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicalShieldSpell { public:                          // interface type 0x93
public:
    void CancelMove();   // 0x0079E300
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0x40];
    float         StaminaRefillDelaySeconds;         // +0x40
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCHeroStats : public CTCBase { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x50];
    long          Stamina;                           // +0x58 (PDB +0x60)
    long          MaxStamina;                        // +0x5C (PDB +0x64)
    long          FramesUntilStaminaRefills;         // +0x60 (PDB +0x68)
    unsigned char _pad_0x64[0x108];
    bool          InfiniteStamina;                   // +0x16C (PDB +0x184)

    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }
    void ModifyStaminaBy(long delta);
};

template <class T> FABLE_FORCEINLINE void GFLimit(T& value, const T& lo, const T& hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}
void CTCHeroStats::ModifyStaminaBy(long delta)
{
    if (InfiniteStamina && delta < 0)
        return;
    Stamina += delta;
    GFLimit(Stamina, 0L, MaxStamina);
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * CMainGameComponent::GetConstantFPS());
    if (Stamina == 0) {
        CTCPhysicalShieldSpell* listener;
        if (ParentThing->GetTC(TCI_PHYSICAL_SHIELD_SPELL, &listener))
            listener->CancelMove();
    }
}