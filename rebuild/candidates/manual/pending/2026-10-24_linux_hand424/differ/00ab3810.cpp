// CVertexStreamFormat::AddDataElement(unsigned long usage, EVertexStreamDataType type) -- append one {usage, data type} element to the stream format's element list (+4)
// Grade: DIFFER(83v83) -- register allocation: retail loads finish via the owner ([ecx+8]) first, keeps usage/type in edx/esi for the placement copy and bumps finish with load/add/store; VC7.1 lane loads both arguments first, frees esi early, re-reads the type from the frame and uses `add [ecx+4],8`
// Leaf: ?AddDataElement@CVertexStreamFormat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB3810, length 83 bytes (+13 int3), `ret 8`.
// Tried (4 grades): brace temporary, named local, user copy ctor on CDataElement, CArray wrapper vs raw vector --
//   all compile identically. Next: a same-TU noinline _M_insert_overflow body (the 00ab3b40 fix), else lane gap.
// Callees: std::vector<CVertexStreamFormat::CDataElement>::_M_insert_overflow(pointer, const CDataElement&,
//   const __false_type&, size_type, bool) @ 0x00AB4150.
// Identity: Ego_r.pdb `?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z`; the twelve callers are the
//   landed CVertexShaderInputStatic/Animated::Initialise (0x00A90560 / 0x00A90630), which already name 0x00AB3810.
//   The manifest's "VectorPushBack" is descriptive.
// Layout: CVertexStreamFormat (16 bytes retail) = CBaseClass vptr + CArray<CDataElement> DataElements (+4, STLport
//   12-byte vector), as the landed Initialise model.
// Shape: the element temporary is built in an 8-byte frame slot (the const& argument of push_back) while the copy at
//   finish forwards the two argument registers; the grow arm passes the tag homed on the dead `usage` slot.
inline void* operator new(unsigned int, void* p) { return p; }

enum EVertexStreamDataType { VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2 };

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd = false);   // 0x00AB4150
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

class CVertexStreamFormat : public CBaseClass {
public:
    class CDataElement {
    public:
        unsigned long         Usage;               // +0
        EVertexStreamDataType Type;                // +4
        CDataElement(unsigned long usage, EVertexStreamDataType type) : Usage(usage), Type(type) { }
    };
    CArray<CDataElement> DataElements;             // +0x04
    void AddDataElement(unsigned long usage, EVertexStreamDataType type);
};

void CVertexStreamFormat::AddDataElement(unsigned long usage, EVertexStreamDataType type)
{
    DataElements.push_back(CDataElement(usage, type));
}
