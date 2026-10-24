// NParticleEngine::CParticleEmitterManager::CParticleEmitterManager() -- the particle engine's emitter manager: an empty active-emitter list capped at 128, the Perlin noise table seeded with 0, 32 visible emitters, decals enabled, no engine yet
// Leaf: ??0CParticleEmitterManager@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEBA90, 93 bytes, __fastcall, returns this.
// Identity: Ego_r.pdb `??0CParticleEmitterManager@NParticleEngine@@QAE@XZ`; the only caller is NParticleEngine::Initialise
//   0x00AEBE57 (`new CParticleEmitterManager`, operator new(0x1018)).
// Callees: malloc 0x00BFEA0E (the inline CParticleEmitterList(0x80) list sentinel), NFractal::CPerlinNoise::CPerlinNoise(
//   unsigned long seed) 0x00F36BE0 (manifest label propagated).
// Layout: Ego_r.pdb CParticleEmitterManager (0x1020) with the retail 8-byte CParticleEmitterList (STLport list = one
//   pointer): ActiveEmitters +0, PerlinNoise +8 (0x1000), MaxVisibleEmitters +0x1008, VisibleEmitterCount +0x100c, the
//   Cleared / EnableDecals bit-fields +0x1010, PEngine +0x1014 = 0x1018 (the operator new size).
// Label: catalog "NScript_CPredicate_And_Initialize" is propagated.
// Shape: the list's inline constructor (sentinel malloc'd and self-linked after the proxy is zeroed), the cap, the noise
//   table; the bit-field assignments of the body are read-modify-write (`and al,0FEh ; or al,2`) and VC7.1 schedules
//   them ahead of the plain initializer-list stores.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace NParticleEngine { class CParticleEmitter; }
struct CCountBlock;
template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
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
    typedef _Alloc allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator_type& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

namespace NFractal {
class CPerlinNoise {
public:
    unsigned long IndexTable[256];                          // +0x000
    float         VectorTable[256][3];                      // +0x400 C3DVector[256]
    CPerlinNoise(unsigned long seed);                       // 0x00F36BE0
};
}

class CIEngine;

namespace NParticleEngine {
class CParticleEmitterList {
public:
    _STL::list<CCountedPointer<CParticleEmitter> > Active;  // +0x0
    long MaxActiveEmitters;                                 // +0x4
    CParticleEmitterList(long maxActiveEmitters)
        : MaxActiveEmitters(maxActiveEmitters)
    {
    }
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
    CParticleEmitterManager();
};

CParticleEmitterManager::CParticleEmitterManager()
    : ActiveEmitters(0x80), PerlinNoise(0), MaxVisibleEmitters(32), VisibleEmitterCount(0), PEngine(0)
{
    Cleared = false;
    EnableDecals = true;
}
}