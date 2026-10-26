// Behaviour test for 00aeef40: src/00aeef40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGuiVarTransferStruct::Add(CGuiVarTransferBase* var)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGuiVarTransferStruct::Add(CGuiVarTransferBase* var) -- takes ownership of a transfer variable: wraps it in a counted pointer and appends that to the member list (+8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEEF40, length 146 bytes (padded to 160).
// Callees: operator new @ 0x00BFEA1A (the 12-byte count block, null-checked), the per-type deleter @ 0x00AF0710 stored in
//          the block, the counted-pointer vector's reallocating insert @ 0x00AF0FA0 (position, value by reference, the
//          empty tag, count 1, true), operator delete @ 0x00BFE9BC.
// Shape: `Members.push_back(CCountedPointer<T>(var))` -- the temporary sits in the 8-byte frame (object word stored before
// the block is made, `Info = var ? new SCountBlock(var) : 0`); the retail push_back is inlined (in-place null-checked copy
// construction with the reference bump, `add [ecx+4],8`, else the insert with the tag in the dead argument slot); the
// temporary's destructor releases the block from the registers.
// Model: local (the member vector at +8; the variable type is opaque).
// Leaf: ?Add@CGuiVarTransferStruct@@
#include <new>
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CGuiVarTransferBase;
void __fastcall DestroyGuiVarTransfer(void* object);   // 0x00AF0710: the per-type deleter stored in the block
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyGuiVarTransfer), m_pObject(object) {}
};
template <class T> class CCountedPointer { public:
    T*           Data;
    SCountBlock* Info;
    CCountedPointer(T* p) : Data(p) { Info = p ? new SCountBlock(p) : 0; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            Info->m_pfnDestroy(Info->m_pObject);
            delete Info;
        }
    }
};
struct SScalarTag { SScalarTag() {} };
template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00AF0FA0
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        new (p) T(value);
    
}
#pragma auto_inline(on)
class CGuiVarTransferStruct { public:
    virtual void v00();
    long                                                m_04;
    CRetailVector< CCountedPointer<CGuiVarTransferBase> > Members;   // +0x08
    void Add(CGuiVarTransferBase* var);
};

void CGuiVarTransferStruct::Add(CGuiVarTransferBase* var)
{
    Members.push_back(CCountedPointer<CGuiVarTransferBase>(var));
}

// ---- stubs and driver ----
typedef CCountedPointer<CGuiVarTransferBase> P;
static unsigned char g_raw[8 * sizeof(P)]; static int g_grow = 0, g_destroy = 0; static void* g_destroyed = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; CRetailVector<P>* v = (CRetailVector<P>*)storage; P* p = v->last; v->last += count; if (v->end < v->last) v->end = v->last; return p;
}
void __fastcall DestroyGuiVarTransfer(void* object) { ++g_destroy; g_destroyed = object; }
void CGuiVarTransferStruct::v00() {}
int main()
{
    P* store = (P*)g_raw;
    CGuiVarTransferStruct s; s.Members.first = store; s.Members.last = store; s.Members.end = store + 1;
    CGuiVarTransferBase* a = (CGuiVarTransferBase*)0x100; CGuiVarTransferBase* b = (CGuiVarTransferBase*)0x200;
    s.Add(a);
    CHECK(s.Members.last == store + 1 && g_grow == 0); CHECK(store[0].Data == a && store[0].Info != 0 && store[0].Info->m_references == 1 && store[0].Info->m_pObject == a && store[0].Info->m_pfnDestroy == DestroyGuiVarTransfer);
    CHECK(g_destroy == 0);
    s.Add(b);   // growth path: the stored copy holds the reference, the temporary's release leaves it at 1
    CHECK(s.Members.last == store + 2 && g_grow == 1); CHECK(store[1].Data == b && store[1].Info != 0 && store[1].Info->m_references == 1 && store[1].Info->m_pObject == b);
    CHECK(g_destroy == 0);
    s.Members.end = store + 8; s.Add(0);   // null: no count block
    CHECK(s.Members.last == store + 3 && store[2].Data == 0 && store[2].Info == 0 && g_destroy == 0);
    store[1].~P(); CHECK(g_destroy == 1 && g_destroyed == b);
    if (fails == 0) printf("00aeef40_TEST PASS\n"); else printf("00aeef40_TEST FAIL\n");
    return fails;
}