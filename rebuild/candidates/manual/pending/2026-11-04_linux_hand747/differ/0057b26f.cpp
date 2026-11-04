// CTCHeroStats::ForceStaminaTo(float fraction) -- sets the stamina to a [0,1] fraction of the maximum (rounded to nearest), restarts the refill timer and cancels the shield spell at zero
// Grade: DIFFER(156v156) -- best a dl=2: GetInterface key packed into [ebp-8] (helper result slot) not the dead fraction slot [ebp+8] (best try n; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?ForceStaminaTo@CTCHeroStats@@QAEXM@Z
// Flags: none
// Retail-asm: 0x0057B299 `fld dword ptr [ebp+8]` + 0x0057B29C `fistp dword ptr [ebp-8]` -- the engine's inline-assembly float-to-long
//   GFFloatToLongNear (shared engine/GFFloatToLongNear.h): `MaxStamina * fraction` stored back to the parameter slot, reloaded and
//   stored with a bare dword fistp. VC7.1 emits no bare dword fistp from C++ (`(long)x` = __ftol2, /QIfist = `fistp qword`). This row
//   carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057B26F, 156 bytes, thiscall `ret 4`; frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`),
//   the sibling of the landed CTCHeroStats::ModifyStaminaBy 0x0057B1DB (same model).
// Identity: Ego_r.pdb / FableWin.pdb ?ForceStaminaTo@CTCHeroStats@@QAEXM@Z.
// Callees: GFLimit<float,float,float> 0x0048DAE0 (out of line here: Ego_r ??$GFLimit@MMM@@YIXAAMABM1@Z; the reloc binding's non-template
//   ?GFLimit@@YIXAAMABM1@Z is wrong, NAME_FIXES.tsv), CTCBase::GetDefinitionManager 0x006869D0, __ftol2, std::bitset<274>::test 0x00410DE0,
//   CVectorMap::LowerBound 0x0040F020, CTCPhysicalShieldSpell::CancelMove 0x0079E300 (landed spellings of ModifyStaminaBy).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/GFFloatToLongNear.h"

class CTCBase;

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

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
    std::bitset<274> m_present;                         // +0x20
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

template <class T, class L, class H> void __fastcall GFLimit(T& value, const L& lo, const H& hi);   // 0x0048DAE0 ??$GFLimit@MMM@@YIXAAMABM1@Z
void CTCHeroStats::ForceStaminaTo(float fraction)
{
    GFLimit(fraction, 0.0f, 1.0f);
    long stamina = GFFloatToLongNear(fraction *= MaxStamina);
    Stamina = stamina;
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * CMainGameComponent::GetConstantFPS());
    if (Stamina == 0) {
        CTCPhysicalShieldSpell* listener;
        if (ParentThing->GetInterface(&listener, 0x93))
            listener->CancelMove();
    }
}
