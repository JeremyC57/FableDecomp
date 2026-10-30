// CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w) -- read count packed C3DVectors from the stream into a temporary CArray and widen them into the C4DVector array with w as the fourth component
// Leaf: ?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1730, 391 bytes, protected static __fastcall (vectors ecx, count edx; in and w on
//   the stack), `ret 8`.
// Identity (proven, two sources): (1) Ego_r.pdb `?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z`
//   -- the only PDB function taking (C4DVector*, unsigned long, CDataInputStream&, float), exactly retail's register / stack
//   shape (`ret 8`: the stream reference and the float); (2) its two callers are CParticleSimulation::ReadFromFile 0x00AE1BC0
//   (call 0x00AE1D42) and CParticleProgram::ReadFromFile 0x00AE3C30 (call 0x00AE4043, w = 0), and the catalog row is
//   already named ReadVectorArray.
// Callees: CArray<C3DVector>::CArray(unsigned long) 0x007C2AB0 (bound `??0?$CArray@VC3DVector@@@@QAE@K@Z`),
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), free 0x00BFEA14 (~CArray).
// Shape: CArray::GetAsCArray() (`size() > 0 ? _M_start : GetZeroSizeArray()`, the zero-size pointer being the array object
//   itself: `lea edi,[esp+14h]`), GetByteSize() tested `> 0`, and the inline stream Read (64-bit position range check,
//   `size <= SourceChunkBytesRemaining` memcpy fast path, else the private overflow call -- the landed CThingComponentSet
//   0x008707A0 form; reached through the engine header's FABLE_CDATAINPUTSTREAM_FRIEND hook). The loop calls the inline
//   C4DVector::Initialise(const C3DVector&, float) (PDB): with an inline member call in the body VC7.1 unrolls the loop
//   by four (retail's `cmp ebx,4 ; jl` + remainder loop); the same stores written field by field, through a reference
//   local, as a while loop or with a signed index are never unrolled (234v391).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#define FABLE_CDATAINPUTSTREAM_FRIEND class CDataInputReader
#include "engine/CDataInputStream.h"
class CDataInputReader : public CDataInputStream {         // inline Read helper (no extra state)
public:
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class C3DVector { public: float X, Y, Z; };               // 12 bytes
class C4DVector {                                         // Ego_r / FableWin: 16 bytes
public:
    float X, Y, Z, W;
    void Initialise(const C3DVector& v, float w) { X = v.X; Y = v.Y; Z = v.Z; W = w; }   // ?Initialise@C4DVector@@QAEXABVC3DVector@@M@Z
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
};
}

template <class T> class CArray : public std::vector<T> {   // engine CArray over the STLport 4.6 vector (0xC bytes)
public:
    explicit CArray(unsigned long size);                          // 0x007C2AB0 (T = C3DVector)
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
    T* GetAsCArray() { return this->size() > 0 ? this->_M_start : const_cast<T*>(GetZeroSizeArray()); }
    unsigned int GetByteSize() const { return this->size() * sizeof(T); }
};

class CParticleSimulation {                                // Ego_r: CParticleSimulation (class)
protected:
    static void __fastcall ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w);
};

void __fastcall CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w)
{
    CArray<C3DVector> temp(count);
    C3DVector* data = temp.GetAsCArray();
    long bytes = temp.GetByteSize();
    if (bytes > 0)
        static_cast<CDataInputReader&>(in).Read(data, bytes);
    for (unsigned long i = 0; i < count; ++i)
        vectors[i].Initialise(temp[i], w);
}