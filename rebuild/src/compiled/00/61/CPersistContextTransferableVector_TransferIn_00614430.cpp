// CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) -- reads a 64-bit integer with CStringParser::ReadInt64 into element `index` of the vector
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614430, 30 bytes (+2 int3 padding).
// Callee: 0x009BA5B0 CStringParser::ReadInt64 (__fastcall, result in edx:eax).
//
// Persist READ hook (vtable slot 3) of CPersistContext_TransferableVector<__int64> (PDB layout of the
// family: { vptr; vector<T>* TransVector; }). The vector's first pointer is read before the call
// (kept in esi: a named local), the parser (second argument) is `this` of the reader, and the index
// is re-read from its slot after the call for the two dword stores (`[esi+ecx*8]`, `[esi+ecx*8+4]`).
struct CPersistContext;
struct CCharString;

struct CStringParser {
    __int64 ReadInt64();                   // 0x009BA5B0
};

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
    CRetailVector<__int64>* TransVector;   // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    __int64* first = TransVector->m_pFirst;
    first[index] = parser->ReadInt64();
}