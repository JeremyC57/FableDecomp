// CEaxSource::Set(unsigned long property, const void* value, SEaxEvent** ppEvents) -- set one EAX source property, then hand back the context's event queue
// Leaf: ?Set@CEaxSource@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61B40, 70 bytes (thiscall, `ret 0Ch`).
// Callees: the context's ClearEvents 0x00C60410, the source's property switch 0x00C60FF0 (`cmp [esp+20h],1` on the
//   third argument: a long "deferred" flag; the value is a pointer to the property block).
// Identity: the manifest's "CDrawGuildSeal_Render" is a propagated label. `this` is an EAX source (vtable 0x012B9344)
//   whose +0x14 is the owning EAX 4.0 context; bit 0x100000 of the property id selects the deferred path and is
//   stripped. No direct or absolute references in the image (an exported/unused entry). Descriptive name.
struct SEaxEvent;

class CEaxContext {
public:
    unsigned char _pad000[0x3c4];
    SEaxEvent* m_pEvents;                          // +0x3c4 queued notifications
    void ClearEvents();                            // 0x00C60410
};

class CEaxSource {
public:
    unsigned char _pad000[0x14];
    CEaxContext* m_pContext;                       // +0x14
    long SetProperty(unsigned long property, const void* value, long deferred);   // 0x00C60FF0
    long Set(unsigned long property, const void* value, SEaxEvent** ppEvents);
};

long CEaxSource::Set(unsigned long property, const void* value, SEaxEvent** ppEvents)
{
    long result;
    m_pContext->ClearEvents();
    if (property & 0x100000)
        result = SetProperty(property & ~0x100000, value, 1);
    else
        result = SetProperty(property, value, 0);
    *ppEvents = m_pContext->m_pEvents;
    return result;
}