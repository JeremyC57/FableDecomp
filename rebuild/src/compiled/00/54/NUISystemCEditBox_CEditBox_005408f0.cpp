// NUISystem::CEditBox::~CEditBox() -- edit box dtor: stop observing the UI manager, then the member dtors (the two character-filter strings, the text, the escape/enter action lists) and ~CChangingStateComponent
// Leaf: ??1CEditBox@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005408F0, 137 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x01246B8C / 0x01246B64 / 0x01246B5C carry RTTI `.?AVCEditBox@NUISystem@@`; rtti_bases: CEditBox :
//   CChangingStateComponent : CComponent (CUIInterface @0, CObserver @4, CActionParam @0x18); Ego_r.pdb has
//   `??1CEditBox@NUISystem@@UAE@XZ`. The catalog label is right.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (static __fastcall, Ego_r.pdb `?GetInstance@CManager@NUISystem@@SIPAV12@XZ`,
//   landed), CObservable::RemoveObserver (virtual slot 5, +0x14: the CObservable virtuals in Ego_r declaration order SetEvent,
//   SetOwnedEvent, AddObserver, AddExclusiveObserver, RemoveExclusiveObserver, RemoveObserver, ... as the landed CObservable rows),
//   ~CWideString 0x0099B510 (x3), std::_List_base<CCountedPointer<NUISystem::CAction> >::clear 0x005356A0 (x2), free 0x00BFEA14
//   (the lists' header nodes), NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0 (tail jump).
// Layout: Ego_r.pdb CEditBox (0x1CC): ActionsOnEnter +0x18C, ActionsOnEscape +0x198 (Dinkumware lists), Text +0x1A4, ...,
//   MaxLength +0x1C0, AllowedCharacters +0x1C4, ForbiddenCharacters +0x1C8. Retail: the base is 0x30 shorter and each STLport list
//   is 4 bytes: ActionsOnEnter +0x15C, ActionsOnEscape +0x160, Text +0x164 ... ForbiddenCharacters +0x188 (the dtor's offsets).
// Shape: user body `CManager::GetInstance()->RemoveObserver(this);` (the CObserver subobject, this+4, computed before the vtable
//   stores); the members are destroyed in reverse order: the strings out of line, each list through the inline STLport
//   ~_List_base (out-of-line clear, then free the header node under its null test).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};

namespace NUISystem {
class CAction;
enum EEvent { };                                            // values not needed here
class CObserver;
class CObservable { public:                                 // virtuals in Ego_r declaration order
    virtual void SetEvent(EEvent event);                    // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);          // slot 2
    virtual void AddExclusiveObserver(CObserver* observer); // slot 3
    virtual void RemoveExclusiveObserver(CObserver* observer);       // slot 4
    virtual void RemoveObserver(CObserver* observer);       // slot 5 (+0x14)
};
class CManager : public CObservable { public:
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};

class CUIInterface { public: virtual ~CUIInterface(); };                                 // vptr +0x00
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };       // +0x04 (EventsToObserve set +0x08, PreventObservation +0x14)
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };           // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    unsigned char _body[0x15c - 0x20];                      // +0x20 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CEditBox : public CChangingStateComponent { public:
    std::list<CCountedPointer<CAction> > ActionsOnEnter;    // +0x15C
    std::list<CCountedPointer<CAction> > ActionsOnEscape;   // +0x160
    CWideString   Text;                                     // +0x164
    long          CursorPos;                                // +0x168
    long          LeftMostCharacter;                        // +0x16C
    long          RightMostCharacter;                       // +0x170
    long          Adjustment;                               // +0x174
    bool          Active;                                   // +0x178
    bool          IsPasswordBox;                            // +0x179
    bool          UsesIME;                                  // +0x17A
    bool          DisallowSpaceAsFirstChar;                 // +0x17B
    bool          DeleteAllOnKeyPress;                      // +0x17C
    long          MaxLength;                                // +0x180
    CWideString   AllowedCharacters;                        // +0x184
    CWideString   ForbiddenCharacters;                      // +0x188
    virtual ~CEditBox();
};

CEditBox::~CEditBox()
{
    CManager::GetInstance()->RemoveObserver(this);
}
} // namespace NUISystem