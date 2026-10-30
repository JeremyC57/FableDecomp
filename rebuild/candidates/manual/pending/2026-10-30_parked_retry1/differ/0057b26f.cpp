// CTCHeroStats::ForceStaminaTo(float) -- sets the stamina to a [0,1] fraction of the maximum, restarts the refill timer and cancels the shield spell at zero
// Leaf: ForceStaminaTo
// Grade: DIFFER(156v156) retry1 -- retail_asm helper in place; residue: the lookup key's frame slot ([ebp-8] v retail [ebp+8])
// Retail-asm: 0x0057B299 `fld dword ptr [ebp+8]` + 0x0057B29C `fistp dword ptr [ebp-8]` -- the engine inline-assembly float-to-long helper on the homed fraction; VC7.1 C++ (long)x calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B26F, 156 bytes; frame-pointer size TU
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
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

extern long g_GameFramesPerSecond;                   // 0x01375550

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

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
    void ForceStaminaTo(float fraction);
};

template <class T> void __fastcall GFLimit(T& value, const T& lo, const T& hi);    // 0x0048DAE0 (float instance, out of line here)
// the engine's inline-assembly float-to-long helper: expanded in place, reads the homed value, writes a frame word
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
void CTCHeroStats::ForceStaminaTo(float fraction)
{
    GFLimit(fraction, 0.0f, 1.0f);
    fraction = MaxStamina * fraction;
    Stamina = FloatToLong(fraction);
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * g_GameFramesPerSecond);
    if (Stamina == 0) {
        CTCPhysicalShieldSpell* listener;
        if (ParentThing->GetInterface(&listener, 0x93))
            listener->CancelMove();
    }
}
