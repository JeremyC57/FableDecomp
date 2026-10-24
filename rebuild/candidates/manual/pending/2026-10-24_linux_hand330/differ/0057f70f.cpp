// Grade: DIFFER(ours v retail) -- scheduling only: retail loads the index ([ebp+10h]) before dereferencing TransVector->first; ours dereferences first
// CPersistContext_TransferableVector<ECreatureGeneratorGenerateType>::TransferOut(CPersistContext* context, CCharString* out, long index) -- writes element `index` of an enum vector through the __fastcall long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F70F, 36 bytes, `ret 0Ch`.
// Callee: 0x009E2560 the __fastcall enum/long text writer (context in ecx, out string in edx, &value on the stack).
// Persist WRITE hook (vtable 0x0124F33C slot 2; slot 3 is TransferIn 0x0057F6E9) of
// CPersistContext_TransferableVector<T> { vptr; vector<T>* TransVector; } with a 4-byte enum element (the manifest
// names the element ECreatureGeneratorGenerateType, the read twin CHeroLogBookEntry::ECategory: both are
// propagated labels of one 4-byte-enum instantiation). The element is copied into a local that VC homes in the
// dead `index` argument slot ([ebp+10h]) and its address handed to the writer. EBP frame + size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPersistContext;
struct CStringParser;
struct CCharString;

void __fastcall TransferEnumOut_009e2560(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

typedef long EGenerateType;                            // 4-byte enum element

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<EGenerateType>* TransVector;         // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    long value = TransVector->m_pFirst[index];
    TransferEnumOut_009e2560(context, out, &value);
}
