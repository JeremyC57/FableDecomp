// _STL::vector<CLightInfo>::vector(unsigned int count) -- (CShaderRenderManager::CLightInfo elements) STLport vector of 56-byte light infos: pointers nulled, count elements malloc'd (none for zero), finish/end-of-storage set, every element copied from a default CLightInfo
// Leaf: ??0?$vector@VCLightInfo@CShaderRenderManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098CCC0, length 119 bytes, `ret 4`.
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, the STLport allocator).
// Callers: CShaderRenderManager::AllocateLights @ 0x0098B420 (the old-lights copy, landed with a same-TU stand-in of this
// ctor) and 0x0098D218. Element: Ego_r CShaderRenderManager::CLightInfo (56 bytes: CShaderGenericLight Light, whose
// Size +0x30 the default ctor zeroes; IsEnabled +0x34 = false; UpdateFlag +0x35 = true). The debug PDB also carries
// ??0?$CArray@VCLightInfo@CShaderRenderManager@@@@QAE@I@Z (CArray<T> wraps the vector), so the owning name may be the
// CArray(unsigned) ctor with this vector(n) body inlined -- the bytes cannot tell.
// Label: the manifest's "std_vector_Reallocate" is descriptive and wrong (this is the sized constructor).
// Model: STLport 4.6 explicit vector(n): _Vector_base(n, alloc) (allocate = n ? malloc(n*56) : 0), then
// uninitialized_fill_n(start, n, T()) -- the default value is a 0x38-byte frame temporary copied with `rep movsd`
// (trivial copy ctor) under the placement-new null test.
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class C4DVector { public: float X, Y, Z, W; };

class CShaderGenericLight {
public:
    C4DVector     Parameters[2];                // +0x00
    long          Types[2];                     // +0x20 EParameterType
    float         Attenuation2;                 // +0x28
    float         AttenuationBase;              // +0x2c
    unsigned long Size;                         // +0x30
    CShaderGenericLight() : Size(0) { }
};

class CShaderRenderManager {
public:
    class CLightInfo {                          // 0x38 bytes
    public:
        CShaderGenericLight Light;              // +0x00
        bool IsEnabled;                         // +0x34
        bool UpdateFlag;                        // +0x35
        CLightInfo() : IsEnabled(false), UpdateFlag(true) { }
    };
};

namespace _STL {

template <class T> __forceinline T* uninitialized_fill_n(T* first, size_t count, const T& value)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor)
        new (cursor) T(value);
    return cursor;
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(size_t count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& alloc, Value p) : Alloc(alloc), _M_data(p) { }
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
    _Vector_base(size_t count, const Alloc& alloc)
        : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0)
    {
        _M_start = _M_end_of_storage.allocate(count);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + count;
    }
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    explicit vector(size_t count)
        : _Vector_base<T, Alloc>(count, Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, count, T());
    }
};

}   // namespace _STL

template _STL::vector<CShaderRenderManager::CLightInfo>::vector(size_t);