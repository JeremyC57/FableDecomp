// CTCInventory::CreateEditBoxActions(NUISystem::CComponent* parent, const CCharString& buttonName, const CCharString& editBoxName, const CWideString& text, bool password, const CWideString& allowedCharacters) -- configure the named edit box (password flag, 25-character limit, text, inactive, allowed characters), give it an Enter action (type 3019), give the named button a left-click action of each type (the 3018 one carries the edit box as its parameter) and the Enter action as its back action, and remember the edit box
// Leaf: ?CreateEditBoxActions@CTCInventory@@QAEXPAVCComponent@NUISystem@@ABVCCharString@@1ABVCWideString@@_N2@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058301A, 321 bytes, `ret 18h`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, `pop ecx` after operator new). No /EHsc (the two counted
//   pointers are destroyed with plain calls).
// Identity: Ego_r.pdb ?CreateEditBoxActions@CTCInventory@@QAEXPAVCComponent@NUISystem@@ABVCCharString@@1ABVCWideString@@_N2@Z
//   (the parked spellings took three longs for the last three parameters: wrong).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (FindChild's by-value argument); CComponent::FindChild
//   (slot 3, Ego_r ?FindChild@CComponent@NUISystem@@UAEPAV12@VCCharString@@@Z); the CEditBox virtuals SetPasswordBox (152),
//   SetMaxLength (153), SetText (143), SetActive (150), SetAllowedCharacters (154), AddActionOnEnter (148); operator new
//   0x00BFEA1A; NUISystem::CAction::CAction 0x0042BE50; CCountedPointer<CAction>(CAction*) 0x0042AA29 / ~ 0x00429160;
//   list<CActionParam*>::push_back 0x0042B646; CComponent::AddActionOnLeftClicked (slot 71) and AddActionOnBack (slot 68)
//   on the button; list<CEditBox*>::push_back 0x005939EE.
// Slots: CEditBox's own block 142..155 follows the Ego_r.pdb definition order of its 16 virtuals against the retail
//   addresses of vtable 0x01246B8C (GetText 0x00540CF0 .. SetForbiddenCharacters 0x00540D40; SetText 143 and
//   AddActionOnEnter 148 are landed under those names); CComponent's slots 66..71 (AddAction, AddActionOnSelected,
//   AddActionOnBack, AddPreAction, AddActionOnInactive, AddActionOnLeftClicked) are the empty `ret 4` stubs
//   0x0052EFF0..0x0052F040 in the Ego_r.pdb order (66 / 67 / 71 agree with the landed CComponent models). The other
//   slots are layout-only fillers (the landed CEditBox::Initialise 0x00540620 spelling).
// The two push_backs are STLport `insert(end(), x)` bodies (`mov edx,[ecx]` = the sentinel, no `_M_next` load):
//   Ego_r.pdb CAction::UIActionParams is list<NUISystem::CActionParam*> and CTCInventory::EditBoxes list<NUISystem::CEditBox*>.
// Shape (chunk 859): one `editBox` variable, as the source would have it; the two STLport list push_backs (0x005939EE for
//   EditBoxes, 0x0042B646 for UIActionParams) have their BODIES in this TU as retail's TU had them from _list.h (push_back =
//   insert(end(), x), kept out of line; insert -> _M_create_node, allocator malloc, placement copy, link). With the body visible
//   VC7.1 knows the callee keeps no pointer to its argument, so `editBox` is not treated as aliased memory: its home store (the
//   dead editBoxName slot [ebp+10h], for the push_back's reference) floats to just before the first virtual call instead of
//   following `mov esi,eax`, and the calls go through the register without reloads (declaration-only push_back: 331 bytes with
//   a reload before every call, or the 2-line store residue with a second register copy; chunks 612/757/840/844/847, 50
//   attempts). The CActionParam conversion of editBox needs no null test (VC7.1 drops it after the virtual calls on the same
//   register); its temporary is homed in the dead `parent` slot [ebp+8].
// Layout: engine/CTCInventory.h (EditBoxes +0x144, opaque) through the layout-rename include; CComponent = CUIInterface
//   (+0) + CObserver (+4, 0x14 bytes) + CActionParam (+0x18) as the landed CEditBox::Initialise; CAction as there.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#define CTCInventory CTCInventoryLayout
#include "engine/CTCInventory.h"
#undef CTCInventory

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
};
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;                                   // the sentinel
    iterator end() { return iterator(_M_node); }
    _List_node<_Tp>* _M_create_node(const _Tp& x)
    {
        _List_node<_Tp>* p = allocator<_List_node<_Tp> >().allocate(1);
        new ((void*)&p->_M_data) _Tp(x);
        return p;
    }
    __declspec(noinline) iterator insert(iterator position, const _Tp& x)
    {
        _List_node<_Tp>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
        return iterator(tmp);
    }
    __declspec(noinline) void push_back(const _Tp& x) { insert(end(), x); }   // same-TU body (STLport _list.h)
};
}
namespace NUISystem {
enum EActionType { };
class CActionParam {
public:
    virtual ~CActionParam();
};
class CAction {                                                 // retail 0x10 bytes
public:
    CAction();                                                  // 0x0042BE50
    EActionType                   ActionType;                   // +0x00
    std::list<CActionParam*>      UIActionParams;               // +0x04 (push_back 0x0042B646)
    std::list<long>               ThingActionParams;            // +0x08
    bool                          Valid;                        // +0x0C
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
    virtual void Slot001(); virtual void Slot002();
    virtual CComponent* FindChild(CCharString name);                       // slot 3
    virtual void Slot004(); virtual void Slot005(); virtual void Slot006(); virtual void Slot007(); virtual void Slot008(); virtual void Slot009();
    virtual void Slot010(); virtual void Slot011(); virtual void Slot012(); virtual void Slot013(); virtual void Slot014(); virtual void Slot015();
    virtual void Slot016(); virtual void Slot017(); virtual void Slot018(); virtual void Slot019(); virtual void Slot020(); virtual void Slot021();
    virtual void Slot022(); virtual void Slot023(); virtual void Slot024(); virtual void Slot025(); virtual void Slot026(); virtual void Slot027();
    virtual void Slot028(); virtual void Slot029(); virtual void Slot030(); virtual void Slot031(); virtual void Slot032(); virtual void Slot033();
    virtual void Slot034(); virtual void Slot035(); virtual void Slot036(); virtual void Slot037(); virtual void Slot038(); virtual void Slot039();
    virtual void Slot040(); virtual void Slot041(); virtual void Slot042(); virtual void Slot043(); virtual void Slot044(); virtual void Slot045();
    virtual void Slot046(); virtual void Slot047(); virtual void Slot048(); virtual void Slot049(); virtual void Slot050(); virtual void Slot051();
    virtual void Slot052(); virtual void Slot053(); virtual void Slot054(); virtual void Slot055(); virtual void Slot056(); virtual void Slot057();
    virtual void Slot058(); virtual void Slot059(); virtual void Slot060(); virtual void Slot061(); virtual void Slot062(); virtual void Slot063();
    virtual void Slot064(); virtual void Slot065();
    virtual void AddAction(CCountedPointer<CAction>& action);               // slot 66
    virtual void AddActionOnSelected(CCountedPointer<CAction>& action);     // slot 67
    virtual void AddActionOnBack(CCountedPointer<CAction>& action);         // slot 68
    virtual void AddPreAction(CCountedPointer<CAction>& action);            // slot 69
    virtual void AddActionOnInactive(CCountedPointer<CAction>& action);     // slot 70
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>& action);  // slot 71
    virtual void Slot072(); virtual void Slot073(); virtual void Slot074(); virtual void Slot075(); virtual void Slot076(); virtual void Slot077();
    virtual void Slot078(); virtual void Slot079(); virtual void Slot080(); virtual void Slot081(); virtual void Slot082(); virtual void Slot083();
    virtual void Slot084(); virtual void Slot085(); virtual void Slot086(); virtual void Slot087(); virtual void Slot088(); virtual void Slot089();
    virtual void Slot090(); virtual void Slot091(); virtual void Slot092(); virtual void Slot093(); virtual void Slot094(); virtual void Slot095();
    virtual void Slot096(); virtual void Slot097(); virtual void Slot098(); virtual void Slot099(); virtual void Slot100(); virtual void Slot101();
    virtual void Slot102(); virtual void Slot103(); virtual void Slot104(); virtual void Slot105(); virtual void Slot106(); virtual void Slot107();
    virtual void Slot108(); virtual void Slot109(); virtual void Slot110(); virtual void Slot111(); virtual void Slot112(); virtual void Slot113();
    virtual void Slot114(); virtual void Slot115(); virtual void Slot116(); virtual void Slot117(); virtual void Slot118(); virtual void Slot119();
    virtual void Slot120(); virtual void Slot121(); virtual void Slot122(); virtual void Slot123(); virtual void Slot124(); virtual void Slot125();
    virtual void Slot126(); virtual void Slot127(); virtual void Slot128(); virtual void Slot129(); virtual void Slot130(); virtual void Slot131();
    virtual void Slot132(); virtual void Slot133(); virtual void Slot134(); virtual void Slot135(); virtual void Slot136(); virtual void Slot137();
    virtual void Slot138(); virtual void Slot139(); virtual void Slot140(); virtual void Slot141();
};
class CEditBox : public CComponent {
public:
    virtual CWideString GetText();                                          // slot 142
    virtual void SetText(const CWideString& text);                          // slot 143
    virtual CComponent* GetTextComponent();                                 // slot 144
    virtual void AddCharacter(unsigned short character);                    // slot 145
    virtual void KeyPressed(unsigned short character);                      // slot 146
    virtual void UpdateDisplayText();                                       // slot 147
    virtual void AddActionOnEnter(CCountedPointer<CAction>& action);        // slot 148
    virtual void AddActionOnEscape(CCountedPointer<CAction>& action);       // slot 149
    virtual void SetActive(bool active);                                    // slot 150
    virtual bool IsActive();                                                // slot 151
    virtual void SetPasswordBox(bool password);                             // slot 152
    virtual void SetMaxLength(long length);                                 // slot 153
    virtual void SetAllowedCharacters(const CWideString& characters);       // slot 154
    virtual void SetForbiddenCharacters(const CWideString& characters);     // slot 155
};
}

class CTCInventory : public CTCInventoryLayout {
public:
    void CreateEditBoxActions(NUISystem::CComponent* parent, const CCharString& buttonName, const CCharString& editBoxName,
                              const CWideString& text, bool password, const CWideString& allowedCharacters);
};

void CTCInventory::CreateEditBoxActions(NUISystem::CComponent* parent, const CCharString& buttonName,
                                        const CCharString& editBoxName, const CWideString& text, bool password,
                                        const CWideString& allowedCharacters)
{
    NUISystem::CEditBox* editBox = static_cast<NUISystem::CEditBox*>(parent->FindChild(editBoxName));
    editBox->SetPasswordBox(password);
    editBox->SetMaxLength(0x19);
    editBox->SetText(text);
    editBox->SetActive(false);
    editBox->SetAllowedCharacters(allowedCharacters);
    CCountedPointer<NUISystem::CAction> enter(new NUISystem::CAction);
    enter->ActionType = (NUISystem::EActionType)0xBCB;
    editBox->AddActionOnEnter(enter);
    NUISystem::CComponent* button = parent->FindChild(buttonName);
    CCountedPointer<NUISystem::CAction> edit(new NUISystem::CAction);
    edit->ActionType = (NUISystem::EActionType)0xBCA;
    edit->UIActionParams.push_back(editBox);
    button->AddActionOnLeftClicked(enter);
    button->AddActionOnLeftClicked(edit);
    button->AddActionOnBack(enter);
    reinterpret_cast<std::list<NUISystem::CEditBox*>*>(EditBoxes)->push_back(editBox);
}