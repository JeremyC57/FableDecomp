// std::vector<CVertexShader>::resize(size_type n) -- STLport resize: erase the tail past n, or fill-insert copies of a default CVertexShader up to n
// Leaf: ?resize@?$vector@VCVertexShader@@V?$allocator@VCVertexShader@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6CBD0, 121 bytes (+7 int3 padding), thiscall, `ret 4`.
// Identity: the manifest's "std::vector::erase" is propagated; the labels already carry the chunk-316 correction
//   (std::vector<CVertexShader>::resize) and the landed particle-group constructors 0x00BBEAF0 / 0x00BC34D0 declare
//   `void resize(unsigned int)` @ 0x00B6CBD0 on their vector<CVertexShader>. CVertexShader (Ego_r.pdb, 8 bytes) =
//   { vptr (CShaderBase); CVertexShaderData* Data; }, vtable 0x0129B3D0 -- engine/CVertexShader.h is data-only, so the
//   virtual class is modelled locally as in the landed 0x00BBA480.
// Callees: CVertexShader::CVertexShader() 0x00A62FB0 (the `_Tp()` temporary, returning this), vector::erase(first, last)
//   0x00B6C050, vector::_M_fill_insert(pos, n, const _Tp&) 0x00B6C5D0, the data's virtual deleting destructor (slot 0).
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over `resize(n, const _Tp& x) { if (n < size())
//   erase(begin() + n, end()); else insert(end(), n - size(), x); }` with insert the inline _M_fill_insert forwarder;
//   the temporary's address comes back in eax from its constructor (pushed as &x), and its inline ~CVertexShader runs
//   on both paths (the own-vtable store, then the reference drop with the virtual delete at zero).
class CVertexShaderData {
public:
    virtual ~CVertexShaderData();                           // slot 0
    void*       VertexDeclaration;                          // +0x04
    void*       VertexShader;                               // +0x08
    const void* FormatDecl;                                 // +0x0C
    long        ReferenceCount;                             // +0x10
    long RemoveReference() { return --ReferenceCount; }
};

class CShaderBase {                                         // vtable 0x0129A030
public:
    virtual ~CShaderBase() { }
};

class CVertexShader : public CShaderBase {                  // vtable 0x0129B3D0, 8 bytes
public:
    CVertexShaderData* Data;                                // +0x04
    CVertexShader();                                        // 0x00A62FB0
    virtual ~CVertexShader()
    {
        if (Data) {
            CVertexShaderData* current = Data;
            if (!current->RemoveReference())
                delete current;
        }
    }
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }

    iterator erase(iterator __first, iterator __last);                       // 0x00B6C050
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);     // 0x00B6C5D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }

    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CVertexShader>::resize(unsigned int);

} // namespace _STL