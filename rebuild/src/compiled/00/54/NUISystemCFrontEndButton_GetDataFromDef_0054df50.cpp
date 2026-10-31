// NUISystem::CFrontEndButton::GetDataFromDef() -- read the button's UI def: a default action of the def's type (a movie action, type 0x41, carries the def's movie file name as a string parameter) appended to Actions, and the input delay
// Grade: RELOCATION_MATCH
// Leaf: ?GetDataFromDef@CFrontEndButton@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DF50, 344 bytes, thiscall, plain `ret`, esp frame.
// Identity (proven): both NUISystem::CFrontEndButton constructors 0x0054E0B0 / 0x0054E120 (vtables 0x01249554 / 0x01249530 /
//   0x01249528, RTTI .?AVCFrontEndButton@NUISystem@@) end with a non-virtual call to this body, and no vtable holds it; the
//   one non-virtual void() member of CFrontEndButton in Ego_r.pdb / FableWin.pdb is ?GetDataFromDef@CFrontEndButton@NUISystem@@QAEXXZ
//   (CText::GetDataFromDef 0x0054ED90 is the same ctor-called def reader). It reads CUIDef::Action / InputDelay /
//   MovieFilename at the retail offsets +0xC4 / +0x184 / +0x1D8 (Ego_r.pdb +0xE0 / +0x1A0 / +0x200 less the STLport
//   container shrink, as the landed CUIDef ctor 0x006301A0) and writes CClickable::InputDelay +0x18C and Actions +0x198.
// Callees: LoadAndReturnDef (slot 108, CDefPointer<const CUIDef> by hidden pointer), operator new 0x00BFEA1A,
//   CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29, CWideString(const CWideString&) 0x0099B720
//   (twice: the local copy and CActionParamString::Value), malloc 0x00BFEA0E (inline list<CActionParam*>::push_back),
//   ~CWideString 0x0099B510, vector<CCountedPointer<CAction> >::_M_insert_overflow 0x0054A2A0, operator delete 0x00BFE9BC.
// Label correction: catalogued as "CDynamicList::Initialize" (the landed ctor 0x0054E0B0 says "CFrontEndButton::Initialize").
// Shape: Actions.push_back goes through the inline STLport vector push_back with a same-TU noinline _M_insert_overflow body
//   that only reads the value (the landed CTextSlider::Initialize 0x00549B20 lever): the action's Data / Info then stay in
//   ebx / esi across the movie-parameter calls as in retail.
// Layout: CComponent = CUIInterface (+0) + CObserver (+4) + CActionParam (+0x18); retail CClickable chain is the PDB less
//   0x30; vtable slots this body does not call are layout-only fillers.
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);                         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }


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
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                  // 0x0054A2A0
public:
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
namespace NUISystem {
enum EActionType { };
enum EActionParamType { };
class CActionParam {
public:
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam();
    EActionParamType ActionParamType;                       // +0x04
};
class CAction {                                             // retail 0x10 bytes
public:
    CAction();                                              // 0x0042BE50
    EActionType                   ActionType;               // +0x00
    std::list<CActionParam*>      UIActionParams;           // +0x04
    std::list<long>               ThingActionParams;        // +0x08
    bool                          Valid;                    // +0x0C
};
}   // namespace NUISystem
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};
namespace NUISystem {
class CUIDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad_0x08[0xc4 - 0x08];
    EActionType Action;                                     // +0xC4
    unsigned char _pad_0xc8[0x184 - 0xc8];
    float InputDelay;                                       // +0x184
    unsigned char _pad_0x188[0x1d8 - 0x188];
    CWideString MovieFilename;                              // +0x1D8
};
class CActionParamString : public CActionParam {            // vtable 0x01249520
public:
    CActionParamString(const CWideString& value) : CActionParam((EActionParamType)4), Value(value) { }
    CWideString Value;                                      // +0x08
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
    unsigned char _body20[0x15c - 0x20];
};
class CChangingStateComponent : public CComponent { };
class CHoverable : public CChangingStateComponent { };
class CClickable : public CHoverable {
public:
    unsigned char _body15c[0x18c - 0x15c];
    float InputDelay;                                       // +0x18C
    float LastInputTime;                                    // +0x190
};
class CFrontEndButton : public CClickable {
public:
    float m_fLastInputTime;                                 // +0x194
    std::vector<CCountedPointer<CAction> > Actions;         // +0x198
    void GetDataFromDef();
};

}
#pragma auto_inline(off)
void* GrowVectorStorage_54df50(void* vector, unsigned int count);
// Same-TU body of the vector growth template (_M_insert_overflow is instantiated in this TU at 0x0054A2A0, as the landed
// CTextSlider::Initialize 0x00549B20): it reads the value through its const reference into the new storage and never touches
// the empty tag, so the caller keeps the counted pointer's words in registers across the call.
namespace std {
template <class _Tp, class _Alloc> __declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    _Tp* p = (_Tp*)GrowVectorStorage_54df50(this, __fill_len);
    if (p)
        new (p) _Tp(__x);
}
}
#pragma auto_inline(on)
namespace NUISystem {

void CFrontEndButton::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    if (def->Action) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->Action;
        if (action->ActionType == 0x41) {
            CWideString movieFilename(def->MovieFilename);
            action->UIActionParams.push_back(new CActionParamString(movieFilename));
        }
        Actions.push_back(action);
    }
    InputDelay = def->InputDelay;
}
}