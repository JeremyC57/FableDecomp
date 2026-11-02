// CTCHeroMorph::FrameUpdate() -- drop the particle-emitter entries whose thing has gone, update the idle particles, then run the morph update
// Leaf: ?FrameUpdate@CTCHeroMorph@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071CD30, 87 bytes (int3 padding 0x0071CD87..8F), thiscall, tail `jmp [edx+70h]` (slot 28).
// Identity (proven, two sources): (1) the only reference is slot 9 of vtable 0x01265CF4 (RTTI .?AVCTCHeroMorph@@ :
//   CTCAppearanceMorphBase : CTCBase), and slot 9 is CTCBase::FrameUpdate (engine/CTCBase.h retail branch, 0x006868A0);
//   (2) Ego_r.pdb donor order UpdateIdleParticles (0x005679D0) -> FrameUpdate (0x00567D50) = retail UpdateIdleParticles 0x0071C9F0 (the
//   direct call here) -> this row; Ego_r / FableWin ?FrameUpdate@CTCHeroMorph@@UAEXXZ. The catalog "vfunc_9" is a placeholder.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, through the
//   inline CIntelligentPointer<CThing>::Get), the pointer's virtual destructor (`push 0 ; call [vtbl]`: STLport _Destroy on the node
//   data), free 0x00BFEA14 (the node allocator's deallocate), CTCHeroMorph::UpdateIdleParticles() 0x0071C9F0 (private, Ego_r AAE),
//   CTCAppearanceMorphBase slot 28 DoMorphUpdate (protected virtual; CTCHeroMorph's override is 0x0071E130).
// Shape: an STLport 4.6 list iterator loop over ParticleEmitters with the inline list::erase (unlink, _Destroy, deallocate) on an
//   empty pointer and ++it otherwise; begin() is tested against the loaded sentinel, end() re-read from +0x14 after each step.
// Layout: engine/CTCBase.h retail opt-in (FABLE_RETAIL_CTCBASE). CTCAppearanceMorphBase (vtable 0x01265B34: slot 21 GetInterfaceType
//   0x0071B790, 28 DoMorphUpdate 0x0071B7A0, 29 GetMeshTextureMorphs pure) adds PMeshMorphSet +0x0C. CTCHeroMorph's Ego_r members at
//   the offsets its ctor 0x0071E180 initialises (ParticleEmitters +0x14, IdleParticleEmitters +0x18, Tattoos +0x1C, CustomToBankIndex
//   +0x28, FrameTattoosChanged +0x34 ... Teenager +0x60); sizeof 0x64 = the `push 64h ; call operator new` in Alloc 0x004D28BB.
//   CBaseIntelligentPointer / CIntelligentPointer<T> as the landed chunk 706 rows (8-byte pointer class, natural alignment). No engine header
//   exists for CTCAppearanceMorphBase / CTCHeroMorph; containers are STLport 4.6 (list 4 bytes, vector / map 0xC).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};

namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
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
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return this->_M_node._M_data; }
    iterator erase(iterator __position) {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*) __position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};
}   // namespace std

class CTCAppearanceMorphBase : public CTCBase {         // vtable 0x01265B34
public:
    class CMeshMorphSet;
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21 0x0071B790
protected:
    virtual void DoMorphUpdate();                       // slot 28 0x0071B7A0
public:
    virtual CMeshMorphSet* GetMeshTextureMorphs() const = 0;   // slot 29
    CCountedPointer<CMeshMorphSet> PMeshMorphSet;       // +0x0C
};

class CTCHeroMorph : public CTCAppearanceMorphBase {    // vtable 0x01265CF4
public:
    virtual void FrameUpdate();
private:
    void UpdateIdleParticles();                         // 0x0071C9F0
public:
    std::list<CIntelligentPointer<CThing> > ParticleEmitters;       // +0x14
    std::list<CIntelligentPointer<CThing> > IdleParticleEmitters;   // +0x18
    unsigned char Tattoos_[0xC];                        // vector<CDefPointer<const CThingBaseDef> > Tattoos +0x1C
    unsigned char CustomToBankIndex_[0xC];              // map<CCharString, pair<long, CDateAndTime> > CustomToBankIndex +0x28
    long          FrameTattoosChanged;                  // +0x34
    long          FrameHeroBecameIdle;                  // +0x38
    bool          IdleParticlesAdded;                   // +0x3C
    bool          Changed;                              // +0x3D
    float         Strength;                             // +0x40
    float         Berserk;                              // +0x44
    float         Will;                                 // +0x48
    float         Skill;                                // +0x4C
    float         Age;                                  // +0x50
    float         Morality;                             // +0x54
    float         Fatness;                              // +0x58
    float         Tan;                                  // +0x5C
    bool          Teenager;                             // +0x60
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroMorph) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCHeroMorph, ParticleEmitters) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCHeroMorph, Teenager) == 0x60);

void CTCHeroMorph::FrameUpdate()
{
    for (std::list<CIntelligentPointer<CThing> >::iterator it = ParticleEmitters.begin(); it != ParticleEmitters.end(); ) {
        if (it->Get() == 0)
            it = ParticleEmitters.erase(it);
        else
            ++it;
    }
    UpdateIdleParticles();
    DoMorphUpdate();
}