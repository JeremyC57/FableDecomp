// CSoundManagerLH::StopAllSounds() -- stop every sound of every emitter on the manager's emitter list, then of the manager's own emitter (descriptive name)
// Leaf: ?StopAllSounds@CSoundManagerLH@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EBC0, length 41 bytes (int3 padding from 0x00A3EBE9 to the next body 0x00A3EBF0),
//   thiscall, tail jump through the vtable.
// Identity: the only reference is slot 16 of vtable 0x0129D2C4 (RTTI CSoundManagerLH : CASoundManager : CBaseClassNonCopyable :
//   CBaseClass); the catalog "CNavigatorManager::InterruptAllNavigators" is a propagated label (wrong class). The sound module is
//   in neither PDB (no CASoundManager / CSoundManagerLH symbol), so the name is descriptive: slot 10 of each emitter (vtable
//   0x0129E6EC, body 0x00AAC2E0, catalog/bound name CSoundEmitterLH::Shutdown) stops every active sound on the sound system's
//   output (slot 22), marks it -1 and clears the active-sound list.
// Layout: the landed CSoundManagerLH ctor 0x00A3EF00 / slot 4 0x00A3ECB0: m_pMain (the manager's own CSoundEmitterLH) +0x44, the
//   STLport list of emitters +0x4c (slot 4 news a CSoundEmitterLH and push_backs it there).
// Shape: STLport list iteration (begin() = sentinel->_M_next, end() re-read from +0x4c on every pass, loop head 16-aligned
//   with `lea ecx,[ecx]`), `this` in edi, the node in esi; the final call is a sibling `jmp [edx+28h]`.
#include "engine/CBaseClassNonCopyable.h"

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class _Tp, class _Alloc>
class _List_base { public:
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> { public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
}

class CASoundEmitter : public CBaseClass { public:          // vtable 0x0129E684
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Shutdown();                                 // slot 10 (+0x28): the catalog's descriptive name (bound QAE, but virtual)
};
class CSoundEmitterLH : public CASoundEmitter { public:     // vtable 0x0129E6EC
    virtual void Shutdown();                                 // slot 10: 0x00AAC2E0 (stops every active sound, clears the list)
};

class CASoundManager : public CBaseClassNonCopyable { public:   // retail 0x40 bytes (landed ctor 0x00991910 model)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void StopAllSounds();                            // slot 16 (+0x40)
    unsigned char _body[0x40 - 4];
};

class CSoundSystem;
class CSoundReceiverLH;
class CSoundManagerLH : public CASoundManager { public:     // vtable 0x0129D2C4
    CSoundSystem*                 m_pSoundSystem;            // +0x40
    CSoundEmitterLH*              m_pMain;                   // +0x44
    CSoundReceiverLH*             m_pReceiver;               // +0x48
    std::list<CSoundEmitterLH*>   Sounds;                    // +0x4c
    virtual void StopAllSounds();
};

void CSoundManagerLH::StopAllSounds()
{
    for (std::list<CSoundEmitterLH*>::iterator it = Sounds.begin(); it != Sounds.end(); ++it)
        (*it)->Shutdown();
    m_pMain->Shutdown();
}