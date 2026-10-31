// CParticleProgram::ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups) -- private static load helper: read the group count, resize the array, then per group its bone index, its vertex-blend count (resizing the blend array) and the raw blend array
// Leaf: ?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3A60, 456 bytes, private static __fastcall (stream in ecx, groups in edx), plain `ret`.
// Identity (two sources): Ego_r.pdb `?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z`
//   (the only PDB function taking (CDataInputStream&, CArray<C3DGroup2>&), exactly retail's ecx/edx shape) and its two
//   callers 0x00AE4518 / 0x00AE4525 inside CParticleProgram::ReadFromFile 0x00AE3C30 (SimGroups / NonSimGroups), the
//   mirror of the landed WriteGroups 0x00AE0680 (WriteToFile's two tail calls).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), vector<C3DGroup2>::resize
//   0x00AE8540 (landed), C3DGroup2::SetBoneIndex 0x00AC4070, C3DGroup2::Resize 0x00AC41D0, C3DGroup2::GetVertexBlends
//   0x00AC4080 (Ego_r donor order SetBoneIndex / GetVertexBlends / PeekVertexBlends / ... / AddVertexBlend / Resize maps to
//   retail 0x00AC4070 / 0x00AC4080 / 0x00AC40A0 / ... / 0x00AC4110 / 0x00AC41D0; bodies agree).
// Layout: retail CArray<T> is an STLport vector (12 bytes); C3DGroup2 {long BoneIndex; CArray<C3DVertexBlend2>} 16 bytes.
// Shape: the inline stream Read of the landed ReadVectorArray 0x00AE1730 (64-bit range check, memcpy fast path, private
//   overflow call); the count loop is a down-counter like WriteGroups.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
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
    template <class T> __forceinline T ReadType() { T value; Read(&value, sizeof(T)); return value; }   // PDB ReadType<T>
    __forceinline unsigned long ReadULONG() { return ReadType<unsigned long>(); }                       // PDB ReadULONG
    __forceinline long ReadSLONG() { return ReadType<long>(); }                                         // PDB ReadSLONG
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
    void resize(size_type __new_size);                     // 0x00AE8540 (T = C3DGroup2), out of line
};
}

template <class T> class CArray : public std::vector<T> {   // engine CArray over the STLport 4.6 vector (0xC bytes)
public:
};

class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes

class C3DGroup2 {                                          // 16 bytes in retail
public:
    long                    BoneIndex;                     // +0x0
    CArray<C3DVertexBlend2> VertexBlends;                  // +0x4
    void SetBoneIndex(long bone);                          // 0x00AC4070
    C3DVertexBlend2* GetVertexBlends();                    // 0x00AC4080
    void Resize(unsigned long size);                       // 0x00AC41D0
};

class CParticleProgram {
private:
    static void __fastcall ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups);
};

void __fastcall CParticleProgram::ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups)
{
    CDataInputReader& in = static_cast<CDataInputReader&>(stream);
    unsigned long count = in.ReadULONG();
    groups.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        C3DGroup2& group = groups[i];
        group.SetBoneIndex(in.ReadSLONG());
        unsigned long blends = in.ReadULONG();
        group.Resize(blends);
        long bytes = blends * sizeof(C3DVertexBlend2);
        C3DVertexBlend2* data = group.GetVertexBlends();
        if (bytes > 0)
            in.Read(data, bytes);
    }
}