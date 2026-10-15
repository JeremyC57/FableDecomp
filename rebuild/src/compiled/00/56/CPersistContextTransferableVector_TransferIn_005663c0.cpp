// CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) -- parses element `index` of the float vector through a default-constructed CPersistTraits<float> (0x0040F7E0)
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005663C0, 44 bytes (+4 int3 padding).
// Callee: 0x0040F7E0 CPersistTraits<float>::TransferIn(CPersistContext*, CStringParser*, float*) (__fastcall).
//
// Persist READ hook (vtable slot 3) of CPersistContext_TransferableVector<float> (PDB: { vptr;
// vector<float>* TransVector; }). A CPersistTraits<float> (PDB: { float DefaultVal; }) lives in the
// `push ecx` frame slot; its constructor value-initialises DefaultVal (`mov dword ptr [esp],0`,
// scheduled early because the object's address escapes) and the traits reader parses into
// &TransVector->first[index] (`lea eax,[ecx+edx*4]`). Retail std::vector is three pointers.
struct CPersistContext;
struct CStringParser;

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x0040F7E0 for float
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, struct CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<float>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<float> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}