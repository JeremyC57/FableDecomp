// CreateCountedFlagRecord_00512c70() -- allocate a 5-byte record whose flag byte starts true and return it in a fresh counted pointer (count block with the record's plain-delete function)
// Leaf: CreateCountedFlagRecord_00512c70
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512C70, 107 bytes (+ int3 padding).
// Label correction: the manifest's "CEngineWorldMap::CreateNewBankMap" (thiscall, one argument) is propagated: this body
//   takes no arguments at all -- ecx is the hidden return slot and it ends in a bare `ret` (a /Gr `__fastcall` free
//   function returning a class by hidden pointer). The record type is unknown; named by address.
// Callees: operator new @ 0x00BFEA1A (5 bytes, then the 12-byte count block), operator delete @ 0x00BFE9BC,
//   the record deleter @ 0x0050D230 (`push ecx ; call operator delete ; pop ecx`: `delete p` of a trivially destructible
//   type; manifest "operator_new" is wrong), stored in the count block and called through it.
// Shape: `CCountedPointer<T> p(new T); return p;` -- VC7.1 has no NRVO, so the named local is copied into the hidden slot
//   (add-reference) and then destroyed (release): the merged `inc ecx ; mov eax,ecx ; dec eax` with both count stores
//   kept. The counted-pointer constructor builds the block only for a non-null object (`new` is null-checked).
struct SFlagRecord_00512c70 {                    // 5 bytes
    unsigned char m_data[4];                     // +0 (left uninitialised)
    bool          m_enabled;                     // +4
    SFlagRecord_00512c70() : m_enabled(true) {}
};

void __fastcall DeleteFlagRecord_00512c70(void* record);   // 0x0050D230

struct SCountBlock {
    long  m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);      // +4: per-type deleter, object in ecx
    void* m_pObject;                             // +8
    SCountBlock(void* object, void (__fastcall* destroy)(void*)) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template <class T>
class CCountedPointer { public:
    T*           m_pObject;                      // +0
    SCountBlock* m_pCount;                       // +4
    CCountedPointer(T* object, void (__fastcall* destroy)(void*)) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(object, destroy);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

CCountedPointer<SFlagRecord_00512c70> __fastcall CreateCountedFlagRecord_00512c70()
{
    CCountedPointer<SFlagRecord_00512c70> record(new SFlagRecord_00512c70, DeleteFlagRecord_00512c70);
    return record;
}