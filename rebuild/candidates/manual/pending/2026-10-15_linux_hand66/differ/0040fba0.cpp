// CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long) -- copies element `index` of the long vector into a local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Grade: DIFFER(ours v retail) -- 37v37 scheduling only: retail loads the index (`mov edx,[esp+0Ch]`) before the vector's first pointer (`mov ecx,[eax]`), the lane loads them the other way round
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040FBA0, 37 bytes (+11 int3 padding).
// Callee: 0x009E2560 TransferOutLong (__fastcall: context in ecx, output string in edx, &value on the stack).
//
// Persist WRITE hook (vtable slot 2) of CPersistContext_TransferableVector<long> (PDB: { vptr;
// vector<long>* TransVector; }). The element is read (`mov eax,[ecx+edx*4]`) into a local that VC
// homes in the index's dead argument slot (`mov [esp+10h],eax ; lea ecx,[esp+0Ch]`), so there is no
// frame of its own; the long writer gets its address.
// Tried: plain `(*TransVector)[index]` (this), the retail-vector iterator object `*(begin()+n)`
// (2 bytes longer), a named `first` pointer local and a vector reference local (the same swap),
// the landed family's parameter reuse (`index = ..; Write(&index)`, the same swap).
struct CPersistContext;
struct CStringParser;
struct CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

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
    long value = (*TransVector)[index];
    TransferOutLong(context, out, &value);
}
