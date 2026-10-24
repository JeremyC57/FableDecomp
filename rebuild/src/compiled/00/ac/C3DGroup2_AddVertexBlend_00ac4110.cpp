// C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend) -- append one {vertex index, blend value} pair to the group's blend list (+4)
// Leaf: ?AddVertexBlend@C3DGroup2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4110, length 63 bytes (+1 int3), `ret 4`.
// Callees: _STL::vector<C3DVertexBlend2>::_M_insert_overflow(pointer, const C3DVertexBlend2&, const __false_type&,
//   size_type, bool) @ 0x00ABEB40 (the landed C3DMeshFileGroupChunk::AddVertex calls the same grow path).
// Identity: Ego_r.pdb `?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z`; callers 0x00AE38B7 / 0x00AE38F2 are the
//   landed CParticleProgram::AddGroup 0x00AE37D0, which already names 0x00AC4110 C3DGroup2::AddVertexBlend. The
//   manifest's "C3DGroup2_InsertVertexInline" is descriptive.
// Layout: C3DGroup2 (retail 16 bytes) {BoneIndex +0, VertexBlends +4}; C3DVertexBlend2 {VertexIndex, BlendValue}.
// Shape: STLport push_back inlined on the member (`add ecx,4`), the 8-byte record copied as two dwords, the grow arm
//   passing the tag homed on the value argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVertexBlend2 {
public:
    unsigned long VertexIndex;                     // +0
    float         BlendValue;                      // +4
};

namespace _STL {

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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd = false);   // 0x00ABEB40
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

}   // namespace _STL

template <class T> class CArray : public _STL::vector<T> { };

class C3DGroup2 {
public:
    long                    BoneIndex;             // +0
    CArray<C3DVertexBlend2> VertexBlends;          // +4
    void AddVertexBlend(const C3DVertexBlend2& blend);
};

void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend)
{
    VertexBlends.push_back(blend);
}