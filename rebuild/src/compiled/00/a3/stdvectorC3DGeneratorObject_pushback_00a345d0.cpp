// std::vector<C3DGeneratorObject>::push_back(const C3DGeneratorObject&) -- appends a mesh generator object
// Retail: Fable.exe 0x00A345D0, 99 bytes: if _M_finish != _M_end_of_storage, the null-tested placement copy (the
// implicit C3DGeneratorObject copy: C3DDummyObject base 0x34 bytes by rep movsd, ObjectName via CCharString copy ctor
// 0x0099EC30, BankIndex, UseLocalOrigin) and _M_finish += 0x40; else _M_insert_overflow(_M_finish, x, __false_type,
// 1, true) (0x00A33C50); `ret 4`.
// Identity: the only caller 0x00A2FB40 pushes onto the vector at C3DMeshStats +0x68, the retail offset of Ego_r.pdb
// C3DMeshStats::MeshGenerators (CArray<C3DGeneratorObject>; Ego_r +0x6C with the 0x14-byte Dinkumware CVectorMap
// before it); the 0x40-byte element matches Ego_r.pdb C3DGeneratorObject (C3DDummyObject base: Transform CMatrix3x4
// +0, BoneIndex +0x30; ObjectName +0x34, BankIndex +0x38, UseLocalOrigin +0x3C) and 0x00A33C50 is the catalog's
// C3DGeneratorObject reallocation. STLport 4.6 push_back; the element has a CCharString, so _IsPODType() is
// __false_type (a user ctor on the tag keeps VC7.1 from zero-storing the temporary).
// Leaf: ?push_back@?$vector@VC3DGeneratorObject@@V?$allocator@VC3DGeneratorObject@@@std@@@std@@QAEXABVC3DGeneratorObject@@@Z
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    void* PStringData;
};

class CMatrix3x4 { public: float M[12]; };

class C3DDummyObject {
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

class C3DGeneratorObject : public C3DDummyObject {
public:
    CCharString   ObjectName;                           // +0x34
    unsigned long BankIndex;                            // +0x38
    bool          UseLocalOrigin;                       // +0x3C
};

namespace std {
    struct __false_type { __false_type() { } };
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;                                  // +0x00
        _Tp* _M_finish;                                 // +0x04
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc> {
    public:
        typedef _Tp* pointer;
        typedef unsigned int size_type;
        typedef __false_type _IsPODType;
        void push_back(const _Tp& __x)
        {
            if (this->_M_finish != this->_M_end_of_storage._M_data) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else
                _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
        }
    protected:
        void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                                bool __atend = false);   // 0x00A33C50
    };

    template class vector<C3DGeneratorObject>;
}