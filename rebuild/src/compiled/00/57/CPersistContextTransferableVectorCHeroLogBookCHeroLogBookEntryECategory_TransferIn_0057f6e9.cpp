// CPersistContext_TransferableVector<CHeroLogBook::CHeroLogBookEntry::ECategory>::TransferIn(CPersistContext* context, CStringParser* parser, long index) -- parses element `index` of an enum vector through the __fastcall long reader 0x009E2590
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F6E9, 38 bytes, `ret 0Ch`.
// Callee: 0x009E2590 the __fastcall enum/long text reader (context in ecx, parser in edx, &result on the stack;
// the reader the landed TransferableComponent<long-like> TransferIn rows call).
// Persist READ hook (vtable 0x0124F33C slot 3; slot 2 is TransferOut 0x0057F70F, slot 1 Resize 0x005813BC, slot 0
// the count 0x0057F6DD) of CPersistContext_TransferableVector<T> { vptr; vector<T>* TransVector; } with a 4-byte
// enum element. The vector's data pointer is read into esi BEFORE the call (a named local), the parsed value lands
// in the dead `parser` argument slot ([ebp+0Ch]) and is stored into first[index] afterwards. The module keeps the
// EBP frame (`optimize("y", off)`) and is size-optimised.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPersistContext;
struct CStringParser;
struct CCharString;

void __fastcall TransferEnumIn_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

typedef long ECategory;                                // CHeroLogBook::CHeroLogBookEntry::ECategory

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
    CRetailVector<ECategory>* TransVector;             // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    ECategory* first = TransVector->m_pFirst;
    long value;
    TransferEnumIn_009e2590(context, parser, &value);
    first[index] = value;
}