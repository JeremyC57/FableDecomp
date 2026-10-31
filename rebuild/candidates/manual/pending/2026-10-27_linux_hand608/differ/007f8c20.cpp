// CTCAnimatingObject::AddQueuedAnimation(const CCharString& name, bool looping) -- when the object's def has an animation of this name, queue its first variant (an anim init of type 6, update mode 1, priority 100) at the back of QueuedAnims
// Grade: DIFFER(233v221) -- names now PDB-exact (AnimExists/GetAnim/CAnimInit::Init/PeekBaseDef, STLport list, CAnimFilterBase virtual dtor gives retail's base-vtable store); residue: (1) the local def release reads the refcount into a register (`mov esi,[eax+4] ; dec esi ; mov [eax],esi`) where retail does `dec dword ptr [ecx+4] ; jne` -- the parked file's signed `long RefCount` gave the RMW form (221 bytes), engine/CIVCountedPointeeBase.h's `mutable unsigned long IVRefCount` does not; (2) VC7.1 shrink-wraps esi/edi (pushed after the GetSubDef test; retail pushes all three up front and keeps this in edi).
// Leaf: ?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8C20, length 221 bytes (+ int3 padding), `ret 8`.
// Name: FableWin.pdb `?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z`.
// Callees: CDefClassBase::GetSubDef<CAnimatingObjectDef>(CDefPointer<const CAnimatingObjectDef>*) const @ 0x007F9330
// (on the thing's base def through the inline CThing::PeekBaseDef, +0x70), CAnimationSet::AnimExists(const CCharString&,
// const CAnimFilterBase&) const @ 0x005DC2E0 and CAnimationSet::GetAnim(const CCharString&, long, const CAnimFilterBase&)
// const @ 0x005DC340 (Ego_r name set: the only CAnimationSet members with those parameter lists; GetAnim returns a
// reference, `mov eax,[eax]` reads its BankIndex), CAnimInit::Init(unsigned char, unsigned long, long, EAnimUpdateMode,
// bool, unsigned long, long) @ 0x0070C050 (FableWin; the inline CAnimInit constructor with the same parameters zeroes
// Frame and calls it), malloc @ 0x00BFEA0E (the STLport list node), the def's virtual Delete (slot 1) at refcount zero.
// Identity: the one-word filter temporaries store vtable 0x0122FFDC = CAnimFilterNoGroup (RTTI; slot 0 operator(), slot 1
// Clone, slot 2 the deleting destructor); the second one's inline destructor stores the base vtable 0x0122FFCC =
// CAnimFilterBase (the first one's store is dead and dropped).
// Layout = Ego_r.pdb CTCAnimatingObject (32 bytes): CTCBase, Animating +0x0C, CurrentAnimationPriority +0x10, QueuedAnims
// +0x14 (STLport list: the node pointer); CAnimatingObjectDef Animation +0x28; CAnimInit 0x2C bytes (node 0x34).
// Shape: the shared engine/CDefPointer.h def handle; the entry is a named reference read after the filter temporary dies;
// the anim init is a named local copied into the node by the null-checked placement new (`rep movsd`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
// Layout: a local CDefPointer<T> (engine/CDefPointer.h's Release gives the register `mov/dec/mov/test` release here,
// retail releases with `dec dword ptr [ecx+4] ; jne` and no null store).
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object && --object->IVRefCount == 0)
            object->Delete();
    }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    CDefPointeeBase* Object;
};
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString { public:
    void* PStringData;
};
class CAnimationEntry { public:
    long BankIndex;                             // +0x00
};
class CAnimFilterBase { public:                 // vtable 0x0122FFCC
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;                        // slot 1
    virtual ~CAnimFilterBase() { }                                     // slot 2
};
class CAnimFilterNoGroup : public CAnimFilterBase { public:   // vtable 0x0122FFDC
    virtual bool operator()(const CAnimationEntry& entry) const;     // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                          // 0x00429A2F
};
class CAnimationSet { public:
    unsigned long Anims[4];                     // +0x00 CVectorMap<unsigned long, CAnimationEntry, ..> (retail 0x10)
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;                          // 0x005DC2E0
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

enum EAnimUpdateMode { };
class CAnimFrameInfo { public:
    float Current;                              // +0x00
    float Last;                                 // +0x04
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
};
class CAnimInit { public:                       // 0x2c bytes (Ego_r)
    EAnimUpdateMode UpdateMode;                 // +0x00
    unsigned char   Type;                       // +0x04
    bool            Looping;                    // +0x05
    long            Anim;                       // +0x08
    float           Speed;                      // +0x0c
    float           Alpha;                      // +0x10
    float           LastAlpha;                  // +0x14
    CAnimFrameInfo  Frame;                      // +0x18
    unsigned long   Flags;                      // +0x20
    unsigned long   TypeFlags;                  // +0x24
    long            Priority;                   // +0x28
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags,
              long priority)
    {
        Init(type, typeFlags, anim, mode, looping, flags, priority);
    }
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags,
              long priority);           // 0x0070C050
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                                 // +0x00
    _List_node_base* _M_prev;                                 // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                              // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _Tp, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        new ((void*)&__p->_M_data) _Tp(__x);
        return __p;
    }
    void _M_insert(_List_node_base* __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    void push_back(const _Tp& __x) { _M_insert(_M_node._M_data, __x); }
};
}   // namespace std

class CAnimatingObjectDef : public CDefClassBase { public:
    CAnimationSet Animation;                    // +0x28
};
class CThingBaseDef : public CDefClassBase { };
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;  // +0x70 (Ego_r +0x80)
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
};

class CTCAnimatingObject { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Animating;                          // +0x0c
    long    CurrentAnimationPriority;           // +0x10
    std::list<CAnimInit> QueuedAnims;           // +0x14
    void AddQueuedAnimation(const CCharString& name, bool looping);
};

void CTCAnimatingObject::AddQueuedAnimation(const CCharString& name, bool looping)
{
    CDefPointer<const CAnimatingObjectDef> def;
    if (ParentThing->PeekBaseDef().GetSubDef(&def)) {
        const CAnimationSet& set = def->Animation;
        if (set.AnimExists(name, CAnimFilterNoGroup())) {
            const CAnimationEntry& entry = set.GetAnim(name, 0, CAnimFilterNoGroup());
            CAnimInit init(6, 0, entry.BankIndex, (EAnimUpdateMode)1, looping, 0, 100);
            QueuedAnims.push_back(init);
        }
    }
}
