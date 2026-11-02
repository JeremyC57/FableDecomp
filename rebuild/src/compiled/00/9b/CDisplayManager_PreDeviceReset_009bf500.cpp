// CDisplayManager::PreDeviceReset() -- before the D3D device is reset: uninitialise the render manager when there is one, let the vsync callback timer drop its queries, tell every registered device-reset callback, then the system manager
// Leaf: ?PreDeviceReset@CDisplayManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BF500, 66 bytes, thiscall, tail `jmp CSystemManager::OnPreDeviceReset` 0x009A52B0.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb ?PreDeviceReset@CDisplayManager@@AAEXXZ (private; Ego_r 0x00B24AB0, right
//   before PostDeviceReset 0x00B24B00 and ResetDevice 0x00B24B50, as retail's 0x009BF500 sits right before the landed PostDeviceReset
//   0x009BF550); its only caller is the landed CDisplayManager::ResetDevice 0x009BFB30 (call at 0x009BFB4E, before the device
//   Reset), which binds it as `?PreDeviceReset@CDisplayManager@@QAEXXZ` (public: wrong access, NAME_FIXES). The body mirrors
//   PostDeviceReset: render manager, callbacks, system manager.
// Callees: ?Uninit@CRenderManagerCore@@QAEXXZ 0x00A0A9D0 (Ego_r 0x00B73F80; PRenderManager is a scoped_ptr<CRenderManager>, and
//   CRenderManager : CRenderManager2D : CRenderManagerCore at +0 per RTTI), ?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ 0x009F73A0
//   (Ego_r 0x00B4ADA0), each CDeviceResetCallback's virtual slot 0 OnPreDeviceReset (slot 1 is OnPostDeviceReset, the bool the landed
//   PostDeviceReset calls; the PDB overrides CMainGameComponent / CMeshBankManager / CFrontendGameComponent::OnPreDeviceReset UAEXXZ /
//   OnPostDeviceReset UAE_NXZ), ?OnPreDeviceReset@CSystemManager@@QAEXXZ 0x009A52B0 (an empty `ret`; Ego_r 0x00AF3280 /
//   OnPostDeviceReset 0x00AF3290 = retail 0x009A52B0 / 0x009A52C0) as the tail.
// Layout: Ego_r.pdb CDisplayManager (0x2EC) with the retail 4-byte STLport list: SystemManager +4, PRenderManager +8,
//   DeviceResetCallbacks (list<CDeviceResetCallback*>) +0x264, LastDeviceCreationParameters +0x268, VsyncCallbackTimer
//   (scoped_ptr<CVsyncCallbackTimer>) +0x294 (Ego_r +0x29C less the 8 list bytes); retail sizeof 0x2C8 (`push 2C8h` before the ctor
//   call at 0x009A67D3). RTTI CDisplayManager : CBaseClassNonCopyable : CBaseClass. No engine header for CDisplayManager; the members
//   this body does not touch are opaque storage at their retail offsets. CRenderManagerCore comes through the layout-rename include
//   of engine/CRenderManagerCore.h (flattened vptr, `struct` key; reached only through a pointer); CRenderManager2D / CRenderManager /
//   CVsyncCallbackTimer / CSystemManager have no shared header and are pointer-only declarations.
// Shape: the STLport list walk `for (it = begin(); it != end(); ++it)` keeps the node in esi and re-reads the header member after
//   every virtual call (the first compare uses the begin() load).
#include "engine/CBaseClassNonCopyable.h"                  // CBaseClassNonCopyable : CBaseClass

namespace boost {                                           // boost::scoped_ptr (the members this body uses)
template <class T> class scoped_ptr {
    T* ptr;
public:
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};
}

namespace std {                                             // STLport 4.6 list (_list.h), the members this body expands
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;           // _Alloc_traits<_Node, _Alloc>::allocator_type
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0 the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
}

class CSystemManager { public:
    void OnPreDeviceReset();                                // 0x009A52B0 ?OnPreDeviceReset@CSystemManager@@QAEXXZ
};
#define CRenderManagerCore CRenderManagerCoreLayout
#include "engine/CRenderManagerCore.h"                      // the generated (flattened, Ego_r) layout; reached only through a pointer here
#undef CRenderManagerCore
class CRenderManagerCore : public CRenderManagerCoreLayout { public:
    void Uninit();                                          // 0x00A0A9D0 ?Uninit@CRenderManagerCore@@QAEXXZ
};
class CRenderManager2D : public CRenderManagerCore { };     // RTTI: CRenderManager : CRenderManager2D : CRenderManagerCore (+0)
class CRenderManager : public CRenderManager2D { };
class CVsyncCallbackTimer { public:
    void OnPreDeviceReset();                                // 0x009F73A0 ?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ
};
class CDeviceResetCallback { public:
    virtual void OnPreDeviceReset() = 0;                    // slot 0
    virtual bool OnPostDeviceReset() = 0;                   // slot 1
};

class CDisplayManager : public CBaseClassNonCopyable {      // vtable 0x0129B3F8, retail sizeof 0x2C8
public:
    CSystemManager*                         SystemManager;          // +0x004
    boost::scoped_ptr<CRenderManager>       PRenderManager;         // +0x008
    unsigned long                           Members00C_[(0x264 - 0x0C) / 4];   // +0x00C RenderTargetSurface .. RenderManagerInitFlags
    std::list<CDeviceResetCallback*>        DeviceResetCallbacks;   // +0x264
    unsigned long                           LastDeviceCreationParameters_[(0x294 - 0x268) / 4];   // +0x268 CCreateDeviceParameters
    boost::scoped_ptr<CVsyncCallbackTimer>  VsyncCallbackTimer;     // +0x294
    unsigned long                           Members298_[(0x2C8 - 0x298) / 4];  // +0x298 BackBufferFormats .. m_bTakingScreenShot
private:
    void PreDeviceReset();
};

void CDisplayManager::PreDeviceReset()
{
    if (PRenderManager.get())
        PRenderManager->Uninit();
    VsyncCallbackTimer->OnPreDeviceReset();
    for (std::list<CDeviceResetCallback*>::iterator it = DeviceResetCallbacks.begin(); it != DeviceResetCallbacks.end(); ++it)
        (*it)->OnPreDeviceReset();
    SystemManager->OnPreDeviceReset();
}