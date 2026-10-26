// CPersistContext_TransferableVector<ETutorialCategory>::TransferOut(CPersistContext*, CCharString*, long) -- copies element `index` of the long vector into a local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040FBA0, 37 bytes (+11 int3 padding).
// Callee: 0x009E2560 TransferOutLong (__fastcall: context in ecx, output string in edx, &value on the stack).
//
// Persist WRITE hook (vtable slot 2) of CPersistContext_TransferableVector<long> (PDB: { vptr;
// vector<long>* TransVector; }). The element is read (`mov eax,[ecx+edx*4]`) into a local that VC
// homes in the index's dead argument slot (`mov [esp+10h],eax ; lea ecx,[esp+0Ch]`), so there is no
// frame of its own; the long writer gets its address.
// Shape: the landed family's `CPersistTraits<T> traits; traits.TransferOut(context, out, &(*TransVector)[index]);`
// with the long traits' TransferOut an inline copy-and-forward (`T v = *value; TransferOutLong(.., &v);`): the
// inline call's argument evaluation gives retail's index-before-first load order (a direct local in the caller
// loads the vector's first pointer first).
class CPersistContext;
class CStringParser;
class CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value) { T v = *value; TransferOutLong(context, out, &v); }
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
    CRetailVector<long>* TransVector;      // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<long> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}