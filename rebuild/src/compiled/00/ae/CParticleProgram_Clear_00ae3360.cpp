// CParticleProgram::Clear() -- empty the cloth particle program: clear the export-particle map, the render triangle / quad / vertex arrays, the constraint stream, both group arrays and the particle / vertex index arrays, zero the non-simulated particles and give the program's memory block back to its pool
// Leaf: ?Clear@CParticleProgram@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3360, 360 bytes, thiscall, plain `ret`.
// Identity (proven): `?Clear@CParticleProgram@@QAEXXZ` is in both the Ego_r.pdb and FableWin.pdb name sets and is the
//   spelling the landed ~CParticleProgram 0x00AE3670 binds for its call to this address (0x00AE367A); the other caller is
//   0x00AE3663. Every member touched sits at its Ego_r/retail offset (CParticleProgram, retail 0x140 bytes). The manifest
//   label "construct" is wrong.
// Callees: std::vector<std::pair<CCharString,unsigned long> >::erase 0x00AE6A20 (ExportParticles, out of line);
//   std::__copy<C3DTriangle2*,C3DTriangle2*,int> 0x00A744B0 (fastcall, RenderTris' erase); memmove 0x00BFEAE6 (the
//   trivial erase of the two CArray<long>); CMemoryDataOutputStream::Clear 0x00996B60 (Constraints);
//   std::vector<C3DGroup2>::erase 0x00AE7F30 (SimGroups / NonSimGroups, out of line);
//   CMovableResourceMemoryPool::FreeResource 0x00A62ED0 (a `jmp` to CHandleBasedMemoryPool::Free).
// Layout: engine/CParticleProgram.h flattens the members and the vptr, so the members are modelled locally with their PDB
//   names at the retail offsets (as the landed ~CParticleProgram 0x00AE3670 and ctor 0x00AE3550).
// Shape: each `clear()` is STLport 4.6 `erase(begin(), end())`: `__copy_ptrs(end, _M_finish, begin, _TrivialAss())`
//   (the trivially assignable long arrays through __copy_trivial / memmove with its `last == first` test on _M_finish
//   twice, the user-class element arrays through the random-access __copy loop), then the trivial _Destroy and the
//   _M_finish store. C3DQuad2 / C3DClothRenderVertex copies inline, C3DTriangle2's stays a call.
class C4DVector { public: float X, Y, Z, W; };
class C2DVector { public: float X, Y; };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class C3DQuad2 { public: unsigned short VertexIndex[4]; };
class C3DClothRenderVertex { public: unsigned short PositionIndex; unsigned short TexCoordIndex; };
class CParticleSimulationRenderData;

extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14

#include "engine/CCountedPointer.h"

class CMovableResource;
class CMovableResourceMemoryPool { public:                    // retail 0x464 bytes (CHandleBasedMemoryPool base)
    void FreeResource(long handle);                           // 0x00A62ED0
};

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};
template <> struct __type_traits<long> {
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
};
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                     _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class T> class CArray : public std::vector<T> { };   // retail CArray: an STLport vector (12 bytes)

class C3DVertexBlend2 { public: unsigned long VertexIndex; float BlendValue; };
class C3DGroup2 { public:                                     // 16 bytes in retail
    long BoneIndex;                                           // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                     // +0x04
};

class CCharStringData;
class CCharString { public:                                   // 4 bytes
    ~CCharString();                                           // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharStringData* PStringData;
};

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {   // retail 0x10
public:
    C    Compare;                                             // +0x0C
    bool Dirty;                                               // +0x0D
};

class CMemoryDataOutputStream { public:                       // retail 0x18 bytes, vtable
    virtual ~CMemoryDataOutputStream();                       // 0x00996B30
    void Clear();                                             // 0x00996B60
    unsigned char _pad_0x04[0x4];
    CArray<char> Data;                                        // +0x08
    unsigned long CurrentPosition;                            // +0x14
};

class CMovableResource { public:                              // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

class C3DBoundingBox { public: float Min[3]; float Max[3]; };

class CParticleSimulation : public CMovableResource { public:  // 0x80 bytes, vtable 0x0129F0C4
    CCountedPointer<CParticleSimulationRenderData> RenderData; // +0x04
    float                       Timestep;              // +0x0c
    bool                        TimestepChanged;       // +0x10
    float                       TimestepMultiplier;    // +0x14
    unsigned long               Size;                  // +0x18
    C4DVector*                  Positions;             // +0x1c
    C4DVector*                  LastPositions;         // +0x20
    C4DVector*                  SavedPositions;        // +0x24
    C4DVector*                  Accelerations;         // +0x28
    float*                      SimulationAlphas;      // +0x2c
    float                       MaximumAdjustment;     // +0x30
    float                       GravityStrength;       // +0x34
    float                       WindStrength;          // +0x38
    bool                        DraggingEnable;        // +0x3c
    bool                        DraggingRotational;    // +0x3d
    float                       DraggingStrength;      // +0x40
    bool                        AccelerationEnable;    // +0x44
    float                       GlobalDamping;         // +0x48
    double                      CurrentTime;           // +0x50
    C3DBoundingBox              BoundingBox;           // +0x58
    bool                        BoundingBoxValid;      // +0x70
    long                        MemoryHandle;          // +0x74
    CMovableResourceMemoryPool* MemoryPool;            // +0x78
    virtual ~CParticleSimulation();                    // 0x00AE0630
    void Clear();                                      // 0x00ADF1E0
};

class CParticleProgram : public CMovableResource { public:    // retail 0x140 bytes, vtable 0x0129F0D8
    class CLoopBegin { public: unsigned long Position; unsigned long Count; };   // 8 bytes (Ego_r)
    CParticleSimulation             InitialSimulation;     // +0x08
    CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > ExportParticles;   // +0x88
    CArray<C3DTriangle2>            RenderTris;            // +0x98
    CArray<C3DQuad2>                RenderQuads;           // +0xa4
    CArray<C3DClothRenderVertex>    RenderVertices;        // +0xb0
    unsigned long                   NonSimCount;           // +0xbc
    C4DVector*                      NonSimPositions;       // +0xc0
    CArray<C2DVector>               IndexedTextureCoords;  // +0xc4
    CArray<C4DVector>               TempVertexPositions;   // +0xd0
    CArray<long>                    ParticleIndices;       // +0xdc
    CArray<long>                    VertexIndices;         // +0xe8
    bool                            BezierEnable;          // +0xf4
    float                           AveragePatchSize;      // +0xf8
    CArray<C3DGroup2>               SimGroups;             // +0xfc
    CArray<C3DGroup2>               NonSimGroups;          // +0x108
    CMemoryDataOutputStream         Constraints;           // +0x114
    CArray<CLoopBegin>              LoopStack;             // +0x12c
    long                            MemoryHandle;          // +0x138
    CMovableResourceMemoryPool*     MemoryPool;            // +0x13c
    virtual ~CParticleProgram();
    void Clear();                                          // 0x00AE3360
};

void CParticleProgram::Clear()
{
    BezierEnable = false;
    AveragePatchSize = 0.0f;
    ExportParticles.clear();
    RenderTris.clear();
    RenderQuads.clear();
    RenderVertices.clear();
    Constraints.Clear();
    SimGroups.clear();
    NonSimGroups.clear();
    ParticleIndices.clear();
    VertexIndices.clear();
    NonSimCount = 0;
    NonSimPositions = 0;
    if (MemoryHandle != -1) {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
    }
}