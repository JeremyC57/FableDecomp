// CSoundEmitterLH::~CSoundEmitterLH() -- the user destructor: reset the class vtable, stop every active sound (Shutdown), destroy the active-sound list (out-of-line clear, then the sentinel node freed), then ~CBaseClass
// Leaf: ??1CSoundEmitterLH@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC4F0, 49 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): vtable 0x0129E6EC stored first (RTTI .?AVCSoundEmitterLH@@ : CASoundEmitter : CBaseClass); its slot-0
//   `??_G` 0x00AACEA0 (landed) expands the same body; the landed clear() 0x00AACC60 names this row as the destructor that
//   clears this+0x14. CSoundEmitterLH is a TLC class absent from both PDBs: the class name is RTTI's, the destructor
//   spelling follows from the virtual ~CBaseClass. No callers (the ??_G has its own copy). The catalog label
//   "CTCInventoryItem::OnDie" is propagated.
// Callees: CSoundEmitterLH::Shutdown 0x00AAC2E0 (vtable slot 10, called directly as every virtual is inside a destructor;
//   descriptive name -- no PDB -- kept in the spelling the landed ??_G 0x00AACEA0 binds), std::_List_base<CCountedPointer<
//   CSoundEmitterLH::CActiveSound> >::clear 0x00AACC60 (landed leaf), free 0x00BFEA14 (the node allocator's deallocate),
//   CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: `virtual ~CSoundEmitterLH() { Shutdown(); }` out of line; ActiveSounds is the real STLport 4.6 list: `~list() {}`
//   inline, ~_List_base inline (clear() out of line, then the null-tested sentinel free through edi); CASoundEmitter
//   declares no destructor (no 0x0129E684 store), so the tail is ~CBaseClass.
// Layout: the landed ctors 0x00AAC530 / 0x00AAC580: manager +0x04, position +0x08, ActiveSounds +0x14 (4-byte STLport
//   list), three flag bytes +0x18..+0x1A, emitter id +0x1C (member names descriptive: no PDB).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#include "engine/CBaseClass.h"                             // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/CCountedPointer.h"                         // retail model (8 bytes: Data, Info)

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
    void clear();                                          // 0x00AACC60 (this instantiation)
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
}   // namespace std

#include "engine/C3DVector.h"
class CSoundManagerLH;

class CASoundEmitter : public CBaseClass {                 // vtable 0x0129E684 (abstract interface, no own destructor)
};

class CSoundEmitterLH : public CASoundEmitter {            // vtable 0x0129E6EC, 0x20 bytes
public:
    class CActiveSound;                                    // vtable 0x0129E67C
    CSoundEmitterLH(CSoundManagerLH* manager);             // 0x00AAC530
    virtual ~CSoundEmitterLH();
    void Shutdown();                                       // 0x00AAC2E0 (vtable slot 10; descriptive)
    CSoundManagerLH*                        m_pManager;    // +0x04
    C3DVector                               m_position;    // +0x08
    std::list<CCountedPointer<CActiveSound> > m_sounds;    // +0x14
    bool                                    m_active;      // +0x18
    bool                                    m_paused;      // +0x19
    bool                                    m_finished;    // +0x1a
    long                                    m_number;      // +0x1c
};

CSoundEmitterLH::~CSoundEmitterLH()
{
    Shutdown();
}