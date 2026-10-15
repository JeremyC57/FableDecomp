// CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) -- parses element `index` of the CCharString vector through a CPersistTraits<CCharString> (0x0040E510) that owns a default string
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B070, 59 bytes (+5 int3 padding).
// Callees: 0x0099E4B0 CCharString::CCharString(), 0x0040E510 CPersistTraits<CCharString>::TransferIn
// (CPersistContext*, CStringParser*, CCharString*) (__fastcall), 0x0099EAE0 CCharString::~CCharString.
//
// Persist READ hook (vtable slot 3) of CPersistContext_TransferableVector<CCharString> (PDB: { vptr;
// vector<CCharString>* TransVector; }). The CPersistTraits<CCharString> local (PDB: { CCharString
// DefaultVal; }, the `push ecx` frame slot) is built first (its implicit constructor default-constructs
// the string, so `this` is parked in esi across the call), the reader parses into
// &TransVector->first[index], and the implicit traits destructor destroys the string. The module is
// built without C++ exceptions (no EH frame around the string's lifetime).
struct CPersistContext;
struct CStringParser;

struct CCharString {
    struct SBlock* m_pBlock;
    CCharString();                         // 0x0099E4B0
    ~CCharString();                        // 0x0099EAE0
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);       // 0x0040E510 for CCharString
    void TransferOut(CPersistContext* context, CCharString* out, T* value);           // 0x0040E630 for CCharString
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CCharString>* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CCharString> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}