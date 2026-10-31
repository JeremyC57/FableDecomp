// CParticleSimulation::WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out) -- narrow count C4DVectors into a temporary CArray<C3DVector> and write it to the stream as one raw block
// Leaf: ?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE18C0, 436 bytes, protected static __fastcall (vectors ecx, count edx; the stream on the
//   stack), `ret 4`.
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb both name
//   `?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z`, the only PDB function taking
//   (const C4DVector*, unsigned long, CDataOutputStream&), exactly retail's shape; it is the writing twin of the landed
//   ReadVectorArray 0x00AE1730; (2) its two callers are CParticleSimulation::WriteToFile 0x00AE2050 (call 0x00AE208C) and
//   CParticleProgram::WriteToFile 0x00AE2230 (call 0x00AE238E), which bind it under the invented
//   `?WriteVectors_00ae18c0@@YIXPBVC4DVector@@KAAVCDataOutputStream@@@Z` (NAME_FIXES.tsv). Manifest label
//   "WriteAnimationData" is propagated.
// Callees: CArray<C3DVector>::CArray(unsigned long) 0x007C2AB0 (bound `??0?$CArray@VC3DVector@@@@QAE@K@Z`), the stream's
//   virtual Write(const void*, long) (slot 4), free 0x00BFEA14 (~CArray).
// Shape: the landed ReadVectorArray model reversed: the loop body calls the inline C4DVector::ToC3DVector() (PDB, returns by
//   value; the 3-dword frame temporary), which makes VC7.1 unroll it by four like retail; GetAsCArray / GetByteSize for the
//   Write. First grade.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CDataOutputStream {                                   // Layout: local model (engine/CDataOutputStream.h flattens the vptr)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);      // slot 4 (+0x10)
};

class C3DVector { public: float X, Y, Z; };               // 12 bytes
class C4DVector {                                         // Ego_r / FableWin: 16 bytes
public:
    float X, Y, Z, W;
    C3DVector ToC3DVector() const { C3DVector v; v.X = X; v.Y = Y; v.Z = Z; return v; }   // ?ToC3DVector@C4DVector@@QBE?AVC3DVector@@XZ
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
    static void __fastcall WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out);
};

void __fastcall CParticleSimulation::WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out)
{
    CArray<C3DVector> temp(count);
    for (unsigned long i = 0; i < count; ++i)
        temp[i] = vectors[i].ToC3DVector();
    out.Write(temp.GetAsCArray(), temp.GetByteSize());
}