// NUISystem::CEditBox::Initialise() -- after the base initialisation, an edit box whose def says its parent is a button wires itself to the parent: an Enter action (type 0xBCB) and a parent left-click action (type 0xBCA) targeting this box are installed on the parent
// Leaf: ?Initialise@CEditBox@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540620, 390 bytes, virtual thiscall (slot 43), plain `ret`, esp frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Initialise@CEditBox@NUISystem@@UAEXXZ; the body calls the base
//   CChangingStateComponent::Initialise 0x0052C730 non-virtually (CEditBox's direct base, rtti_bases.tsv) and its own
//   AddActionOnEnter (slot 148, landed 0x00540D70), and reads CUIDef::EditBoxParentIsButton (retail +0x1CC: the landed CUIDef
//   ctor 0x006301A0 puts ActionOrder at +0x1C0, Ego_r.pdb EditBoxParentIsButton follows it).
// Callees: CChangingStateComponent::Initialise 0x0052C730, LoadAndReturnDef (slot 108, CDefPointer<const CUIDef> by hidden
//   pointer), GetParent (slot 52), operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*)
//   0x0042AA29, AddActionOnEnter (slot 148), malloc 0x00BFEA0E (the inline STLport list<CActionParam*>::push_back of `this`
//   as the CActionParam base at +0x18), CComponent::AddActionOnLeftClicked (slot 71, Ego_r.pdb, landed CFrontEndScreen
//   override 0x0054E4F0) on the parent twice, operator delete 0x00BFE9BC (counted-pointer release through
//   CCPPointerInfo::DeleteFunc).
// Label correction: catalogued as "CEditBox Initialise" (right); no NAME_FIXES.
// Layout: CComponent = CUIInterface (+0) + CObserver (+4, 0x14 bytes) + CActionParam (+0x18), rtti_bases.tsv; vtable slots
//   this body does not call are layout-only fillers. The two action types are raw EActionType values (no enumerator names
//   recovered).
extern "C" void* __cdecl malloc(unsigned int size);                         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(T* data);                               // 0x0042AA29 (T = NUISystem::CAction)
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1
    mutable unsigned long IVRefCount;                       // +0x04
    long Release() const
    {
        long result = (long)--IVRefCount;
        if (result == 0)
            Delete();
        return result;
    }
};
enum ENullDef { NULL_DEF = 0 };
template <class T> class CDefPointer {
public:
    const CIVCountedPointeeBase* Object;
    CDefPointer(ENullDef = NULL_DEF) : Object(0) { }
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return (T*)Object; }
};
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
namespace NUISystem {
enum EActionType { };
class CActionParam {
public:
    virtual ~CActionParam();
};
class CAction {                                             // retail 0x10 bytes
public:
    CAction();                                              // 0x0042BE50
    EActionType                   ActionType;               // +0x00
    std::list<CActionParam*>      UIActionParams;           // +0x04
    std::list<long>               ThingActionParams;        // +0x08
    bool                          Valid;                    // +0x0C
};
class CUIDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad_0x08[0x1cc - 0x08];
    bool EditBoxParentIsButton;                             // +0x1CC
};
class CUIInterface {
public:
    virtual void Slot000();
};
class CObserver {
public:
    virtual void ObserverSlot000();
    unsigned char _body[0x10];
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual void Slot001(); virtual void Slot002(); virtual void Slot003(); virtual void Slot004();
    virtual void Slot005(); virtual void Slot006(); virtual void Slot007(); virtual void Slot008();
    virtual void Slot009(); virtual void Slot010(); virtual void Slot011(); virtual void Slot012();
    virtual void Slot013(); virtual void Slot014(); virtual void Slot015(); virtual void Slot016();
    virtual void Slot017(); virtual void Slot018(); virtual void Slot019(); virtual void Slot020();
    virtual void Slot021(); virtual void Slot022(); virtual void Slot023(); virtual void Slot024();
    virtual void Slot025(); virtual void Slot026(); virtual void Slot027(); virtual void Slot028();
    virtual void Slot029(); virtual void Slot030(); virtual void Slot031(); virtual void Slot032();
    virtual void Slot033(); virtual void Slot034(); virtual void Slot035(); virtual void Slot036();
    virtual void Slot037(); virtual void Slot038(); virtual void Slot039(); virtual void Slot040();
    virtual void Slot041(); virtual void Slot042();
    virtual void Initialise();                                              // slot 43
    virtual void Slot044(); virtual void Slot045(); virtual void Slot046(); virtual void Slot047();
    virtual void Slot048(); virtual void Slot049(); virtual void Slot050(); virtual void Slot051();
    virtual CComponent* GetParent();                                        // slot 52 (+0xD0)
    virtual void Slot053(); virtual void Slot054(); virtual void Slot055(); virtual void Slot056();
    virtual void Slot057(); virtual void Slot058(); virtual void Slot059(); virtual void Slot060();
    virtual void Slot061(); virtual void Slot062(); virtual void Slot063(); virtual void Slot064();
    virtual void Slot065(); virtual void Slot066(); virtual void Slot067(); virtual void Slot068();
    virtual void Slot069(); virtual void Slot070();
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>& action);  // slot 71 (+0x11C)
    virtual void Slot072(); virtual void Slot073(); virtual void Slot074(); virtual void Slot075();
    virtual void Slot076(); virtual void Slot077(); virtual void Slot078(); virtual void Slot079();
    virtual void Slot080(); virtual void Slot081(); virtual void Slot082(); virtual void Slot083();
    virtual void Slot084(); virtual void Slot085(); virtual void Slot086(); virtual void Slot087();
    virtual void Slot088(); virtual void Slot089(); virtual void Slot090(); virtual void Slot091();
    virtual void Slot092(); virtual void Slot093(); virtual void Slot094(); virtual void Slot095();
    virtual void Slot096(); virtual void Slot097(); virtual void Slot098(); virtual void Slot099();
    virtual void Slot100(); virtual void Slot101(); virtual void Slot102(); virtual void Slot103();
    virtual void Slot104(); virtual void Slot105(); virtual void Slot106(); virtual void Slot107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;             // slot 108 (+0x1B0)
    virtual void Slot109(); virtual void Slot110(); virtual void Slot111(); virtual void Slot112();
    virtual void Slot113(); virtual void Slot114(); virtual void Slot115(); virtual void Slot116();
    virtual void Slot117(); virtual void Slot118(); virtual void Slot119(); virtual void Slot120();
    virtual void Slot121(); virtual void Slot122(); virtual void Slot123(); virtual void Slot124();
    virtual void Slot125(); virtual void Slot126(); virtual void Slot127(); virtual void Slot128();
    virtual void Slot129(); virtual void Slot130(); virtual void Slot131(); virtual void Slot132();
    virtual void Slot133(); virtual void Slot134(); virtual void Slot135(); virtual void Slot136();
    virtual void Slot137(); virtual void Slot138(); virtual void Slot139(); virtual void Slot140();
    virtual void Slot141(); virtual void Slot142(); virtual void Slot143(); virtual void Slot144();
    virtual void Slot145(); virtual void Slot146(); virtual void Slot147();
    unsigned char _body1c[0x15c - 0x1c];
};
class CChangingStateComponent : public CComponent {
public:
    virtual void Initialise();                              // 0x0052C730
};
class CEditBox : public CChangingStateComponent {
public:
    virtual void Initialise();
    virtual void AddActionOnEnter(CCountedPointer<CAction>& action);       // slot 148 (+0x250), 0x00540D70
};

void CEditBox::Initialise()
{
    CChangingStateComponent::Initialise();
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    if (def->EditBoxParentIsButton && GetParent()) {
        CCountedPointer<CAction> enterAction(new CAction);
        enterAction->ActionType = (EActionType)0xBCB;
        AddActionOnEnter(enterAction);
        CCountedPointer<CAction> clickAction(new CAction);
        clickAction->ActionType = (EActionType)0xBCA;
        clickAction->UIActionParams.push_back(this);
        GetParent()->AddActionOnLeftClicked(enterAction);
        GetParent()->AddActionOnLeftClicked(clickAction);
    }
}
}