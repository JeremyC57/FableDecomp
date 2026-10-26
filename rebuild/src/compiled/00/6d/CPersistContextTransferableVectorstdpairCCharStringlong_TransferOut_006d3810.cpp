// CPersistContext_TransferableVector<std::pair<CCharString,long> >::TransferOut(CPersistContext* context, CCharString* out, long index) -- writes element `index` of a vector of (CCharString, long) pairs through a CPersistTraits whose default value is ("", 0)
// Leaf: ?TransferOut@CPersistContext_TransferableVector@@UAEXPAVCPersistContext@@PAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D3810, 101 bytes (+11 int3 padding), `ret 0Ch`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with "" @ 0x0122D70E, CCharString::CCharString(const
// CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0 (x2), CPersistTraits<pair<CCharString,long>>::TransferOut
// @ 0x006D3880 (thiscall, `ret 0Ch`).
// Persist WRITE hook (vtable 0x0125ED48 slot 2) -- the write twin of the landed TransferIn 0x006D4180 (slot 3) and
// Resize 0x006D4AA0 (slot 1). The manifest's `<CCharString>` is propagated: the 8-byte stride (`lea ecx,[edx+eax*8]`)
// and the pair default name the element. Shape (as 0x006D4180): the traits object (esp+8: the pair) is built inline as
// `DefaultVal("", 0)`; the IMPLICIT CCharString("") conversion temporary (esp+4) is copied into `first` and destroyed
// right after `second = 0`; the element address is `first + index*8`; `this` is parked in esi.
class CPersistContext;
class CStringParser;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<pair<CCharString, long> > {
    pair<CCharString, long> DefaultVal;
    CPersistTraits() : DefaultVal("", 0) { }
    void TransferOut(CPersistContext* context, CCharString* out, pair<CCharString, long>* value);       // 0x006D3880
    void TransferIn(CPersistContext* context, CStringParser* parser, pair<CCharString, long>* value);   // 0x006D41F0
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<pair<CCharString, long> >* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<pair<CCharString, long> > traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}