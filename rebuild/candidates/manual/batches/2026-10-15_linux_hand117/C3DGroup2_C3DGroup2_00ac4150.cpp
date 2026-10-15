// C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk& chunk) -- bone index from the chunk, then every vertex blend appended to the array
// Leaf: ??0C3DGroup2
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC4150, 128 bytes. Callees: C3DMeshFileGroupChunk::GetBoneIndex 0x00AB9F60,
// ::GetVertexCount 0x00ABA350, ::GetList 0x00ABC370 (the manifest labels of the first two are propagated),
// the array's grow-and-insert 0x00ABEB40 (where, value, tag, count, flag).
// Layout from the PDB (C3DGroup2: BoneIndex +0, VertexBlends CArray<C3DVertexBlend2> +4, 8-byte elements
// {VertexIndex, BlendValue}). The array is a member with an inline constructor (three zero stores through
// `lea esi,[ebp+4]`); it must be the real STLport structure (_Vector_base(const allocator&) zeroing start and
// finish, the end-of-storage pointer in an _STLP_alloc_proxy built from an allocator() temporary) -- a plain
// three-pointer init list schedules `xor eax,eax` / `mov ecx,edi` differently; BoneIndex is a body assignment; push_back is the retail inline form (placement copy
// when there is room, else the out-of-line insert with the empty tag homed in the dead argument slot).
#include <new>

struct C3DVertexBlend2 {
    unsigned long VertexIndex;              // +0
    float BlendValue;                       // +4
};

struct SScalarTag { SScalarTag() { } };

template <class T> struct allocator { allocator() {} };

template <class T>
struct _STLP_alloc_proxy {                  // STLport: the end-of-storage pointer over an empty allocator
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>&, T* p) : _M_data(p) {}
};

template <class T>
struct _Vector_base {
    T* m_pBegin;                            // _M_start
    T* m_pEnd;                              // _M_finish
    _STLP_alloc_proxy<T> m_EndOfStorage;    // _M_end_of_storage
    _Vector_base(const allocator<T>& a) : m_pBegin(0), m_pEnd(0), m_EndOfStorage(a, 0) {}
};

template <class T>
struct CArray : _Vector_base<T> {           // retail CArray = STLport vector: begin/end/capacity
    CArray() : _Vector_base<T>(allocator<T>()) {}
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00ABEB40
    void push_back(const T& value)
    {
        if (this->m_pEnd != this->m_EndOfStorage._M_data) {
            new (this->m_pEnd) T(value);
            ++this->m_pEnd;
        }
        else
            InsertGrow(this->m_pEnd, value, SScalarTag(), 1, true);
    }
};

struct C3DMeshFileGroupChunk {
    long GetBoneIndex() const;                          // 0x00AB9F60
    unsigned long GetVertexCount() const;               // 0x00ABA350
    const C3DVertexBlend2* GetList() const;             // 0x00ABC370
};

struct C3DGroup2 {
    long BoneIndex;                                     // +0x00
    CArray<C3DVertexBlend2> VertexBlends;               // +0x04
    C3DGroup2(const C3DMeshFileGroupChunk& chunk);
};

C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk& chunk)
{
    BoneIndex = chunk.GetBoneIndex();
    unsigned long count = chunk.GetVertexCount();
    const C3DVertexBlend2* blends = chunk.GetList();
    for (unsigned long i = 0; i < count; ++i)
        VertexBlends.push_back(blends[i]);
}
