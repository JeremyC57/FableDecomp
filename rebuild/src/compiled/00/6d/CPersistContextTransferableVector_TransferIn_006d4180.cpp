// CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index) -- parse element `index` of a vector of (CCharString, long) pairs through a CPersistTraits whose default value is ("", 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4180, length 101 bytes (+ int3 padding).
// Leaf: ?TransferIn@CPersistContext_TransferableVector@@UAEXPAVCPersistContext@@PAVCStringParser@@J@Z
// Slot 3 of the CPersistContext_TransferableVector vtable 0x0125ED48 (catalogued "vfunc_3"; slot 3 is the persist READ
// hook, as the landed TransferIn 0x0049B070). The element type is 8 bytes {CCharString, long}: the traits reader
// @ 0x006D41F0 (catalogued as the propagated CPersistTraits<CSimVoice>::TransferIn; Ego_r.pdb CSimVoice is 28 bytes)
// parses a "(name, number)" pair into it.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with "" @ 0x0122D70E, CCharString::CCharString(const
// CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0 (x2), CPersistTraits<pair<CCharString,long>>::TransferIn @ 0x006D41F0.
// Shape: the traits object (esp+8: the pair) is built inline as `DefaultVal("", 0)`: the IMPLICIT CCharString("") conversion
// temporary (esp+4; an explicit `CCharString("")` argument reuses the constructor's eax for the copy) is copied into
// `first` and destroyed right after `second = 0`; the element address is `first + index*8`; `ret 0Ch`.
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
    void TransferIn(CPersistContext* context, CStringParser* parser, pair<CCharString, long>* value);   // 0x006D41F0
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<pair<CCharString, long> >* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<pair<CCharString, long> > traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}