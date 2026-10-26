// CTCCombat::HasAppropriateFollowingCombo(const CAnimationEntry* entry) -- true when the creature's combat animation set holds an animation that can follow this entry's combo stage in the current chain
//
// Retail 0x00733950, 144 bytes (`ret 4`). Callees: CAnimationEntry::GetComponent
// <CAnimComponentCombatComboChain>(T**) @ 0x00738980 (manifest "GetTCInterface_LowerBound"),
// CThingCreatureBase::GetCombatAnimationSet @ 0x00663260 (twice), STLport for_each<const
// pair<unsigned long,CAnimationEntry>*, CGetMaxComboStage> @ 0x00738F30 (`__fastcall`: hidden result
// in ecx, first in edx, last and the 8-byte functor on the stack), CAnimationSet::
// IsAppropriateEntryExisting<CIsCombatAnimAppropriateFollowingCombo> @ 0x00738F90 (landed).
// Layout = engine/CTCCombat.h: ComboChainPos +0x0c, ComboChainID +0x10 (-999 = none),
// ParentCreature +0x24; the combo component's ComboStage +8 / ComboID +0xc (landed 0x00738F90 model).
// Shape: the component comes from a `__forceinline` helper whose out pointer VC homes in the consumed
// argument slot, so the caller holds the value in ebp; the body is `if (combo->ComboID != NONE)
// {...} return false;` (separate `xor al,al` exit); the chain's max stage is an if/else on
// ComboChainID inside that block (the for_each result temporary then shares the frame with the
// 12-byte predicate), -777 and a missing chain both giving the shared `xor eax,eax`; the predicate
// {combo, chain pos, max stage} is a temporary passed by reference.
class CAnimComponentCombatComboChain { public:
    void*         m_pVtable;               // +0x00
    unsigned long IVRefCount;              // +0x04
    long          ComboStage;              // +0x08
    long          ComboID;                 // +0x0C
};

enum { COMBO_ID_NONE = -999, COMBO_STAGE_NONE = -777 };

class CAnimationEntry { public:
    long  BankIndex;                       // +0x00
    long  AnimName;                        // +0x04 CDefString
    long  GroupName;                       // +0x08 CDefString
    void* Components;                      // +0x0C CIVCountedPointer<CAnimComponentMap>
    void GetComponent(CAnimComponentCombatComboChain** out) const;   // 0x00738980
};

__forceinline CAnimComponentCombatComboChain* GetComboChain(const CAnimationEntry* entry)
{
    CAnimComponentCombatComboChain* combo;
    entry->GetComponent(&combo);
    return combo;
}

struct CAnimationSetEntry {                // pair<unsigned long, CAnimationEntry>
    unsigned long   first;
    CAnimationEntry second;
};

class CGetMaxComboStage { public:
    long MaxComboStage;                    // +0
    long ComboID;                          // +4
    CGetMaxComboStage(long comboID) : MaxComboStage(COMBO_STAGE_NONE), ComboID(comboID) {}
};

CGetMaxComboStage __fastcall for_each(const CAnimationSetEntry* first, const CAnimationSetEntry* last, CGetMaxComboStage f);   // 0x00738F30

class CIsCombatAnimAppropriateFollowingCombo { public:
    CAnimComponentCombatComboChain* CombatAnimCombo;   // +0
    long CurrentChainPos;                               // +4
    long MaxComboStage;                                 // +8
    CIsCombatAnimAppropriateFollowingCombo(CAnimComponentCombatComboChain* combo, long pos, long maxStage)
        : CombatAnimCombo(combo), CurrentChainPos(pos), MaxComboStage(maxStage) {}
};

class CAnimationSet { public:
    CAnimationSetEntry* m_pFirst;          // +0x00 Anims (CVectorMap)
    CAnimationSetEntry* m_pLast;           // +0x04
    CAnimationSetEntry* m_pEnd;            // +0x08
    bool IsAppropriateEntryExisting(const CIsCombatAnimAppropriateFollowingCombo& predicate) const;   // 0x00738F90
};

class CThingCreatureBase { public:
    CAnimationSet* GetCombatAnimationSet();     // 0x00663260
};

class CTCCombat { public:
    virtual void v00();
    long                obj;                    // +0x04
    unsigned char       _pad_0x08[0x4];         // +0x08
    long                ComboChainPos;          // +0x0c
    long                ComboChainID;           // +0x10
    long                ComboChainGracePeriodCountdown;   // +0x14
    long                HandednessGracePeriodCountdown;   // +0x18
    long                UnarmedCombatIdleCountdown;       // +0x1c
    void*               CombatManager;          // +0x20
    CThingCreatureBase* ParentCreature;         // +0x24

    bool HasAppropriateFollowingCombo(const CAnimationEntry* entry);
};

bool CTCCombat::HasAppropriateFollowingCombo(const CAnimationEntry* entry)
{
    CAnimComponentCombatComboChain* combo = GetComboChain(entry);
    if (combo->ComboID != COMBO_ID_NONE)
    {
        CAnimationSet* set = ParentCreature->GetCombatAnimationSet();
        long maxStage;
        if (ComboChainID == COMBO_ID_NONE)
            maxStage = 0;
        else
        {
            CAnimationSet* chainSet = ParentCreature->GetCombatAnimationSet();
            maxStage = for_each(chainSet->m_pFirst, chainSet->m_pLast, CGetMaxComboStage(ComboChainID)).MaxComboStage;
            if (maxStage == COMBO_STAGE_NONE)
                maxStage = 0;
        }
        return set->IsAppropriateEntryExisting(CIsCombatAnimAppropriateFollowingCombo(combo, ComboChainPos, maxStage));
    }
    return false;
}