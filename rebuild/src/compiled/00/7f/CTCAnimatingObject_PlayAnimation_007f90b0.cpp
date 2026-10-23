// CTCAnimatingObject::PlayAnimation(const CCharString& name, long priority, bool looping) -- unless an animation of at least this priority is playing, drop the queue and play the first variant of the named animation from the object's def
// Leaf: ?PlayAnimation@CTCAnimatingObject@@QAEXABVCCharString@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F90B0, length 174 bytes, `ret 0Ch`.
// Callees: CDefClassBase::GetSubDef<CAnimatingObjectDef> @ 0x007F9330 (on the thing's base def, +0x70),
// CAnimationSet::HasAnimation(const CCharString&, const CAnimFilterBase&) @ 0x005DC2E0, CTCAnimatingObject::ClearQueuedAnims
// @ 0x007F8BE0, CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340, the index
// overload CTCAnimatingObject::PlayAnimation(long, long, bool) @ 0x007F9010, the def's virtual Destroy (slot 1).
// Label correction: both overloads take (priority, looping) -- Ego_r.pdb ?PlayAnimation@CTCAnimatingObject@@QAEXABVCCharString@@J_N@Z
// and ?PlayAnimation@CTCAnimatingObject@@QAEXJJ_N@Z (the earlier (bool, long) model read the wrong slot for the priority test).
// Identity: the filter temporaries are CAnimFilterNoGroup (vtable 0x0122FFDC); the second one's inline destructor stores the
// CAnimFilterBase vtable 0x0122FFCC. The first is homed in the GetSubDef frame word, the second in the dead `name` slot.
// Layout: Ego_r.pdb CTCAnimatingObject: Animating +0x0C, CurrentAnimationPriority +0x10, QueuedAnims +0x14;
// CAnimatingObjectDef Animation +0x28.
// Shape: a CDefPointer local with an inline DecRefToZero release (`dec [ebp+4] ; jne`); GetSubDef has a same-TU stand-in
// body that writes the out word only inline (the landed 0x00848730 / 0x00744ED0 idiom), so VC knows the def's address does
// not escape: the def stays in ebp from the call to the release and the out word is reused by the first filter; the base
// def is a named local (GetSubDef setup order), the set pointer a named local (lea edi,[ebp+28h]) and the entry a named
// local (the second filter dies before the index call, keeping its base-vtable dtor store).
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
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;                                // 0x005DC2E0
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
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* p = Object;
        if (p && DecRefToZero(p->RefCount))
            p->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CAnimatingObjectDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    CAnimationSet Animation;                    // +0x28
};
struct CThingBaseDef {
    bool GetSubDef_007f9330(CDefPointer<CAnimatingObjectDef>* out) const;   // 0x007F9330
};
struct CThing {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
};
void* FindDef_CThingBaseDef_GetSubDef();         // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007F9330 is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 / 0x0076C8A0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef_007f9330(CDefPointer<CAnimatingObjectDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

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
    void ClearQueuedAnims();                                   // 0x007F8BE0
    void PlayAnimation(long animIndex, long priority, bool looping);   // 0x007F9010
    void PlayAnimation(const CCharString& name, long priority, bool looping);
};

void CTCAnimatingObject::PlayAnimation(const CCharString& name, long priority, bool looping)
{
    if (Animating && priority <= CurrentAnimationPriority)
        return;
    CDefPointer<CAnimatingObjectDef> def;
    const CThingBaseDef* baseDef = ParentThing->PBaseDef;
    if (baseDef->GetSubDef_007f9330(&def)) {
        const CAnimationSet* set = &def->Animation;
        if (set->HasAnimation(name, CAnimFilterNoGroup())) {
            ClearQueuedAnims();
            const CAnimationEntry* entry = set->GetAnimation(name, 0, CAnimFilterNoGroup());
            PlayAnimation(entry->AnimIndex, priority, looping);
        }
    }
}