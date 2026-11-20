// CPersistContext_TransferableVector<unsigned __int64>::TransferOut(CPersistContext*, CCharString*, long) -- formats element `index` of the unsigned 64-bit vector ("%I64u") and appends it to the output string
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614450, 49 bytes.
// Callees: 0x0099F930 GFUHUGEToCharString(unsigned __int64), 0x0099F0A0
// CCharString::operator+=(const CCharString&), 0x0099EAE0 CCharString::~CCharString.
//
// Persist WRITE hook (vtable slot 2) of CPersistContext_TransferableVector<unsigned __int64>. Same
// body as the component writer 0x0044B800, but the CCharString temporary is built in the index's
// dead argument slot (`lea ecx,[esp+14h]` after the two halves are pushed, destroyed through
// `lea ecx,[esp+0Ch]`), so the function has no frame.
// The vector's first pointer is a named local: through the member chain VC loads it into ecx and
// the index into eax, where retail loads the index into ecx and keeps `first` in eax.
class CPersistContext;
class CStringParser;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};
// Naming (NF183, 2026-11-20, q4628): 0x0099F930 is the free __fastcall GFUHUGEToCharString (Ego_r.pdb + FableWin.pdb
//   ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z, the spelling the landed 0x004FB990 / 0x004B5EE0 / 0x008304A0 bind), was a stand-in
//   CCharString(unsigned __int64) constructor (same registers: hidden result in ecx, the value on the stack).
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);   // 0x0099F930

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
    CRetailVector<unsigned __int64>* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    unsigned __int64* first = TransVector->m_pFirst;
    *out += GFUHUGEToCharString(first[index]);
}