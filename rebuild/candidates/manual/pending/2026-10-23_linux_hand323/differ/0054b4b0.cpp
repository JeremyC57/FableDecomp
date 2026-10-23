// NUISystem::CSlider::GetDataFromDef() -- read the slider's range/step block from its def and reset every value copy to the minimum; add the def's on-back action and a fixed 0x57 action; when the def names arrow components create them (right arrow: action 0x123 with LONG parameter 0x15, left: 0x14) bound to the slider; refresh the value, clamp Max to Min and hand a FLOAT parameter holding the value to every slider action
// Grade: DIFFER(1413v1381) -- first 0x209 bytes exact; the DefaultAction block's push_back: retail caches the counted pointer's count block in edi across the inline copy (inc) and the local's destructor (dec/deleter/delete, no reloads) and reads end/finish through the vector pointer, ours re-reads the frame word after every store; the arrow blocks then differ by one mov (retail copies the def word to eax before the push). Tried: __forceinline push_back, /Oa (worse)
// Leaf: GetDataFromDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054B4B0, 1381 bytes, plain `ret` (catalog name "CSlider::Initialize").
// Callees: component slots 108 GetDef (+0x1b0), 67 AddAction (+0x10c), 150 UpdateValue (+0x258); arrow slot 80 (+0x140) AddAction;
//   operator new 0x00BFEA1A, malloc 0x00BFEA0E (list nodes), operator delete 0x00BFE9BC, CAction::CAction 0x0042BE50,
//   CCountedPointer<CAction>(CAction*) 0x0042AA29 (deleter 0x00429F43), CAction::AddUIActionParam(CActionParam*, const CCountedPointer<CAction>&)
//   0x0053FDE0, STLport vector<CCountedPointer<CAction> >::_M_insert_overflow 0x0054A2A0, CFrontEndManager::GetInstance 0x0041E5F2,
//   CManager::CreateComponent(long) 0x0041D21B; parameter vtables CActionParamLONG 0x012366F4 (type 3), CActionParamFLOAT 0x01248CD0 (type 6).
// Layout: retail CSlider = Ego_r.pdb minus 0x30 (landed ctor 0x0054C050): Min +0x18c, Max +0x194, Step +0x19c, Value +0x1a4, Move +0x1ac,
//   OldValue +0x1ad, StartValue +0x1b5, Actions +0x1c0, PDefAction +0x1d4, PLeftArrow +0x1d8, PRightArrow +0x1dc, PreviousValue +0x1e0;
//   the slider's CActionParam base sits at +0x18.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }

namespace NUISystem {
struct C2DVector { float X, Y; };
template <class T> struct CRetailListNode { CRetailListNode* _M_next; CRetailListNode* _M_prev; T _M_data; };
template <class T> struct CRetailListIterator {
    CRetailListNode<T>* _M_node;
    CRetailListIterator(CRetailListNode<T>* node) : _M_node(node) { }
    CRetailListIterator(const CRetailListIterator& other) : _M_node(other._M_node) { }
};
template <class T> struct CRetailList {
    CRetailListNode<T>* _M_node;
    __forceinline CRetailList() : _M_node(0) {
        CRetailListNode<T>* n = (CRetailListNode<T>*)malloc(sizeof(CRetailListNode<T>));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
    CRetailListIterator<T> end() { return CRetailListIterator<T>(_M_node); }
    CRetailListNode<T>* _M_create_node(const T& x) {
        CRetailListNode<T>* p = (CRetailListNode<T>*)malloc(sizeof(CRetailListNode<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    CRetailListIterator<T> insert(CRetailListIterator<T> position, const T& x) {
        CRetailListNode<T>* tmp = _M_create_node(x);
        CRetailListNode<T>* n = position._M_node;
        CRetailListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};
struct CActionParam {
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) { }
    virtual void v00();
};
struct CActionParamLONG : CActionParam {               // vtable 0x012366F4
    long Value;
    CActionParamLONG(long value) : CActionParam(3), Value(value) { }
    virtual void v00();
};
struct CActionParamFLOAT : CActionParam {              // vtable 0x01248CD0
    float Value;
    CActionParamFLOAT(float value) : CActionParam(6), Value(value) { }
    virtual void v00();
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) { }
};
struct CAction;
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object);                         // 0x0042AA29
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer() {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    T* operator->() const { return m_pObject; }
};
struct CAction {                                        // retail 0x10 bytes
    long ActionType;                                    // +0x00
    CRetailList<CActionParam*> UIActionParams;          // +0x04
    CRetailList<long> ThingActionParams;                // +0x08
    bool Valid;                                         // +0x0c
    CAction();                                          // 0x0042BE50
    void AddUIActionParam(CActionParam* param) { UIActionParams.push_back(param); }
    void AddUIActionParam(CActionParam* param, const CCountedPointer<CAction>& self);   // 0x0053FDE0
};
void __fastcall DestroyAction(void* action);           // 0x00429F43
struct CInlineAction {                                  // the same 16-byte CAction, constructed inline at the 0x57 site
    long ActionType;
    CRetailList<CActionParam*> UIActionParams;
    CRetailList<long> ThingActionParams;
    bool Valid;
    __forceinline CInlineAction() : ActionType(0), Valid(true) { }
};
struct CInlineActionPointer {
    CInlineAction* m_pObject;
    SCountBlock* m_pCount;
    CInlineActionPointer(CInlineAction* object) : m_pObject(object) {
        m_pCount = m_pObject ? new SCountBlock(DestroyAction, m_pObject) : 0;
    }
    ~CInlineActionPointer() {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct __false_type { __false_type() { } };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);   // 0x0054A2A0
    __forceinline void push_back(const T& x) {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int i) const { return _M_start[i]; }
};

struct CDefVirtual { virtual void v00(); virtual void Destroy(); };
struct CRefCounted { long m_count; };
struct CSliderDef : CDefVirtual, CRefCounted {
    unsigned char _def008[0xc4 - 0x08];
    long DefaultAction;                                 // +0xc4
    unsigned char _def0c8[0xcc - 0xc8];
    long BackAction;                                    // +0xcc
    unsigned char _def0d0[0x164 - 0xd0];
    long SliderData[8];                                 // +0x164 .. +0x180
    unsigned char _def184[0x18c - 0x184];
    long LeftArrowDef;                                  // +0x18c
    long RightArrowDef;                                 // +0x190
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer {
    CSliderDef* m_p;
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CSliderDef* operator->() const { return m_p; }
};
struct CArrowComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void AddClickAction(const CCountedPointer<CAction>& action);   // slot 80 (+0x140) descriptive
};
struct CManager { CArrowComponent* CreateComponent(long definition); };   // 0x0041D21B
struct CFrontEndManager : CManager { static CFrontEndManager* __fastcall GetInstance(); };   // 0x0041E5F2

#pragma pack(push, 1)
struct CSlider {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066();
    virtual void AddAction(const CCountedPointer<CAction>& action);   // slot 67 (+0x10c) descriptive
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer GetDef();                       // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149();
    virtual void UpdateValue();                         // slot 150 (+0x258)
    unsigned char _pad004[0x18 - 0x04];
    CActionParam  m_actionParamBase;                    // +0x18 (the slider's CActionParam base)
    unsigned char _pad020[0x184 - 0x20];
    long          SliderData[2];                        // +0x184
    C2DVector     Min;                                  // +0x18c
    C2DVector     Max;                                  // +0x194
    C2DVector     Step;                                 // +0x19c
    C2DVector     Value;                                // +0x1a4
    bool          Move;                                 // +0x1ac
    C2DVector     OldValue;                             // +0x1ad
    C2DVector     StartValue;                           // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;   // +0x1c0
    unsigned char _pad1cc[0x1d4 - 0x1cc];
    long          PDefAction;                           // +0x1d4
    CArrowComponent* PLeftArrow;                        // +0x1d8
    CArrowComponent* PRightArrow;                       // +0x1dc
    C2DVector     PreviousValue;                        // +0x1e0
    void GetDataFromDef();
};
#pragma pack(pop)

void CSlider::GetDataFromDef()
{
    CDefPointer def = GetDef();
    SliderData[0] = def->SliderData[0];
    SliderData[1] = def->SliderData[1];
    *(long*)&Min.X = def->SliderData[2];
    *(long*)&Min.Y = def->SliderData[3];
    *(long*)&Max.X = def->SliderData[4];
    *(long*)&Max.Y = def->SliderData[5];
    *(long*)&Step.X = def->SliderData[6];
    *(long*)&Step.Y = def->SliderData[7];
    PreviousValue = Value = OldValue = StartValue = Min;
    Move = false;
    PDefAction = def->DefaultAction;
    if (def->BackAction) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->BackAction;
        AddAction(action);
    }
    CInlineActionPointer selectAction(new CInlineAction);
    selectAction.m_pObject->ActionType = 0x57;
    AddAction((const CCountedPointer<CAction>&)selectAction);
    if (def->DefaultAction) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->DefaultAction;
        Actions.push_back(action);
    }
    if (def->RightArrowDef) {
        PRightArrow = CFrontEndManager::GetInstance()->CreateComponent(def->RightArrowDef);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLONG(0x15));
        action->AddUIActionParam(&m_actionParamBase, action);
        PRightArrow->AddClickAction(action);
    }
    if (def->LeftArrowDef) {
        PLeftArrow = CFrontEndManager::GetInstance()->CreateComponent(def->LeftArrowDef);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLONG(0x14));
        action->AddUIActionParam(&m_actionParamBase, action);
        PLeftArrow->AddClickAction(action);
    }
    UpdateValue();
    PreviousValue = Value;
    if (Max.X < Min.X)
        Max.X = Min.X;
    if (!Actions.empty()) {
        CActionParam* param = new CActionParamFLOAT(Value.X);
        for (unsigned int i = 0; i < Actions.size(); ++i)
            Actions[i]->AddUIActionParam(param);
    }
}
}
