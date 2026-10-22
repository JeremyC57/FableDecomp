// Behaviour test for 0040fba0: src/0040fba0.cpp copied verbatim, the long writer stubbed so its arguments are observable.
// CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long)
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long) -- copies element `index` of the long vector into a local and hands its address to the long writer 0x009E2560
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
struct CPersistContext;
struct CStringParser;
struct CCharString;
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

// ---- stubs ----
static CPersistContext* g_ctx = 0; static CCharString* g_out = 0; static long g_val = 0; static long* g_ptr = 0; static int g_calls = 0;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value) { ++g_calls; g_ctx = context; g_out = out; g_val = *value; g_ptr = value; }
long CPersistContext_TransferableVector::GetCount() { return 0; }
void CPersistContext_TransferableVector::Resize(long) {}
void CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) {}
int main()
{
    static long data[4] = { 10, -20, 30, 40 };
    CRetailVector<long> vec; vec.m_pFirst = data; vec.m_pLast = data + 4; vec.m_pEnd = data + 4;
    CPersistContext_TransferableVector tv; tv.TransVector = &vec;
    CPersistContext* ctx = (CPersistContext*)0x1234; CCharString* out = (CCharString*)0x5678;
    tv.CPersistContext_TransferableVector::TransferOut(ctx, out, 1);
    CHECK(g_calls == 1); CHECK(g_ctx == ctx); CHECK(g_out == out); CHECK(g_val == -20);
    CHECK(g_ptr != &data[1]);   // the writer gets the address of a copy, not of the element
    tv.CPersistContext_TransferableVector::TransferOut(ctx, out, 3);
    CHECK(g_calls == 2); CHECK(g_val == 40);
    if (fails == 0) printf("0040fba0_TEST PASS\n"); else printf("0040fba0_TEST FAIL\n");
    return fails;
}