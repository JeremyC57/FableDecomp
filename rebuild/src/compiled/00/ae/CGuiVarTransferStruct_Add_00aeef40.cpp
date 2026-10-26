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