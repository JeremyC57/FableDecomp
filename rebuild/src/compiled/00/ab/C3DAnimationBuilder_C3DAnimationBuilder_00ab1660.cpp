// C3DAnimationBuilder::C3DAnimationBuilder() -- CBaseClassNonCopyable base, this class's vtable, an empty AddedBonePrefixes string vector (+4), then the two default bone prefixes "x" and "sub" pushed into it
// Leaf: ??0C3DAnimationBuilder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1660, length 219 bytes (+5 int3), plain `ret`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString(const char*, long) @ 0x0099EBF0,
//   CCharString(const CCharString&) @ 0x0099EC30 (the placement copy), vector<CCharString>::_M_insert_overflow
//   @ 0x00433530, ~CCharString @ 0x0099EAE0; literals "x" @ 0x0123089C and "sub" @ 0x0129E9C0.
// Identity: vtable 0x0129E9C8 = C3DAnimationBuilder (rtti-vtables.tsv; slot 0 = the landed deleting destructor
//   0x00AB3550); Ego_r.pdb `??0C3DAnimationBuilder@@QAE@XZ` and the member AddedBonePrefixes (CArray<CCharString>).
//   The manifest's "CParticleMorphs_InitializeEntry" is propagated and wrong.
// Shape: the vtable store precedes the vector's three zero stores (STLport `_Vector_base(allocator_type())` member
//   constructor); each prefix is an implicit CCharString conversion temporary (re-addressed with `lea` for the
//   placement copy and the overflow call) handed to the inlined STLport push_back; the empty __false_type tag gets the
//   high byte of the second frame word; VC7.1 tail-duplicates the last temporary's destructor and the return.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                             // +0x00
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};

class CBaseClassNonCopyable {
protected:                                         // ctor and virtual dtor are protected (IAE / MAE)
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    vector() : _Vector_base<_Tp, _Alloc>(allocator_type()) { }
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                       // 0x00433530
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

}   // namespace std

template <class T> class CArray : public std::vector<T> { };

class C3DAnimationBuilder : public CBaseClassNonCopyable {     // vtable 0x0129E9C8
public:
    CArray<CCharString> AddedBonePrefixes;         // +0x04
    C3DAnimationBuilder();
    virtual ~C3DAnimationBuilder();
};

C3DAnimationBuilder::C3DAnimationBuilder()
{
    AddedBonePrefixes.push_back("x");
    AddedBonePrefixes.push_back("sub");
}