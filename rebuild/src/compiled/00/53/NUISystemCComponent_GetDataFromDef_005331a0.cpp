// NUISystem::CComponent::GetDataFromDef() -- read the component's UI def: queue its destruction action, copy the type/flag bits and layer, insert every def state into the state map (keyed by index), create and attach every child component, and copy the shape-children list
// Grade: RELOCATION_MATCH -- the last scheduling diff (pair key stored right after the hint-slot push instead of last before the
//   call) closes with a same-TU non-template body for the map's insert_unique (explicit specialisation, auto_inline off): VC7.1
//   then sees the value reference does not escape, so the pair stores no longer have to precede the hint's arg-slot copy.
// Leaf: ?GetDataFromDef@CComponent@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) NUISystem::CComponent::GetDataFromDef @ 0x005331A0, 756 bytes, plain ret.
// Callees: vtable 0x0124608C slots 108 LoadAndReturnDef 0x00530EC0, 130 AddActionOnDestruction 0x00532D00, 59 AddChild 0x00533B50;
//   CAction() 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29, CUIState(const CUIStateDef&) 0x0052F440,
//   map<unsigned long,CUIState> _Rb_tree::insert_unique(iterator, const value_type&) 0x0042D2C6, NUISystem::CManager::GetInstance 0x0041E5F2,
//   CreateComponent 0x0041D21B, CCountedPointer<CComponent>::DeleteData 0x00428B10, vector<unsigned long>::operator= 0x0045BC09.
// Layout: CComponent as the landed CComponent::Initialise 0x005339B0 (States +0x24, ShapeChildren +0xe4, Flags..Layer +0x12c..+0x12f);
//   CUIDef = Ego_r.pdb CUIDef - 4 up to States, then -4 per 12-byte STLport container (Children +0x70, Layer +0xb4, ActionOnDestruction +0xd4,
//   DrawFromViewport +0x188, LayerIndependant +0x1dc, BastardChild +0x1f8, UseRelativeZoom/Position +0x208/+0x209, ShapeChildren +0x214).
// Shape: map::insert inline over an out-of-line insert_unique (same-TU body below) (hint built in a register, copied into the arg slot); STLport
//   vector operator[] as *(begin() + n) gives retail's ecx reuse in the child loop; a named CUIState local (a temporary copies through eax).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
};
template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned char       _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator insert_unique(iterator position, const V& value);    // 0x0042D2C6
};
template <class K, class T> class map {                     // STLport map (12 bytes)
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree<K, value_type> _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator insert(iterator position, const value_type& value) { return _M_t.insert_unique(position, value); }
};
template <class T> class vector {                           // STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int i) { return *(begin() + i); }
    vector& operator=(const vector& other);                 // 0x0045BC09 (vector<unsigned long>)
};
}

class CCPPointerInfo { public:                                     // counted-pointer block
    long RefCount;                                          // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    static void __fastcall DeleteData(void* data);
    CCountedPointer(T* data)
        : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, DeleteData);
        else
            Info = 0;
    }
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

struct CRefCounted {                                        // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() { }
    CDefPointer(const CDefPointer& other);
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    T* operator->() const { return m_p; }
};

namespace NUISystem {
struct C2DVector { float X, Y; };
struct CRGBColour {                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); }
};
class CUIStateDef { public: unsigned char _data[0x7c]; };          // retail 0x7c bytes (Ego_r.pdb 0x84)
class CUIState { public:                                           // 0x24 bytes
    unsigned long StateChangeFlag;                          // +0x00
    long          StateChangeType;                          // +0x04
    C2DVector     Position;                                 // +0x08
    C2DVector     Zoom;                                     // +0x10
    CRGBColour    Colour;                                   // +0x18
    float         UpdateTime;                               // +0x1c
    bool          LinearChange;                             // +0x20
    CUIState(const CUIStateDef& def);                       // 0x0052F440
};
class CUIDef : public CDefBase { public:                                  // Ego_r.pdb CUIDef - 4 up to States, then the 12-byte STLport containers
    unsigned char _pad08[0x3c - 0x08];
    long          Type;                                     // +0x3c EType
    std::vector<CUIStateDef> States;                        // +0x40
    unsigned char _pad4c[0x70 - 0x4c];
    std::vector<long> Children;                             // +0x70
    unsigned char _pad7c[0xb4 - 0x7c];
    long          Layer;                                    // +0xb4
    float         Angle;                                    // +0xb8
    bool          PositionIsCenter;                         // +0xbc
    bool          TextLineBreak;                            // +0xbd
    bool          ScaleText;                                // +0xbe
    bool          Independant;                              // +0xbf
    unsigned char _padc0[0xd4 - 0xc0];
    long          ActionOnDestruction;                      // +0xd4 EActionType
    unsigned char _padd8[0x188 - 0xd8];
    bool          DrawFromViewport;                         // +0x188
    unsigned char _pad189[0x1dc - 0x189];
    bool          LayerIndependant;                         // +0x1dc
    unsigned char _pad1dd[0x1f8 - 0x1dd];
    bool          BastardChild;                             // +0x1f8
    unsigned char _pad1f9[0x208 - 0x1f9];
    bool          UseRelativeZoom;                          // +0x208
    bool          UseRelativePosition;                      // +0x209
    unsigned char _pad20a[0x214 - 0x20a];
    std::vector<unsigned long> ShapeChildren;               // +0x214
};
class CAction { public:                                            // 16 bytes
    long          ActionType;                               // +0x00 EActionType
    void*         UIActionParams;                           // +0x04 list<CActionParam*>
    void*         ThingActionParams;                        // +0x08 list<long>
    bool          Valid;                                    // +0x0c
    CAction();                                              // 0x0042BE50
};
class CComponent;
class CManager { public:
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
    CComponent* CreateComponent(long defIndex);             // 0x0041D21B
};
}
template <> CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction* data);   // 0x0042AA29
template <> void __fastcall CCountedPointer<NUISystem::CComponent>::DeleteData(void* data);   // 0x00428B10

namespace NUISystem {
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChild(const CCountedPointer<CComponent>& child);           // slot 59 (+0xec) 0x00533B50
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<CUIDef> LoadAndReturnDef();                            // slot 108 (+0x1b0) 0x00530EC0
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void AddActionOnDestruction(const CCountedPointer<CAction>& action); // slot 130 (+0x208) 0x00532D00
    unsigned char _pad04[0x24 - 0x04];
    std::map<unsigned long, CUIState> States;               // +0x24
    unsigned char _pad30[0xe4 - 0x30];
    std::vector<unsigned long> ShapeChildren;               // +0xe4
    unsigned char _padf0[0x12c - 0xf0];
    unsigned char Flags;                                    // +0x12c
    unsigned char Type;                                     // +0x12d
    unsigned char FlagsSetTwo;                              // +0x12e
    char          Layer;                                    // +0x12f
    long          DefIndex;                                 // +0x130
    void GetDataFromDef();
};

}
// Same-TU stand-in for the out-of-line insert_unique (0x0042D2C6, a size-optimised tree TU). It must be a NON-template body
// (explicit specialisation) that only READS the value: with the extern declaration, or a body passing the value on to an
// extern _M_insert, VC7.1 treats the pair temporary as escaped and orders every pair store ahead of the hint's arg-slot copy.
#pragma auto_inline(off)
template <> std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState> >::iterator
std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState> >::insert_unique(iterator position, const std::pair<const unsigned long, NUISystem::CUIState>& value)
{
    if (_M_node_count != 0 && value.first < *(const unsigned long*)(position._M_node + 1))
        return position;
    return iterator(_M_header);
}
#pragma auto_inline(on)
namespace NUISystem {
void CComponent::GetDataFromDef()
{
    CDefPointer<CUIDef> def = LoadAndReturnDef();
    if (def->ActionOnDestruction) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = def->ActionOnDestruction;
        AddActionOnDestruction(action);
    }
    Flags = (unsigned char)def->Type & 0x1f;
    Type = (unsigned char)def->Type;
    if (def->BastardChild)
        Flags |= 0x80;
    if (def->Independant)
        Flags |= 0x40;
    if (def->LayerIndependant)
        Flags |= 0x20;
    if (def->DrawFromViewport)
        FlagsSetTwo |= 0x01;
    if (def->PositionIsCenter)
        FlagsSetTwo |= 0x02;
    if (def->UseRelativeZoom)
        FlagsSetTwo |= 0x40;
    if (def->UseRelativePosition)
        FlagsSetTwo |= 0x80;
    Layer = (char)def->Layer;
    for (unsigned long i = 0; i < def->States.size(); ++i) {
        CUIState state(def->States[i]);
        States.insert(States.begin(), std::pair<const unsigned long, CUIState>(i, state));
    }
    for (unsigned long j = 0; j < def->Children.size(); ++j) {
        CCountedPointer<CComponent> child(CManager::GetInstance()->CreateComponent(def->Children[j]));
        AddChild(child);
    }
    ShapeChildren = def->ShapeChildren;
}

}