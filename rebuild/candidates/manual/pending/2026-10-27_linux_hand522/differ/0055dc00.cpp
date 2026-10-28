// CTCPhotojournalCapture::CreateScreen() -- open the photo-journal capture screen: mark it open and first-frame, create PC_PHOTOJOURNAL_CAPTURE, wire its caption edit box (Enter: caption action 0xBC7 with the box as parameter, Escape: action 0xBC8), keep the screen component, and have the owner's inventory capture the back buffer
// Grade: DIFFER(502v495) -- register allocation plus the GetTC key reload: retail keeps the created screen in edi (plus its home for the push_back) and spills `this` to the frame (ebx then reused as the zero / list-node register, frame 0x1c), ours keeps `this` in ebp and re-reads the screen from its home before every use (frame 0x18); call order, strings and actions are otherwise retail's, but the GetTC key is reloaded from its frame slot after LowerBound (retail compares the constant: the landed copy-key GetTC form fixes that, proven on 0x00663990). Tried: one-expression GetInstance()->CreateComponent (string built before GetInstance), optimize("a") with an explicit CCharString temporary (the action words are reloaded in retail, so not /Oa), the manager local (this, best).
// Leaf: ?CreateScreen@CTCPhotojournalCapture@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055DC00, 495 bytes, thiscall, plain `ret`, esp frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CreateScreen@CTCPhotojournalCapture@@AAEXXZ (private, void()); the body writes
//   CTCPhotojournalCapture::ScreenOpen +0x19 / FirstRenderFrame +0x18 / FrameCounter +0x14 and pushes onto Components +0x2C
//   (Ego_r.pdb offsets, all before any STLport container), names the screen "PC_PHOTOJOURNAL_CAPTURE" (0x0124C558) and its
//   child "PC_PHOTOJOURNAL_CAPTURE_EDIT_BOX" (0x0124C534); its only reference is the tail jump of 0x0055DDF0
//   (`DiscardScreenShot = false`, the OpenPhotojournalCapture shape). ghidra_out/labels_external963_corrections already
//   names it CTCPhotojournalCapture::CreateScreen.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (Ego_r.pdb ?GetInstance@CManager@NUISystem@@SIPAV12@XZ, the 0xD0-byte
//   singleton), CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCharString 0x0099EBF0 / ~CCharString
//   0x0099EAE0, the screen's virtual Initialise (slot 43), ChangeState(unsigned long) (slot 48) and FindChild(CCharString)
//   (slot 3, by value: the landed CFrontEndScreen::Initialise 0x0054E4B0), operator new 0x00BFEA1A, CAction::CAction
//   0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29, malloc 0x00BFEA0E (inline list<CActionParam*>::push_back),
//   CEditBox::AddActionOnEnter / AddActionOnEscape (slots 148 / 149, landed 0x00540D70 / 0x00540DA0), operator delete
//   0x00BFE9BC, vector<NUISystem::CComponent*>::_M_insert_overflow 0x00485DBC, CVectorMap<ETCInterfaceType, CTCBase*>::
//   LowerBound 0x0040F020 (inline CThing::HasTC / GetTC on ParentThing, interface 0x11), NInventory::CTCInventoryBase::
//   CaptureBackBufferToTexture 0x005BC6ED.
// Label correction: catalogued as the forge label "AddPhotoJournalCaptureUI".
// Names: 0x005BC6ED is Ego_r.pdb ?CaptureBackBufferToTexture@CTCInventoryBase@NInventory@@QAEXXZ (an NInventory::
//   CTCInventoryBase void() member in that class's 0x005BC... range whose body is a singleton's back-buffer capture,
//   0x00406CC0); probable. The two action types and interface 0x11 are raw enum values (no enumerator names recovered).
// Layout: local view (no engine/ header for CTCPhotojournalCapture; engine/CTCBase.h is a flattened POD); CThing
//   UsingComponents (bitset<274>) +0x20 and Components +0x44 (retail; the landed CThing models).
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
struct __true_type { __true_type() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend);                  // 0x00485DBC (T = NUISystem::CComponent*)
public:
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const { return (_M_w[__pos / 32] & (1UL << (__pos % 32))) != 0; }
};
}
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public: void* PStringData; };
class CThing;
enum ETCInterfaceType { };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key);                   // 0x0040F020
    value_type* end() { return Data._M_finish; }
    value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
    std::vector<value_type> Data;
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
};
namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
    void CaptureBackBufferToTexture();                      // 0x005BC6ED
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
class CUIInterface {
public:
    virtual void Slot000();
};
class CObserver {
public:
    virtual void ObserverSlot000();
    unsigned char _body[0x10];
};
class CComponent;
class CManager {
public:
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, bool dynamic);   // 0x0041DB1D
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual void Slot001(); virtual void Slot002();
    virtual CComponent* FindChild(CCharString name);                        // slot 3 (+0x0C)
    virtual void Slot004(); virtual void Slot005(); virtual void Slot006(); virtual void Slot007();
    virtual void Slot008(); virtual void Slot009(); virtual void Slot010(); virtual void Slot011();
    virtual void Slot012(); virtual void Slot013(); virtual void Slot014(); virtual void Slot015();
    virtual void Slot016(); virtual void Slot017(); virtual void Slot018(); virtual void Slot019();
    virtual void Slot020(); virtual void Slot021(); virtual void Slot022(); virtual void Slot023();
    virtual void Slot024(); virtual void Slot025(); virtual void Slot026(); virtual void Slot027();
    virtual void Slot028(); virtual void Slot029(); virtual void Slot030(); virtual void Slot031();
    virtual void Slot032(); virtual void Slot033(); virtual void Slot034(); virtual void Slot035();
    virtual void Slot036(); virtual void Slot037(); virtual void Slot038(); virtual void Slot039();
    virtual void Slot040(); virtual void Slot041(); virtual void Slot042();
    virtual void Initialise();                                              // slot 43 (+0xAC)
    virtual void Slot044(); virtual void Slot045(); virtual void Slot046(); virtual void Slot047();
    virtual void ChangeState(unsigned long state);                          // slot 48 (+0xC0)
    virtual void Slot049(); virtual void Slot050(); virtual void Slot051(); virtual void Slot052();
    virtual void Slot053(); virtual void Slot054(); virtual void Slot055(); virtual void Slot056();
    virtual void Slot057(); virtual void Slot058(); virtual void Slot059(); virtual void Slot060();
    virtual void Slot061(); virtual void Slot062(); virtual void Slot063(); virtual void Slot064();
    virtual void Slot065(); virtual void Slot066(); virtual void Slot067(); virtual void Slot068();
    virtual void Slot069(); virtual void Slot070(); virtual void Slot071(); virtual void Slot072();
    virtual void Slot073(); virtual void Slot074(); virtual void Slot075(); virtual void Slot076();
    virtual void Slot077(); virtual void Slot078(); virtual void Slot079(); virtual void Slot080();
    virtual void Slot081(); virtual void Slot082(); virtual void Slot083(); virtual void Slot084();
    virtual void Slot085(); virtual void Slot086(); virtual void Slot087(); virtual void Slot088();
    virtual void Slot089(); virtual void Slot090(); virtual void Slot091(); virtual void Slot092();
    virtual void Slot093(); virtual void Slot094(); virtual void Slot095(); virtual void Slot096();
    virtual void Slot097(); virtual void Slot098(); virtual void Slot099(); virtual void Slot100();
    virtual void Slot101(); virtual void Slot102(); virtual void Slot103(); virtual void Slot104();
    virtual void Slot105(); virtual void Slot106(); virtual void Slot107(); virtual void Slot108();
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
class CChangingStateComponent : public CComponent { };
class CEditBox : public CChangingStateComponent {
public:
    virtual void AddActionOnEnter(CCountedPointer<CAction>& action);       // slot 148 (+0x250), 0x00540D70
    virtual void AddActionOnEscape(CCountedPointer<CAction>& action);      // slot 149 (+0x254), 0x00540DA0
};
}

class CTCPhotojournalCapture : public CTCBase {
public:
    CWideString Description;                                // +0x0C
    bool DiscardScreenShot;                                 // +0x10
    long FrameCounter;                                      // +0x14
    bool FirstRenderFrame;                                  // +0x18
    bool ScreenOpen;                                        // +0x19
    double LastRenderTime;                                  // +0x20
    CWideString FileName;                                   // +0x28
    std::vector<NUISystem::CComponent*> Components;         // +0x2C
private:
    void CreateScreen();
};

void CTCPhotojournalCapture::CreateScreen()
{
    ScreenOpen = true;
    FirstRenderFrame = true;
    FrameCounter = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* screen = manager->CreateComponent("PC_PHOTOJOURNAL_CAPTURE", false);
    screen->Initialise();
    screen->ChangeState(1);
    NUISystem::CEditBox* editBox = (NUISystem::CEditBox*)screen->FindChild("PC_PHOTOJOURNAL_CAPTURE_EDIT_BOX");
    if (editBox) {
        CCountedPointer<NUISystem::CAction> enterAction(new NUISystem::CAction);
        enterAction->ActionType = (NUISystem::EActionType)0xBC7;
        enterAction->UIActionParams.push_back(editBox);
        editBox->AddActionOnEnter(enterAction);
        CCountedPointer<NUISystem::CAction> escapeAction(new NUISystem::CAction);
        escapeAction->ActionType = (NUISystem::EActionType)0xBC8;
        editBox->AddActionOnEscape(escapeAction);
    }
    Components.push_back(screen);
    if (ParentThing->HasTC((ETCInterfaceType)0x11))
        ((NInventory::CTCInventoryBase&)ParentThing->GetTC((ETCInterfaceType)0x11)).CaptureBackBufferToTexture();
}
