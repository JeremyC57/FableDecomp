// C3DMeshFileXUVTangentChunk::AddTangent(const C3DVector& uTangent, const C3DVector& vTangent) -- append one U and one V tangent to the mesh file's UV-tangent chunk arrays
// Leaf: ?AddTangent@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA64E0, 137 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?AddTangent@C3DMeshFileXUVTangentChunk@@QAEXABVC3DVector@@0@Z`; no direct caller in retail (an
//   unreferenced out-of-line copy next to the chunk's other methods 0x00AA57F0/0x00AA6A60). The two pushes go to the
//   12-byte-element arrays at +0x10 and +0x1C, which the landed ReadFromFile 0x00AA6A60 reads as UTangents/VTangents.
//   The catalog's "Vector_PushBack_3Floats_Pair" is descriptive.
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, tag, 1, true) 0x0066E370 (shared by both pushes).
// Layout: retail chunk = CChunkedFileChunk {vptr, ChunkType[5] +4, Parent +0xC}, UTangents +0x10, VTangents +0x1C as
//   12-byte STLport vectors (engine/C3DMeshFileXUVTangentChunk.h carries the 16-byte Ego_r arrays at +0x10/+0x20, so the
//   class is modelled locally); C3DVector is 3 floats.
// Shape: two inline STLport push_backs: `lea ecx,[esi+member]`, the placement copy (three dword moves) under its null
//   test and `add [ecx+4],0Ch`, or the overflow call with the empty tag homed in the first argument's slot.
inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x0066E370
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class T> class CArray : public std::vector<T> { };   // retail CArray<T>: the 12-byte STLport vector

class CChunkedFileSuperChunk;

class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();
    char                    ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;                     // +0x0c
};

class C3DMeshFileXUVTangentChunk : public CChunkedFileChunk {
public:
    CArray<C3DVector> UTangents;                        // +0x10
    CArray<C3DVector> VTangents;                        // +0x1c
    void AddTangent(const C3DVector& uTangent, const C3DVector& vTangent);
};

void C3DMeshFileXUVTangentChunk::AddTangent(const C3DVector& uTangent, const C3DVector& vTangent)
{
    UTangents.push_back(uTangent);
    VTangents.push_back(vTangent);
}