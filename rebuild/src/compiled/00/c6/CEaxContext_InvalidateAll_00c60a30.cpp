// CEaxContext::InvalidateAll() -- queue an environment-changed event and mark every registered source dirty
// Leaf: ?InvalidateAll@CEaxContext@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C60A30, 154 bytes (thiscall, plain `ret`).
// Callees: CPKObject::operator new 0x00C3C360 (0x98-byte event, no null check), CEaxSource::MarkAllDirty 0x00C60FE0.
// Identity: the manifest's "CShadowRendererSceneData_OnAnimationLoaded" is a propagated label. When event queueing is on
//   (+0x3d0 == 1) it appends a type-2 event carrying the committed environment (+0x340) to the +0x3c4 queue, sets the
//   context's sign-bit "resend" flag (+0x3b4) and calls MarkAllDirty on each source, stopping at the first failure.
//   Descriptive name.
// Shape: the loop's failure exit must be `return result;` inside the loop body: `break` or a loop condition on `result`
//   splits the live range (eax result, edi zero register, 144 bytes) or tests at the loop top.
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
    long InvalidateAll();
};

long CEaxContext::InvalidateAll()
{
    long result = 0;
    if (m_queueEvents == 1)
    {
        SEaxEvent* ev = (SEaxEvent*)CPKObject::operator new(sizeof(SEaxEvent));
        ev->m_flags = 0;
        ev->m_type = 2;
        ev->m_environment = m_environment;
        ev->m_pNext = 0;
        if (!m_pEvents)
            m_pEvents = ev;
        else
        {
            SEaxEvent* tail = m_pEvents;
            while (tail->m_pNext)
                tail = tail->m_pNext;
            tail->m_pNext = ev;
        }
    }
    m_sourceFlags |= 0x80000000;
    for (CEaxSourceNode* node = m_pSources; node != 0; node = node->m_pNext)
    {
        result = node->m_pSource->MarkAllDirty();
        if (result < 0)
            return result;
    }
    return result;
}