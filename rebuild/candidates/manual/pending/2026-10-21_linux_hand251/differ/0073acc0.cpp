// Grade: DIFFER(331v331) -- scheduling only: retail materialises the early 'return false' (xor al,al) right after the argument's release, before the pops; ours schedules it between pop ebp and pop ebx like the true path's mov al,1
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase> mode) -- refuse a mode whose type is already queued; otherwise remember the current (front) mode's type when it wants replacing, tell it it is leaving, append the new mode, resume the front mode, re-pick the look component's turning anims, schedule the next idle mode in 30 seconds and finally drop the replaced type
// Leaf: AddMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073ACC0, 336 bytes (`ret 8`: the counted pointer by value, destroyed by the callee).
// Callees: the mode's virtual slots 1 GetModeType, 5 OnResume (+0x14), 6 OnLeave (+0x18) and 11 (+0x2c, "replace me when another
// mode is added"; named descriptively), list<CCountedPointer<CModeBase>>::push_back @ 0x007421C0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCLook::UpdateTurningAnims @ 0x00754050, __ftol2 @ 0x00BFEA70, CWorld::GetWorldFrame() @
// 0x0049D870 (static), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator delete @ 0x00BFE9BC (count block).
// Globals: g_framesPerSecond @ 0x01375550, 30.0f @ 0x012673C4.
// Layout: as the landed RemoveMode 0x0073ABA0 (Ego_r.pdb CTCCreatureModeManager with the retail 4-byte STLport list PModes +0x0c,
// TimeForNextIdleMode +0x14; node {next, prev, CCountedPointer {object, block}}; the count block {references, destroy fn (object in
// ecx), object}).
// Shape: an inline find over the list (end held in a register), the "already queued" path releases the argument and returns false;
// the replaced type is preset 0 and RemoveMode runs after the idle time is stored.
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
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsReplacedByNewMode();                 // slot 11 (+0x2c)
};
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4 (object in ecx)
    void* m_pObject;                                    // +8
};
void operator delete(void*);
struct CModePointer {                                   // CCountedPointer<CModeBase>
    CModeBase*   m_pObject;
    SCountBlock* m_pCount;
    CModeBase* operator->() const { return m_pObject; }
    CModePointer(const CModePointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CModePointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
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
    void push_back(const CModePointer& x);              // 0x007421C0
};
inline void* operator new(unsigned int, void* where) { return where; }
SModeNode* AllocateModeNode(CModeList* list);           // stand-in: the node allocation and link-in
#pragma auto_inline(off)
// Same-TU body of list::push_back 0x007421C0: link a new node before the sentinel and copy the value into it.
__declspec(noinline) void CModeList::push_back(const CModePointer& x)
{
    SModeNode* node = AllocateModeNode(this);
    if (node)
        new (&node->m_data) CModePointer(x);
}
#pragma auto_inline(on)
struct CTCCreatureModeManager {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CModeList PModes;                                   // +0x0c
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    void RemoveMode(unsigned long modeType);            // 0x0073ABA0
    bool AddMode(CModePointer mode);
};

bool CTCCreatureModeManager::AddMode(CModePointer mode)
{
    unsigned long type = mode->GetModeType();
    SModeNode* end = PModes.m_pHead;
    SModeNode* node = end->m_pNext;
    for (; node != end; node = node->m_pNext) {
        if (node->m_data->GetModeType() == type)
            break;
    }
    if (node != PModes.m_pHead)
        return false;
    unsigned long replacedType = 0;
    if (!PModes.empty()) {
        if (PModes.front()->IsReplacedByNewMode())
            replacedType = PModes.front()->GetModeType();
        PModes.front()->OnLeave();
        PModes.push_back(mode);
        PModes.front()->OnResume();
    } else {
        PModes.push_back(mode);
        PModes.front()->OnResume();
    }
    if (ParentThing->HasTC(0x43))
        ((CTCLook*)ParentThing->GetTC(0x43))->UpdateTurningAnims();
    long idleDelay = (long)(g_framesPerSecond * 30.0f);
    TimeForNextIdleMode = CWorld::GetWorldFrame() + idleDelay;
    if (replacedType)
        RemoveMode(replacedType);
    return true;
}
