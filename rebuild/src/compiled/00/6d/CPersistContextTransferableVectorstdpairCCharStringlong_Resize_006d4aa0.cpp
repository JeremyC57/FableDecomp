// CPersistContext_TransferableVector<std::pair<CCharString,long> >::Resize(unsigned long count) -- resize the transferred vector of (CCharString, long) pairs to `count`, new elements copied from the traits default ("", 0) (STLport resize(n, x) inlined)
// Leaf: ?Resize@CPersistContext_TransferableVector@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4AA0, 143 bytes (+17 int3 padding), `ret 4`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with "" @ 0x0122D70E, CCharString::CCharString(const
// CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0 (temporary + one per arm), vector<pair<CCharString,long>>::
// erase(first, last) @ 0x006D44F0 and _M_fill_insert(pos, n, x) @ 0x006D4850 (manifest names are guesses; thiscall).
// Slot 1 of the transferable-vector vtable 0x0125ED48 (slot 2 TransferOut 0x006D3810, slot 3 the landed TransferIn
// 0x006D4180). Shape: the traits object is built first exactly as in the two transfer hooks (DefaultVal("", 0): the
// implicit "" temporary copied into `first`, destroyed after `second = 0`; `this` parked in esi), THEN TransVector is
// read into ecx and STLport's resize(n, x) runs inline on traits.DefaultVal: size `(finish - start) >> 3`, the shrink
// arm passes `start + n` and the cached finish, the grow arm reloads finish and passes (finish, n - size, &x); each arm
// destroys the traits string and returns on its own (`ret 4`).
class CPersistContext;
class CStringParser;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* first, _Tp* last);                                    // 0x006D44F0
    void _M_fill_insert(_Tp* position, unsigned int n, const _Tp& x);     // 0x006D4850
    void insert(_Tp* position, unsigned int n, const _Tp& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned int newSize, const _Tp& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
};
}

typedef std::pair<CCharString, long> SNamedLong;

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<SNamedLong> {
    SNamedLong DefaultVal;
    CPersistTraits() : DefaultVal("", 0) { }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(unsigned long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    std::vector<SNamedLong>* TransVector;          // +0x4
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    CPersistTraits<SNamedLong> traits;
    TransVector->resize(count, traits.DefaultVal);
}