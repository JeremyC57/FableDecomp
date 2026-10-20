// CTCAnimatingObject::PlayDefaultAnimation() -- drop the queued animations and play the appearance's first "DEFAULT" animation (type 6, mode 1, looping, percent 100) with no priority
// Grade: DIFFER(ours v retail) -- scheduling only: the GetSubDef out-pointer zero store sits before the [+0x70] load in ours, after the push in retail (a base-def local first renames registers)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8F30, length 222 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (on the thing's base def, +0x70; result ignored),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&,
// long, const CAnimFilterBase&) @ 0x005DC340, free @ 0x00BFEA14 (the inlined STLport list clear), CAnimInit::Set @ 0x0070C050,
// CTCAnimatingObject::PlayAnimation(const CAnimInit&, long) @ 0x007F8DC0, the def's virtual Destroy (slot 1).
// Identity: the one-word filter temporary is CAnimFilterNoGroup (vtable 0x0122FFDC); its inline destructor stores the
// CAnimFilterBase vtable 0x0122FFCC.
// Layout: Ego_r.pdb CTCAnimatingObject QueuedAnims +0x14 (STLport list head node); CAppearanceDef Animation +0x34.
// Shape: the def is read from the out pointer as the object expression of the GetAnimation call (after the name string
// is built) and kept in ebx for the release at the end.
extern "C" void __cdecl free(void* p);                         // 0x00BFEA14

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

struct CBlendPair {
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

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct CRetailList {         // STLport list: the head node pointer
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
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
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
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
    void PlayAnimation(const CAnimInit& init, long priority);  // 0x007F8DC0
    void PlayDefaultAnimation();
};

void CTCAnimatingObject::PlayDefaultAnimation()
{
    CDefPointer<CAppearanceDef> out;
    ParentThing->PBaseDef->GetSubDef(&out);
    CAppearanceDef* def;
    const CAnimationEntry* entry;
    {
        CCharString name("DEFAULT");
        def = out.Object;
        entry = def->Animation.GetAnimation(name, 0, CAnimFilterNoGroup());
    }
    QueuedAnims.clear();
    CAnimInit init(6, 0, entry->AnimIndex, 1, true, 0, 100);
    PlayAnimation(init, -1);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}
