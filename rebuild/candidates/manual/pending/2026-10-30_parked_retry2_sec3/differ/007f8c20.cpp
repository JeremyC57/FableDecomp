// CTCAnimatingObject::AddQueuedAnimation(const CCharString& name, bool flag) -- when the object's def has an animation of this type, queue its first variant (an anim init of type 6, mode 1, percent 100) at the back of QueuedAnims
// Grade: DIFFER(221v221) -- 62 difflines; STLport list model from landed 0x007B7C80 gives the length; residue = register colouring in the CDefPointer release + list insert; CAnimInit callee must be respelled Init (NAME_FIXES 0x0070C050)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8C20, length 221 bytes (+ int3 padding), `ret 8`.
// Callees: CDefClassBase::GetSubDef<CAnimatingObjectDef> @ 0x007F9330 (on the thing's base def, +0x70; the manifest
// instantiation name is a byte twin), CAnimationSet::HasAnimation(long, const CAnimFilter&) @ 0x005DC2E0 (auto label
// "CAnimationDef_FindAnimationByName"), CAnimationSet::GetAnimation(long, long, const CAnimFilter&) @ 0x005DC340 (auto
// label "Animation_FindNthValid"), CAnimInit::Set @ 0x0070C050, malloc @ 0x00BFEA0E (the STLport list node), the def's
// virtual Destroy (slot 1) when its reference count drops to zero.
// Identity: the one-word filter temporaries store vtable 0x0122FFDC = CAnimFilterNoGroup (RTTI); the second one's inline
// destructor stores the base vtable 0x0122FFCC = CAnimFilterBase (the first one's store is dead and dropped).
// Layout = Ego_r.pdb CTCAnimatingObject (32 bytes): CTCBase, Animating +0x0C, CurrentAnimationPriority +0x10, QueuedAnims
// +0x14 (STLport list: the head node pointer); CAnimatingObjectDef Animation +0x28; CAnimInit 0x2C bytes (node 0x34).
// Shape: the second filter is homed in the dead `name` slot (the reference is passed through in ebx); the anim init is a named local copied into the node
// by the null-checked placement new (`rep movsd`, a POD copy); the head node is read before the node is allocated.
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* PStringData;
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const = 0;
    virtual CAnimFilterBase* Clone() const = 0;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;                          // 0x005DC2E0
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

struct CBlendPair {                             // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
    void _M_hook(_List_node_base* position)
    {
        _List_node_base* previous = position->_M_prev;
        _M_next = position;
        _M_prev = previous;
        previous->_M_next = this;
        position->_M_prev = this;
    }
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct CRetailList {         // STLport list: the head node pointer (the landed 0x007B7C80 model)
    _List_node_base* _M_node;
    void push_back(const T& x) { _M_insert(_M_node, x); }
    void _M_insert(_List_node_base* position, const T& x)
    {
        _List_node<T>* node = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new ((void*)&node->_M_data) T(x);
        node->_M_hook(position);
    }
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CAnimatingObjectDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    CAnimationSet Animation;                    // +0x28
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<CAnimatingObjectDef>* out) const;   // 0x007F9330
};
struct CThing {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
};

class CTCAnimatingObject {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Animating;                          // +0x0c
    long    CurrentAnimationPriority;           // +0x10
    CRetailList<CAnimInit> QueuedAnims;         // +0x14
    void AddQueuedAnimation(const CCharString& name, bool flag);
};

void CTCAnimatingObject::AddQueuedAnimation(const CCharString& name, bool flag)
{
    CDefPointer<CAnimatingObjectDef> def;
    if (ParentThing->PBaseDef->GetSubDef(&def)) {
        const CAnimationSet* set = &def->Animation;
        if (set->HasAnimation(name, CAnimFilterNoGroup())) {
            CAnimInit init(6, 0, set->GetAnimation(name, 0, CAnimFilterNoGroup())->AnimIndex, 1, flag, 0, 100);
            QueuedAnims.push_back(init);
        }
    }
}
