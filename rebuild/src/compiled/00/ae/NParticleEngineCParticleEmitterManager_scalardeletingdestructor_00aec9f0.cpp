// NParticleEngine::CParticleEmitterManager::`scalar deleting destructor'(unsigned int flags) -- destroy the emitter manager (clear the active-emitter list, free its sentinel) and free it when bit 0 of the flags is set; returns this
// Leaf: ??_GCParticleEmitterManager@NParticleEngine@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEC9F0, 45 bytes, __fastcall `ret 4`.
// Identity: FableWin.pdb `??_GCParticleEmitterManager@NParticleEngine@@QAEPAXI@Z` (non-virtual QAE: the class has no vtable,
//   so this is the helper VC7.1 emits for a `delete` of the class; no live direct callers are needed for the COMDAT).
// Callees: _List_base<CCountedPointer<CParticleEmitter> >::clear() 0x00AEC840, free 0x00BFEA14 (the list sentinel),
//   operator delete 0x00BFE9BC (flag bit 0).
// Layout: CParticleEmitterManager (0x1018) starts with its CParticleEmitterList, whose first member is the STLport list of
//   counted emitter pointers; the rest (Perlin noise table, counts, flags, engine pointer) has trivial destructors.
// Label: catalog "NEntityEvents_CEventList_Destroy" is propagated.
// Shape: VC7.1's own ??_G for a class with a user-declared (inline, empty) destructor, produced by the `delete (T*)data`
//   in CCountedPointer<CParticleEmitterManager>::DeleteData (0x00AECA50, same TU): ~CParticleEmitterManager ->
//   ~CParticleEmitterList -> STLport ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }, then
//   `if (flags & 1) operator delete(this)`.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

namespace NParticleEngine { class CParticleEmitter; }
struct CCountBlock;

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                           // 0x00AEC840
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
};
}

namespace NFractal {
class CPerlinNoise {
public:
    unsigned long IndexTable[256];                          // +0x000
    float         VectorTable[256][3];                      // +0x400 C3DVector[256]
};
}

class CIEngine;

namespace NParticleEngine {
class CParticleEmitterList {
public:
    _STL::list<CCountedPointer<CParticleEmitter> > Active;  // +0x0
    long MaxActiveEmitters;                                 // +0x4
    ~CParticleEmitterList() { }
};

class CParticleEmitterManager {
public:
    CParticleEmitterList  ActiveEmitters;                   // +0x0000
    NFractal::CPerlinNoise PerlinNoise;                     // +0x0008
    long                  MaxVisibleEmitters;               // +0x1008
    long                  VisibleEmitterCount;              // +0x100c
    bool                  Cleared : 1;                      // +0x1010 bit 0
    bool                  EnableDecals : 1;                 // +0x1010 bit 1
    CIEngine*             PEngine;                          // +0x1014
    ~CParticleEmitterManager() { }
};
}

template class CCountedPointer<NParticleEngine::CParticleEmitterManager>;