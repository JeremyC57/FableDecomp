// CPersistContext_TransferableVector<float>::TransferOut(CPersistContext*, CCharString*, long) -- formats element `index` of the float vector with six decimals and appends it to the output string
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005663F0, 54 bytes (+10 int3 padding).
// Callees: 0x009A08B0 CCharString::FormatDouble (__fastcall, hidden result in ecx, digit count in edx,
// the double on the stack), 0x0099F0A0 CCharString::operator+=(const CCharString&),
// 0x0099EAE0 CCharString::~CCharString.
//
// Persist WRITE hook (vtable slot 2) of CPersistContext_TransferableVector<float> (PDB: { vptr;
// vector<float>* TransVector; }). Same body as the float component writer 0x0040F870, but the
// CCharString temporary needs no frame: once the element is loaded (`fld [eax+ecx*4]`) the index's
// argument slot is dead and VC builds the formatted string there (`lea ecx,[esp+14h]` after the
// 8-byte double push, destroyed through `lea ecx,[esp+0Ch]`).
// The vector's first pointer is a named local: through the member chain VC loads it into ecx and
// the index into edx, where retail loads the index into the dead `this` register first.
class CPersistContext;
class CStringParser;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<float>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    float* first = TransVector->m_pFirst;
    *out += CCharString::FormatDouble(6, first[index]);
}