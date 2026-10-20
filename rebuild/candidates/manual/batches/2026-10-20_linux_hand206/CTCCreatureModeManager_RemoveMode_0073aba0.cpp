// CTCCreatureModeManager::RemoveMode(unsigned long modeType) -- remove every mode of the type; when the current (front) mode was one of them it is told it is leaving and the new front mode is resumed (and the thing's look component re-picks its turning anims); the next idle mode is due in 30 seconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073ABA0, 170 bytes (`ret 4`).
// Callees: the mode's virtual slots 1 GetModeType, 6 OnLeave (+0x18, as the landed ClearModes 0x0073AE70) and 5 (+0x14, the
// resume hook; named descriptively), list<CCountedPointer<CModeBase>>::remove_if<CModeBase::CIsType> @ 0x0073E790 (the predicate
// by value: one dword), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCLook::UpdateTurningAnims @ 0x00754050,
// __ftol2 @ 0x00BFEA70, CWorld::GetWorldFrame() @ 0x0049D870 (static). Globals: g_framesPerSecond @ 0x01375550, 30.0f @ 0x012673C4.
// Layout: Ego_r.pdb CTCCreatureModeManager with the retail 4-byte STLport list: PModes +0x0c (sentinel node pointer; node
// {next, prev, CCountedPointer {object, block}}), TimeLeftForCurrentMode +0x10, TimeForNextIdleMode +0x14 (PDB +0x1c); CThing
// look component = interface 0x43 (bit 3 of the +0x28 word).
// Shape: the "was current" bool is preset false (bl); the list address is formed once (`lea esi`) for the front reads and the
// remove_if call; the idle delay `(long)(fps * 30.0f)` is a named local computed before GetWorldFrame (written inline in the sum
// VC7.1 calls GetWorldFrame first and folds the add into a `sub` of the negated product).
extern long g_framesPerSecond;                          // 0x01375550
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CWorld {
    static long GetWorldFrame();                        // 0x0049D870
};
struct CTCLook {
    void UpdateTurningAnims();                          // 0x00754050
};
struct CModeBase {                                      // CTCCreatureModeManager::CModeBase
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnResume();                            // slot 5 (+0x14)
    virtual void OnLeave();                             // slot 6 (+0x18)
    struct CIsType {                                    // remove_if predicate
        unsigned long Type;
        CIsType(unsigned long type) : Type(type) { }
    };
};
struct SCountBlock;
struct CModePointer {                                   // CCountedPointer<CModeBase>
    CModeBase*   m_pObject;
    SCountBlock* m_pCount;
    CModeBase* operator->() const { return m_pObject; }
};
struct SModeNode {
    SModeNode*   m_pNext;
    SModeNode*   m_pPrev;
    CModePointer m_data;                                // +0x08
};
struct CModeList {                                      // retail STLport list: the sentinel node pointer
    SModeNode* m_pHead;
    bool empty() const { return m_pHead->m_pNext == m_pHead; }
    CModePointer& front() { return m_pHead->m_pNext->m_data; }
    void remove_if(CModeBase::CIsType predicate);       // 0x0073E790
};
struct CTCCreatureModeManager {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CModeList PModes;                                   // +0x0c
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    void RemoveMode(unsigned long modeType);
};

void CTCCreatureModeManager::RemoveMode(unsigned long modeType)
{
    bool removingCurrent = false;
    if (!PModes.empty() && PModes.front()->GetModeType() == modeType) {
        removingCurrent = true;
        PModes.front()->OnLeave();
    }
    PModes.remove_if(CModeBase::CIsType(modeType));
    if (removingCurrent && !PModes.empty()) {
        PModes.front()->OnResume();
        if (ParentThing->HasTC(0x43))
            ((CTCLook*)ParentThing->GetTC(0x43))->UpdateTurningAnims();
    }
    long idleDelay = (long)(g_framesPerSecond * 30.0f);
    TimeForNextIdleMode = CWorld::GetWorldFrame() + idleDelay;
}
