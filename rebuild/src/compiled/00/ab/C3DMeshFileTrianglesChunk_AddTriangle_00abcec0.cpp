// C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2& triangle) -- append one 6-byte triangle (three vertex indices) to the chunk's triangle list (+0x2C)
// Leaf: ?AddTriangle@C3DMeshFileTrianglesChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABCEC0, length 65 bytes (+15 int3), `ret 4`.
// Callees: std::vector<C3DTriangle2>::_M_insert_overflow(pointer, const C3DTriangle2&, const __false_type&, size_type,
//   bool) @ 0x00A71550 (the shared triangle-vector grow path).
// Identity: Ego_r.pdb `?AddTriangle@C3DMeshFileTrianglesChunk@@QAEXABVC3DTriangle2@@@Z`; the only caller is
//   COptimisedPrimitive::CopyTo (0x00ACCAB9, three uint16 indices built on its frame). The manifest's
//   "VertexData_AppendInterleaved" is descriptive.
// Layout: retail 12-byte STLport containers put TriangleList at +0x2C (the Ego_r header has +0x34), the same local
//   model as the landed GetList 0x00ABC440.
// Shape: the STLport push_back inlined on the member (`add ecx,2Ch` once, finish/end read through the owner first);
//   the POD triangle copies as a dword plus a word; the grow arm passes the tag homed on the value argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd = false);   // 0x00A71550
    public:
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace std

template <class T> class CArray : public std::vector<T> { };

class C3DMeshFileTrianglesChunk {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _chunkBase[0x28];        // +0x04 CChunkedFileChunk members
    CArray<C3DTriangle2>  TriangleList;            // +0x2C
    void AddTriangle(const C3DTriangle2& triangle);
};

void C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2& triangle)
{
    TriangleList.push_back(triangle);
}