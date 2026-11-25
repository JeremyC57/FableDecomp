// CTCCombat::SetNextComboChainPos(const CMeleeCombatAbilityDef*) -- advances the combo chain to the ability's combo: when the ability's combo def carries a stage, the chain position is incremented (or started at 1) and the chain id taken from the combo; if that fails or no appropriate following combo exists the chain is reset (-777 / -999)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734570, length 94 bytes (+ int3 padding).
// Callees: the def's typed sub-def lookup @ 0x00738980 (a __fastcall member on the def class at +0x74
// writing the combo sub-def through its out pointer; manifest "GetTCInterface_LowerBound"),
// CTCCombat::HasAppropriateFollowingCombo(const CDefClassBase*) @ 0x00733950.
// Layout = engine/CTCCombat.h: ComboChainPos +0x0c (-777 = none), ComboChainID +0x10 (-999 = none). The
// ability def's def class at +0x74; the combo sub-def's stage +8 and chain id +0xc.
// Shape: the out pointer is a plain local that VC homes in the consumed argument slot
// (`lea ecx,[esp+0Ch]`); `pos == -777 ? 1 : pos + 1` is an if/else on the member; the reset tail is
// shared by the no-stage test and the failed follow-up test.
struct CComboDef {
    unsigned char _pad[8];
    long          Stage;                                    // +0x08 (-777 = none)
    long          ChainID;                                  // +0x0c
};

class CDefClassBase { public:
};

class CMeleeCombatAbilityDef { public:
    unsigned char  _pad[0x74];
    CDefClassBase* DefClass;                                // +0x74
};

#include "engine/CTCCombat.h"

struct CTCCombat_Methods : CTCCombat {
    bool HasAppropriateFollowingCombo(CDefClassBase* defClass);   // 0x00733950
    void SetNextComboChainPos(const CMeleeCombatAbilityDef* ability);
};

// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00738980 ??$GetComponent@VCAnimComponentCombatComboChain@@@CAnimationEntry@@QBEXPAPBVCAnimComponentCombatComboChain@@@Z (was ?GetComboDef@CDefClassBase@@QAEXPAPBUCComboDef@@@Z)
class CAnimComponentCombatComboChain;
class CAnimationEntry { public: template <class T1> void GetComponent(T1 const**) const; };
void CTCCombat_Methods::SetNextComboChainPos(const CMeleeCombatAbilityDef* ability)
{
    CDefClassBase* defClass = ability->DefClass;
    const CComboDef* combo;
    reinterpret_cast<const CAnimationEntry*>(defClass)->GetComponent<CAnimComponentCombatComboChain>(reinterpret_cast<CAnimComponentCombatComboChain const**>(&combo));
    if (combo->Stage != -777) {
        if (ComboChainPos == -777)
            ComboChainPos = 1;
        else
            ComboChainPos = ComboChainPos + 1;
        ComboChainID = combo->ChainID;
        if (HasAppropriateFollowingCombo(defClass))
            return;
    }
    ComboChainPos = -777;
    ComboChainID = -999;
}