// CThingPhysical::~CThingPhysical() -- store the physical-thing vtable, take the thing off the thing manager's physical list, destroy the two creator pointers and the health-over-time list, then ~CThing
// Leaf: ??1CThingPhysical@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5D50, 62 bytes, thiscall, tail `jmp ~CThing` 0x004C8310.
// Identity (proven): the first store is vtable 0x01244E84 (RTTI .?AVCThingPhysical@@ : CThing : CClass : CBaseObject : CBaseClass);
//   Ego_r.pdb / FableWin.pdb ??1CThingPhysical@@UAE@XZ (a user dtor: the store and the RemoveFromPhysicalList body). Callers: the
//   slot-0 `??_G` 0x005297D0 of 0x01244E84 and the derived dtors' tail jumps (the landed ~CThingGameObject 0x00838A00 binds this
//   spelling). The catalog label CTCFishingSpot::~CTCFishingSpot is propagated; the landed ??_G binds the stand-in
//   `?Destroy@CEngineSubPrimitiveGameMesh@@QAEXXZ` (NAME_FIXES.tsv).
// Callees: CThingManager::RemoveFromPhysicalList(CThingPhysical&) 0x0051DFE0 (Ego_r / FableWin
//   ?RemoveFromPhysicalList@CThingManager@@QAEXAAVCThingPhysical@@@Z; called only here: the landed 0x00838A00 donor-order proof) on
//   CThing::PCurrentThingManager 0x013B8A1C (private static, through the protected inline CThing::GetThingManager()),
//   CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (x2), std::_List_base<CThingPhysical::CHealthOverTimeHelper>::
//   ~_List_base 0x005283C0 (clear inlined, out of line; its ~list thunk 0x00528C40 jumps to it), CThing::~CThing 0x004C8310 (tail).
// Layout: engine/CThingPhysical.h retail offsets (sizeof 0xC0): HealthOverTimeEffects (list<CHealthOverTimeHelper>, 4) +0x98,
//   Scale +0x9C, PLastCreator +0xA0, POriginalCreator +0xA8 (CIntelligentPointer<CThingPhysical>, 8 each), MaxHealth +0xB0,
//   Health +0xB4, CanComeBetweenCameraAndObservedThing +0xB8, the Damageable.. bit-field byte +0xBC. The header flattens the vptr
//   and CThing has no header, so the polymorphic chain is local over engine/CClass.h's retail opt-in (the landed ~CThingGameObject
//   0x00838A00 pattern: the header included under a layout name, the size asserted against it); CThing's own members (+0x14 ..
//   +0x98: UniqueID .. the flag bytes) stay opaque storage.
// Shape: own vtable store, the user body `GetThingManager().RemoveFromPhysicalList(*this);`, the members in reverse order (the
//   real STLport 4.6 list text: VC7.1 keeps this instantiation's ~_List_base out of line), the base dtor as the tail.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"                                 // CClass : CBaseObject : CBaseClass, 0x14 bytes
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"                         // retail offsets / sizeof (flattened vptr)
#undef CThingPhysical

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
}   // namespace std

class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                         // 0x00A01B50
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CThingPhysical;
class CThingManager {
public:
    void RemoveFromPhysicalList(CThingPhysical& thing);    // 0x0051DFE0
};

class CThing : public CClass {                             // RTTI .?AVCThing@@ : CClass; retail sizeof 0x98 (CThingPhysical members start at +0x98)
public:
    virtual ~CThing();                                     // 0x004C8310
protected:
    CThingManager& GetThingManager() { return *PCurrentThingManager; }   // Ego_r / FableWin ?GetThingManager@CThing@@IAEAAVCThingManager@@XZ (inline)
private:
    static CThingManager* PCurrentThingManager;            // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A
    unsigned char CThingMembers_[0x98 - 0x14];             // UniqueID +0x18 .. the CThing flag bytes (Ego_r CThing, retail STLport sizes)
};

class CThingPhysical : public CThing {                     // vtable 0x01244E84, 0xC0 bytes
public:
    class CHealthOverTimeHelper {                          // Ego_r.pdb, 8 bytes
    public:
        long  FramesLeftForAddingHealth;                   // +0x0
        float HealthLeftToAdd;                             // +0x4
    };
    virtual ~CThingPhysical();
    std::list<CHealthOverTimeHelper>   HealthOverTimeEffects;     // +0x98
    float                              Scale;                     // +0x9c
    CIntelligentPointer<CThingPhysical> PLastCreator;             // +0xa0
    CIntelligentPointer<CThingPhysical> POriginalCreator;         // +0xa8
    float                              MaxHealth;                 // +0xb0
    float                              Health;                    // +0xb4
    long                               CanComeBetweenCameraAndObservedThing;   // +0xb8
    unsigned char                      Damageable : 1;            // +0xbc
    unsigned char                      Killable : 1;
    unsigned char                      RespondToHits : 1;
    unsigned char                      EvadeAllHits : 1;
    unsigned char                      NegateAllHits : 1;
    unsigned char                      AbleToBeEngagedInCombat : 1;
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysical) == sizeof(CThingPhysicalFields));

CThingPhysical::~CThingPhysical()
{
    GetThingManager().RemoveFromPhysicalList(*this);
}