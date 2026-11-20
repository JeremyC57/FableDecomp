// NPlayerGui::CDrawMeleeActionButtonEffects::Update() -- pick the hero's current melee ability def (the flourish ability when a flourish is valid, else the block-counter ability when a block counter is valid), restart the flash timers when it changes, clear the effects when there is none, and count the timers down
// Leaf: Update
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064BE45, length 373 bytes (frame pointer, `ret`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::PeekPControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0
// (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCCombat::
// IsMeleeFlourishValid @ 0x00733EE0, ::GetPMeleeFlourishAbility @ 0x00733480 (CDefPointer by hidden pointer), ::IsMeleeBlockCounterValid
// @ 0x00733290, ::GetPMeleeBlockCounterAbility @ 0x007334B0 (thiscall on the CTCCombat component, CDefPointer by hidden pointer; all four
// Ego_r.pdb / FableWin.pdb const members), ClearEffects @ 0x006445BA (thiscall on the Effects vector), the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CDrawMeleeActionButtonEffects minus 4 (the 16-byte PDB vector is 12 in retail): PlayerManager +0x08, Effects +0x28,
// FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, FlourishEffect +0x40, PMeleeAbilityDef +0x44 (Ego_r.pdb member
// names, offsets minus 8 after the two 16-byte PDB vectors; FableWin.pdb ?SetFlourishEffect@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXJ@Z
// writes FlourishEffect; name-fix pass 2026-11-12, were the descriptive names EffectCount (+0x40) and PCurrentAbility (+0x44)). CTCCombat = interface 0x30.
// Shape: frame-pointer size TU; IS_THING_ALIVE (out of line in retail) has a same-TU read-only stand-in body so the hero's frame word
// does not escape and is reused for the lookup key and the second temporary; the component comes from the out-param GetInterface
// (the type constant 0x30 is held in ebx); both getters are called on the CTCCombat component; the chosen def is a CDefPointer local
// assigned from each getter's full-expression temporary (engine/CDefPointer.h operator=: the local is known null, so only the AddRef
// copy remains); the temporaries and the member release through the shared CDefPointer (clear only inside the non-null arm).
// Naming (NF138, 2026-11-20; q4747): the local CDefPointer / CAbilityDef / CRefCounted / CDefVirtual models replaced by engine/CDefPointer.h
//   (CDefPointer<const CMeleeCombatAbilityDef>, Ego_r.pdb PMeleeAbilityDef type); the CTCCombat declarations respelled in place to the
//   PDB members: GetPMeleeFlourishAbility QAE CDefPointer<CAbilityDef> -> QBE CDefPointer<const CMeleeCombatAbilityDef>, the stand-ins
//   IsShowingBaseAbility / GetPBaseAbility -> IsMeleeBlockCounterValid / GetPMeleeBlockCounterAbility, IsMeleeFlourishValid QAE -> QBE.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
// agentNF16: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
const CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key) const
{
    const value_type* it = _M_start;
    int len = int(_M_finish - _M_start);
    while (len > 0) {
        int half = len / 2;
        const value_type* middle = it + half;
        if (middle->first < key) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    mutable std::bitset<274> m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);          // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;               // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMeleeCombatAbilityDef;
class CTCCombat { public:
    bool IsMeleeFlourishValid() const;                                              // 0x00733EE0
    CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeFlourishAbility() const;     // 0x00733480
    bool IsMeleeBlockCounterValid() const;                                          // 0x00733290
    CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeBlockCounterAbility() const; // 0x007334B0
};
struct CEffectVector {
    void ClearEffects();                           // 0x006445BA
};
struct CDrawMeleeActionButtonEffects {
    unsigned char            _pad0[0x08];
    CPlayerManager*          PlayerManager;        // +0x08
    unsigned char            _pad1[0x28 - 0x0c];
    CEffectVector            Effects;              // +0x28
    unsigned char            _pad2[0x34 - 0x28 - sizeof(CEffectVector)];
    long                     FlashTimer;           // +0x34
    long                     ComboLoadingIconTimer;// +0x38
    long                     FlourishCounter;      // +0x3c
    long                     FlourishEffect;          // +0x40
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef; // +0x44
    void Update();
};

void CDrawMeleeActionButtonEffects::Update()
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    CDefPointer<const CMeleeCombatAbilityDef> ability;
    if (IS_THING_ALIVE(hero)) {
        CTCCombat* combat;
        if (hero->GetInterface(&combat, 0x30)) {
            if (combat->IsMeleeFlourishValid()) {
                ability = combat->GetPMeleeFlourishAbility();
            }
            else if (combat->IsMeleeBlockCounterValid()) {
                ability = combat->GetPMeleeBlockCounterAbility();
            }
            if (ability.Object != PMeleeAbilityDef.Object) {
                ComboLoadingIconTimer = -1;
                FlashTimer = 15;
            }
        }
    }
    PMeleeAbilityDef = ability;
    if (FlashTimer == 0)
        ComboLoadingIconTimer = 30;
    if (!ability.Object) {
        Effects.ClearEffects();
        FlourishCounter = -1;
        FlourishEffect = 0;
        ComboLoadingIconTimer = -1;
        FlashTimer = -1;
    }
    if (ComboLoadingIconTimer > 0)
        --ComboLoadingIconTimer;
    if (FlashTimer > 0)
        --FlashTimer;
}