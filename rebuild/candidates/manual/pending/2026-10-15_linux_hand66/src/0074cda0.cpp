// CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) -- parses element `index` of the CDefIndex vector through a default-constructed CPersistTraits<CDefIndex> (0x00A3E0D0)
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CDA0, 44 bytes (+4 int3 padding).
// Callee: 0x00A3E0D0 CPersistTraits<CDefIndex>::TransferIn(CPersistContext*, CStringParser*, CDefIndex*) (__thiscall).
//
// Persist READ hook (vtable slot 3) of CPersistContext_TransferableVector<CDefIndex> (PDB: { vptr;
// vector<CDefIndex>* TransVector; }). A CPersistTraits<CDefIndex> (PDB: { CDefIndex DefaultVal; }, CDefIndex = { long Val; }) lives in the
// `push ecx` frame slot; its constructor value-initialises DefaultVal (`mov dword ptr [esp],0`,
// scheduled early because the object's address escapes) and the traits reader parses into
// &TransVector->first[index] (`lea eax,[ecx+edx*4]`). Retail std::vector is three pointers.
struct CPersistContext;
struct CStringParser;

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
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x00A3E0D0 for CDefIndex
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, struct CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CDefIndex>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CDefIndex> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}
