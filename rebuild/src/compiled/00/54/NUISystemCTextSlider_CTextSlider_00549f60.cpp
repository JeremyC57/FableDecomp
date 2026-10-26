// NUISystem::CTextSlider::CTextSlider(long id) -- construct a text slider: draggable base, empty action vector, null pointers, then Initialize()
// Leaf: ??0CTextSlider@NUISystem@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549F60, 114 bytes. __fastcall, `ret 4`.
// Callees: NUISystem::CDraggableInto::CDraggableInto(long) 0x0052CC50; NUISystem::CTextSlider::Initialize() 0x00549B20.
//          Vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@) at +0 / +4 / +0x18.
// Shape: one `xor eax,eax` shared by the init-list zeroes in declaration order: Actions (3 words), PActionsOnBack,
//        PActionsOnSelect, InputDelay, PDefAction, PLeftArrow, PRightArrow, PAcceptButton, MaxChild (the selection words,
//        Move, LastInputTime and OriginalSelectedChild are left to Initialize()).
// Layout: as RevertValue 0x005491A0 (retail = PDB - 0x30 up to Actions, - 0x34 after the 12-byte STLport vector).
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
template <class T> struct CRetailList;
struct CComponent;
namespace NUISystem {
class CAction;
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
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
    CDraggableInto(long id);                     // 0x0052CC50
};
class CTextSlider : public CDraggableInto { public:            // vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@)
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
    CTextSlider(long id);
    virtual void v000();
    void Initialize();                           // 0x00549B20
    void RevertValue();
};
}
namespace NUISystem {
CTextSlider::CTextSlider(long id)
    : CDraggableInto(id), PActionsOnBack(0), PActionsOnSelect(0), InputDelay(0), PDefAction(0), PLeftArrow(0),
      PRightArrow(0), PAcceptButton(0), MaxChild(0)
{
    Initialize();
}
}