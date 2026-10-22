// CTCInventoryExperience vtable slot 89 (bool undo) -- with `undo`, give back every undoable stat and ability purchase
// (drop the bought levels, re-upgrade them so the derived values rebuild, refund the spent experience into the hero
// stats' totals), clear both undo maps and mark the spending screen as opened; then restore the previous hero screen,
// close the base inventory, delete every UI component, and reset the UI manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057770A, 445 bytes, `ret 4`, no frame pointer (size TU: `pop ecx` after
// the cdecl tree increment).
// Shape: the two undo loops walk their maps through ONE raw `SRbNode* node` variable declared before the first loop (per-loop
// typed views `SUndoNode<K>* it = (SUndoNode<K>*)node;`): VC7.1 keeps a named variable in one register across its webs, so
// the ability loop inherits the stat loop's assignment (iterator esi, counter/spent edi). Separate per-loop iterator objects
// (or per-loop node pointers) let the allocator colour the second loop the other way round (448v448, registers swapped).
// Callees: the interface getters 0x0056FF3D (0x68 CTCHeroExperience) / 0x0056FF81 (4 CTCHeroStats) / 0x0056FFC5 (0x6F
// hero abilities), CTCHeroExperience::ModifyTrainableStatLevel 0x006D6810 / UpgradeTrainableStat 0x006D8980, the
// abilities' GetAbilityLevel 0x005EDE93 / SetAbilityLevel 0x005EE01A (label CEngineLandscapeRenderer::...) /
// UpgradeAbility 0x005EEEE6, _Rb_tree_increment 0x004292C0, the two undo-map clears 0x00579EA3 / 0x00579ECC (labels
// CTCCarryable::OnKill), the player GUI's hero-screens getter 0x00437CD0 and its screen-type setter 0x0066E799,
// CTCInventoryBase::OnInventoryClosed 0x005BD597, vector<CComponent*>::erase(first, last) 0x00485AA7, NUISystem::CManager::
// GetInstance 0x0041E5F2 (virtual slots 6, 29, 37), the global list clear 0x004855A3 on 0x013B8A98.
// Layout: retail CTCInventoryBase Components +0x0C; CTCInventoryExperience UndoableStatBuys +0x140, UndoableAbilityBuys
// +0x14C, OpenedExperienceSpending +0x1A0, PreviousScreenType +0x220 (landed ctor 0x0057832D); SExpSpendUndo with 4-byte
// lists (Levels +0x10, General..Will +0x14..+0x20 in the value, node +0x24..+0x34); CTCHeroStats total +0x13C and
// ExperienceSpentOn +0x18 (retail).
#pragma optimize("s", on)

struct SRbNode {
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
};
extern "C" SRbNode* __cdecl _Rb_tree_increment(SRbNode* node);   // 0x004292C0
struct SLongList { void* _M_node; };
struct SExpSpendUndo {
    SLongList Generals, Strengths, Skills, Wills;
    long Levels;                                             // +0x10
    long General;                                            // +0x14
    long Strength;                                           // +0x18
    long Skill;                                              // +0x1C
    long Will;                                               // +0x20
};
template <class K> struct SUndoNode : SRbNode {
    K             first;                                     // +0x10
    SExpSpendUndo second;                                    // +0x14
};
template <class K> struct CUndoIterator {
    SUndoNode<K>* _M_node;
    SUndoNode<K>* operator->() const { return _M_node; }
    bool operator!=(const CUndoIterator& o) const { return _M_node != o._M_node; }
    CUndoIterator& operator++() { _M_node = (SUndoNode<K>*)_Rb_tree_increment(_M_node); return *this; }
};
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
struct CStatUndoMap {
    SUndoNode<EHeroTrainableStatType>* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    CUndoIterator<EHeroTrainableStatType> begin() { CUndoIterator<EHeroTrainableStatType> it; it._M_node = (SUndoNode<EHeroTrainableStatType>*)_M_header->_M_left; return it; }
    CUndoIterator<EHeroTrainableStatType> end() { CUndoIterator<EHeroTrainableStatType> it; it._M_node = _M_header; return it; }
    void clear();                                            // 0x00579EA3
};
struct CAbilityUndoMap {
    SUndoNode<EHeroAbility>* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    CUndoIterator<EHeroAbility> begin() { CUndoIterator<EHeroAbility> it; it._M_node = (SUndoNode<EHeroAbility>*)_M_header->_M_left; return it; }
    CUndoIterator<EHeroAbility> end() { CUndoIterator<EHeroAbility> it; it._M_node = _M_header; return it; }
    void clear();                                            // 0x00579ECC
};

struct CTCHeroExperience {
    void ModifyTrainableStatLevel(EHeroTrainableStatType stat, long delta);   // 0x006D6810
    void UpgradeTrainableStat(EHeroTrainableStatType stat);  // 0x006D8980
};
struct CTCHeroStats {
    unsigned char _pad00[0x18];
    long* ExperienceSpentOn;                                 // +0x18 vector<long>
    unsigned char _pad1c[0x13c - 0x1c];
    long ExperienceSpent;                                    // +0x13C
    void AddExperienceSpent(long amount) { ExperienceSpent += amount; }
};
struct CTCHeroAbilities {
    long GetAbilityLevel(EHeroAbility ability);              // 0x005EDE93
    void SetAbilityLevel(EHeroAbility ability, long level);  // 0x005EE01A
    void UpgradeAbility(EHeroAbility ability);               // 0x005EEEE6
};

namespace NUISystem {
struct CComponent { virtual ~CComponent(); };
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05();
    virtual void ResetA();                                   // slot 6 (+0x18)
    virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12();
    virtual void m13(); virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18();
    virtual void m19(); virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28();
    virtual void ResetB();                                   // slot 29 (+0x74)
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34(); virtual void m35();
    virtual void m36();
    virtual void ResetC();                                   // slot 37 (+0x94)
    static CManager* GetInstance();                          // 0x0041E5F2
};
}
struct CGlobalList { void clear(); };                        // 0x004855A3
extern CGlobalList g_UIList;                                 // 0x013B8A98
struct CHeroInfoScreens { void SetScreenType(long type); };  // 0x0066E799
struct CPlayerGui { CHeroInfoScreens* GetHeroInfoScreens(); };   // 0x00437CD0
extern CPlayerGui* g_pPlayerGui;                             // 0x013B8790

struct CComponentVector {                                    // vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return _M_start[n]; }
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    NUISystem::CComponent** erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(begin(), end()); }
};

struct CTCInventoryBase {
    void*            __vftable;                              // +0x00
    void*            ParentThing;                            // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                             // +0x0C
    void OnInventoryClosed();                                // 0x005BD597
};
struct CTCInventoryExperience : CTCInventoryBase {
    unsigned char   _pad18[0x140 - 0x18];
    CStatUndoMap    UndoableStatBuys;                        // +0x140
    CAbilityUndoMap UndoableAbilityBuys;                     // +0x14C
    unsigned char   _pad158[0x1a0 - 0x158];
    bool            OpenedExperienceSpending;                // +0x1A0
    unsigned char   _pad1a1[0x220 - 0x1a1];
    long            PreviousScreenType;                      // +0x220
    CTCHeroExperience* GetTCHeroExperience();                // 0x0056FF3D
    CTCHeroStats*      GetTCHeroStats();                     // 0x0056FF81
    CTCHeroAbilities*  GetTCHeroAbilities();                 // 0x0056FFC5
    void vfunc_89(bool undo);
};

void CTCInventoryExperience::vfunc_89(bool undo)
{
    if (undo) {
        SRbNode* node;
        for (node = UndoableStatBuys._M_header->_M_left; node != UndoableStatBuys._M_header; node = _Rb_tree_increment(node)) {
            SUndoNode<EHeroTrainableStatType>* it = (SUndoNode<EHeroTrainableStatType>*)node;
            GetTCHeroExperience()->ModifyTrainableStatLevel(it->first, -it->second.Levels);
            for (long i = 0; i != it->second.Levels; ++i)
                GetTCHeroExperience()->UpgradeTrainableStat(it->first);
            long spent = it->second.Will + it->second.Skill + it->second.Strength + it->second.General;
            GetTCHeroStats()->AddExperienceSpent(spent);
            GetTCHeroStats()->ExperienceSpentOn[it->first] += spent;
        }
        for (node = UndoableAbilityBuys._M_header->_M_left; node != UndoableAbilityBuys._M_header; node = _Rb_tree_increment(node)) {
            SUndoNode<EHeroAbility>* it = (SUndoNode<EHeroAbility>*)node;
            GetTCHeroAbilities()->SetAbilityLevel(it->first, GetTCHeroAbilities()->GetAbilityLevel(it->first) - it->second.Levels);
            for (long i = 0; i != it->second.Levels; ++i)
                GetTCHeroAbilities()->UpgradeAbility(it->first);
            long spent = it->second.Will + it->second.Skill + it->second.Strength + it->second.General;
            GetTCHeroStats()->AddExperienceSpent(spent);
            GetTCHeroStats()->ExperienceSpentOn[10] += spent;
        }
        UndoableStatBuys.clear();
        UndoableAbilityBuys.clear();
        OpenedExperienceSpending = true;
    }
    g_pPlayerGui->GetHeroInfoScreens()->SetScreenType(PreviousScreenType);
    OnInventoryClosed();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ResetA();
    NUISystem::CManager::GetInstance()->ResetB();
    g_UIList.clear();
    NUISystem::CManager::GetInstance()->ResetC();
}