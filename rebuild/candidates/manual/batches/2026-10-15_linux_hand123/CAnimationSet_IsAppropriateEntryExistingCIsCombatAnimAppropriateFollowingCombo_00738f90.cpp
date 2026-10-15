// CAnimationSet::IsAppropriateEntryExisting<CIsCombatAnimAppropriateFollowingCombo>(const CIsCombatAnimAppropriateFollowingCombo& predicate) const -- true when some entry of the set is a valid follow-up of the current combat combo (same combo id, next stage, or the "finisher" stage while the chain still has room)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00738F90, 99 bytes, __thiscall, ret 4, returns bool.
// Leaf: IsAppropriateEntryExisting
// Callee: CAnimationEntry::GetComponent<CAnimComponentCombatComboChain>(T** out) 0x00738980 (out of line,
// component type 4; labelled GetTCInterface_LowerBound).
// Layout: Ego_r.pdb -- CAnimationSet::Anims is a CVectorMap<unsigned long, CAnimationEntry> (a retail
// vector of 0x14-byte {key, CAnimationEntry} pairs: begin +0, end +4); CIsCombatAnimAppropriateFollowingCombo
// is {CombatAnimCombo, CurrentChainPos, MaxComboStage}; CAnimComponentCombatComboChain
// (engine/CAnimComponentCombatComboChain.h) holds ComboStage +8 and ComboID +0x0C.
// Shape: the predicate is passed by reference (kept in edi) and its operator() is inlined; its component
// out-pointer local is homed in the dead predicate argument slot (`lea eax,[esp+10h]`). The end
// pointer is a local taken before the loop (ebx); the walk tests the empty case first. ComboID -999
// marks "no combo"; stage -888 is the open finisher stage, allowed while CurrentChainPos <= MaxComboStage.
struct CAnimComponentCombatComboChain {
    void* m_pVtable;                       // +0x00
    unsigned long IVRefCount;              // +0x04
    long ComboStage;                       // +0x08
    long ComboID;                          // +0x0C
};

enum {
    COMBO_ID_NONE = -999,
    COMBO_STAGE_FINISHER = -888
};

struct CDefString { long TablePos; };

struct CAnimationEntry {
    long BankIndex;                        // +0x00
    CDefString AnimName;                   // +0x04
    CDefString GroupName;                  // +0x08
    void* Components;                      // +0x0C CIVCountedPointer<CAnimComponentMap>
    template <class T> void GetComponent(T** out) const;   // <CAnimComponentCombatComboChain>: 0x00738980
};

struct CIsCombatAnimAppropriateFollowingCombo {
    CAnimComponentCombatComboChain* CombatAnimCombo;   // +0
    long CurrentChainPos;                               // +4
    long MaxComboStage;                                 // +8
    bool operator()(const CAnimationEntry& entry) const
    {
        CAnimComponentCombatComboChain* combo;
        entry.GetComponent(&combo);
        if (combo->ComboID == COMBO_ID_NONE || combo->ComboID != CombatAnimCombo->ComboID)
            return false;
        if (combo->ComboStage == CombatAnimCombo->ComboStage + 1)
            return true;
        if (CurrentChainPos <= MaxComboStage && combo->ComboStage == COMBO_STAGE_FINISHER)
            return true;
        return false;
    }
};

struct CAnimationSetEntry {                // pair<unsigned long, CAnimationEntry>
    unsigned long first;
    CAnimationEntry second;
};

struct CAnimationSet {
    CAnimationSetEntry* m_pFirst;          // +0x00 Anims (CVectorMap)
    CAnimationSetEntry* m_pLast;           // +0x04
    CAnimationSetEntry* m_pEnd;            // +0x08
    long m_compare;                        // +0x0C
    long m_10;                             // +0x10
    long DefaultFlags;                     // +0x14
    template <class P> bool IsAppropriateEntryExisting(const P& predicate) const
    {
        CAnimationSetEntry* end = m_pLast;
        for (CAnimationSetEntry* it = m_pFirst; it != end; ++it)
            if (predicate(it->second))
                return true;
        return false;
    }
};

template bool CAnimationSet::IsAppropriateEntryExisting<CIsCombatAnimAppropriateFollowingCombo>(const CIsCombatAnimAppropriateFollowingCombo&) const;
