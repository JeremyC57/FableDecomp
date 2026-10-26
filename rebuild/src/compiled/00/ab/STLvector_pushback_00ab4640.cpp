// std::vector::push_back(const CVertexShaderInput::CStreamInfo& x) -- vector<CVertexShaderInput::CStreamInfo>::push_back: copy-construct the stream description (index + vertex stream format) at the end, or grow through _M_insert_overflow when full
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4640, 76 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: the 20-byte element of the landed _M_insert_overflow 0x00AB4540 (vector<CVertexShaderInput::CStreamInfo>:
//   Ego_r.pdb CStreamInfo = StreamIndex +0, CVertexStreamFormat StreamFormat +4; CVertexStreamFormat = vptr +0,
//   CArray<CDataElement> DataElements +4, retail 16 bytes with the 12-byte CArray). The vtable 0x0129D578 stored at +4
//   is CVertexStreamFormat's (rtti-vtables). The catalog label "vector_InsertPair" is descriptive.
// Callees: CArray<CVertexStreamFormat::CDataElement>(const CArray&) @ 0x00A5E0A0 (out of line), _M_insert_overflow(pos,
//   x, __false_type, 1, true) @ 0x00AB4540 (the tag lives in x's dead argument slot).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- placement new keeps its null test; the implicit
//   CStreamInfo copy inlines CVertexStreamFormat's copy constructor: CBaseClass's vptr store is dead, the derived vptr
//   is stored before the member copy call.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CBaseClass {
public:
    virtual ~CBaseClass();
};
template <class T> class CArray {                   // retail CArray: the 12-byte STLport vector
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray(const CArray& other);                    // CArray<CVertexStreamFormat::CDataElement>: 0x00A5E0A0
    ~CArray();
};
class CVertexStreamFormat : public CBaseClass {     // vtable 0x0129D578, 16 bytes
public:
    class CDataElement { public:                           // 8 bytes
        unsigned int Register;                      // +0x00
        int          DataType;                      // +0x04
    };
    CArray<CDataElement> DataElements;              // +0x04
    virtual ~CVertexStreamFormat();
};
class CVertexShaderInput {
public:
    class CStreamInfo {                             // 20 bytes
    public:
        unsigned long       StreamIndex;            // +0x00
        CVertexStreamFormat StreamFormat;           // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __false_type _IsPODType;
    void push_back(const _Tp& __x);
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);   // vector<CStreamInfo>: 0x00AB4540
private:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};

template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<CVertexShaderInput::CStreamInfo>::push_back(const CVertexShaderInput::CStreamInfo&);
}