// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase> mode) -- refuse a mode whose type is already queued; otherwise remember the current (front) mode's type when it wants replacing, tell it it is leaving, append the new mode, resume the front mode, re-pick the look component's turning anims, schedule the next idle mode in 30 seconds and finally drop the replaced type
// Grade: RELOCATION_MATCH (chunk 844, 3 attempts after chunk 840): std::find_if(PModes.begin(), end, CIsModeOfType(type)) over an STLport-style list iterator with a user copy ctor, then `if (it != PModes.end()) return false;` -- the by-value iterator copy leaves retail's early `xor al,al` ahead of `pop ebp` (chunk 840's a2 iterator loop with a cached end interleaved it).
// Leaf stand-in: the local CModePointer model mangles as `UCModePointer@@`; the PDB symbol is ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (engine/CTCCreatureModeManager.h opt-in FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER); see NOTES.md.
// Leaf: ?AddMode@CTCCreatureModeManager@@QAE_NUCModePointer@@@Z
// Retail symbol: ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (Ego_r.pdb, engine/CTCCreatureModeManager.h FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER); the local CModePointer model spells the parameter type differently.
// Earlier grade: DIFFER(ours v retail) -- the early `return false` sets al (`xor al,al`) before the register pops in retail, after `pop ebp` in ours; best build DIFFER(331v331), 2 masked diff lines (chunk 757, 10 attempts)
// Flags: none
// Earlier note: DIFFER(331v331) -- scheduling only: retail materialises the early 'return false' (xor al,al) right after the argument's release, before the pops; ours schedules it between pop ebp and pop ebx like the true path's mov al,1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073ACC0, 336 bytes (`ret 8`: the counted pointer by value, destroyed by the callee).
// Callees: the mode's virtual slots 1 GetModeType, 5 OnResume (+0x14), 6 OnLeave (+0x18) and 11 (+0x2c, "replace me when another
// mode is added"; named descriptively), list<CCountedPointer<CModeBase>>::push_front @ 0x007421C0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCLook::UpdateTurningAnims @ 0x00754050, __ftol2 @ 0x00BFEA70, CWorld::GetFrame() @
// 0x0049D870 (static), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator delete @ 0x00BFE9BC (count block).
// Globals: g_framesPerSecond @ 0x01375550, 30.0f @ 0x012673C4.
// Layout: as the landed RemoveMode 0x0073ABA0 (Ego_r.pdb CTCCreatureModeManager with the retail 4-byte STLport list PModes +0x0c,
// TimeForNextIdleMode +0x14; node {next, prev, CCountedPointer {object, block}}; the count block {references, destroy fn (object in
// ecx), object}).
// Shape: an inline find over the list (end held in a register), the "already queued" path releases the argument and returns false;
// the replaced type is preset 0 and RemoveMode runs after the idle time is stored.
#include "fable_compiler.h"
class CMainGameComponent {                               // the landed CThingDeadCreature::WorldUpdate 0x00836260 spelling
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CTCBase;
// agentNF15: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->_M_finish || interfaceType < it->first)
            it = map->_M_finish;
        return it->second;
    }
};
class CWorld { public:
    static long __fastcall GetFrame();                  // 0x0049D870 (bound ?GetFrame@CWorld@@SIJXZ; the chunk 313 label GetWorldFrame / GFGetCurrentFrame are the same getter)
};
class CTCLook { public:
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
struct CModeIterator {                                  // STLport 4.6 _List_iterator: a user copy ctor (iterator(const iterator&))
    SModeNode* m_pNode;
    CModeIterator() { }
    CModeIterator(SModeNode* node) : m_pNode(node) { }
    CModeIterator(const CModeIterator& other) : m_pNode(other.m_pNode) { }
    CModePointer& operator*() const { return m_pNode->m_data; }
    CModePointer* operator->() const { return &m_pNode->m_data; }
    CModeIterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
    bool operator==(const CModeIterator& o) const { return m_pNode == o.m_pNode; }
    bool operator!=(const CModeIterator& o) const { return m_pNode != o.m_pNode; }
};
namespace std {
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}
struct CIsModeOfType {
    unsigned long Type;
    CIsModeOfType(unsigned long type) : Type(type) { }
    bool operator()(const CModePointer& mode) const { return mode->GetModeType() == Type; }
};
struct CModeList {                                      // retail STLport list: the sentinel node pointer
    SModeNode* m_pHead;
    typedef CModeIterator iterator;
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
    bool empty() const { return m_pHead->m_pNext == m_pHead; }
    CModePointer& front() { return m_pHead->m_pNext->m_data; }
    void push_front(const CModePointer& x);              // 0x007421C0
};
inline void* operator new(unsigned int, void* where) { return where; }
SModeNode* AllocateModeNode(CModeList* list);           // stand-in: the node allocation and link-in
#pragma auto_inline(off)
// Same-TU body of list::push_front 0x007421C0: link a new node after the sentinel (insert(begin(), x): retail 0x007421C0 loads [sentinel]._M_next and links before it) and copy the value into it.
__declspec(noinline) void CModeList::push_front(const CModePointer& x)
{
    SModeNode* node = AllocateModeNode(this);
    if (node)
        new (&node->m_data) CModePointer(x);
}
#pragma auto_inline(on)
class CTCCreatureModeManager { public:
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
    CModeList::iterator end = PModes.end();
    CModeList::iterator it = std::find_if(PModes.begin(), end, CIsModeOfType(type));
    if (it != PModes.end())
        return false;
    unsigned long replacedType = 0;
    if (!PModes.empty()) {
        if (PModes.front()->IsReplacedByNewMode())
            replacedType = PModes.front()->GetModeType();
        PModes.front()->OnLeave();
        PModes.push_front(mode);
        PModes.front()->OnResume();
    } else {
        PModes.push_front(mode);
        PModes.front()->OnResume();
    }
    if (ParentThing->HasTC(0x43))
        ((CTCLook*)ParentThing->GetTC(0x43))->UpdateTurningAnims();
    long idleDelay = (long)(CMainGameComponent::GetConstantFPS() * 30.0f);
    TimeForNextIdleMode = CWorld::GetFrame() + idleDelay;
    if (replacedType)
        RemoveMode(replacedType);
    return true;
}