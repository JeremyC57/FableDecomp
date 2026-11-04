// CTCHeroReceiveItems::OnKill() -- kill every still-alive item thing the hero was about to receive, then the base component's OnKill
// Leaf: ?OnKill@CTCHeroReceiveItems@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CA630, 62 bytes (int3 padding 0x007CA66E..F), thiscall, tail `jmp CTCBase::OnKill` 0x006868F0.
// Identity (proven, two sources): (1) the only reference is slot 14 (CTCBase OnKill, engine/CTCBase.h) of vtable 0x0126EBFC, RTTI
//   .?AVCTCHeroReceiveItems@@ : CTCBase; the tail target 0x006868F0 is CTCBase::OnKill (slot 14 of CTCBase 0x0125BE0C);
//   (2) Ego_r.pdb / FableWin.pdb ?OnKill@CTCHeroReceiveItems@@UAEXXZ, and the loop walks Ego_r's first member Items
//   (list<pair<CIntelligentPointer<CThingObject>, bool> >) at +0x0C. The forge label is a stand-in.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ; through
//   the inline CIntelligentPointer<CThingObject>::Get), CThing::Kill(bool) 0x004C9B80 (?Kill@CThing@@QAEX_N@Z), CTCBase::OnKill()
//   0x006868F0.
// Shape: STLport 4.6 list iterator loop (`begin()` against the cached header for the first test, `end()` re-read from the
//   member after each Kill: `cmp esi,[edi+0Ch]`); the item pointer is fetched once per node into a local and tested with the PDB
//   template IS_THING_ALIVE<CThingObject*> (??$IS_THING_ALIVE@PAVCThingObject@@@@YI_NABQAVCThingObject@@@Z, inline in release;
//   body written over the inline ?IsDead@CThing@@QBE_NXZ). The `lea ecx,[ecx]` at the loop head is VC7.1's alignment filler.
// Layout: engine/CTCBase.h retail opt-in (FABLE_RETAIL_CTCBASE); Items +0x0C is a 4-byte STLport list (node {next, prev,
//   pair{CIntelligentPointer first +8, bool second +0x10}}); no engine header exists for CTCHeroReceiveItems.
//   Every retail member is present (Ego_r order; offsets and sizeof 0xAC from the retail ctor 0x007CA830 and its `push 0ACh`
//   allocation); the two CEnginePrimitiveHandle members are opaque 8-byte storage (no model yet). CThingObject's
//   chain (RTTI CThingObject : CThingGameObject : CThingPhysical : CThing : CClass : CBaseObject) restated over
//   engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT) and the engine/CClass.h layout-rename; CThing::Dead = bit 0 of +0x91.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
    void clear();                                                 // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return this->_M_node._M_data; }
};
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { };
}

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
};

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char _pad14[0x91 - 0x14];
    unsigned char Dead : 1;                             // +0x91 bit 0 (Ego_r CThing::Dead)
    bool IsDead() const { return Dead; }                // FableWin ?IsDead@CThing@@QBE_NXZ (inline in release)
    void Kill(bool immediately);                        // 0x004C9B80
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingObject : public CThingGameObject { };

#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

class C2DBoxF {                                         // Ego_r 0x10
public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
};
#include "engine/C3DVector.h"
class CRightHandedSet {                                 // Ego_r 0x18
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

class CTCHeroReceiveItems : public CTCBase {            // vtable 0x0126EBFC
public:
    virtual void OnKill();
    std::list<std::pair<CIntelligentPointer<CThingObject>, bool> > Items;     // +0x0C
    std::vector<long>                     PickedUpItems;              // +0x10
    CIntelligentPointer<CThingObject>     PCurrentItem;               // +0x1C
    bool                                  PersistCurrentItem;         // +0x24
    bool                                  ReceivingItems;             // +0x25
    bool                                  DisplayingItems;            // +0x26
    bool                                  ForceClear;                 // +0x27
    bool                                  SendMessageW;               // +0x28 (Ego_r spelling)
    CIntelligentPointer<CThing>           PContainer;                 // +0x29 (1-aligned pointer class)
    float                                 AmountThroughForAreaResize; // +0x34
    long                                  FullSizeTimer;              // +0x38
    C2DBoxF                               InitialAreaItemTook;        // +0x3C
    C2DBoxF                               OldAreaItemTaking;          // +0x4C
    C2DBoxF                               CurrentAreaItemTaking;      // +0x5C
    CRightHandedSet                       OldItemRHSet;               // +0x6C
    CRightHandedSet                       CurrentItemRHSet;           // +0x84
    unsigned char                         ItemPrimitiveHandle_[0x8];       // CEnginePrimitiveHandle ItemPrimitiveHandle +0x9C (opaque: vptr 0x0122F598 + CIVCountedPointer)
    unsigned char                         BackgroundPrimitiveHandle_[0x8]; // CEnginePrimitiveHandle BackgroundPrimitiveHandle +0xA4 (opaque)
};

FABLE_STATIC_ASSERT(sizeof(CTCHeroReceiveItems) == 0xAC);  // retail `push 0ACh ; call operator new` before the ctor 0x007CA830
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, PCurrentItem) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, PContainer) == 0x29);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, AmountThroughForAreaResize) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, OldItemRHSet) == 0x6C);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, ItemPrimitiveHandle_) == 0x9C);

void CTCHeroReceiveItems::OnKill()
{
    for (std::list<std::pair<CIntelligentPointer<CThingObject>, bool> >::iterator it = Items.begin(); it != Items.end(); ++it) {
        CThingObject* item = it->first.Get();
        if (IS_THING_ALIVE(item))
            item->Kill(true);
    }
    CTCBase::OnKill();
}