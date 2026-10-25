// CEaxContext::~CEaxContext() -- free the queued events, then delete every source node still on the list
// Leaf: ??1CEaxContext@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C60930, 98 bytes (thiscall; EH frame for the CPKObject base, vtable stores
//   0x012B9340 on entry and the base 0x012B5868 on exit).
// Callees: ClearEvents 0x00C60410 (frees the +0x3c4 event queue), each node's deleting destructor through vtable slot 0.
// Identity: the manifest's "CDrawGuildSeal_Destructor" is a propagated label. The deleting wrapper is 0x00C60AD0 (landed
//   as a CLandscapeBackgroundPatch `??_G`); the vtable 0x012B9340 is the EAX 4.0 context's. Descriptive name.
// Shape: the node is read once before the loop and re-read at the bottom, while the loop TESTS the member
//   (`mov ecx,[esi+8]; mov eax,ecx; test eax,eax`): `while (node)`, `for (...; node; node = m_pSources)` and
//   `while ((node = m_pSources) != 0)` all drop the copy (96 bytes); `while (m_pSources) { node = m_pSources; ... }`
//   reloads and null-checks the delete (103).
class CPKObject {                                  // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* operator new(unsigned int size);  // 0x00C3C360 (through the allocator hook 0x0139C97C)
    static void operator delete(void* p);          // 0x00C3C370 (through the free hook 0x0139C980)
};

class CEaxSource;

class CEaxSourceNode : public CPKObject {          // vtable 0x012B9338, 0xc bytes
public:
    CEaxSourceNode();                              // 0x00C5FF50 (out of line: only the vtable store)
    CEaxSourceNode* m_pNext;                       // +0x04
    CEaxSource*     m_pSource;                     // +0x08
};

struct SEaxEvent {                                 // 0x98-byte queued notification
    unsigned long m_flags;                         // +0x00
    unsigned long m_type;                          // +0x04 2 = environment changed
    unsigned long m_environment;                   // +0x08
    unsigned char _pad0c[0x88];
    SEaxEvent*    m_pNext;                         // +0x94
};

class CEaxSource {
public:
    long MarkAllDirty();                           // 0x00C60FE0: dirty mask +0x368 |= 0x80000000, returns 0
};

class CEaxContext : public CPKObject {             // vtable 0x012B9340
public:
    long             m_primarySlot;                // +0x004
    CEaxSourceNode*  m_pSources;                   // +0x008 sources registered with the context
    unsigned char    _pad00c[0x334];               // +0x00c
    unsigned long    m_environment;                // +0x340 (the committed reverb's ulEnvironment)
    unsigned char    _pad344[0x70];                // +0x344
    long             m_sourceFlags;                // +0x3b4 sign bit: every source must re-send its properties
    unsigned char    _pad3b8[0xc];                 // +0x3b8
    SEaxEvent*       m_pEvents;                    // +0x3c4 queued notifications
    unsigned char    _pad3c8[8];                   // +0x3c8
    long             m_queueEvents;                // +0x3d0 1 = queue notifications
    long             m_lastError;                  // +0x3d4
    void ClearEvents();                            // 0x00C60410
    virtual ~CEaxContext();
};

CEaxContext::~CEaxContext()
{
    ClearEvents();
    CEaxSourceNode* node = m_pSources;
    while (m_pSources)
    {
        m_pSources = node->m_pNext;
        delete node;
        node = m_pSources;
    }
}