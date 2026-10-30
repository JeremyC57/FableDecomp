// CChunkedFile::~CChunkedFile() -- (label propagated) virtual reset of a script-side object: clears two flags and a counter and empties the CScriptThing member by assigning a default-constructed one
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDDCE0, 153 bytes of code. Not a CChunkedFile function (that class is 20 bytes; this
// body writes +0x48..+0x58): it is slot 3 of the no-RTTI vtable around 0x012DCB40 (the only reference to the address),
// so the class is named by that vtable and the method descriptively.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 and CBaseClass::~CBaseClass @ 0x0099A2E0 (the temporary's base, both out of line),
//          operator delete @ 0x00BFE9BC; the count block's destroy hook through `call [block+4]` with the object at
//          [block+8] in ecx.
// Shape: `m_thing = CScriptThing();` -- the 12-byte temporary {CScriptThing vtable 0x01238C8C, PImp {0, 0}} is built
// with the zero register (ebx; the count word through a second zero register); the inlined CCountedPointer
// assignment compares the blocks (constant 0 on the right, so `cmp [esi+58h],ebx`), releases ours without a
// second null test, and stores the (zero) pair it read into locals BEFORE the release (so the stores are
// the zero register and the add-ref folds away; reading the source after the release reloads it and adds an inc); the temporary's destructor stores its vtable first, releases its
// own (null) block, clears both words and calls CBaseClass::~CBaseClass. Counted-pointer model of the landed
// CScriptThing deleting destructor @ 0x004AB170.
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CCountedPointer {
    void* m_pObject;                        // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    __forceinline ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        void* object = other.m_pObject;     // both source words read before the release
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:               // vtable 0x01238C8C
    CCountedPointer PImp;                   // +0x4
    CScriptThing() { }
    virtual ~CScriptThing() { }
};

struct CVtable_012DCB40 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Reset();                   // slot 3
    unsigned char _pad04[0x48 - 4];
    bool m_flagA;                           // +0x48
    bool m_flagB;                           // +0x49
    long m_counter;                         // +0x4c
    CScriptThing m_thing;                   // +0x50
};

void CVtable_012DCB40::Reset()
{
    m_flagA = false;
    m_flagB = false;
    m_counter = 0;
    m_thing = CScriptThing();
}