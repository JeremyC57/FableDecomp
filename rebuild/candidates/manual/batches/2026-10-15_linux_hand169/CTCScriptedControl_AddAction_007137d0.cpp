// CTCScriptedControl::AddAction(CActionBase* action) -- attach the action to this control (PTCScript, virtual slot 3), wrap it in a CCountedPointer and append it to PActionList
// Leaf: AddAction
// Retail: Fable.exe 0x007137D0, 126 bytes (`ret 4`).
// Callees: CActionBase virtual slot 3 (+0x0C, no arguments), operator new @ 0x00BFEA1A (the 12-byte count block),
// malloc @ 0x00BFEA0E (STLport node allocator, 16-byte list node), operator delete @ 0x00BFE9BC (count block);
// the count block's destroy pointer is CCountedPointer<CActionBase>'s DeleteData @ 0x007180C0, called as a member
// pointer on the item (`mov ecx,[blk+8] ; call [blk+4]`).
// Layout = Ego_r.pdb CTCScriptedControl (0x34): PActionList (STLport list<CCountedPointer<CActionBase>>) +0x14;
// CActionBase (vtable 0x01265504): PTCScript +0x04. Count block {references +0, destroy +4, item +8}.
// Shape: a temporary CCountedPointer built from the raw pointer, STLport push_back = insert(end()) with the
// placement copy's null test; the temporary's release keeps no zero stores (register-only local).
#include <stddef.h>
void* __cdecl operator new(size_t size);            // 0x00BFEA1A
void __cdecl operator delete(void* block);          // 0x00BFE9BC
extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

struct CTCScriptedControl;
struct CActionBase {                                // CTCScriptedControl::CActionBase, vtable 0x01265504
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void OnAddedToControl();                // slot 3 (+0x0c)
    CTCScriptedControl* PTCScript;                  // +0x04
    bool Activated;                                 // +0x08
    bool Terminated;                                // +0x09
    void DeleteData();                              // 0x007180C0 (CCountedPointer<CActionBase>::DeleteData)
};
template <class T> struct SCountBlock {
    long m_references;                              // +0x00
    void (T::*m_pfnDestroy)();                      // +0x04
    T*   m_pItem;                                   // +0x08
    SCountBlock(T* item) : m_references(1), m_pfnDestroy(&T::DeleteData), m_pItem(item) { }
};
template <class T> struct CCountedPointer {
    T* Data;                                        // +0x00
    SCountBlock<T>* Info;                           // +0x04
    CCountedPointer(T* item) : Data(item), Info(new SCountBlock<T>(item)) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
    }
};
struct SActionNode {
    SActionNode* _M_next;                           // +0x00
    SActionNode* _M_prev;                           // +0x04
    CCountedPointer<CActionBase> _M_data;           // +0x08
};
struct CActionList {                                // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    void push_back(const CCountedPointer<CActionBase>& x)
    {
        SActionNode* pos = _M_node;
        SActionNode* tmp = (SActionNode*)malloc(sizeof(SActionNode));
        new (&tmp->_M_data) CCountedPointer<CActionBase>(x);
        SActionNode* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
};
struct CTCScriptedControl {
    virtual void v00();
    void*         ParentThing;                      // +0x04 (CTCBase)
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    unsigned char PScriptHandleImp[8];              // +0x0c CIntelligentPointer<CScriptGameResourceObjectScriptedThing>
    CActionList   PActionList;                      // +0x14
    void AddAction(CActionBase* action);
};

void CTCScriptedControl::AddAction(CActionBase* action)
{
    action->PTCScript = this;
    action->OnAddedToControl();
    PActionList.push_back(CCountedPointer<CActionBase>(action));
}
