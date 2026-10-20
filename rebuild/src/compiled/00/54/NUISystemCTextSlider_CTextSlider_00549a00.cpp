// NUISystem::CTextSlider::~CTextSlider() -- if the selection moved, push the starting selection back into every action's target and re-run the actions; delete the back/select action lists; destroy the action vector and the draggable base
// Leaf: ??1CTextSlider@NUISystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549A00, 280 bytes (+8 int3), `ret`.
// Callees: CFrontEndManager::GetInstance() @ 0x0041E5F2 (static, `__fastcall`) and its virtual slot 14 (+0x38,
//   ExecuteAction), list<CCountedPointer<CAction> >::clear() @ 0x005356A0 (manifest "remove_if<...>" is propagated),
//   free @ 0x00BFEA14, operator delete @ 0x00BFE9BC, the action range destroyer @ 0x0054A1C0 (`__fastcall` first/last +
//   allocator reference; manifest "_Destroy_range<...>" names another instantiation), NUISystem::CDraggableInto::
//   ~CDraggableInto @ 0x0052CCF0 (manifest "CTCIdleScheduler::Clear" is propagated).
// Vtables: 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@) at +0 / +4 / +0x18.
// Layout: the landed ctor 0x00549F60 / RevertValue 0x005491A0 model (retail = PDB - 0x30 up to Actions, - 0x34 after).
// Shape: the revert loop is RevertValue's (size() re-read each pass, operator[] as `*(begin() + i)`) with
//   StartSelectedChild as the value; each list pointer is a `delete` of a list whose inline destructor clears it and frees
//   its header node; the vector's implicit member destruction is `_Destroy(first, last, alloc)` then free.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
void operator delete(void* p);                   // 0x00BFE9BC
struct CCPPointerInfo { long Count; };
template <class T> struct CCountedPointer {
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
struct CActionAllocator {                        // std::allocator<CCountedPointer<CAction> > (empty; the user ctor keeps
    CActionAllocator() { }                       //   VC7.1 from zero-filling the temporary byte)
};
namespace NUISystem { struct CAction; }
void __fastcall DestroyActionRange(CCountedPointer<NUISystem::CAction>* first, CCountedPointer<NUISystem::CAction>* last,
                                   const CActionAllocator& allocator);   // 0x0054A1C0
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    ~CRetailVector()
    {
        DestroyActionRange(_M_start, _M_finish, CActionAllocator());
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct CRetailList {          // STLport list: the header-node pointer
    void* _M_node;
    void clear();                                // 0x005356A0
    ~CRetailList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct CComponent;
namespace NUISystem {
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
struct CDraggableInto : CComponentInterface, CDrawableInterface, CDraggableInterface {
    unsigned char _pad1c[0x15c - 0x1c];
    ~CDraggableInto();                           // 0x0052CCF0
};
struct CTextSlider : CDraggableInto {            // vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@)
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x16c
    CRetailList<CCountedPointer<CAction> >* PActionsOnBack;    // +0x178
    CRetailList<CCountedPointer<CAction> >* PActionsOnSelect;  // +0x17c
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    unsigned long MaxChild;                      // +0x19c
    virtual void v000();
    ~CTextSlider();
};
}
struct CSliderValue { unsigned char _pad[8]; unsigned long Selected; };          // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };           // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                             // +0x00
namespace NUISystem {
struct CAction {
    virtual void v00();
    CSliderTargetRef* PTarget;                   // +0x04
};
}
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
};
namespace NUISystem {
CTextSlider::~CTextSlider()
{
    if (StartSelectedChild != SelectedChild) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            Actions[i]->PTarget->Target->Value->Selected = StartSelectedChild;
            CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
}