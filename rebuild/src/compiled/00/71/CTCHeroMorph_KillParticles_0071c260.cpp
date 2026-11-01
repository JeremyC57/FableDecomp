// CTCHeroMorph::KillParticles() -- kill and drop every morph particle emitter (back to front), then the idle particle emitters
// Leaf: ?KillParticles@CTCHeroMorph@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071C260, 110 bytes, thiscall, tail `jmp` to KillIdleParticles.
// Identity (proven): FableWin.pdb ?KillParticles@CTCHeroMorph@@QAEXXZ, whose donor neighbours (SetTan, KillIdleParticles,
//   KillParticles, HasAPermanentTattooCoveringArea, InitialiseCustomTattoos, OnInitialActivate, AddParticles) sit in the same
//   order in retail (0x0071C0C0 .. 0x0071C7C0; OnInitialActivate 0x0071C790 is CTCHeroMorph vtable 0x01265CF4 slot 6); the list
//   at +0x14 is Ego_r CTCHeroMorph::ParticleEmitters (after the 8-byte CCountedPointer PMeshMorphSet at +0xc) and the tail
//   target 0x0071C1F0 runs the same loop over +0x18 = IdleParticleEmitters (the 4-byte STLport list), i.e. KillIdleParticles.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (Ego_r/FableWin ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ),
//   CThing::Kill(bool) 0x004C9B80, free 0x00BFEA14 (the STLport node deallocate), the element's virtual destructor
//   (`push 0 ; call [vtbl]`), KillIdleParticles 0x0071C1F0 (tail).
// Shape: `while (!empty()) { iterator it = end(); --it; if ((*it).Get()) back()->Kill(true); pop_back(); }` over the real
//   STLport 4.6 list text (back() = *(--end()); pop_back = erase(--end()): unlink, _Destroy (explicit virtual dtor), deallocate).
//   The test reads the last node through a NAMED loop-scope iterator (`mov ecx,[eax+4] ; add ecx,8`), the Kill through
//   back()'s temporary (`mov eax,[ecx+4] ; lea ecx,[eax+8]`): back() for both, or an inline direct _M_prev read for both,
//   gives one shape at both sites (probes 1-10). KillIdleParticles 0x0071C1F0 has the same bytes over +0x18 and is written
//   the same way; retail calls it (tail jmp) and so does this TU.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_decr() { _M_node = _M_node->_M_prev; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    typedef _Tp& reference;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator end() { return iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& back() { return *(--end()); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
};
} // namespace std
class CThing : public CBaseObject { public:
    void Kill(bool);                                     // 0x004C9B80
};
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                  // slot 0 (scalar deleting)
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    CBaseObject* PItem;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};
class CTCAppearanceMorphBase { public: class CMeshMorphSet; };   // Ego_r nested type (opaque here)
class CTCHeroMorph : public CTCBase {
public:
    void KillIdleParticles();
    void KillParticles();
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;   // +0x0c
    std::list<CIntelligentPointer<CThing> >   ParticleEmitters;        // +0x14
    std::list<CIntelligentPointer<CThing> >   IdleParticleEmitters;    // +0x18
};
void CTCHeroMorph::KillIdleParticles()
{
    while (!IdleParticleEmitters.empty()) {
        std::list<CIntelligentPointer<CThing> >::iterator it = IdleParticleEmitters.end();
        --it;
        if ((*it).Get())
            IdleParticleEmitters.back()->Kill(true);
        IdleParticleEmitters.pop_back();
    }
}
void CTCHeroMorph::KillParticles()
{
    while (!ParticleEmitters.empty()) {
        std::list<CIntelligentPointer<CThing> >::iterator it = ParticleEmitters.end();
        --it;
        if ((*it).Get())
            ParticleEmitters.back()->Kill(true);
        ParticleEmitters.pop_back();
    }
    KillIdleParticles();
}