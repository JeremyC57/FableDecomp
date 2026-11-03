// CEAXListener::RemoveSource(CEAXBuffer* source) -- unlink a source from the EAX context's source list and delete its node
// Leaf: ?RemoveSource@CEAXListener@@QAEJPAVCEAXBuffer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5FF60, 78 bytes (thiscall, `ret 4`).
// Callees: the node's deleting destructor through vtable slot 0 (`push 1; call [edx]`; 0x00C60A00 frees with 0x00C3C370).
// Identity: the manifest's "CConsoleVariable_Unregister" is a propagated label. The list head is the EAX 4.0 context's
//   +8 source list (nodes: vtable 0x012B9338, +4 next, +8 source), the inverse of AddSource 0x00C609A0. Descriptive name.
// Shape: walk with a trailing `prev`, `break` on the hit, then unlink + `delete node`; always returns 0.
extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)
extern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370
class CCodaObjectBase {                                  // vtable 0x012B5868: the library's root class
public:
    virtual ~CCodaObjectBase() { }
    static void* operator new(unsigned int size) { return CodaMalloc(size); }  // 0x00C3C360 (through the allocator hook 0x0139C97C)
    static void operator delete(void* p) { CodaFree(p); }          // 0x00C3C370 (through the free hook 0x0139C980)
};

class CEAXBuffer;

class CEAXBufferList : public CCodaObjectBase {          // vtable 0x012B9338, 0xc bytes
public:
    CEAXBufferList();                              // 0x00C5FF50 (out of line: only the vtable store)
    CEAXBufferList* m_pNext;                       // +0x04
    CEAXBuffer*     m_pSource;                     // +0x08
};

struct _CHANGES {                                 // 0x98-byte queued notification
    unsigned long m_flags;                         // +0x00
    unsigned long m_type;                          // +0x04 2 = environment changed
    unsigned long m_environment;                   // +0x08
    unsigned char _pad0c[0x88];
    _CHANGES*    m_pNext;                         // +0x94
};

class CEAXBuffer {
public:
    long MarkAllDirty();                           // 0x00C60FE0: dirty mask +0x368 |= 0x80000000, returns 0
};

class CEAXListener : public CCodaObjectBase {             // vtable 0x012B9340
public:
    long             m_primarySlot;                // +0x004
    CEAXBufferList*  m_pSources;                   // +0x008 sources registered with the context
    unsigned char    _pad00c[0x334];               // +0x00c
    unsigned long    m_environment;                // +0x340 (the committed reverb's ulEnvironment)
    unsigned char    _pad344[0x70];                // +0x344
    long             m_sourceFlags;                // +0x3b4 sign bit: every source must re-send its properties
    unsigned char    _pad3b8[0xc];                 // +0x3b8
    _CHANGES*       m_pEvents;                    // +0x3c4 queued notifications
    unsigned char    _pad3c8[8];                   // +0x3c8
    long             m_queueEvents;                // +0x3d0 1 = queue notifications
    long             m_lastError;                  // +0x3d4
    long RemoveSource(CEAXBuffer* source);
};

long CEAXListener::RemoveSource(CEAXBuffer* source)
{
    CEAXBufferList* prev = 0;
    CEAXBufferList* node = m_pSources;
    while (node)
    {
        if (node->m_pSource == source)
            break;
        prev = node;
        node = node->m_pNext;
    }
    if (node)
    {
        if (prev)
            prev->m_pNext = node->m_pNext;
        else
            m_pSources = node->m_pNext;
        delete node;
    }
    return 0;
}