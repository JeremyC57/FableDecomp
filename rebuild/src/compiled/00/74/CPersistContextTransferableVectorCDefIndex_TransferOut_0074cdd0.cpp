// CPersistContext_TransferableVector<CDefIndex>::TransferOut(CPersistContext*, CCharString*, long) -- writes element `index` of the CDefIndex vector through a default-constructed CPersistTraits<CDefIndex> (0x00A3E180)
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CDD0, 44 bytes.
// Callee: 0x00A3E180 CPersistTraits<CDefIndex>::TransferOut(CPersistContext*, CCharString*, CDefIndex*) (__fastcall).
//
// Persist WRITE hook (vtable slot 2) of CPersistContext_TransferableVector<CDefIndex> (PDB: { vptr;
// vector<CDefIndex>* TransVector; }). A CPersistTraits<CDefIndex> (PDB: { CDefIndex DefaultVal; }, CDefIndex = { long Val; }) lives in the
// `push ecx` frame slot; its constructor value-initialises DefaultVal (`mov dword ptr [esp],0`,
// scheduled early because the object's address escapes) and the traits writer formats
// &TransVector->first[index] (`lea eax,[ecx+edx*4]`). Retail std::vector is three pointers.
struct CPersistContext;
struct CStringParser;
struct CCharString;

struct CDefIndex {
    long Val;
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferOut(CPersistContext* context, CCharString* out, T* value);   // 0x00A3E180 for CDefIndex
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CDefIndex>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CDefIndex> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}