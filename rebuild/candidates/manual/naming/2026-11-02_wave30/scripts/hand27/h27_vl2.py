# h27_vl2.py: naming wave 27, family vl2 (DECISIONS_W26_RESOLVED D6) -- the vector<long> grow-insert leftovers whose stand-in
#  template also carries a second target; each moved target takes its STLport 4.6 spelling (the own row's, bound elsewhere):
#  vector<long>::_M_insert_overflow 0x00440990 ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z,
#  vector<float>::_M_insert_overflow 0x004408A0 ?_M_insert_overflow@?$vector@MV?$allocator@M@std@@@std@@IAEXPAMABMABU__true_type@2@I_N@Z,
#  vector<unsigned short>::_M_insert_overflow 0x005135D0 ?_M_insert_overflow@?$vector@GV?$allocator@G@std@@@std@@IAEXPAGABGABU__true_type@2@I_N@Z,
#  vector<CResourceEntry>::resize 0x00F385A0 ?resize@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@V?$allocator@...@std@@@std@@QAEXIABU..@@Z,
#  vector<long>(const vector<long>&) 0x00514160 ??0?$vector@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = '// Naming wave 27 (family vl2, DECISIONS_W26 D6): %s'
def note(a, txt, leaf=None):
    p, rel, t = he._rw(a, 'src')
    if 'family vl2' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if leaf:
        if i: L[i[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf); i = [1]
    L.insert((i[0] + 1) if i else 1, NOTE % txt); he._w(p, '\n'.join(L))

# 0x008865C0 CBlendedEnvironmentTheme::BuildFromSource: CArray<long> / CArray<float> over std::vector<T, allocator<T> >
a = '008865c0'
he.edit(a, 'src', '''struct __true_type {};
template <class T> class CArray { public:             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    template <class Tag> __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned long fill_len, bool atend) { StandInSink((void*)position, (unsigned long)fill_len + *(const unsigned char*)&x); }   // 0x00440990 / 0x004408A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1UL, true);
        }
    }
};''', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type {};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00440990 / 0x004408A0
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
// same-TU stand-in body (the instantiations 0x00440990 / 0x004408A0 are graded on their own rows)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend) { StandInSink((void*)position, (unsigned long)fill_len + *(const unsigned char*)&x); }
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray over the STLport vector''')
note(a, 'CArray<T> derives from STLport std::vector<T, allocator<T> >, whose protected\n//   _M_insert_overflow(.., const __true_type&, size_type unsigned int, bool) names the vector<long> 0x00440990 and vector<float>\n//   0x004408A0 instantiations as their own rows define them. Was a CArray<T> member template (?$CArray@J / @M ..K_N).')

# 0x00F3F5A0 CTriangleStripifier2::ReorderTriangleListInPlace: CArray<unsigned short> / CArray<long> over std::vector<T, allocator<T> >
a = '00f3f5a0'
he.edit(a, 'src', '''struct SScalarTag { SScalarTag() { } };
template<class T> class CArray { public:                              // retail STLport vector: first, last, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x005135D0
    void push_back(const T& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
};''', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport 4.6 vector: first, last, end_of_storage
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    _Tp* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    _Tp& operator[](unsigned long n) const { return *(begin() + n); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool atEnd);   // 0x005135D0
public:
    void push_back(const _Tp& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
};
}
template<class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray over the STLport vector''')
he.edit(a, 'src', '''template<class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{''', '''template <class T, class _Alloc>
__declspec(noinline) void std::vector<T, _Alloc>::_M_insert_overflow(T* where, const T& value, const __true_type& tag, unsigned int count, bool atEnd)
{''')
note(a, 'CArray<T> derives from STLport std::vector<T, allocator<T> >, whose protected\n//   _M_insert_overflow(.., const __true_type&, size_type unsigned int, bool) names the vector<unsigned short> 0x005135D0 instantiation as\n//   its own row defines it. Was CArray<T>::InsertGrow(.., const SScalarTag&, unsigned long, bool).')

# 0x00F372B0 CGPUScratchBufferTextureManager::AllocResourceEntry: the members are CArray<T> (Ego_r.pdb) over std::vector<T, allocator<T> >
a = '00f372b0'
he.edit(a, 'src', '''struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; }
    void resize(unsigned long newSize, const T& value);     // 0x00F385A0 (vector<CResourceEntry>)
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00440990 (vector<long>)
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
''', '''inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() {} };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    _Tp& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; }
    void resize(unsigned int newSize, const _Tp& value);    // 0x00F385A0 (vector<CResourceEntry>)
protected:
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fillLength, bool atEnd);   // 0x00440990 (vector<long>)
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray (Ego_r.pdb member type)
void* GrowVectorStorage_s7(void* vector, unsigned long count);
''')
he.edit(a, 'src', '''template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fillLength, bool atEnd)
{''', '''template <class T, class _Alloc>
__declspec(noinline) void std::vector<T, _Alloc>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned int fillLength, bool atEnd)
{''')
he.edit_all(a, 'src', 'CRetailVector<', 'CArray<')
note(a, 'the members are CArray<T> (Ego_r.pdb) over STLport std::vector<T, allocator<T> >:\n//   vector<CResourceEntry>::resize(size_type unsigned int, const T&) 0x00F385A0 and the protected vector<long>::_M_insert_overflow\n//   0x00440990 take their own rows\' spellings. Was a row-local CRetailVector<T> (?resize@?$CRetailVector@.. / ?_M_insert_overflow@?$CRetailVector@J..K_N).')

# 0x00774100 CTCShop::GetWantedItemsList: the STLport 4.6 vector<long> structure (vector : _Vector_base, _M_end_of_storage an
#  _STLP_alloc_proxy over allocator<long>); the copy ctor 0x00514160 and grow-insert 0x00440990 take their own rows' spellings and the
#  row's Leaf its Ego_r / FableWin spelling ?GetWantedItemsList@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ.
a = '00774100'
he.edit(a, 'src', '''struct CLongAllocator {                             // STLport allocator<long>: empty
    CLongAllocator() { }
};
struct CLongAllocProxy : CLongAllocator {           // _STLP_alloc_proxy<long*, long, allocator<long> >
    long* m_data;
    CLongAllocProxy(const CLongAllocator& alloc, long* p) : CLongAllocator(alloc), m_data(p) { }
};
struct CLongVectorBase {                            // _Vector_base<long, allocator<long> >
    long* first;
    long* last;
    CLongAllocProxy end;
    CLongVectorBase(const CLongAllocator& alloc) : first(0), last(0), end(alloc, 0) { }
};
struct SScalarTag { SScalarTag() { } };
struct CLongVector : CLongVectorBase {              // retail STLport vector<long> (12 bytes)
    CLongVector() : CLongVectorBase(CLongAllocator()) { }
    CLongVector(const CLongVector& other);          // 0x00514160
    ~CLongVector() { if (first) free(first); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end.m_data) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};''', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };   // STLport allocator<long>: empty
struct __true_type { __true_type() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:      // _STLP_alloc_proxy<long*, long, allocator<long> >
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:    // _Vector_base<long, allocator<long> >
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {   // 12 bytes
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    vector(const vector<_Tp, _Alloc>& __x);         // 0x00514160
    ~vector() { if (this->_M_start) free(this->_M_start); }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00440990
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1, true);
    }
};
}''')
he.edit(a, 'src', '''void CLongVector::InsertGrow(long* position, const long& x, const SScalarTag&, unsigned long n, bool atEnd)
{
    unsigned long oldSize = last - first;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    long* newStart = len ? (long*)malloc(len * sizeof(long)) : 0;
    long* newFinish = CopyTrivial(first, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, last, newFinish);
    if (first)
        free(first);
    first = newStart;
    last = newFinish;
    end.m_data = newStart + len;
}
CLongVector::CLongVector(const CLongVector& other) : CLongVectorBase(CLongAllocator())
{
    unsigned long length = other.last - other.first;
    first = length ? (long*)malloc(length * sizeof(long)) : 0;
    last = first;
    end.m_data = first + length;
    last = CopyTrivial(other.first, other.last, first);
}''', '''template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int n, bool atEnd)
{
    unsigned long oldSize = this->_M_finish - this->_M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    _Tp* newStart = len ? (_Tp*)malloc(len * sizeof(_Tp)) : 0;
    _Tp* newFinish = CopyTrivial(this->_M_start, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, this->_M_finish, newFinish);
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = newStart;
    this->_M_finish = newFinish;
    this->_M_end_of_storage._M_data = newStart + len;
}
template <class _Tp, class _Alloc>
__declspec(noinline) std::vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& other) : _Vector_base<_Tp, _Alloc>(_Alloc())
{
    unsigned long length = other._M_finish - other._M_start;
    this->_M_start = length ? (_Tp*)malloc(length * sizeof(_Tp)) : 0;
    this->_M_finish = this->_M_start;
    this->_M_end_of_storage._M_data = this->_M_start + length;
    this->_M_finish = CopyTrivial(other._M_start, other._M_finish, this->_M_start);
}''')
he.edit_all(a, 'src', 'CLongVector GetWantedItemsList() const;', 'std::vector<long> GetWantedItemsList() const;')
he.edit_all(a, 'src', 'CLongVector CTCShop::GetWantedItemsList() const', 'std::vector<long> CTCShop::GetWantedItemsList() const')
he.edit_all(a, 'src', '    CLongVector wanted;', '    std::vector<long> wanted;')
note(a, 'the vector is STLport 4.6 std::vector<long, allocator<long> > (: _Vector_base, the\n//   _STLP_alloc_proxy end pointer): the copy ctor 0x00514160 ??0?$vector@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z and the protected\n//   grow-insert 0x00440990 take their own rows\' spellings, and the row its Ego_r / FableWin Leaf. Was CLongVector (CLongVectorBase /\n//   CLongAllocProxy, InsertGrow(.., const SScalarTag&, unsigned long, bool)).',
     '?GetWantedItemsList@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ')

# 0x00AEBAF0 NParticleEngine::CParticleEmitterManager::GetActiveEmitterNames (D5 + D6): Ego_r / FableWin spell it const with a
#  CArray<CCharString>& (?GetActiveEmitterNames@CParticleEmitterManager@NParticleEngine@@QBEJAAV?$CArray@VCCharString@@@@@Z); the
#  CCharStringVector stand-in becomes CArray<CCharString> over the same STLport vector template, whose __false_type grow-insert
#  0x00433530 takes the established spelling ?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAEX
#  PAVCCharString@@ABV3@ABU__false_type@2@I_N@Z (bound by 0x0044A3B0 and seven more). push_back picks the tag by STLport's
#  __type_traits<_Tp>::is_POD_type (long: __true_type).
a = '00aebaf0'
he.edit(a, 'src', '''struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString* begin() { return _M_start; }
    CCharString& operator[](unsigned long i) { return *(begin() + i); }
};
''', '''inline void* operator new(unsigned int, void* where) { return where; }
''')
he.edit(a, 'src', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport vector<long>
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990
public:
    void push_back(const _Tp& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(value);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
    _Tp& operator[](unsigned long i) { return _M_start[i]; }
};
}''', '''namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type is_POD_type; };   // STLport type_traits.h
template <> struct __type_traits<long> { typedef __true_type is_POD_type; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport 4.6 vector (vector<long>, vector<CCharString>)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990 (long)
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __false_type&, unsigned int fill_len, bool atend);     // 0x00433530 (CCharString)
public:
    void push_back(const _Tp& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(value);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, value, typename __type_traits<_Tp>::is_POD_type(), 1, true);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](unsigned long i) { return *(begin() + i); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray over the STLport vector''')
he.edit(a, 'src', '''__declspec(noinline) void CCharStringVector::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    CCharString* start = (CCharString*)malloc(len * sizeof(CCharString));
    CCharString* finish = start;
    for (CCharString* p = _M_start; p != position; ++p, ++finish)
        new (finish) CCharString(*p);
    for (unsigned long i = 0; i < fill_len; ++i, ++finish)
        new (finish) CCharString(x);
    if (!atend)
        for (CCharString* p = position; p != _M_finish; ++p, ++finish)
            new (finish) CCharString(*p);
    for (CCharString* p = _M_start; p != _M_finish; ++p)
        p->~CCharString();''', '''template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const std::__false_type&, unsigned int fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    _Tp* start = (_Tp*)malloc(len * sizeof(_Tp));
    _Tp* finish = start;
    for (_Tp* p = _M_start; p != position; ++p, ++finish)
        new (finish) _Tp(*p);
    for (unsigned long i = 0; i < fill_len; ++i, ++finish)
        new (finish) _Tp(x);
    if (!atend)
        for (_Tp* p = position; p != _M_finish; ++p, ++finish)
            new (finish) _Tp(*p);
    for (_Tp* p = _M_start; p != _M_finish; ++p)
        p->~_Tp();''')
he.edit(a, 'src', '''    long GetActiveEmitterNames(CCharStringVector& names);
};
long CParticleEmitterManager::GetActiveEmitterNames(CCharStringVector& names)''', '''    long GetActiveEmitterNames(CArray<CCharString>& names) const;   // const, CArray<CCharString> (Ego_r.pdb / FableWin.pdb)
};
long CParticleEmitterManager::GetActiveEmitterNames(CArray<CCharString>& names) const''')
note(a, 'GetActiveEmitterNames is const and takes a CArray<CCharString>& (Ego_r.pdb /\n//   FableWin.pdb), CArray<T> over the STLport vector template whose __false_type grow-insert 0x00433530 takes the established\n//   spelling. Was a non-const member over a CCharStringVector stand-in (?_M_insert_overflow@CCharStringVector@@..K_N).',
     '?GetActiveEmitterNames@CParticleEmitterManager@NParticleEngine@@QBEJAAV?$CArray@VCCharString@@@@@Z')
